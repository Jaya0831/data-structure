//
//  AVL.h
//  bst
//
//  Created by 刘佳翊 on 2020/2/29.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef AVL_h
#define AVL_h
#include "BST.h"
#define Balanced(x) (stature((x).lChild)==stature((x).rChild)) //理想平衡
#define BalFac(x) (stature((x).lChild)-stature((x).rChild)) //平衡因子
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lc))
#define AvlBalanced(x) ((-2<Balfac(x))&&(Balfac(x)<2)) //AVL平衡条件
template <typename T> 
class AVL: public BST<T>{
public ://BST::search()等接口，可直接沿用
    BinNodePosi(T) insert(const T &);//插入重写
    bool remove(const T &);//删除重写
    BinNodePosi(T) rotateAt(BinNodePosi(T));
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T&e){
    BinNodePosi(T) & x=search(e);//
    if(x) return x;//若存在e，忽略
    x=new BinNode<T>(e,this->_hot);//若不存在,创建节点插入
    this->_size++;//更新
    BinNodePosi(T) xx=x;
    //以下，从x的父亲出发，向上一次检查祖先是否平衡
    for(BinNodePosi(T) g=x->parent;g;g=g->parent){
        if(!AvlBalanced(*g)){
            FromParentTo(*g)=rotateAt(tallerChild(tallerChild(g)));
            break;//g恢复 平衡后，局部子树高度必然复原；其祖先也是，故不用更新高度，调整结束
        }
        else{
            updateHeight(g);//更新其高度（平衡性虽不变，高度却可能改变
        }
        return xx;
    }
}
template <typename T>
BinNodePosi(T) AVL<T>::rotateAt(BinNodePosi(T) v) {//传入的v是三个节点中孙子节点，返回跟节点
    BinNodePosi(T) p=v->parent;
    BinNodePosi(T) g=p->parent;//p是v的父亲，g是曾祖父
    if(IsLChild(*p)){//p是左孩子
        if(IsLChild(*v)){//v是左孩子
            //zig
            p->parent=g->parent;//向上连接
            return this->onnect34(v,p,g,v->lChild,v->rChild,p->rChild,g->rChild);
        }
        else {//v是右孩子
            //zag-zig
            v->parent = g->parent;
            return this->connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
        }
    }
    else{//p是右孩子
        if(IsLChild(*v)){//v是左孩子
            //zig-zag
            v->parent=g->parent;
            return this->connect34(g,v,p,g->lChild,v->lChild,v->rChild,p->rChild);
        }
        else{//v是右孩子
            //zag
            p->parent=g->parent;
            return this->connect34(g,p,v,g->lChild,p->lChild,v->lChild,v->rChild);
        }
    }
}
template <typename T>
bool AVL<T>::remove(const T & e){
    BinNodePosi(T) & x=search(e);
    if(!x) return false;
    removeAt(x,this->_hot);
    this->_size--;
    //以上为删除节点的常规操作
    //以下为失衡时的恢复平衡 操作
    for(BinNodePosi(T) g =this->_hot;g;g=g->parent){
        if (!AvlBalanced(*g))
        {
            g=FromParentTo(*g)=rotateAt(tallerChild(tallerChild(g)));
        }
        updateHeight(g);
    }
    return true;
}
#endif /* AVL_h */
