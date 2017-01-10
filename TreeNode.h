/*
 * TreeNode.h
 *
 *  Created on: 03/10/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include "Token.h"

class TreeNode {
public:
	TreeNode();
    TreeNode(Token token);
    //TreeNode::TreeNode(int data)
	virtual ~TreeNode();
	Token value;
	TreeNode* right;
	TreeNode* left;
	TreeNode* next;
};

#endif /* TREENODE_H_ */

