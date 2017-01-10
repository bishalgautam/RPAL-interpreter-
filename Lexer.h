/*
 * Lexer.h
 *
 *  Created on: 02/28/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#include <string>
#include "Token.h"

#ifndef LEXER_H_
#define LEXER_H_

class Lexer {
	private:
		std::string codeString;
        bool createHash;
        bool createHash1;
        int size;
		int readPtr;
		bool isOperator(char ch);
		bool isKeyword(std::string tokenValue);

	public:
		Lexer(std::string codeString);
		Lexer();
        void operator=(const Lexer& laObj);
		virtual ~Lexer();
		Token getNextToken();
		Token peekNextToken();
};

#endif /* LEXER_H_ */
