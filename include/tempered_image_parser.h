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
/*(const std::string& image_file,
                        std::vector<vec_t> *images,
                        float_t scale_min = -1.0,
                        float_t scale_max = 1.0,
                        int x_padding = 2,
                        int y_padding = 2)*/
{

    char someDir[] = "/Users/yuechuan/Develop/FakeImageKiller/res/CASIA2/Tp/";
    std::vector<std::string> filenames;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (someDir)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            //printf ("%s\n", ent->d_name);

            filenames.push_back(ent->d_name);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return -1;
    }

    std::string path_to_img;
    int y_padding = 0;
    int x_padding = 0;
    int width;
    int height;
    double scale_min = -1.0;
    double scale_max = 1.0;


    Mat img;

    cout << "File counter: " << filenames.size() << endl;
    int m = 0;
    int i = 0;
    for (i = 0; i < 42; ++i) {
        path_to_img = someDir + filenames[i];
        //cout << "path_to_img: " << path_to_img << endl;

        img = imread(path_to_img, CV_LOAD_IMAGE_COLOR);
        if(img.empty()) continue;
        cvtColor(img, img, CV_BGR2GRAY);

        //imshow("cvt_color_test", img);


        vec_t image;

        width = img.cols + 2 * x_padding;
        height = img.rows + 2 * y_padding;

        image.resize(width * height, scale_min);

        for (size_t y = 0; y < img.rows; y++) {
            for (size_t x = 0; x < img.cols; x++) {

                image[width * (y + y_padding) + x + x_padding]
                        = (img.data[x * img.cols + y] / 255.0) * (scale_max - scale_min) + scale_min;
            }
        }

        images->push_back(image);

        cout << i << endl;
    }


    return 0;
}




