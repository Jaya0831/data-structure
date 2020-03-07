//
//  BinTree.h
//  bst
//
//  Created by 刘佳翊 on 2020/2/29.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef BinTree_h
#define BinTree_h
//BinNode类
#include "BinNode.h"

//BinTree模版类
template <typename T> class BinTree{
protected:
    int _size;//规模
    BinNodePosi(T) _root;//根结点
    virtual int updateHeight(BinNodePosi(T) x);//更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);//更新x及其祖先的高度
public:
    int size() const{//const修饰类的成员函数，则该成员函数不能修改类中任何非const成员函数。一般写在函数的最后来修饰。也不能调用类中任何非const成员函数。
        return _size;
    }

    bool empty() const{
        return !_root;//判空
    }
    BinNodePosi(T) root() const{
        return _root;//树根
    }
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const &e);//插入节点作为x的右孩子
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x,T const &e);//插入节点作为x的左孩子
};
//更新节点x的高度实现
#define stature(p) ((p)?(p)->height:-1)
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x){
    return x->height=1+max(stature(x->lChild),stature(x->rChild));
}
//更新x及其祖先的高度实现
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x){
    while (x) {
        updateHeight(x);
        x=x->parent;
    }
}
//节点插入
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x,T const &e){
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rChild;
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x,T const &e){
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lChild;
}


#endif /* BinTree_h */
