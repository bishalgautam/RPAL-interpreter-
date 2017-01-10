/*
 * CSEMachine.cpp
 *
 *  Created on: 04/20/2016 
 *      Author: Bishal Gautam
 */

#include "CSEMachine.h"
#include "TreeNode.h"
#include "Parser.h"
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <utility>

//typedef pair<int,string> key_pair;

CSEMachine::CSEMachine() {

}

CSEMachine::~CSEMachine() {
	 
}
 
void CSEMachine::evaluateTree(){
	createControlStructures(this->inputTree);
	Token tokenEnv("env",envCounter);
	stack<Token> controlStack;
	stack<Token> executionStack;
	controlStack.push(tokenEnv);
	envMap[0] = -1;
	vector<Token> delta0 = deltaMap[0];
	for(int i=0;i<delta0.size();i++){
		controlStack.push(delta0[i]);
	}
	executionStack.push(tokenEnv);
	
	while(controlStack.size() != 1){
		Token currToken = controlStack.top();
		controlStack.pop();
		processCurrentToken(currToken,controlStack,executionStack);
		
	}
	if(printCalled == false)
		cout<<endl; 
	 
}

CSEMachine::CSEMachine(TreeNode* root){
	this->inputTree = root;
	this->deltaCounter = 0;
	this->currDeltaNum = 0;
	this->envCounter = 0;
	this->evrnStack.push(0);
	this->currEnv = 0;
	this->envMap = map<int,int>();
	this->printCalled = false;
}

void CSEMachine::processCurrentToken(Token &currToken,stack<Token> &controlStack, stack<Token> &executionStack){
	 
	if(currToken.type == Parser::Type[4]){
		Token tokenOne = executionStack.top();
		executionStack.pop();
		Token tokenTwo = executionStack.top();
		executionStack.pop();
		Token resultToken = applyOperator(tokenOne, tokenTwo, currToken);
		executionStack.push(resultToken);
		
	}else if(currToken.type =="not"){
		Token tokenOne = executionStack.top();
		executionStack.pop();
		if(tokenOne.value == "false"){
			executionStack.push(Token("true","true"));
		}else{
			executionStack.push(Token("false","false"));
		}
	 
	}else if(currToken.type == "neg"){
		Token tokenOne = executionStack.top();
		executionStack.pop();
		int paramVal = atoi(tokenOne.value.c_str());
		paramVal = -paramVal;
		Token newToken(to_String(paramVal), Parser::Type[2]);
		executionStack.push(newToken);
		
	}else if(currToken.type == "gamma"){
		Token topExeToken = executionStack.top();
		executionStack.pop();
		if(topExeToken.type == "lambdaClosure"){
			Token env("env",++envCounter);
			 
			envMap[envCounter] = topExeToken.lambdaEnv;
			evrnStack.push(envCounter);
			currEnv = envCounter;
			
			if(topExeToken.isTuple == true){

				string tuple = topExeToken.lambdaParam;
				vector<string> params = split(tuple,',');
				Token valueTuple = executionStack.top();
				 
				executionStack.pop();
				vector<Token> tupleVector = valueTuple.tuple;
				 unsigned int i=0;
					while(i < params.size()){
					if(params[i] != ""){
						pair<int,string> keyPair(envCounter,params[i].c_str());
						paramMap[keyPair] = tupleVector[i];
					}
					 i++;
				}
				 
			}else{
				 string paramName = topExeToken.lambdaParam;
				Token paramToken = executionStack.top();
				executionStack.pop();
				 
				pair<int,string> keyPair(envCounter,paramName);
				paramMap[keyPair] = paramToken;
				
				 
			}
			controlStack.push(env);
			executionStack.push(env);
			int lambdaNum = topExeToken.lambdaNum;
			vector<Token> delta = deltaMap[lambdaNum];
			unsigned int i =0;
			while( i< delta.size()){
				controlStack.push(delta[i]);
				i++;
			}
		}else if(topExeToken.type == "YSTAR"){
			Token nextToken = executionStack.top();
			executionStack.pop();
			nextToken.type ="eta";
			executionStack.push(nextToken);
			//cout <<endl;;
		}else if(topExeToken.type == "eta"){
			Token lambdaToken = topExeToken;
			lambdaToken.type = "lambdaClosure";
			executionStack.push(topExeToken);
			executionStack.push(lambdaToken);
			Token gammaToken("gamma","gamma");
			controlStack.push(gammaToken);
			controlStack.push(gammaToken);
			
		}else if(topExeToken.value == "Stern" || topExeToken.value == "stern"){
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(2,tokenValue.size()-3);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}else if(topExeToken.value == "Stem" || topExeToken.value == "stem"){
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(1,1);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}else if(topExeToken.value == "Conc" || topExeToken.value == "conc"){
			Token tokenOne = executionStack.top();
			executionStack.pop();
			Token tokenTwo = executionStack.top();
			executionStack.pop();
			 
			string concatValue = tokenOne.value.substr(1,tokenOne.value.size()-2)+tokenTwo.value.substr(1,tokenTwo.value.size()-2);
			concatValue = "'"+concatValue+"'";
			 
			Token newToken(concatValue,Parser::Type[1]);
			executionStack.push(newToken);
			 
			controlStack.pop();
		}else if(topExeToken.value == "ItoS" || topExeToken.value == "itos"){
			Token tokenOne = executionStack.top();
			executionStack.pop();
			tokenOne.type = Parser::Type[1];
			tokenOne.value = "'"+tokenOne.value+"'";
			executionStack.push(tokenOne);
			 
		}else if(topExeToken.value == "Print" || topExeToken.value == "print"){
			printCalled = true;
			 
			Token t = executionStack.top();
			executionStack.pop();
			if(t.isTuple == false){
				if(t.type== Parser::Type[1]){
					string tempStr =unescape(t.value.substr(1,t.value.size()-2));
					cout << tempStr;
					if(tempStr[tempStr.size()-1] == '\n')
						cout<<endl;
					 
				}else if(t.type == "lambdaClosure"){
					cout <<"[lambda closure: "<<t.lambdaParam<<": "<<t.lambdaNum<<"]";
				}else{
					 
					cout<<t.value;
				}
				Token dummyToken("dummy","dummy");
				executionStack.push(dummyToken);
			}else{
				vector<Token> tupleVector = t.tuple;
				 
				unsigned int i = 0;
				while( i < tupleVector.size()){
					if(i==0){
						cout<<"(";
					}else{
						cout<<", ";
					}
					if(tupleVector[i].type == Parser::Type[1]){
						cout<< unescape(tupleVector[i].value.substr(1,tupleVector[i].value.size()-2));
					}else if(tupleVector[i].isTuple == true ){
						cout<<"Inside else if"<<endl;
						vector<Token> innerTuple = tupleVector[i].tuple;
						cout << "Size" << innerTuple.size()<<endl;
						if(innerTuple.size() == 1){
							if(innerTuple[0].type == Parser::Type[1])
								cout<< unescape(innerTuple[0].value.substr(1,innerTuple[0].value.size()-2));
						}
					}else{
						cout << tupleVector[i].value;
					}
					if(i==tupleVector.size() -1){
						cout<<")";
					}
					i++;
				}
			}
			 
		}else if(topExeToken.value == "Isinteger"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type==Parser::Type[2] ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Istruthvalue"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.value=="true" || t.value=="false" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isstring"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type==Parser::Type[1] ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Istuple"){
			 
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.isTuple==true ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isdummy"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.value=="dummy" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isfunction"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type=="lambdaClosure" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Order"){
			 
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(Token(to_String(t.tuple.size()),Parser::Type[2]));
		}else if(topExeToken.value == "Null"){
			 
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push((t.value == "nil") ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.isTuple == true){
			Token t = executionStack.top();
			executionStack.pop();
			if(t.type == Parser::Type[2]){
				int indx = atoi(t.value.c_str());
				indx -=1;
				executionStack.push(topExeToken.tuple[indx]);
			}
		}
	}else if(currToken.type == Parser::Type[0]&& isParamter(currToken)){
		string varName = currToken.value;
		int temp = currEnv;
		 
		pair<int,string> keyPair(temp,varName);
		map<key_pair,Token>::iterator it = paramMap.find(keyPair);
		while(paramMap.end() == it && temp>=0){
			temp = envMap[temp];
			keyPair.first = temp;
			 
			it = paramMap.find(keyPair);
		}
		if(paramMap.end() != it){
			Token paramValToken = it->second;
			 
			executionStack.push(paramValToken);
		}
	}else if(currToken.type =="env"){
		Token topToken = executionStack.top();
		executionStack.pop();
		executionStack.pop();
		executionStack.push(topToken);
		evrnStack.pop();
		currEnv = evrnStack.top();
		
	}else if(currToken.type == "beta"){
		Token topToken = executionStack.top();
		executionStack.pop();
		vector<Token> delta;
		if(topToken.value == "true"){
			delta = deltaMap[currToken.betaIfDeltaNum];
		}else{
			delta = deltaMap[currToken.betaElseDeltaNum];
		}
		
		for(unsigned int i=0;i<delta.size();i++){
			controlStack.push(delta[i]);
		}
	}else if(currToken.value == "tau"){
		
		int tauCount = currToken.tauCount;

		string tuple="(";
		vector<Token> tupleVector;
		for(int i=0;i<tauCount;i++){
			Token t = executionStack.top();
			tupleVector.push_back(t);
			executionStack.pop();
			if(i == tauCount -1)
				tuple += t.value;
			else
				tuple += t.value +", ";
		}
		tuple +=")"; 
		Token newToken(tuple,"tuple");
		newToken.tuple = tupleVector;
		newToken.isTuple = true;
		executionStack.push(newToken);
	}else if(currToken.value == "nil"){
		currToken.isTuple = true;
		executionStack.push(currToken);
	}else if(currToken.value == "aug"){	 
		Token tuple = executionStack.top();
		executionStack.pop();
		Token toAdd = executionStack.top();
		executionStack.pop();
		if(tuple.value == "nil"){			 
			Token newToken(toAdd.value,"tuple");
			newToken.isTuple = true;
			newToken.tuple = vector<Token>();
			newToken.tuple.push_back(toAdd);
			executionStack.push(newToken);
		}else{
			tuple.tuple.push_back(toAdd);
			executionStack.push(tuple);
		}
	}else if(currToken.type == "lambdaClosure"){
		 
		currToken.lambdaEnv = currEnv;
		executionStack.push(currToken);
	}else{
		executionStack.push(currToken);
	}
}


Token CSEMachine::applyOperator(Token tokenOne, Token tokenTwo, Token currToken){
	string tokenVal = currToken.value;
	 
	if(tokenOne.type == Parser::Type[2]){
		int firstVal = atoi(tokenOne.value.c_str());
		int secondVal = atoi(tokenTwo.value.c_str());		 
		int resultVal = 0;
		if(tokenVal == "*"){
			resultVal = firstVal*secondVal;
			return Token(to_String(resultVal),tokenOne.type);
		}else if(tokenVal == "/"){
			resultVal = firstVal/secondVal;
			return Token(to_String(resultVal),tokenOne.type);
		}else if(tokenVal == "-"){
			resultVal = firstVal-secondVal;
			return Token(to_String(resultVal),tokenOne.type);
		}else if(tokenVal == "+"){
			resultVal = firstVal+secondVal;
			return Token(to_String(resultVal),tokenOne.type);
		}else if(tokenVal == "**"){
			resultVal = pow(firstVal,secondVal);
			return Token(to_String(resultVal),tokenOne.type);
		}else if(tokenVal == "gr"){
			return (firstVal > secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ls"){		 
			return (firstVal < secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ge"){
			return (firstVal >= secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "le"){
			return (firstVal <= secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "eq"){
			return (firstVal == secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ne"){
			return (firstVal != secondVal ? Token("true","true"):Token("false","false"));
		}
	}else if(tokenOne.type == Parser::Type[1]){ // String operators
		if(tokenVal == "ne"){
			return (tokenOne.value != tokenTwo.value ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "eq"){
			return (tokenOne.value == tokenTwo.value ? Token("true","true"):Token("false","false"));
		}
	}else if(tokenOne.type == "true" || tokenOne.type == "false"){ // Boolean operators	 
		if(tokenVal == "or"){ 
			return ((tokenOne.type=="true" || tokenTwo.type=="true")? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "&"){
			return ((tokenOne.type=="true" && tokenTwo.type=="true")? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "eq"){
			return ((tokenOne.type==tokenTwo.type)? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "ne"){
			return ((tokenOne.type != tokenTwo.type)? Token("true","true"): Token("false","false"));
		}
	}
	return Token("","");
}

void CSEMachine::createControlStructures(TreeNode* root){
	 
	pendingDeltaQueue.push(root);
	while(!pendingDeltaQueue.empty()){
		vector<Token> currentDelta;
		TreeNode* currStartNode = pendingDeltaQueue.front();
		pendingDeltaQueue.pop();
		preOrderTraversal(currStartNode, currentDelta);
		deltaMap[currDeltaNum++] = currentDelta;
	}

}

vector<string> CSEMachine::split(string inputString, char delimiter){
	vector<string> result;
	string::iterator it;
	string temp;
	for(it = inputString.begin();it < inputString.end();it++){
		if(*it == delimiter){
			result.push_back(temp);
			temp = string();
		}else{
			temp +=*it;
		}
	}
	return result;
}

void CSEMachine::preOrderTraversal(TreeNode* root, vector<Token> &currentDelta){
	if(root->value.type == "lambda"){
		if(root->left->value.value != ","){
			Token lambdaClosure("lambdaClosure",root->left->value.value, ++deltaCounter);
			currentDelta.push_back(lambdaClosure);
		}else{
			TreeNode* commaChild = root->left->left;
			string tuple;
			while(commaChild != NULL){
				tuple += commaChild->value.value + ",";
				commaChild = commaChild->right;
			}
			Token lambdaClosure("lambdaClosure",tuple, ++deltaCounter);
			lambdaClosure.isTuple = true;
			currentDelta.push_back(lambdaClosure);
		} 
		pendingDeltaQueue.push(root->left->right);
		if(root->right !=NULL)
					preOrderTraversal(root->right,currentDelta);
	}else if(root->value.value == "->"){
		Token betaToken("beta",deltaCounter+1,deltaCounter+2);
		currentDelta.push_back(betaToken);
		pendingDeltaQueue.push(root->left->right);
		pendingDeltaQueue.push(root->left->right->right);
		root->left->right->right = NULL;
		root->left->right = NULL;  
		deltaCounter +=2;
		if(root->left != NULL)
				preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
				preOrderTraversal(root->right,currentDelta);
	}else{
		currentDelta.push_back(root->value);
		if(root->left != NULL)
				preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
					preOrderTraversal(root->right,currentDelta);
	}

}


string CSEMachine::to_String(int intValue){
	ostringstream oss;
	oss<<intValue;
	return oss.str();
}

string CSEMachine::unescape(const string& s)
{
  string res;
  string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case '\\': c = '\\'; break;
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
       
      default:
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    res += c;
  }

  return res;
}

bool CSEMachine::isParamter(Token currToken){
	string varName = currToken.value;
	int temp = currEnv;
	pair<int,string> keyPair(temp,varName);
	map<key_pair,Token>::iterator iter = paramMap.find(keyPair);
	while(paramMap.end() == iter && temp>=0){
		temp = envMap[temp];
		keyPair.first = temp;
		//cout << endl; 
		iter = paramMap.find(keyPair);
	}
	if(paramMap.end() != iter){
		return true;
	}else{
		return false;
	}
}



