/*
 * Token.cpp
 *
 *  Created on: 03/10/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#include "Token.h"
#include <vector>

using namespace std;

Token::Token() {
    isTuple = false;
    construct();
	}

Token::Token(std::string value, std::string type){
	this->value = value;
	this->type = type;
}

Token::Token(std::string type, int envNum){
	this->type = type;
	this->envNum = envNum;
	isTuple = false;
	construct();
}

void Token::construct(){
	this->tuple = vector<Token>();
}

Token::Token(std::string type,std::string lambdaParam, int lambdaNum){
	this->type = type;
	this->lambdaParam = lambdaParam;
	this->lambdaNum = lambdaNum;
	isTuple = false;
	construct();
}

Token::Token(std::string type, int betaIfDeltaNum, int betaElseDeltaNum){
	this->type = type;
	this->betaIfDeltaNum = betaIfDeltaNum;
	this->betaElseDeltaNum = betaElseDeltaNum;
	isTuple = false;
	construct();
}

Token::~Token(){
    
    // delete this->value;
    // delete this->type;
    
	}
