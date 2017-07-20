//
// Created by Talha Rehman on 6/30/17.
//

#include "traincrf.h"
#include "csv/csv.h"

#include <boost/algorithm/string.hpp>



using namespace std;
using namespace io;
using namespace cv;


void traincrf::printvector(vector<string> input) {

    for (int index = 0; index<input.size(); index++){
        cout<<input[index]<<" ";
    }
    cout<<endl;
}


void traincrf::csvtorows(){


    wordlabelpairs.clear();

    CSVReader<8, trim_chars<' '>, double_quote_escape<',', '"'> > csvinput(pathtodata);

    pair<string, string> cell;
    //contains (word,label) paris for cells of one row
    vector<pair<string, string> > thisrow;
    //contains (word,label) paris for cells of entire form

    string token;
    string item, des, qty, um, price1, price2,qtyord,qtyshp;
    int num=0;
    vector<std::string> tokens;

    csvinput.read_header(ignore_missing_column, "ITEM NUMBER", "DESCRIPTION", "QTY", "U/M", "UNIT PRICE", "EXT PRICE","QTY ORD","QTY SHP");


    while (csvinput.read_row(item, des, qty, um, price1, price2, qtyord, qtyshp)) {

        num++;

        thisrow.clear();

        //add 2 rows of items(numeric)
        if (num % 2==0) {
            thisrow.push_back(pair<string, string>(to_string(rand()%1000000), "ITEM"));
            thisrow.push_back(pair<string, string>(to_string(rand()%1000000), "ITEM"));

        }
       else
            //add single row of item
            thisrow.push_back(pair<string, string>(item, "ITEM"));

        if(num %20) {

            boost::split(tokens, des, boost::is_any_of(" "));
            for(int i=0; i<tokens.size();i++){
                if(tokens[i].length() > 4)
                    thisrow.push_back(pair< string, string>(tokens[i], "DES"));
            }

        }
        else{
            //remove white space from description, necessary for training crf
            des.erase(std::remove(des.begin(), des.end(), ' '), des.end());
            thisrow.push_back(pair<string, string>(des, "DES"));
        }

        //play with quanitity
        if (format==1)
            thisrow.push_back(pair<string, string>(qty, "QTY"));
        else {
            thisrow.push_back(pair<string, string>(qtyord, "QTY"));
            thisrow.push_back(pair<string, string>(qtyshp, "QTY"));
        }


//        thisrow.push_back(pair<string, string>(um, "UM"));

        if (num % 5) {
            thisrow.push_back(pair<string, string>(um, "UM"));

            thisrow.push_back(pair<string, string>('$' + price1, "PRICE"));
        }
        else {
            thisrow.push_back(pair<string, string>(um+um, "UM"));
            thisrow.push_back(pair<string, string>(price1, "PRICE"));
        }

        thisrow.push_back(pair<string, string>(price2, "PRICE"));

        if (num % 4 == 2) {

            srand(time(NULL));
            random_shuffle(thisrow.begin(), thisrow.end());
        }

        thisrow.push_back(pair<string, string>(".", "PERIOD"));

        for (int index = 0; index < thisrow.size(); index++) {
            wordlabelpairs.push_back(thisrow[index]);
        }



    }


}



void traincrf::train(){


    if(mode=="row")
        csvtorows();
    if (mode == "col"){
        csvtocols();
    }

    computefeatures();

}


void traincrf::computefeatures() {



    //contains features for a single (word,lable) pair
    vector<string> features;
    int next;
    int prev;

    for (int index=0; index<wordlabelpairs.size();index++ ){

        next = index+1;
        prev= index-1;


        features.clear();
        //word
        features.push_back(wordlabelpairs[index].first);
        //if the word is number
        features.push_back(is_number(wordlabelpairs[index].first));
        //if the word is all alphabets
        features.push_back(is_alpha(wordlabelpairs[index].first));

        //if previous word is numeric
        if (index==0)
            features.push_back("0");
        else
            features.push_back(is_number(wordlabelpairs[prev].first));

        //if next word is numeric
        if (index==wordlabelpairs.size()-1)
            features.push_back("0");
        else
            features.push_back(is_number(wordlabelpairs[next].first));

        //length of word
        features.push_back(to_string(wordlabelpairs[index].first.length()));

//        //has decimal
//        features.push_back(to_string(wordlabelpairs[index].first.find(".")<5));

        //previous word
        if (index==0)
            features.push_back("null");
        else
            features.push_back(wordlabelpairs[prev].first);

        //next word
        if (index==wordlabelpairs.size()-1)
            features.push_back("null");
        else
            features.push_back(wordlabelpairs[next].first);

        //annotation/label
        features.push_back(wordlabelpairs[index].second);


        output.push_back(features);
    }

//    for(int i=0; i<output.size();i++){
//
//        printvector(output[i]);
//    }



}


string traincrf::is_number(const std::string& s)
{
    return to_string(!s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c) { return !(std::isdigit(c)|| c=='.'); }) == s.end());
}


string traincrf::is_alpha(const std::string& s)
{
    return to_string(!s.empty() && std::find_if(s.begin(),
                                                s.end(), [](char c) { return !std::isalpha(c); }) == s.end());
}

void traincrf::outputtofile(){

    //train data
    string data=pathtooutput+"/data.txt";
    fstream outfile;

    outfile.open(data,ios::out);
    count = 0;

    for(int i = 0; i<output.size(); i++){

        if(output[i][0] == ".") {
            outfile << endl;
            continue;
        }


        if(output[i][0] == " " || output[i][0] == "") {
            continue;
        }


        for(int j = 0; j < output[i].size(); j++){
            outfile << output[i][j]<<" ";
        }
        outfile << endl;
        count++;
    }

}

void traincrf::setINpath(string in){
    pathtodata=in;
}

void traincrf::setOUTpath(string out){
    pathtooutput=out;
}

void traincrf::batchprocesstrain(string path, int modein, string runmode){

    fstream files(path);
    string csvfile;

    format = modein;
    mode  = runmode;

    while(getline(files, csvfile)){

        cout<<"Processing: "<<csvfile<<endl;

        setINpath(csvfile);
        train();
    }

    outputtofile();

}


void traincrf::splitdata() {

    string crfpath="/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp";

    fstream data(pathtooutput+"/data.txt");
    fstream train(crfpath+"/traindata-"+ mode +".txt",ios::out);
    fstream test(crfpath+"/testdata-"+ mode +".txt",ios::out);


    string line;

    int index=0;

    while(getline(data, line)){

        if(index < int(0.8*count))
            train << line<<endl;
        else
            test << line<<endl;

        index++;
    }

}

void traincrf::trainnetwork(){
    if (mode == "row")
        system("/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/runtraintestrow.sh");
    else
        system("/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/runtraintestcol.sh");

};



void traincrf::csvtocols(){

    wordlabelpairs.clear();

    pair<string, string> cell;
    //contains (word,label) paris for cells of one row
    vector<pair<string, string> > thiscol;
    //contains (word,label) paris for cells of entire form

    string token;
    string item, des, qty, um, price1, price2,qtyord,qtyshp;
    int num=0;
    vector<std::string> tokens;

    CSVReader<8, trim_chars<' '>, double_quote_escape<',', '"'> > csvinput(pathtodata);
    csvinput.read_header(ignore_missing_column, "ITEM NUMBER", "DESCRIPTION", "QTY", "U/M", "UNIT PRICE", "EXT PRICE","QTY ORD","QTY SHP");

    vector<pair<string,string>> lstdes, lstitem, lstqty, lstum, lstprice1, lstprice2, lstqtyord, lstqtyshp;


    while (csvinput.read_row(item, des, qty, um, price1, price2, qtyord, qtyshp)) {

        num++;

        thiscol.clear();

        //add single row of item
        if(1)
            lstitem.push_back(pair<string, string>(item, "ITEM"));
        else
            lstitem.push_back(pair<string, string>(to_string(rand()%1000000), "ITEM"));


        //remove white space from description, necessary for training crf
        boost::split(tokens, des, boost::is_any_of(" "));
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i].length() > 4)
                lstdes.push_back(pair<string, string>(tokens[i], "DES"));

        }

        //play with quanitity
        if (format==1)
            lstqty.push_back(pair<string, string>(qty, "QTY"));
        else {
            lstqty.push_back(pair<string, string>(qtyord, "QTY"));
            lstqty.push_back(pair<string, string>(qtyshp, "QTY"));
        }

        lstum.push_back(pair<string, string>(um, "UM"));

        lstprice1.push_back(pair<string, string>(price1, "PRICE"));
        lstprice2.push_back(pair<string, string>("$"+price2, "PRICE"));

    }

    if(num > 7) {
        lstitem.clear();
        lstum.clear();
        for (int i = 0; i < num; i++) {
            lstitem.push_back(pair<string, string>(to_string(rand() % 1000000), "ITEM"));
            lstum.push_back(pair<string, string>(um+um, "UM"));

        }
    }


    for(int i=0; i<lstitem.size(); i++)
        wordlabelpairs.push_back(lstitem[i]);
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));


    for(int i=0; i<lstdes.size(); i++)
        wordlabelpairs.push_back(lstdes[i]);
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));

    if(format == 1) {
        for (int i = 0; i < lstqty.size(); i++)
            wordlabelpairs.push_back(lstqty[i]);
    }
    else{
        for (int i = 0; i < lstqtyord.size(); i++)
            wordlabelpairs.push_back(lstqtyord[i]);
        wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));

        for (int i = 0; i < lstqtyshp.size(); i++)
            wordlabelpairs.push_back(lstqtyshp[i]);

    }
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));

    for(int i=0; i<lstum.size(); i++)
        wordlabelpairs.push_back(lstum[i]);
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));

    for(int i=0; i<lstprice1.size(); i++)
        wordlabelpairs.push_back(lstprice1[i]);
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));

    for(int i=0; i<lstprice2.size(); i++)
        wordlabelpairs.push_back(lstprice2[i]);
    wordlabelpairs.push_back(pair<string, string>(".", "PERIOD"));


}

