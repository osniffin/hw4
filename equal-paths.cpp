#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int checkHeight(Node * root)
{
  if(!root) 
    return 0;
  
   int heightL = checkHeight(root->left);
   int heightR = checkHeight(root->right);
  

  return 1+ max(heightL, heightR);
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root==nullptr)
      return true;

    int leftLength = checkHeight(root->left);
    int rightLength = checkHeight(root->right);

    if((rightLength==1 &&leftLength==0)||(rightLength==0&&leftLength==1))
      return true;

    return leftLength==rightLength;
}


