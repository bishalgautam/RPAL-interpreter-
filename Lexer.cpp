/*
 * Lexer.cpp
 *
 *  Created on: 02/28/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#include <iostream>
#include "Lexer.h"
#include "Token.h"
#include <string>
#include <cstdlib>
//#include <ext/hash_set>
#include <unordered_set>

using namespace std;


Lexer::Lexer(std::string codeString) {
	this->codeString = codeString;
	this->size = codeString.size();
	this->readPtr = 0;
    this->createHash = false;
    this->createHash1 =false;
}

Lexer::~Lexer() {

}

Token Lexer::getNextToken()
{
	Token token;
	//Handle punctuation characters
	//Handle String if next character start with ''
	while(1){
		if(readPtr == size)
			throw "No more tokens";
		char ch = codeString.at(readPtr++);
       // for next character is a space , leave it.
		if(isspace(ch) or ch == '\t' or ch == '\n'){
			continue;
		}//for next character a letter, build the identifier and store type as identifier.
        else if(isalpha(ch)){
			token.value +=ch;
			while(1){
				if(readPtr != size){
					ch = codeString.at(readPtr++);
					if(isalpha(ch) or isdigit(ch) or ch == '_'){
						token.value +=ch;
					}else{
						readPtr--;
						break;
					}
				}else{
					break;
				}
			}
			if(isKeyword(token.value)){
				token.type = "Keyword";
			}else{
				token.type = "Identifier";
			}
		}//for next character a digit, build the integer and store type as integer.
        else if(isdigit(ch)){
			token.value +=ch;
			while(1){
				if(readPtr != size){
					ch = codeString.at(readPtr++);
					if(isdigit(ch)){
						token.value +=ch;
					}else{
						readPtr--;
						break;
					}
				}else{
					break;
				}
			}
			token.type = "Integer";
		}//for next character a operator, store its type as operator.
        else if (isOperator(ch)){
			if(ch == '/' && codeString.at(readPtr++) == '/'){
				while(1){
                    // skip the comment line.
					ch = codeString.at(readPtr++);
					if(ch == '\n'){
						readPtr--;
						break;
					}else if(isalpha(ch) or isdigit(ch) or isOperator(ch) or isspace(ch) or ch=='\t'
							or ch=='\'' or ch == '(' or ch==')' or ch==';' or ch==',' or ch=='\\'){
						continue;
					}
				}
				continue;
			}else{
				if(ch == '/')
					readPtr--;
				token.value +=ch;
				while(1){
					if(readPtr != size){
						ch = codeString.at(readPtr++);
						if(isOperator(ch)){
							token.value +=ch;
						}else{
							readPtr--;
							break;
						}
					}else{
						break;
					}
				}
				token.type = "Operator";
			}
		}else if(ch == '(' or ch == ')' or ch == ';' or ch == ','){
			token.type = ch;
			token.value = ch;
        }else if(ch == '\''){
            token.value += ch;
            while(1){
                ch = codeString.at(readPtr++);
                if(ch == '\\'){
                    char ch1 = codeString.at(readPtr++);
                    if(ch1 =='t' or ch1 == 'n' or ch1=='\\' or ch1=='\''){
                        token.value += ch;
                        token.value += ch1;
                    }else{
                        throw "Problem with creating <STRING> token";
                    }
                }else if(ch == '\''){
                    token.value +=ch;
                    token.type ="String";
                    return token;
                } else if(isalpha(ch) or isdigit(ch) or isOperator(ch) or ch==')' or ch=='(' or ch==';' or ch==','or isspace(ch)){
                    token.value +=ch;
                    }
                }
        }
		return token;
	}

}

Token Lexer::peekNextToken(){
	if(readPtr == size)
		throw "No more tokens";
	Token token = getNextToken();
	int count = token.value.size();
	while(count-- !=0){
		readPtr--;
	}
	return token;
}
std::unordered_set<std::string> tr1; // unordered set for checking for keyword.
std::unordered_set<std::string> tr2; //unordered set for finding the operator.

bool Lexer::isOperator(char ch){
    if(!createHash1){
        char operators[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
        
        for (int i=0 ; i< 26; ++i) {
            tr2.insert(string(1,operators[i]));
        }
        createHash1 = true;
        }
        std::unordered_set<std::string>::const_iterator got = tr2.find (string(1,ch));
    
        if ( got == tr2.end() )
            return false;
        else
            return true;
}


bool Lexer::isKeyword(std::string tokenValue){
   
    if(!createHash){
      std::string keywords[] = {"let","where","aug","or","not","gr","ge","ls","le","eq","ne","true","false","nil","dummy","in","fn"
        ,"within","and","rec","list"};
    
    for (int i=0 ; i< 21; ++i) {
        tr1.insert(keywords[i]);
        }
        createHash = true;
    }
    std::unordered_set<std::string>::const_iterator got = tr1.find (tokenValue);
    
    if ( got == tr1.end() )
        return false;
    else
        return true;
        }

void Lexer::operator=(const Lexer& laObj){
	this->codeString = laObj.codeString;
	this->size = laObj.size;
	this->readPtr = laObj.readPtr;
}

Lexer::Lexer(){

}
