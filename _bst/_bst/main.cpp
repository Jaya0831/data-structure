//
//  main.cpp
//  _bst
//
//  Created by 刘佳翊 on 2020/3/12.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

# include <iostream>
# include "BinTree.h"
# include "BinNode.h"
using namespace std;
void visit(int x){
    printf("%d ",x);
}
//测试
int main(int argc, const char * argv[]) {
//BinNode<int>* bn=new BinNode<int>();
    BinTree<int>* myTree=new BinTree<int>();
    myTree->insertAsRoot(1);
    BinNodePosi(int) bn2=myTree->insertAsLC(myTree->root(),2);
    myTree->insertAsRC(myTree->root(),3);
    myTree->insertAsLC(bn2,4);
    BinNodePosi(int) bn5=myTree->insertAsRC(bn2,5);
    myTree->insertAsLC(bn5,6);
    printf("myTree->size():%d\n",myTree->size());
    printf("myTree->root()->data:%d\n",myTree->root()->data);
    printf("myTree->root()->height:%d\n",myTree->root()->height);
    printf("bn5->height:%d\n",bn5->height);
    myTree->root()->travLevel(visit);
    printf("\n");
    myTree->root()->travIn(visit);
    printf("\n");
    myTree->root()->travPre(visit);
    printf("\n");
    BinTree<int>* yourTree=new BinTree<int>();
    yourTree->insertAsRoot(7);
    yourTree->insertAsLC(yourTree->root(), 8);
    yourTree->insertAsRC(yourTree->root(), 9);
    myTree->attachAsRC(bn5, yourTree);
    myTree->root()->travLevel(visit);
    printf("\n");
    myTree->remove(bn5->rChild);
    myTree->root()->travLevel(visit);
    printf("\n");
    BinTree<int>* newTree=myTree->secede(bn2);
    newTree->root()->travLevel(visit);
    printf("\n");
    myTree->root()->travLevel(visit);
    printf("\n");
}
