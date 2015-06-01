# FakeImageKiller

Tempered image recognition based on convolution neural network.

* Training Set: [CASIA 2.0](http://forensics.idealtest.org:8080/)
* CNN: [tiny-cnn](https://github.com/nyanp/tiny-cnn)

## Requirement

* gcc 4.7+
* g++ 4.7+
* boost
* opencv2

## Usage

	$ git clone https://github.com/4x7y/FakeImageKiller.git
	$ mkdir -p FakeImageKiller/build
	$ cd FakeImageKiller/build
	$ cmake ..
	$ make
	$ ./FakeImageKiller

## Acknowledge

* **CASIA Image Tempering Detection Evaluation Database**

	Credits for the use of the CASIA Image Tempering Detection Evaluation Database (CAISA TIDE) V2.0 are given to the National Laboratory of Pattern Recognition, Institute of Automation, Chinese Academy of Science, Corel Image Database and the photographers. [http://forensics.idealtest.org](http://forensics.idealtest.org)

* **Tiny-CNN**

	Tiny-cnn is a C++11 implementation of deep learning (convolutional neural networks).
