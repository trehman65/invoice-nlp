//
// Created by Talha Rehman on 7/17/17.
//

#ifndef PRICERIGHTNLP_TESTCRF_H
#define PRICERIGHTNLP_TESTCRF_H

#include <iostream>
#include "vector"
#include "fstream"
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "traincrf.h"
#include <boost/algorithm/string.hpp>
#include "VerticalProfiles.h"


using namespace std;

class testcrf : public traincrf  {

private:
    string pathtodata;
    string pathtooutput;
    string mode;

public:

    testcrf(string pathI, string pathO){
        pathtodata=pathI;
        pathtooutput=pathO;

    }
    void texttofeaturesrow(string);
    void texttofeaturescol(string);
    void batchprocesstest(string,string);

    vector<pair<int,int>> detectcols(vector<int> );
    void readwordboxes(string ,vector<pair<string, cv::Rect>>& );


    void testnetwork();


};


#endif //PRICERIGHTNLP_TESTCRF_H
