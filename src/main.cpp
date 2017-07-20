#include <iostream>
#include "csv/csv.h"
#include "traincrf.h"
#include "testcrf.h"
#include "visualize.h"

using namespace std;
using namespace io;

void runrow();
void runcol();
void runboth();


int main(int argc, char *argv[] ) {

    runboth();
    return 0;
}

void runrow(){

    string inputdir="/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir";
    string crfoutrow = "/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/outputrow/";
    string visoutrow = "/Users/talha/Coding/ClionProjects/pricerightnlp/outputdir/rowwise/";
    string outtrain="/Users/talha/Coding/ClionProjects/pricerightnlp/outputdir";
    string outtest="/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/";
    string dir1="/Users/talha/Coding/ClionProjects/pricerightnlp/WBMasonData/A/files.txt";
    string dir2="/Users/talha/Coding/ClionProjects/pricerightnlp/WBMasonData/E/files.txt";
    string batchtest= "/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir/files2.txt";

    cout<<"Trainig CRF"<<endl;
    traincrf train(inputdir, outtrain);
    train.batchprocesstrain(dir1, 1,"row");
    train.batchprocesstrain(dir2, -1,"row");
    train.splitdata();
    train.trainnetwork();

    cout<<"Testing CRF"<<endl;
    testcrf test(inputdir, outtest);
    test.batchprocesstest(batchtest,"row");
    test.testnetwork();

    cout<<endl<<"Display output"<<endl;

    visualize displayitrow(crfoutrow,visoutrow);
    displayitrow.batchprocessdraw("/Users/talha/Coding/ClionProjects/pricerightnlp");


}


void runcol(){


    string inputdir="/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir";
    string crfoutcol = "/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/outputcol/";
    string visoutcol = "/Users/talha/Coding/ClionProjects/pricerightnlp/outputdir/colwise/";
    string outtrain="/Users/talha/Coding/ClionProjects/pricerightnlp/outputdir";
    string outtest="/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/";
    string dir1="/Users/talha/Coding/ClionProjects/pricerightnlp/WBMasonData/A/files.txt";
    string dir2="/Users/talha/Coding/ClionProjects/pricerightnlp/WBMasonData/E/files.txt";
    string batchtest= "/Users/talha/Coding/ClionProjects/pricerightnlp/inputdir/files2.txt";

    cout<<"Trainig CRF"<<endl;
    traincrf train(inputdir, outtrain);
    train.batchprocesstrain(dir1, 1,"col");
    train.batchprocesstrain(dir2, -1,"col");
    train.splitdata();
    train.trainnetwork();

    cout<<"Testing CRF"<<endl;
    testcrf test(inputdir, outtest);
    test.batchprocesstest(batchtest,"col");
    test.testnetwork();

    cout<<endl<<"Display output"<<endl;
    visualize displayitcol(crfoutcol,visoutcol);
    displayitcol.batchprocessdraw("/Users/talha/Coding/ClionProjects/pricerightnlp");


}

void runboth(){

    runrow();
    runcol();
    visualize displayitcompare("","");
    displayitcompare.batchprocesscompare("/Users/talha/Coding/ClionProjects/pricerightnlp");


}