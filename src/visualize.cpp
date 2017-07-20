//
// Created by Talha Rehman on 7/17/17.
//

#include "visualize.h"
#include "json/json.h"


void visualize::drawlabels(string filename){

    //open image
    //read json

    string pathtoimg = dir1+filename;
    string pathtogt = dir1+filename+".json";
    string pathtoout = dir2+filename+".txt";

    cout<<"Displaying: "<<filename<<endl;
//    cout<<pathtoimg<<" "<<pathtogt<<" "<<pathtoout<<endl;


    vector<pair<string, Rect>> wordandbox;
    Mat image;

    image= imread(pathtoimg);


//    string pathtolabelprob=dir3+filename+".label.txt";
//    fstream outlabelprob(pathtolabelprob,ios::out);

    fstream jsonfile(pathtogt);
    auto temp = json::parse(jsonfile);
    json::value arr = temp["ocrresult"]["words"]["word"];
    const json::array words = as_array(arr);
    pair<string, Rect> thiswordbox;
    string line;
    vector<std::string> tokens;
    vector<std::string> labelprob;
    string label;
    string prob;


    for(auto it = words.begin(); it != words.end(); ++it) {
        const json::value &v = *it;

        thiswordbox.first = to_string(v["content"]);

        thiswordbox.second = Rect(stoi(to_string(v["left"])),image.rows - stoi(to_string(v["top"])),stoi(to_string(v["right"])) - stoi(to_string(v["left"])),stoi(to_string(v["top"])) - stoi(to_string(v["bottom"])));
        fstream outputlabels(pathtoout);

        while (getline(outputlabels, line)) {

            if(line.length() >15){

                boost::split(tokens, line, boost::is_any_of("\t"));
                boost::split(labelprob, tokens.back(), boost::is_any_of("/"));

                label = labelprob[0];
                 prob = labelprob[1];

                if (thiswordbox.first == tokens[0]) {

                    if (label == "ITEM")
                        rectangle(image, thiswordbox.second, Scalar(255, 0, 0), 4);
                    else if (label == "DES")
                        rectangle(image, thiswordbox.second, Scalar(0, 255, 0), 4);
                    else if (label == "QTY")
                        rectangle(image, thiswordbox.second, Scalar(0, 0, 255), 4);
                    else if (label == "UM")
                        rectangle(image, thiswordbox.second, Scalar(255, 255, 0), 4);
                    else if (label == "PRICE")
                        rectangle(image, thiswordbox.second, Scalar(0, 255, 255), 4);

    //               cout<<thiswordbox.first<<" "<<label<<endl ;

//                    outlabelprob<<thiswordbox.first<<" "<<label<<" "<<prob<<endl;
                    break;

                }

            }
        }
        outputlabels.close();

    }



    string outimage = dir3+filename;
//    cout<<outimage;

    imwrite(outimage,image);


}

void visualize::batchprocessdraw(string pathtofiles){

    fstream files(pathtofiles+"/inputdir/files2.txt");
    dir1 = pathtofiles+"/inputdir/";

    //cout<<dir2<<endl<<dir3;

    string line;

    while(getline(files, line)){

        drawlabels(line);

    }
}


void visualize::compare(string filename) {


    //open image
    //read json

    dir1 = "/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir/";
    string pathtoimg = dir1 + filename;
    string pathtogt = dir1 + filename + ".json";
    string pathtooutrow = "/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/outputrow/" + filename + ".txt";
    string pathtooutcol = "/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/outputcol/" + filename + ".txt";


    vector<pair<string, Rect>> wordandbox;
    Mat image;
    cout << "Comparing: " << filename << endl;

    image = imread(pathtoimg);


    string pathtolabelprob = dir3 + filename + ".label.txt";
    fstream outlabelprob(pathtolabelprob, ios::out);

    fstream jsonfile(pathtogt);
    auto temp = json::parse(jsonfile);
    json::value arr = temp["ocrresult"]["words"]["word"];
    const json::array words = as_array(arr);
    pair<string, Rect> thiswordbox;
    string line;
    vector<std::string> tokens;
    vector<std::string> labelprob;
    string label;
    string prob;


    for (auto it = words.begin(); it != words.end(); ++it) {
        const json::value &v = *it;

        thiswordbox.first = to_string(v["content"]);

        thiswordbox.second = Rect(stoi(to_string(v["left"])), image.rows - stoi(to_string(v["top"])),
                                  stoi(to_string(v["right"])) - stoi(to_string(v["left"])),
                                  stoi(to_string(v["top"])) - stoi(to_string(v["bottom"])));

        fstream outputlabelsrow(pathtooutrow);
        fstream outputlabelscol(pathtooutcol);


        while (getline(outputlabelsrow, line)) {

            if (line.length() > 15) {

                boost::split(tokens, line, boost::is_any_of("\t"));
                boost::split(labelprob, tokens.back(), boost::is_any_of("/"));

                if (thiswordbox.first == tokens[0]) {

                    label = labelprob[0];
                    prob = labelprob[1];

                    break;

                }

            }
        }
        outputlabelsrow.close();

        while (getline(outputlabelscol, line)) {

            if (line.length() > 15) {

                boost::split(tokens, line, boost::is_any_of("\t"));
                boost::split(labelprob, tokens.back(), boost::is_any_of("/"));

                if (thiswordbox.first == tokens[0]) {

                    if (strtof(prob.c_str(), 0) < strtof(labelprob[1].c_str(), 0)) {
                        label = labelprob[0];
                        prob = labelprob[1];
                    }
                    break;

                }

            }
        }

        if (label == "ITEM")
            rectangle(image, thiswordbox.second, Scalar(255, 0, 0), 4);
        else if (label == "DES")
            rectangle(image, thiswordbox.second, Scalar(0, 255, 0), 4);
        else if (label == "QTY")
            rectangle(image, thiswordbox.second, Scalar(0, 0, 255), 4);
        else if (label == "UM")
            rectangle(image, thiswordbox.second, Scalar(255, 255, 0), 4);
        else if (label == "PRICE")
            rectangle(image, thiswordbox.second, Scalar(0, 255, 255), 4);


    }

    string outimage = "/Users/talha/Coding/ClionProjects/pricerightnlp/outputdir/better/" + filename;

    imwrite(outimage, image);


}


void visualize::batchprocesscompare(string pathtofiles){

    fstream files(pathtofiles+"/inputdir/files2.txt");
    dir1 = pathtofiles+"/inputdir/";

    cout<<dir2<<endl<<dir3;

    string line;

    while(getline(files, line)){

        compare(line);

    }
}



