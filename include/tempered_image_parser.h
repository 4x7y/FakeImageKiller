//
// Created by Xue Yuechuan on 6/4/15.
//

#pragma once
#include "util.h"
#include <fstream>
#include <cstdint>
#include <boost/detail/endian.hpp>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

using namespace boost::filesystem;
using namespace std;
using namespace cv;
using namespace tiny_cnn;

void getFileNames(String config_path,  vector<string> &fileNames)
{
    path inputPath (config_path);
    try
    {
        if (exists(inputPath))
        {
            if (is_regular_file(inputPath))
            {
                cout << inputPath << " is not a directory, its size is "
                << file_size(inputPath) << '\n';
            }
            else if (is_directory(inputPath))
            {
                typedef vector<path> VecPath;
                VecPath paths;

                copy(directory_iterator(inputPath), directory_iterator(), back_inserter(paths));
                sort(paths.begin(), paths.end());

                for(VecPath::const_iterator it = paths.begin(); it != paths.end(); it++)
                {
                    string fileNam = it->filename().string();
                    if(fileNam.find("."+config_path) != string::npos)
                        fileNames.push_back(fileNam);
                    //cout<< *it<<endl;
                }
            }
            else
                cout << inputPath << " exists, but is neither a regular file nor a directory\n";
        }
        else
            cout << inputPath << " does not exist\n";
    }

    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }
}


void parse_tempered_images()
/*(const std::string& image_file,
                        std::vector<vec_t> *images,
                        float_t scale_min = -1.0,
                        float_t scale_max = 1.0,
                        int x_padding = 2,
                        int y_padding = 2)*/
{
    std::vector<String> filenames;
    String config_path = "/home/cloud/Develop/FakeImageKiller/res/Tp/";

    getFileNames(config_path, filenames);

    for ( int i = 0 ; i < filenames.size() ; i++ )
    {
        cout << filenames[i] << endl;
    }
}


