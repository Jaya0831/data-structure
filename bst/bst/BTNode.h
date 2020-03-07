//
// Created by 刘佳翊 on 2020/3/5.
//

#ifndef BST_BTNODE_H
#define BST_BTNODE_H

#include <vector>
using namespace std;

#define BTNodePosi(T) BTNode<T>*

template <typename T> struct BTNode{//B-树节点
    BTNodePosi(T) parent;//父节点
    vector<T> key;//数值向量
    vector< BTNodePosi(T) > child;//孩子向量（长度比数值向量多1
    BTNode(){
        parent=NULL;
        child.insert(0,NULL);
    }
    BTNode(T e,BTNodePosi(T) lc=NULL,BTNodePosi(T) rc=NULL){
        parent=NULL;//作为根结点，而且初始时
        key.insert(0,e);//仅一个关键码
        child.insert(0,lc);child.insert(1,rc);//两个孩子
        if(lc) lc->parent=this;
        if(rc) rc->parent=this;
    }
};
#endif //BST_BTNODE_H
