/*
    Copyright (c) 2013, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "layer.h"
#include "product.h"
#include "dropout.h"

namespace tiny_cnn {

template<typename Activation, typename Filter = filter_none>
class fully_connected_layer : public layer<Activation> {
public:
    fully_connected_layer(layer_size_t in_dim, layer_size_t out_dim)
        : layer<Activation>(in_dim, out_dim, size_t(in_dim) * out_dim, out_dim), filter_(out_dim) {}

    size_t connection_size() const override {
        return size_t(this->in_size_) * this->out_size_ + this->out_size_;
    }

    size_t fan_in_size() const override {
        return this->in_size_;
    }

    const vec_t& forward_propagation(const vec_t& in, size_t index) {

        for_(this->parallelize_, 0, this->out_size_, [&](const blocked_range& r) {
            for (int i = r.begin(); i < r.end(); i++) {
                float_t z = 0.0;
                for (int c = 0; c < this->in_size_; c++)
                    z += this->W_[c*this->out_size_ + i] * in[c];

                z += this->b_[i];
                this->output_[index][i] = this->a_.f(z);
            }
        });

        auto& this_out = this->filter_.filter_fprop(this->output_[index], index);

        return this->next_ ? this->next_->forward_propagation(this_out, index) : this_out;
    }

    const vec_t& back_propagation(const vec_t& current_delta, size_t index) {
        const vec_t& curr_delta = this->filter_.filter_bprop(current_delta, index);
        const vec_t& prev_out = this->prev_->output(index);
        const activation::function& prev_h = this->prev_->activation_function();
        vec_t& prev_delta = this->prev_delta_[index];
        vec_t& dW = this->dW_[index];
        vec_t& db = this->db_[index];

        for (int c = 0; c < this->in_size_; c++) {
            // propagate delta to previous layer
            // prev_delta[c] += current_delta[r] * W_[c * out_size_ + r]
            prev_delta[c] = vectorize::dot(&curr_delta[0], &this->W_[c*this->out_size_], this->out_size_);
            prev_delta[c] *= prev_h.df(prev_out[c]);
        }

        for_(this->parallelize_, 0, this->out_size_, [&](const blocked_range& r) {
            // accumulate weight-step using delta
            // dW[c * out_size + i] += current_delta[i] * prev_out[c]
            for (int c = 0; c < this->in_size_; c++)
                vectorize::muladd(&curr_delta[0], prev_out[c], r.end() - r.begin(), &dW[c*this->out_size_ + r.begin()]);

            for (int i = r.begin(); i < r.end(); i++) 
                db[i] += curr_delta[i];
        });

        return this->prev_->back_propagation(this->prev_delta_[index], index);
    }

    const vec_t& back_propagation_2nd(const vec_t& current_delta2) {
        const vec_t& prev_out = this->prev_->output(0);
        const activation::function& prev_h = this->prev_->activation_function();

        for (int c = 0; c < this->in_size_; c++) 
            for (int r = 0; r < this->out_size_; r++)
                this->Whessian_[c*this->out_size_ + r] += current_delta2[r] * sqr(prev_out[c]);

        for (int r = 0; r < this->out_size_; r++)
            this->bhessian_[r] += current_delta2[r];

        for (int c = 0; c < this->in_size_; c++) { 
            this->prev_delta2_[c] = 0.0;

            for (int r = 0; r < this->out_size_; r++) 
                this->prev_delta2_[c] += current_delta2[r] * sqr(this->W_[c*this->out_size_ + r]);

            this->prev_delta2_[c] *= sqr(prev_h.df(prev_out[c]));
        }

        return this->prev_->back_propagation_2nd(this->prev_delta2_);
    }

protected:
    Filter filter_;
};

} // namespace tiny_cnn
