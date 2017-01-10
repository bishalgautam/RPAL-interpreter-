/*
 * ReadFile.cpp
 *
 *  Created on: 03/07/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */
#include <iostream>
#include <fstream>
#include <string>
#include "ReadFile.h"

using namespace std;

ReadFile::ReadFile(char* fileName) {
	this->fileName = fileName;
}

ReadFile::~ReadFile() {
}
std::string ReadFile::getFileContent(){
	std::string returnString;
	std::string tempString;
    
    try {
       ifstream inputFile;
    inputFile.open(this->fileName, ios::in);
    if(inputFile.is_open()){
        while(inputFile.good()){
            getline(inputFile,tempString);
            returnString +=  tempString+'\n';
            }
    }else{
            cerr << "Error: Couldnt Open the Input File";
    }
    
        } catch (exception& ex) {
        cout << ex.what() << '\n';
        }
	return returnString;
}

