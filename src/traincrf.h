//
// Created by Talha Rehman on 6/30/17.
//

#ifndef PRICERIGHTNLP_CSVTOFEATUREVECTOR_H
#define PRICERIGHTNLP_CSVTOFEATUREVECTOR_H


#include <iostream>
#include "vector"
#include "fstream"
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "json/json.h"

//open cv headers
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

class traincrf {

private:
    string pathtodata;
    string pathtooutput;
    int format;
    int count;
    string mode;

    vector<vector<string>> output;
    vector<pair<string, string> > wordlabelpairs;


public:
    traincrf(string pathI, string pathO){
        pathtodata=pathI;
        pathtooutput=pathO;

    }

    traincrf(){

    }

    void train();
    void batchprocesstrain(string, int, string);

    void outputtofile();
    void printvector(vector<string>);

    string is_number(const std::string&);
    string is_alpha(const std::string&);

    void setINpath(string);
    void setOUTpath(string);

    void splitdata();

    void csvtorows();
    void computefeatures();

    void trainnetwork();

    void csvtocols();






    };


#endif //PRICERIGHTNLP_CSVTOFEATUREVECTOR_H
