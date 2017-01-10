/*
 * Parser.h
 *
 *  Created on: 03/09/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */
#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Token.h"
#include "TreeNode.h"
#include "Lexer.h"
#include "Stack.h"
#include <cstdlib>

using namespace std;

class Parser
{
	private:
       
		Lexer lexAn;
		bool moreTokens;
        bool nextmore;
		Stack stack;
		Token nextToken;
        //TreeNode* newNode();
        //int count;
		void processNode(Token token,std::string dots);
		void preOrder(TreeNode* t, std::string dots);
		void read(Token token);
		void buildTree(Token token, int numofNodes);
		void parse_E();
		void parse_Ew();
		void parse_T();
		void parse_Ta();
		void parse_Tc();
		void parse_B();
		void parse_Bt();
		void parse_Bs();
		void parse_Bp();
		void parse_A();
		void parse_At();
		void parse_Af();
		void parse_Ap();
		void parse_R();
		void parse_Rn();
		void parse_D();
		void parse_Da();
		void parse_Dr();
		void parse_Db();
		void parse_Vb();
		void parse_V1();
		void parse_BpHelper(Token nextToken, string tokenValue);
		void parse_RHelper(Token t, string value);
	public:
        static string Type[];
		Parser(Lexer la);
		~Parser();
		void printTree();
		void parse();
        TreeNode* getTree();
};




#endif /*End of PARSER_H */
