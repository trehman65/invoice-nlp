//
// Created by Talha Rehman on 4/21/17.
//

#include "VerticalProfiles.h"
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

void VerticalProfiles::plotRect(cv::Mat &image,vector<cv::Rect> boxes){

    for(int i=0; i<boxes.size(); i++){
        rectangle(image,boxes[i],Scalar(255,0,0),2, LINE_8);
    }
}

void VerticalProfiles::drawFilledRect(cv::Mat &image, vector<cv::Rect> boxes) {

    for(int i=0; i<boxes.size(); i++){
        rectangle(image,boxes[i],Scalar(0,0,0),2, CV_FILLED);
    }

}

void VerticalProfiles::verticalProjectionProfiles(cv::Mat imgBin, vector<int> &verticalProfiles) {

    Point pt;

    verticalProfiles.clear();

    for (int i=0; i<imgBin.cols; i++){

        verticalProfiles.push_back(0);
        for(int j=0; j<imgBin.rows; j++){
            if(imgBin.at<uchar>(j, i)==0){
                verticalProfiles[i]++;
            }

        }

    }

    //to truncate zeros in start and end
    //vector<int> temp;
    //VerticalProfiles::truncateVector(verticalProfiles,temp);
    //verticalProfiles.clear();
    //VerticalProfiles::noiseFiltering(temp,verticalProfiles,20);


}

void VerticalProfiles::binarizeShafait(Mat &gray, Mat &binary, int w, double k) {
    Mat sum, sumsq;
    gray.copyTo(binary);
    int half_width = w >> 1;
    integral(gray, sum, sumsq, CV_64F);
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            int x_0 = (i > half_width) ? i - half_width : 0;
            int y_0 = (j > half_width) ? j - half_width : 0;
            int x_1 = (i + half_width >= gray.rows) ? gray.rows - 1 : i + half_width;
            int y_1 = (j + half_width >= gray.cols) ? gray.cols - 1 : j + half_width;
            double area = (x_1 - x_0) * (y_1 - y_0);
            double mean = (sum.at<double>(x_0, y_0) + sum.at<double>(x_1, y_1) - sum.at<double>(x_0, y_1) -
                           sum.at<double>(x_1, y_0)) / area;
            double sq_mean = (sumsq.at<double>(x_0, y_0) + sumsq.at<double>(x_1, y_1) - sumsq.at<double>(x_0, y_1) -
                              sumsq.at<double>(x_1, y_0)) / area;
            double stdev = sqrt(sq_mean - (mean * mean));
            double threshold = mean * (1 + k * ((stdev / 128) - 1));
            if (gray.at<uchar>(i, j) > threshold)
                binary.at<uchar>(i, j) = 255;
            else
                binary.at<uchar>(i, j) = 0;
        }
    }
}

void VerticalProfiles::plot(vector<float> temp) {

    plt::plot(temp);
    plt::show();
}

void VerticalProfiles::plot(vector<int> temp) {

    plt::plot(temp);
    plt::show();
}


void VerticalProfiles::gaussianSmoothing(vector<int> &input,vector<int> &blur, int windowSize) {

    int i=0;
    while(i<input.size()-windowSize){
        blur.push_back(sumNextN(input, windowSize, i)/windowSize);
        i++;
    }


}

int VerticalProfiles::sumNextN(vector<int> input, int values, int index) {

    int sum=0;
    for(int i=0; i<values; i++){
        sum+=input[index];
        index++;
    }

    return sum;
}

void VerticalProfiles::noiseFiltering(vector<int> &input, vector<int> &output, int threshold) {

    for(int i=0; i<input.size(); i++){
        if(input[i]>threshold){
            output.push_back(input[i]);
        }
    }
}

void VerticalProfiles::normalizeHistogram(vector<int> &input, vector<float> &normalised) {

    float sum=0;
    for (int i=0; i<input.size(); i++){
        sum+=input[i];
    }

    for (int i=0; i<input.size(); i++){
        normalised.push_back(input[i]/sum);
    }
}
void VerticalProfiles::displayVector(vector<int> input) {
    for (int i=0; i<input.size(); i++){
        cout<<input[i]<<" ";
    }
}

void VerticalProfiles::truncateVector(vector<int> &input, vector<int> &output) {

    int start=0;
    int end=0;

    vector<int> inverted;
    VerticalProfiles::truncateStart(input,start);
    VerticalProfiles::invertVector(input,inverted);
    VerticalProfiles::truncateStart(inverted,end);

    end=input.size()-end;

    for(int i=start; i<end; i++){
        output.push_back(input[i]);
    }
    cout<<"Debug: "<<output.size()<<endl;

}

void VerticalProfiles::truncateStart(vector<int> &input, int &index) {

    int i=0;
    index=0;

    while(i<input.size()){

        if(input[i]==0){
               index++;
                i++;
        }
        else
            return;
    }

}

void VerticalProfiles::invertVector(vector<int> &input, vector<int> &inverted) {

    for (int i=input.size()-1; i>=0; i--){
        inverted.push_back(input[i]);
    }

}

void VerticalProfiles::hammingCalculator(vector<float> &input, vector<int> &hammingvector, float threshold) {

    for(int i=0; i<input.size(); i++){
        if(input[i]<threshold){
            hammingvector.push_back(0);
        }
        else{
            hammingvector.push_back(1);
        }
    }

}


float VerticalProfiles::calculateMean(vector<float> input) {
    float mean;
    float sum=0;

    for(int i=0; i<input.size(); i++){
        sum+=input[i];
    }
    return sum/input.size();
}


void VerticalProfiles::process(string fname,vector<int> &hammingvector) {

    Mat imgRgb=imread(fname);

    vector<int> final;

    Mat imgBin;
    Mat imgGray;
    vector<int> verticalprofiles;
    vector<int> filterverticalprofiles;
    vector<float> normverticalprofiles;


    cvtColor(imgRgb,imgGray,CV_BGR2GRAY);
    VerticalProfiles::binarizeShafait(imgGray,imgBin,50,0.3);
    VerticalProfiles::verticalProjectionProfiles(imgBin,verticalprofiles);
    VerticalProfiles::gaussianSmoothing(verticalprofiles,filterverticalprofiles,40);
    VerticalProfiles::normalizeHistogram(filterverticalprofiles,normverticalprofiles);
    VerticalProfiles::hammingCalculator(normverticalprofiles,hammingvector ,VerticalProfiles::calculateMean(normverticalprofiles));
    //VerticalProfiles::plot(hammingvector);

}