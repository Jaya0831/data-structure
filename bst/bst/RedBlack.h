//
// Created by 刘佳翊 on 2020/3/6.
//

#ifndef BST_REDBLACK_H
#define BST_REDBLACK_H

#include "BST.h"
template <typename T>
class RedBlack:public BST<T>{
public:
    //BST::search()等其余接口可直接沿用
    BinNodePosi(T) insert(const T & e);//插入（重写）
    bool remove(const T & e);//删除（重写）
protected:
    void solveDoubleRed(BinNodePosi(T) x);//双红修正
    void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
    int updateHeight(BinNodePosi(T) x);//更新节点x的高度（黑深度）
};
template <typename T> int RedBlack<T>::updateHeight(BinNode<T> *x) {
    x->height=max(stature(x->lChild),stature(x->rChild));
    if(IsBlack(x)) x->height++;
    return x->height;//只计黑节点
}
template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T & e){
    BinNodePosi(T) &x=search(e);//确认目标节点是否存在
    if(x) return x;
    x=new BinNode<T>(e,this->_hot,NULL,NULL,-1);//创建新插入节点
    this->_size++;
    solveDoubleRed(x);//如有必要进行双红修正
    return x?x:this->_hot->parent;
}

#endif //BST_REDBLACK_H
