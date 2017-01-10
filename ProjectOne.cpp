/*
 * ProjectOne.cpp
 *
 *  Created on: 03/11/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "ReadFile.h"
#include "Lexer.h"
#include "Parser.h"
#include "standardizer.h"
#include "TreeNode.h"
#include "CSEMachine.h"
using namespace std;

void preOrder(TreeNode* t, std::string dots);
void formattedPrint(Token t,std::string dots);
void computeResult(int argc,char** argv);

int main(int argc,char *argv[]) {
	if(argc < 3){
        computeResult(argc,argv);
		return 0;
	}
	int j;
	bool astFound = false;
	for(int j=0;j<argc;j++){
		if(strcmp(argv[j],"-ast") == 0){
			astFound = true;
			break;
		}
	}
	if(astFound == true){
		ReadFile fr(argv[argc-1]);
		string fileContent = fr.getFileContent();
		if(fileContent.size() == 0)
			return 0;
		Lexer lexAl(fileContent);
		Parser recpar(lexAl);
		recpar.parse();
		 
        
        TreeNode* root = recpar.getTree();
		//cout << root->value.type;
		preOrder(root, "");
		//cout<<"Preorrder traversal done"<<endl;
		standardizer standard;
		TreeNode* transformedRoot = standard.transTree(root);

		cout << "Tree transformation done "<<endl;
		//preOrder(transformedRoot,"");
		CSEMachine machine(transformedRoot);
		machine.evaluateTree();
		cout << "CSE machine evaluation done "<< endl;
		//delete root;
		//delete transformedRoot;


	}else{
		//cout<< "check ---------"<<endl;
	}
	return 0;
}

void computeResult(int argc,char** argv){
	//cout<< "Inside compute"<<endl;
	ReadFile fr(argv[argc-1]);
	string fileContent = fr.getFileContent();
	if(fileContent.size() == 0)
		return;
	Lexer lexAl(fileContent);
	Parser recpar(lexAl);
	recpar.parse();
	//cout<< "Parsing done"<<endl;
	TreeNode* root = recpar.getTree();
	standardizer standard;
	TreeNode* transformedRoot = standard.transTree(root);
	//cout<< "Transformation done"<<endl;
	CSEMachine machine(transformedRoot);
    //cout << "CSEMachine transformed"<<endl;
	machine.evaluateTree();
	//cout << "Evaluation done"<<endl;
}

void preOrder(TreeNode* t, std::string dots){
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void formattedPrint(Token t,std::string dots){
	if(t.type == Parser::Type[0]){
		cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}else if(t.type == Parser::Type[2]){
		cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}else if(t.type == Parser::Type[1]){
		cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}else if(t.value == "true" or t.value == "false" or t.value == "nil" or t.value== "dummy"){
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}else{
		cout <<dots<<t.value<<endl;
	}
}