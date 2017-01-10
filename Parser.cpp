/*
 * Parser.cpp
 *
 *  Created on: 03/09/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */
#include <iostream>
#include "Parser.h"
#include "Token.h"
#include <stack>
 
using namespace std;

Parser::Parser(Lexer la){
	this->lexAn = la;
	this->moreTokens = true;
    this->nextmore = false;
}

Parser::~Parser(){

}

 string Parser:: Type[] = {"Identifier","String","Integer","Keyword", "Operator"};

void Parser::read(Token token){
	if(moreTokens == false)
		throw "Error";
	if(token.value != nextToken.value){
		//cout << "Expected "<<token.value << " but found" <<nextToken.value;
		exit(1);
	}
	if(token.type == Type[0] or token.type == Type[1]  or token.type == Type[2]){
		buildTree(token, 0);
	}
	try{
		nextToken = lexAn.getNextToken();
	}catch(const char* message){
		moreTokens = false;
		Token endToken("$$","$$");
		nextToken = endToken;
	}
}


void Parser::buildTree(Token token, int numofNodes){
	TreeNode* createdNode = new TreeNode;
	createdNode->value = token;
	if(numofNodes != 0){
		int i=0;
        while (i != numofNodes-1) {
            TreeNode* curr = stack.pop();
            stack.linkChild(curr);
            i++;
        }
        

		TreeNode* top = stack.pop();
		if(top != NULL)
			createdNode->left = top;
	}
	stack.push(createdNode);
}


void Parser::parse_E(){
	//cout<<"Starts :  parse_E()"<<endl;
	if(nextToken.value == "let"){
		Token keyTokenLet("let",Type[3]);
		read(keyTokenLet);
            parse_D();
		Token keyTokenIn("in",Type[3]);
		read(keyTokenIn);
            parse_E();
		Token treeNodeToken("let","let");
		buildTree(treeNodeToken,2);
	} else if(nextToken.value == "fn"){
		Token keyTokenfn("fn","fn");
		read(keyTokenfn);
		int n=0;
		do{
			parse_Vb();
			n++;
		}while(nextToken.type == Type[0] or nextToken.type == "(");
		Token opTokenDot(".",Type[4]);
		read(opTokenDot);
            parse_E();
		Token treeNodeToken("lambda","lambda");
		buildTree(treeNodeToken,n+1);
	} else{
		parse_Ew();
	}
	//cout<<"End :  parse_E()"<<endl;
    
}

void Parser::parse_Ew(){
	//cout<<"Starts : parse_Ew()"<<endl;
	parse_T();
	if(nextToken.value == "where"){
		Token keyTokenwhr("where",Type[3]);
		read(keyTokenwhr);
            parse_Dr();
		Token treeNodeToken("where","where");
		buildTree(treeNodeToken,2);
	}
	//cout<<"End :  parse_Ew()"<<endl;
}



void Parser::parse_T(){
	//cout<<"Starts : parse_T()"<<endl;
	parse_Ta();
	if(nextToken.value == ","){
		int n = 0;
		do{
			read(nextToken);
			parse_Ta();
			n++;
		}while(nextToken.value == ",");
		Token tauToken("tau","tau");
        tauToken.tauCount = n+1;
		buildTree(tauToken,n+1);
	}
	//cout<<"End :  parse_T()"<<endl;
}

void Parser::parse_Ta(){
	//cout<<"Starts : parseTa()"<<endl;
	parse_Tc();
	while(nextToken.value == "aug"){
		Token temp = nextToken;
		read(nextToken);
            parse_Tc();
		buildTree(temp,2);
	}
	//cout<<"End :  parse_Ta()"<<endl;
}

void Parser::parse_Tc(){
	//cout<<"Starts : parse_Tc()"<<endl;
	parse_B();
	if(nextToken.value == "->"){
		read(nextToken);
            parse_Tc();
		Token elseToken("|",Type[4]);
		read(elseToken);
            parse_Tc();
		Token nodeToken("->","->");
		buildTree(nodeToken,3);
	}
	//cout<<"End :  parse_Tc()"<<endl;
}

void Parser::parse_B(){
	//cout<<"Starts : parseB()"<<endl;
	parse_Bt();
	while(nextToken.value == "or"){
        nextToken.type = Parser::Type[4];
		Token temp = nextToken;
		read(nextToken);
            parse_Bt();
		buildTree(temp,2);
	}
	//cout<<"End :  parse_B()"<<endl;
}

void Parser::parse_Bt(){
	//cout<<"Starts : parse_Bt()"<<endl;
	parse_Bs();
	while(nextToken.value == "&"){
         nextToken.type = Parser::Type[4];
		Token temp = nextToken;
		read(nextToken);
            parse_Bs();
		buildTree(temp,2);
	}
	//cout<<"End : parse_Bt()"<<endl;
}

void Parser::parse_Bs(){
	//cout<<"Starts : parse_Bs()"<<endl;
	if(nextToken.value == "not"){
        nextToken.type = "not";
		Token temp = nextToken;
		read(nextToken);
            parse_Bp();
		buildTree(temp,1);
	}else{
            parse_Bp();
	}
	//cout<<"End : parse_Bs()"<<endl;
}

void Parser::parse_Bp(){
	//cout<<"Starts : parse_Bp()"<<endl;
	parse_A();
	if(nextToken.value=="gr" or nextToken.value == ">"){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken, "gr");
	}else if(nextToken.value == "ls" or nextToken.value == "<"){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken,"ls");
	}else if(nextToken.value =="ge" or nextToken.value == ">=" ){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken, "ge");
	}else if(nextToken.value =="le" or nextToken.value == "<=" ){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken, "le");
	}else if(nextToken.value =="eq" ){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken, "eq");
	}else if(nextToken.value =="ne"){
        nextToken.type = Parser::Type[4];
		parse_BpHelper(nextToken, "ne");
	}
	//cout<<"End : parse_Bp()"<<endl;
}

void Parser::parse_BpHelper(Token nextToken, string tokenValue){
	//cout<<"Starts : parse_BpHelper()"<<endl;
	read(nextToken);
        parse_A();
	Token token(tokenValue, Parser::Type[4]);
	buildTree(token,2);
	//cout<<"End : parse_BpHelper()"<<endl;
}

void Parser::parse_A(){
	//cout<<"Starts : parse_A()"<<endl;
	if(nextToken.value == "-"){
		Token optTokenNeg("-",Type[4]);
		read(optTokenNeg);
            parse_At();
		Token nodeToken("neg","neg");
		buildTree(nodeToken,1);
	}else if(nextToken.value == "+"){
		Token optTokenPos("+",Type[4]);
		read(optTokenPos);
            parse_At();
	}else{
            parse_At();
	}
	while(nextToken.value == "+" or nextToken.value == "-"){
		Token temp = nextToken;
		read(nextToken);
            parse_At();
		buildTree(temp,2);
	}
	//cout<<"End : parse_A()"<<endl;
}

void Parser::parse_At(){
	//cout<<"Starts : parse_At()"<<endl;
	parse_Af();
	while(nextToken.value == "*" or nextToken.value=="/"){
		Token temp = nextToken;
		read(nextToken);
		parse_Af();
		buildTree(temp,2);
	}
	//cout<<"End : parse_At()"<<endl;
}

void Parser::parse_Af(){
	//cout<<"Starts : parse_Af()"<<endl;
	parse_Ap();
	while(nextToken.value == "**"){
		Token temp = nextToken;
		read(nextToken);
		parse_Af();
		buildTree(temp,2);
	}
	//cout<<"End : parse_Af()"<<endl;
}

void Parser::parse_Ap(){
	//cout<<"Starts : parse_Ap()"<<endl;
	parse_R();
	while(nextToken.value == "@"){
		Token temp = nextToken;
		read(nextToken); //Read '@'
		if(nextToken.type != Type[0] )
			throw "Expected Identifier found in parseAp()";
		read(nextToken); //Read identifier
		parse_R();
		buildTree(temp,3);
	}
	//cout<<"End : parse_Ap()"<<endl;
}

void Parser::parse_R(){
	//cout<<"Starts :  parse_R()"<<endl;
	parse_Rn();
	while(nextToken.type == Type[0]  or nextToken.type == Type[1] or nextToken.type == Type[2] or
			nextToken.value == "true" or nextToken.value == "false" or nextToken.value == "nil" or
			nextToken.value == "(" or nextToken.value == "dummy"){
		parse_Rn();
		Token treeNodeToken("gamma","gamma");
		buildTree(treeNodeToken,2);
	}
	//cout<<"End : parse_R()"<<endl;
}

void Parser::parse_Rn(){
	//cout<<"Starts : parse_Rn()"<<endl;
	if(nextToken.type == Type[0]  or nextToken.type == Type[1] or nextToken.type == Type[2] ){
			read(nextToken);
	}else if(nextToken.value=="true"){
		parse_RHelper(nextToken, "true");
	}else if(nextToken.value == "false"){
		parse_RHelper(nextToken, "false");
	}else if(nextToken.value == "nil"){
		parse_RHelper(nextToken, "nil");
	}else if(nextToken.value == "dummy"){
		parse_RHelper(nextToken,"dummy");
	}else if(nextToken.value == "("){
		read(nextToken);
		parse_E();
		Token t(")",")");
		read(t);
	}
	//cout<<"End : parse_Rn()"<<endl;
}

void Parser::parse_RHelper(Token t, string value){
	//cout<<"Starts : parseRHelper()"<<endl;
	read(t);
	Token nodeToken(value,value);
	buildTree(nodeToken,0);
	//cout<<"End of parseRHelper()"<<endl;
}

void Parser::parse_D(){
	//cout<<"Starts : parseD()"<<endl;
	parse_Da();
	if(nextToken.value == "within"){
		read(nextToken);
		parse_D();
		Token nodeToken("within", "within");
		buildTree(nodeToken,2);
	}
	//cout<<"End of parseD()"<<endl;
}

void Parser::parse_Da(){
	//cout<<"Starts : parse_Da()"<<endl;
	parse_Dr();
	if(nextToken.value== "and"){
		int n = 1;
		Token temp = nextToken;
		while(nextToken.value == "and"){
			read(nextToken);
			parse_Dr();
			n++;
		}
		buildTree(temp,n);
	}
	//cout<<"End of parse_Da()"<<endl;
}

void Parser::parse_Dr(){
	//cout<<"Starts : parse_Dr()"<<endl;
	if(nextToken.value == "rec"){
		Token temp = nextToken;
		read(nextToken);
		parse_Db();
		buildTree(temp,1);
	}else{
		parse_Db();
	}
	//cout<<"End of parse_Dr()"<<endl;
}

void Parser::parse_Db(){
	//cout<<"Starts :  parse_Db()"<<endl;
	
	//if the next token is V1
	if(nextToken.value == "("){
		read(nextToken);
		parse_D();
		Token token(")",Type[4]);
		read(token);
	} else if(nextToken.type == Type[0] && (lexAn.peekNextToken().value == "," || lexAn.peekNextToken().value == "=")){
		// if nexttoken is '=' OR ',' =>  Vl '=' E
		parse_V1();
		Token token("=",Type[4]);
		read(token);
		parse_E();
		//cout<< "Return from E" << endl;
		Token nodeToken("=","=");
		buildTree(nodeToken,2);
        } else{   // '<ID>' Vb+ '=' E
		read(nextToken);
		int n = 1;
		parse_Vb();
		while(nextToken.type == Type[0]  or nextToken.value== "("){
			n++;
			parse_Vb();
		}
		Token token("=",Type[4]);
		read(token);
		parse_E();
		Token nodeToken("function_form", "function_form");
		buildTree(nodeToken, n+2);
	}
	//cout<<"End : parse_Db()"<<endl;
}

void Parser::parse_Vb(){
	//cout<<"Inside parseVb()"<<endl;
	if(nextToken.type == Type[0] ){
		read(nextToken);
	}else if(nextToken.value == "("){
		read(nextToken);
		if(nextToken.value == ")"){
			read(nextToken);
			Token nodeToken("()","()");
			buildTree(nodeToken,0);
		}else{
			parse_V1();
			Token t(")",")");
			read(t);
		}
	}
	//cout<<"End : parse_Vb()"<<endl;
}

void Parser::parse_V1(){
	//cout<<"Starts :  parse_V1()"<<endl;
	read(nextToken);
	int n =1;
	if(nextToken.value == ","){
		while(nextToken.value == ","){
			n++;
			read(nextToken); //Reading ,
			read(nextToken); //Reading ID
		}
		Token nodeToken(",",",");
		buildTree(nodeToken,n);
	}
	//cout<<"End : parseV1()"<<endl;
}


void Parser::printTree(){
	//cout<<"Stack size: "<<stack.getSize()<<endl;
	TreeNode* t = stack.pop();
    stack.push(t);
	preOrder(t,std::string(""));
}

//stack<TreeNode*> nodeStack;

void Parser::preOrder(TreeNode* node , std::string dots){
	processNode(node->value,dots);
	string dots1 = "." + dots;
	if(node->left != NULL)
		preOrder(node->left,dots1);
	if(node->right != NULL)
		preOrder(node->right,dots);
}
void Parser::processNode(Token token,std::string dots){
	if(token.type == Type[0]){
		cout <<dots<<"<ID:"<< token.value<<'>'<<endl;
	}else if(token.type == Type[2]){
		cout <<dots<<"<INT:"<< token.value<<'>'<<endl;
	}else if(token.type == Type[1]){
		cout << dots<<"<STR:"<<token.value<<'>'<<endl;
	}else if(token.value == "true" or token.value == "false" or token.value == "nil" or token.value== "dummy"){
		cout << dots<<'<'<<token.value<<'>'<<endl;
	}else{
		cout <<dots<<token.value<<endl;
	}
}
void Parser::parse(){
	nextToken = lexAn.getNextToken();
	//cout << nextToken.value<<endl;
	parse_E();
}
TreeNode* Parser::getTree(){
	return stack.pop();
}
