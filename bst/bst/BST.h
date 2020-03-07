//
//  BST.h
//  bst
//
//  Created by 刘佳翊 on 2020/2/29.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef BST_h
#define BST_h
#include "BinNode.h"
#include "BinTree.h"

template <typename T> class BST: public BinTree<T>{//由BinTree派生
public://以virtual修饰，一便派生类重写
    virtual BinNodePosi(T) & search(const T &);//查找，&：左值引用限定符，指示函数只能被左值对象调用。
    virtual BinNodePosi(T) insert(const T &);//插入
    virtual bool remove(const T &);//删除
protected:
    BinNodePosi(T) _hot;//命中节点的父亲
    BinNodePosi(T) connect34(//3+4重构
        BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T)
    );
    BinNodePosi(T) rotateAt(BinNodePosi(T));//旋转调整
};
//search
template <typename T> BinNodePosi(T) & BST<T>::search(const T & e){
    return searchIn(this->_root, e, _hot=NULL);
}
template <typename T> static BinNodePosi(T) & searchIn(BinNodePosi(T) & v, const T & e, BinNodePosi(T) & hot){
    //参数： 当前树根，目标关键码，记忆热点
    if(!v||(e==v->data)) return v;//足以确定失败，成功
    hot=v;//先记下当前（非空）节点
    return searchIn(((e<v->data)?v->lChild:v->rChild),e,hot);
}//运行时间正比于返回节点v的深度，不超过树高O(h)
//insert
template <typename T> BinNodePosi (T) BST<T>::insert (const T& e){
    BinNodePosi(T) & x= search (e);//查找目标值
    if (!x)//不含有相同元素
    {
        x=new BinNode<T>(e,_hot);//zaix处创建节点，并以hot为父节点
        this->_size++;
        updateHeightAbove(x);//更新全树规模，更新x及其祖先高度
    }
    return x;//无论原来树中是否由饿，此时x->data=e
}
template <typename T> bool BST<T>::remove(const T & e){
    BinNodePosi(T) & x =search(e);//定位目标
    if (!x) return false;//若不存在该元素返回false
    removeAt(x, _hot);//调用实际删除函数
    this->_size--;//更新全树的规模
    updateHeightAbove(_hot);//更新_hot及其历代祖先的高度
    return true;
}
template <typename T> 
static BinNodePosi(T)
removeAt(BinNodePosi(T) & x, BinNodePosi (T) & hot){
    BinNodePosi(T) w=x;//记录被摘除的节点，初值同x？？？？？？？？？？
    BinNodePosi(T) succ=NULL;//记录被删除的节点的接替者
    if (!HasLChild(*x)) succ=x=x->lChild;
    else if (!HasRChild((*x))) succ=x=x->rChild;
    else{
        //因为该节点有左右孩子，所以在这里找它的直接后继时只有可能是先找到右孩子再一路向左
            //如果x和它的直接后继是父子关系，说明后继是它的右孩子，那么这个时候就要替换掉后继父亲的rchild（就是后继本身）
            //如果x和它的直接后继不是父子关系，说明后继是后继父亲的左孩子，这时就要替换掉后继父亲的左孩子（就是后继本身）
        w=w->succ();swap(x->data,w->data);//令*x与其后继*w互换数据
        BinNodePosi(T) u=w->parent;//原问题转化为第二种情况
        (u==x?u->rChild:u->lChild)=succ=w->rchild;
        //如果x和它的直接后继是父子关系,u==x
        //如果x和它的直接后继不是父子关系,u!=x
        //而无论是哪一种情况，至多有一个右节点，所以不管左节点是否为空，都用rchild去代替被删除元素
    }
    hot=w->parent;//记录被删除节点的父亲
    if (succ) succ->parent=hot;//将被删除节点的接替者与hot相连
    release(w->data);
    release(w);//释放被删除节点
    return succ;//返回接替者
}
template <typename T>
BinNodePosi(T) BST<T>::connect34(
        BinNode<T> *a, BinNode<T> *b, BinNode<T> *c, BinNode<T> *T0, BinNode<T> *T1, BinNode<T> *T2,
                                 BinNode<T> *T3) {
    a->lChild=T0;
    if(T0) T0->parent=a;
    a->rChild=T1;
    if(T1) T1->parent=a;
    c->lChild=T2;
    if(T2) T2->parent=c;
    c->rChild=T3;
    if(T3) T3->parent=c;
    b->lChild=a;a->parent=b;
    b->rChild=c;c->parent=b;
    updateHeight(b);//因为a和c都不变（短的子树只可能更短，长的不变）
    return b;
}

#endif /* BST_h */

