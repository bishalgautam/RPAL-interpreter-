/*
 * CSEMachine.h
 *
 *  Created on:  04/20/2016
 *      Author:  Bishal Gautam 
 */

#ifndef CSEMACHINE_H_
#define CSEMACHINE_H_

#include <map>
#include <stack>
#include "Token.h"
#include "TreeNode.h"
#include <list>
#include <vector>
#include <queue>
#include <sstream>
#include <utility>

using namespace std;
typedef map<string,Token> envMap;
typedef pair<int,string> keyPair;
typedef pair<int,string> key_pair;

class CSEMachine {
public:
	CSEMachine();
	CSEMachine(TreeNode* input);
	virtual ~CSEMachine();
	void evaluateTree();
private:

	map<int, vector<Token> > deltaMap;
	map<keyPair,Token> paramMap;
	map<int,int> envMap;
	
	ostringstream oss;
	
	int deltaCounter;
	int currDeltaNum;
	int envCounter;
	int currEnv;
	
	void createControlStructures(TreeNode* root);
	void preOrderTraversal(TreeNode* root, vector<Token> &currentDelta);
	void processCurrentToken(Token &currToken, stack<Token> &controlStack, stack<Token> &executionStack);
	void printTuple(Token t);
	
	TreeNode* inputTree;
	queue<TreeNode*> pendingDeltaQueue;
	stack<int> evrnStack;
	Token applyOperator(Token firstToken, Token secondToken, Token currToken);
	string to_String(int intValue);
	vector<string> split(string inputString, char delimiter);
	string unescape(const string& s);
	
	bool notFunction(string value);
	bool isParamter(Token currToken);
	bool printCalled;
	
	
	
	
	
};


#endif /* CSEMACHINE_H_ */
