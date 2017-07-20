//
// Created by Talha Rehman on 7/17/17.
//

#ifndef PRICERIGHTNLP_VISUALIZE_H
#define PRICERIGHTNLP_VISUALIZE_H

#include <iostream>
#include "fstream"

//open cv headers
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <ctype.h>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace cv;

class visualize {

private:
    string dir1;
    string dir2;
    string dir3;
public:

    visualize(string a, string b){

        dir2 = a;
        dir3 = b;
    }

    void drawlabels(string);
    void batchprocessdraw(string);
    void compare(string);
    void batchprocesscompare(string);


};


#endif //PRICERIGHTNLP_VISUALIZE_H
