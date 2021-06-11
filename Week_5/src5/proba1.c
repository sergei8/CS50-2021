#include <stdlib.h>
#include <stdio.h>
typedef struct BiNode{
    int data;
    struct BiNode *lchild;
    struct BiNode *rchild;    //left and right child pointer
}BiNode;
int CreateBiTree(BiNode **T);
char tmp = 'C';

int main() {
    BiNode *t; 
    CreateBiTree(&t);
    return 0;
}

int CreateBiTree(BiNode **T) {          //create a binary tree by preorder traversal
    if(tmp == 'C') {
        tmp = 'A';
        *T = (BiNode *)malloc(sizeof(BiNode));
        T[0]-> data = tmp;
        CreateBiTree(&(T[0]->lchild));
        CreateBiTree(&(T[0]->rchild));
    }        
    else  T[0] = NULL;
    return 1;
}