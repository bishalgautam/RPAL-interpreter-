/*
 * Stack.h
 *
 *  Created on: 03/10/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#ifndef STACK_H_
#define STACK_H_

#include "TreeNode.h"

class Stack {
public:
    Stack();
    virtual ~Stack();
    TreeNode* pop();
    void push(TreeNode* treeNode);
    void linkChild(TreeNode* treeNode);
    //Getter
    int getSize();
private:
    TreeNode* topPtr;
	int size;

};

#endif /* STACK_H_ */
