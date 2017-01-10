/*
 * standardizer.h
 *
 *  Created on: 04/12/2016
 *      Author: Bishal Gautam 
 */

#ifndef STANDARDIZER_H_
#define STANDARDIZER_H_

#include "TreeNode.h"

class standardizer {
private:
	TreeNode* newNode(TreeNode* key);
public:
	Token lambda;
	Token gamma;
    bool flag;
	standardizer();
	virtual ~standardizer();
	void printNodes(TreeNode* node);
	TreeNode* transAT(TreeNode* node);
	TreeNode* transFCN_FORM(TreeNode* node);
	TreeNode* transLAMDA(TreeNode* node);
	TreeNode* transWITHIN(TreeNode* node);
	TreeNode* transAND(TreeNode* node);
	TreeNode* transREC(TreeNode* node);
	TreeNode* transTree(TreeNode* node);
	TreeNode* transLET(TreeNode* node);
	TreeNode* transWHERE(TreeNode* node);
	// TreeNode* transform(TreeNode* node);
	// TreeNode* trans___(TreeNode* node);
	
};


#endif /* STANDARDIZER_H_ */
