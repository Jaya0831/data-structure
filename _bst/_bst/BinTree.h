//
//  BinTree.h
//  _bst
//
//  Created by 刘佳翊 on 2020/3/12.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef BinTree_h
#define BinTree_h
//BinNode类
#include "BinNode.h"
#include <vector>

//BinTree模版类
template <typename T> class BinTree{
protected:
    int _size;//规模
    BinNodePosi(T) _root;//根结点
    virtual int updateHeight(BinNodePosi(T) x);//更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);//更新x及其祖先的高度
public:
    BinTree():_size(0), _root(nullptr){}
    ~BinTree();

    int size() const{//const修饰类的成员函数，则该成员函数不能修改类中任何非const成员函数。一般写在函数的最后来修饰。也不能调用类中任何非const成员函数。
        return _size;
    }
    bool empty() const{
        return !_root;//判空
    }
    BinNodePosi(T) root() const{
        return _root;//树根
    }

    BinNodePosi(T) insertAsRoot(const T &e);//插入节点作为根结点✅
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const &e);//插入节点作为x的右孩子✅
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x,T const &e);//插入节点作为x的左孩子✅
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x,BinTree<T>* &bt);//插入子树作为x的右孩子✅
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x,BinTree<T>* &bt);//插入子树作为x的左孩子✅

    int removeAt(BinNodePosi(T) x);//remove的递归辅助函数
    int remove(BinNodePosi(T) x);//删除x和x的所有后代,返回删掉的节点个数✅
    BinTree<T>* secede(BinNodePosi(T) x);//将x和x的后代从二叉树中脱离，形成一个单独的二叉树✅
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
BinNodePosi(T) BinTree<T>::insertAsRoot(const T &e) {
    if(this->_root)
        return nullptr;
    this->_size++;
    this->_root=new BinNode<T>(e);
    return this->_root;
}
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

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNode<T> *x, BinTree<T>* &bt) {
    if(x->rChild)
        return nullptr;
    _size+=bt->size();
    x->rChild=bt->root();
    bt->_root->parent=x;
    updateHeightAbove(x->rChild);//为什么不从x开始向上更新？
    //我觉得没必要的操作👇
    bt->_root = nullptr;
    bt->_size=0;
    bt= nullptr;
    return x->rChild;
}
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNode<T> *x, BinTree<T>* &bt) {
    if(x->lChild)
        return nullptr;
    _size+=bt->size();
    x->lChild=bt->root();
    bt->_root->parent=x;
    updateHeightAbove(x->lChild);//为什么不从x开始向上更新？
    //我觉得没必要的操作👇
    bt->_root = nullptr;
    bt->_size=0;
    bt= nullptr;
    return x->lChild;
}
template <typename T>
int BinTree<T>::removeAt(BinNode<T> *x) {
    int n=0;//记录删除个数
    if(x== nullptr) return 0;
    //🐱啊！
    n=1+removeAt(x->lChild)+removeAt(x->rChild);
    delete x;
    return n;
}
template <typename T>
int BinTree<T>::remove(BinNode<T> *x) {
    if (!x) {
        return 0;
    }
    if(x!=_root) x->parent->lChild==x?x->parent->lChild= nullptr:x->parent->rChild= nullptr;
    else _root=nullptr;
    BinNodePosi(T) p=x->parent;
    int n=removeAt(x);
    updateHeightAbove(p);
    _size-=n;
    return n;
}
template <typename T>
BinTree<T>* BinTree<T>::secede(BinNode<T> *x) {
    //子树失联
    if(x!=_root) (x->parent->lChild==x)?(x->parent->lChild= nullptr):(x->parent->rChild= nullptr);
    else _root= nullptr;
    //更新高度
    updateHeightAbove(x->parent);
    //封装新树
    BinTree<T>* bt=new BinTree();
    bt->_root=x;
    x->parent=nullptr;
    bt->_size=x->size();
    return bt;
}

#endif /* BinTree_h */
