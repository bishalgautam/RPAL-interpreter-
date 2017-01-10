/*
 * TreeNode.cpp
 *
 *  Created on: 03/10/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#include "TreeNode.h"

TreeNode::TreeNode() {
	right = NULL;
	left = NULL;
}

TreeNode::~TreeNode() {
	if(right != NULL)
		delete right;
	if(left != NULL)
		delete left;
}

TreeNode::TreeNode(Token token){
	this->value = token;
	right = NULL;
	left = NULL;
}
