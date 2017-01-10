/*
 * standardizer.cpp
 *
 *  Created on: 04/12/2016
 *      Author: Bishal Gautam 
 */


#include "standardizer.h"
#include "Stack.h"
#include <iostream>
#include <queue>

using namespace std;

standardizer::standardizer() {
	
	this->gamma = Token("gamma","gamma");
	this->lambda = Token("lambda","lambda");
    this->flag = true;
}

TreeNode* standardizer::transTree(TreeNode* root){
	TreeNode* top = root;
    TreeNode* nextChild = top->left;
	bool setfirstChild = false;
	
	while(nextChild != NULL){
		TreeNode* transformedChild = transTree(nextChild);
		
		if(setfirstChild == false){
			top->left = transformedChild;
			top = top->left;
			setfirstChild = true;
		}else{
			top->right = transformedChild;
			top = top->right;
		}
		nextChild = nextChild->right;
	}
	Token rootToken = root->value;
	
	//  enum{
    //     let,
    //     where,
    //     function_form,
	// 	within,
	// 	and,
	// 	@,
	// 	lambda,
	// 	rec
		
    //     } tokenValue;
		
	// 	switch(rootToken.value){
    //     case 0 :
    //         root = transLET(root);
    //         break;
    //     case 1 :
    //         root = transWHERE(root);
    //         break;
    //     case 2 :
    //         root = transFCN_FORM(root);
    //         break;
	// 	case 3 :
    //         root = transWITHIN(root);
    //         break;
    //     case 4 :
    //         root = transAND(root);
    //         break;
    //     case 5 :
    //         root = transAT(root);
    //         break;
	// 	case 6 :
    //         root = transLAMDA(root);
    //         break;
	// 	case 7 :
	// 		root = transREC(root);
	// 	    break;		
    //     default :
    //         cout << "No such name!" << endl;
    //     }

		
	if(rootToken.value=="let"){
		root = transLET(root);
	}else if(rootToken.value =="where"){
		root = transWHERE(root);
	}else if(rootToken.value == "function_form"){
		root = transFCN_FORM(root);
	}else if(rootToken.value == "within"){
		root = transWITHIN(root);
	}else if(rootToken.value == "and"){
		root = transAND(root);
	}else if(rootToken.value == "@"){
		root = transAT(root);
	}else if(rootToken.value == "lambda"){
		root = transLAMDA(root);
	}else if(rootToken.value == "rec"){
		root = transREC(root);
	}
	return root;
}



TreeNode* standardizer::transLET(TreeNode* letNode){
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	gammaNode->left = lambdaNode;
	lambdaNode->right = letNode->left->left->right;
	lambdaNode->left = letNode->left->left;
	lambdaNode->left->right = letNode->left->right;
	return gammaNode;
}


TreeNode* standardizer::transWHERE(TreeNode* whereNode){
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	gammaNode->left = lambdaNode;
	gammaNode->left->right = whereNode->left->right->left->right;
	gammaNode->left->left = whereNode->left->right->left;
	gammaNode->left->left->right = whereNode->left;
	gammaNode->left->left->right->right = NULL;
	return gammaNode;
}


TreeNode* standardizer::transWITHIN(TreeNode* withinNode){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	gammaNode->left = lambdaNode;
	lambdaNode->right = withinNode->left->left->right; // gamma node
	lambdaNode->left = withinNode->left->left;  //E2
	lambdaNode->left->right = withinNode->left->right->left->right;  // X node 
	equalNode->left = withinNode->left->right->left;  
	equalNode->left->right = gammaNode;
	return equalNode;
}


TreeNode* standardizer::transFCN_FORM(TreeNode* fcnNode){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	int count = 0;
	Stack stack;
	stack.push(fcnNode->left);  
	TreeNode* childNode = fcnNode->left->right; 
	
	while(childNode != NULL){
		count++;
		stack.push(childNode);
		childNode = childNode->right;
	}
	 
	 count = count-1;

	for (int i =count ; i != 0 ; i--){
	TreeNode* firstNode = stack.pop();
	TreeNode* secondNode = stack.pop();
	secondNode->right = firstNode;
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	lambdaNode->left = secondNode;
	stack.push(lambdaNode);
	}
	TreeNode* firstNode = stack.pop();
	TreeNode* secondNode = stack.pop();
	
	secondNode->right = firstNode;
	equalNode->left = secondNode;
	return equalNode;
}


TreeNode* standardizer::transAND(TreeNode* nodeAND){
	Token equalToken("=","=");
	Token commaToken(",",",");
	Token tauToken("tau","tau");
	TreeNode* tauNode = new TreeNode();
	TreeNode* equalNode = new TreeNode(equalToken);
	TreeNode* commaNode = new TreeNode(commaToken);
	equalNode->left = commaNode;
	commaNode->right = tauNode;
	TreeNode* andChild = nodeAND->left;  
	queue<TreeNode*> equalQueue;
	
	int tauCount = 0;
	while(andChild != NULL){
		equalQueue.push(andChild);
		andChild = andChild->right;
		tauCount++;
	}
	
	tauToken.tauCount = tauCount;
	tauNode->value = tauToken;
	TreeNode* currParam = NULL;
	TreeNode* currValue = NULL;
	
	while(!equalQueue.empty()){
		TreeNode* eqNode = equalQueue.front();
		equalQueue.pop();
		TreeNode* param = eqNode->left;
		TreeNode* value = eqNode->left->right;
		param->right = NULL;
		if(currParam == NULL){
			currParam = param;
			currValue = value;
			commaNode->left = currParam;
			tauNode->left = currValue;
		}else{
			currParam->right = param;
			currValue->right = value;
			currParam = param;
			currValue = value;
		}
	}
	return equalNode;
}

TreeNode* standardizer::transAT(TreeNode* root){
	TreeNode* gammaNode1 = new TreeNode(this->gamma);
	TreeNode* gammaNode2 = new TreeNode(this->gamma);
	gammaNode1->left = gammaNode2;// gamma node
	TreeNode* e2 = root->left->right->right;
	TreeNode* n = root->left->right;
	n->right = NULL;
	TreeNode* e1  =root->left;
	e1->right = NULL;
	gammaNode2->left = n;
	n->right = e1;
	gammaNode2->right = e2;
	return gammaNode1;
}

TreeNode* standardizer::transLAMDA(TreeNode* root){

	if(root->left->right->right == NULL)
		return root;
	Stack stack;
	TreeNode* childNode = root->left;
	while(childNode != NULL){
		stack.push(childNode);
		TreeNode* nextChild = childNode->right;
		childNode->right = NULL;
		childNode = nextChild;
	}
	while(stack.getSize() != 1){
		TreeNode* rightChild = stack.pop();
		TreeNode* leftChild = stack.pop();
		TreeNode* lambdaNode = new TreeNode(this->lambda);
		lambdaNode->left = leftChild;
		leftChild->right = rightChild;
		stack.push(lambdaNode);
	}
	return stack.pop();
}

TreeNode* standardizer::transREC(TreeNode* root){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	Token ystarToken("YSTAR","YSTAR");
	TreeNode* ystarNode = new TreeNode(ystarToken);
	TreeNode* E = root->left->left->right;
	TreeNode* X1 = root->left->left;
	X1->right = NULL;
	TreeNode* X2 = new TreeNode(X1->value);
	equalNode->left = X1;
	X1->right = gammaNode;
	gammaNode->left = ystarNode;// Y node
	ystarNode->right= lambdaNode;
	lambdaNode->left = X2;
	X2->right = E;
	return equalNode;

}

TreeNode* standardizer::newNode(TreeNode* temp){
	return new TreeNode(temp->value);
}

void standardizer::printNodes(TreeNode* node){
	cout<<node->value.value<<endl;
	if(node->left !=NULL)
		printNodes(node->left);
	if(node->right !=NULL)
		printNodes(node->right);
}

standardizer::~standardizer() { 
	
}