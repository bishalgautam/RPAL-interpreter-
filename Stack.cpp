/*
 * Stack.cpp
 *
 *  Created on: 03/10/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#include "Stack.h"
#include <iostream>

Stack::Stack() {
	topPtr = NULL;
	size = 0;
}

Stack::~Stack() {
	while(topPtr != NULL){
		TreeNode* currPtr = topPtr;
		topPtr = topPtr->next;
		delete currPtr;
	}
}

TreeNode* Stack::pop(){
	if(topPtr == NULL)
		return NULL;
	TreeNode* currPtr = topPtr;
	topPtr = topPtr->next;
	size--;
	return currPtr;
}


void Stack::push(TreeNode* treeNode){
	treeNode->next = topPtr;
	size++;
	topPtr = treeNode;
}

void Stack::linkChild(TreeNode* treeNode){
	//std::cout<< "Top element value "<<topPtr<<std::endl;
	topPtr->right = treeNode;
}


int Stack::getSize(){
	return this->size;
}
