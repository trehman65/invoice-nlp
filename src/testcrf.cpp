//
// Created by Talha Rehman on 7/17/17.
//

#include "testcrf.h"


void testcrf::texttofeaturesrow(string filename) {

    string path= "/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir/";
    string pathtotext = path+filename+".txt";
    ifstream file(pathtotext);
    string line;

//    cout<<endl<<pathtotext;

    vector<std::string> tokens;
    vector<string> words;

    while(getline(file, line)) {

        boost::split(tokens, line, boost::is_any_of(" "));

        if(line.length()>1) {

            for (int i = 0; i < tokens.size(); i++) {
//                cout<<tokens[i]<<endl;
                tokens[i].erase(std::remove(tokens[i].begin(),tokens[i].end(),' '),tokens[i].end());
                if (tokens[i].length() > 0)
                    words.push_back(tokens[i]);
            }
            words.push_back(".");
        }
    }

    int next;
    int prev;

    vector<string> temp;

    boost::split(temp,pathtotext, boost::is_any_of("/"));
    string outputfilename = temp[temp.size()-1];

    string outfilefinalname= pathtooutput+"/inputrow/"+outputfilename;

    fstream out(outfilefinalname,ios::out);

    for (int index=0; index<words.size();index++ ){

        next = index+1;
        prev= index-1;

        if(words[index]=="."){
            out << endl;
            continue;
        }


        //word
        out<<words[index]<<" ";
        //if the word is number
        out<<is_number(words[index])<<" ";
        //if the word is all alphabets
        out<<is_alpha(words[index])<<" ";

        //if previous word is numeric
        if (index==0)
            out<<"0"<<" ";
        else
            out<<is_number(words[prev])<<" ";

        //if next word is numeric
        if (index==words.size()-1)
            out<<"0"<<" ";
        else
            out<<is_number(words[next])<<" ";

        //length of word
        out<<to_string(words[index].length())<<" ";
//
//        //has decimal
//       out << to_string(words[index].find(".")<5);


        //previous word
        if (index==0)
            out<<"null"<<" ";
        else
            out<<words[prev]<<" ";

        //next word
        if (index==words.size()-1)
            out<<"null"<<" ";
        else
            out<<words[next]<<" ";

        out<<endl;
    }

}


void testcrf::batchprocesstest(string pathtofiles,string modein) {

    fstream files(pathtofiles);
    string line;

    mode = modein;

    while(getline(files, line)){

        if(mode == "row")
            texttofeaturesrow(line);
        else{
            texttofeaturescol(line);

        }


    }
}



void testcrf::texttofeaturescol(string filename) {

    string path= "/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir/";
    string pathtoimage = path+filename;
    string pathtojson = path+filename+".json";
//    cout<<pathtoimage<<endl;
    Mat img = imread(pathtoimage);
    int rows = img.rows;

    vector<int> hamming;
    pair<int,int> point;
    bool flag=0;
    vector<pair<int,int>> colsinfo;

    VerticalProfiles test;
    test.process(pathtoimage,hamming);
    colsinfo=detectcols(hamming);


    //read json
    //map boxes to cols
    //put all the boxes that are in same col into a line and then add
    //perios and next col intot the file and calsulate featrues

    vector<pair<string, cv::Rect>> wordandbox;
    readwordboxes(pathtojson,wordandbox);

    float mean;


    vector<string> cols[colsinfo.size()];


    for(int i=0; i<wordandbox.size(); i++) {


        mean = 0.5 * (wordandbox[i].second.y + wordandbox[i].second.y + wordandbox[i].second.width);

        for(int j=0; j<colsinfo.size(); j++) {
            if (mean >= colsinfo[j].first - 25 && mean <= colsinfo[j].second + 25) {

                cols[j].push_back(wordandbox[i].first);
                //cout<<wordandbox[i].first<<" ";


            }
        }
    }

    vector<string> words;
    for(int i=0; i<colsinfo.size(); i++){
        for(int j=0; j<cols[i].size(); j++){

            if(cols[i][j].length()>0){
                words.push_back(cols[i][j]);
//                    cout<<cols[i][j]<<" ";
            }
        }
        words.push_back(".");
    }



    int next;
    int prev;

    vector<string> temp;

    string finalname=pathtooutput+"/inputcol/"+filename+".txt";

    fstream out(finalname,ios::out);

    for (int index=0; index<words.size();index++ ){

        next = index+1;
        prev= index-1;

        if(words[index]=="."){
            out << endl;
            continue;
        }

        //word
        out<<words[index]<<" ";
        //if the word is number
        out<<is_number(words[index])<<" ";
        //if the word is all alphabets
        out<<is_alpha(words[index])<<" ";

        //if previous word is numeric
        if (index==0)
            out<<"0"<<" ";
        else
            out<<is_number(words[prev])<<" ";

        //if next word is numeric
        if (index==words.size()-1)
            out<<"0"<<" ";
        else
            out<<is_number(words[next])<<" ";

        //length of word
        out<<to_string(words[index].length())<<" ";
//
//        //has decimal
//       out << to_string(words[index].find(".")<5);


        //previous word
        if (index==0)
            out<<"null"<<" ";
        else
            out<<words[prev]<<" ";

        //next word
        if (index==words.size()-1)
            out<<"null"<<" ";
        else
            out<<words[next]<<" ";

        out<<endl;
    }

}



vector<pair<int,int>> testcrf::detectcols(vector<int> hamming){

    bool flag=0;
    pair<int,int> point;
    vector<pair<int,int>> cols;


    for(int i=0; i<hamming.size(); i++){

        if(hamming[i]==1 && flag == 0){

            flag=1;
            point.first=i;
        }
        else if(hamming[i]==0){

            if (flag==1) {
                point.second=i;
                //if ((point.second-point.first) > 10){
                if(1){
                    cols.push_back(point);
//                    cout << point.first<<" "<<point.second << endl;
                }
            }
            flag = 0;
        }

    }

    return cols;

};

void testcrf::readwordboxes(string pathtojson,vector<pair<string, cv::Rect>>& wordandbox){

    fstream file(pathtojson);
    auto temp = json::parse(file);
    json::value arr = temp["ocrresult"]["words"]["word"];
    const json::array words = as_array(arr);
    pair<string, Rect> thiswordbox;

    for(auto it = words.begin(); it != words.end(); ++it) {
        const json::value &v = *it;

        thiswordbox.first = to_string(v["content"]);
        thiswordbox.second = Rect(stoi(to_string(v["bottom"])),stoi(to_string(v["left"])),stoi(to_string(v["right"])) - stoi(to_string(v["left"])),stoi(to_string(v["top"])) - stoi(to_string(v["bottom"])));
        wordandbox.push_back(thiswordbox);
        //  cout<<thiswordbox.first<<endl;
    }


}



void testcrf::testnetwork() {

    if (mode == "row")
        system("/Users/talha/Coding/ClionProjects/pricerightnlp/testrow.sh");
    else
        system("/Users/talha/Coding/ClionProjects/pricerightnlp/testcol.sh");

}

