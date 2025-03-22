#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <string>
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
void traverseTree(Node* node, int& depth, string pathSoFar, bool& result){
  if(node==nullptr){return;}
  if(node->left==nullptr && node->right==nullptr){
    if((int)pathSoFar.size()!=depth){
      result=false;
    }
    return;
  }

  traverseTree(node->left, depth, pathSoFar + "L", result);
  traverseTree(node->right, depth, pathSoFar +"R", result);
}

int pathLength(Node* node){
  if(node==nullptr){return 0;}
  
  int left = pathLength(node->left);
  int right = pathLength(node->right);
  return 1+max(left,right);
}


bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){
      return true;
    }
    int depth = (pathLength(root) - 1);
    bool result = true;
    string pathSoFar = "";

    traverseTree(root, depth, pathSoFar, result);

    return result;
}