//
// Created by Xue Yuechuan on 6/4/15.
//

#pragma once

#include "util.h"
#include <fstream>
#include <cstdint>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <dirent.h>

using namespace std;
using namespace cv;
using namespace tiny_cnn;

int parse_tempered_images(std::vector<vec_t> *images)
{
    char TpDir[] = "../res/CASIA2/Tp/";
    char AuDir[] = "../res/CASIA2/Au/";
    std::vector<std::string> filenames;

    std::string path_to_img;
    int y_padding = 0;
    int x_padding = 0;
    int width;
    int height;
    double scale_min = -1.0;
    double scale_max = 1.0;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (TpDir)) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            filenames.push_back(ent->d_name);
        }
        closedir (dir);
    } else {
        perror ("");
        return -1;
    }

    Mat img;
    vec_t image;

    cout << "File counter: " << filenames.size() << endl;
    int i = 0;
    for (i = 0; ; ++i) {
        path_to_img = TpDir + filenames[i];
        img = imread(path_to_img, CV_LOAD_IMAGE_COLOR);
        if(img.empty()) continue;

        //if(img.cols!=384 || img.rows!=256) {
        //    continue;
        //}
        cout  << "x: " << img.cols << "\ty: " << img.rows << endl;
        cvtColor(img, img, CV_BGR2GRAY);

        width = 28 + 2 * x_padding;
        height = 28 + 2 * y_padding;
        image.resize(width * height, scale_min);

        for (size_t y = 0; y < 28/*img.rows*/; y++) {
            for (size_t x = 0; x < 28/*img.cols*/; x++) {
                image[width * (y + y_padding) + x + x_padding]
                        = (img.data[x * img.cols + y] / 255.0) * (scale_max - scale_min) + scale_min;
            }
        }
        images->push_back(image);
	if(images->size() == 20) {
	    break;
	}
        //cout << i << endl;
    }
    cout << "---------------" << endl;    
    filenames.clear();

    if ((dir = opendir (AuDir)) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            filenames.push_back(ent->d_name);
        }
        closedir (dir);
    } else {
        perror ("");
        return -1;
    }

    for (i = 0; ; ++i) {
        path_to_img = AuDir + filenames[i];
        img = imread(path_to_img, CV_LOAD_IMAGE_COLOR);
        if(img.empty()) continue;

        //if(img.cols!=384&&img.rows!=256) {
        //    i--;
        //    continue;
        //}
        cout  << "x: " << img.cols << "\ty: " << img.rows << endl;
        cvtColor(img, img, CV_BGR2GRAY);

        width = 28 + 2 * x_padding;
        height = 28 + 2 * y_padding;
        image.resize(width * height, scale_min);

        for (size_t y = 0; y < 28 /*img.rows*/; y++) {
            for (size_t x = 0; x < 28 /*img.cols*/; x++) {
                image[width * (y + y_padding) + x + x_padding]
                        = (img.data[x * img.cols + y] / 255.0) * (scale_max - scale_min) + scale_min;
            }
        }

        images->push_back(image);
	if(images->size() == 40) break;
    }
    cout << "picture numbers: " << images->size() << endl;
    cout << "image size: " << image.size() << endl;
    return 0;
}

int parse_tempered_labels(std::vector<label_t> *labels)
{
    for (int i = 0; i < 20; ++i) {
        labels->push_back((label_t)0);
    }
    for (int i = 0; i < 20; ++i) {
        labels->push_back((label_t)1);
    }
    for (size_t i = 0; i < 40; ++i) {
	printf("%d ", (*labels)[i]);
    }
    cout << endl;
    return 0;

}



