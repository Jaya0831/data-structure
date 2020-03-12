//
//  BTree.h
//  _bst
//
//  Created by 刘佳翊 on 2020/3/12.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef BTree_h
#define BTree_h
#include "BTNode.h"
template <typename T>
class BTree{
protected:
    int _size;//关键码总数
    int _order;//阶次
    BTNodePosi(T) _root;//根
    BTNodePosi(T) _hot;//search最后访问的非空节点的位置
    void solveOverflow(BTNodePosi(T));//因插入而上溢后的分离处理
    void solveUnderflow(BTNodePosi(T));//因删除而下溢后的合并处理
public:
    BTNodePosi(T) search(const T &  e);//查找
    bool insert(const T & e);//插入
    bool remove(const T & e);//删除
};
template <typename T>
BTNodePosi(T)  BTree<T>::search(const T& e){
    BTNodePosi(T) v= _root;//从根结点出发查找
    _hot=NULL;
    while(v){//当v不为空时
        int r=v->key.search(e);//search返回值????????????
        if(0<=r&&e==v->key[r]) return v;//查找成功
        _hot=v;//记录当前节点
        v=v->child[r+1];//进入下一节点（孩子）
    }//v为空时退出，表示查找失败
    return NULL;
}
template <typename T>
bool BTree<T>::insert(const T & e){
    BTNodePosi(T)  v=search(e);//调用BTree的查找函数
    if(v) return false;//插入失败
    int r=_hot->Key.search(e);//调用vector的查找函数，确定插入位置
    _hot->key.insert(r+1,e);//插入节点
    _hot->child.insert(r+2,NULL);//在插入节点左边创建空子树指针
    _size++;//update
    solveOverflow(_hot);//如果上溢，需做分裂
    return true;
}
template <typename T>
bool BTree<T>::remove(const T & e){
    BTNodePosi(T) v=search(e);//调用BTree的查找函数,找到的是节点但还没有具体到节点里面的关键值
    if(!v) return false;//确认e存在
    int r=v->key.search(e);//调用vector的查找函数，确定删除的关键值在节点中的位置
    if(v->child[0]){//若v非叶子节点，则
        BTNodePosi(T) u=v->child[r+1];
        while (v->child[0]) u=u->child[0];//在右子树中一直向左，可找到其直接后继(必定是某叶节点）
        v->key[r]=u->key[0];//将后继的第一个关键码也就是待删除关键码的直接后继赋到待删除关键码的位置，
        // 从而只需删除后继的第一个关键码就相当于删除了待删除关键码
        v=u;
        r=0;
    }
    v->key.remove(r);v->child.remove(r+1);_size--;//其实删除任意一个孩子都可以，因为这些都是外部空节点
    solveUnderflow(v);//如果有必要，需做旋转和合并
    return true;
}
template <typename T>
void BTree<T>::solveOverflow(BTNode<T> *v) {//tips：_order表示最大孩子数
    if(v->child.size()<=_order) return;//未发生上溢
    //把关键码拆分两部分和中间的一个关键码，中间关键码进入父节点中，余下两个关键码成为两个节点作为中间关键码的左右子树
    int s=_order/2;//左右均分，左边>=右边
    BTNodePosi(T) u=new BTNode<T>();//新建一个节点，用来储存右半部分的关键码和孩子，作为右节点
    //转移数据
    for(int i=0;i<_order-s-i;i++) {
        u->key.insert(i, v->key.remove(s + 1));//将右半部分第一个关键码删除并放入新建节点中
        u->child.insert(i, v->child.remove(s + i));//将右半部分第一个孩子删除并放入新建节点中（这时转移的是关键码左边的孩子）
    }
    u->child[_order-s-1]=v->child.remove(s+1);
    if(u->child[0]){//若u的孩子非空（即不是外部空节点），更新孩子指向
        for(int i=0;i<(u->child.size());i++){
            (u->child[i])->parent=u;
        }
    }
    //插入父节点
    BTNodePosi(T) p=v->parent;
    if(!p){//v是根结点，没有父节点
        _root=p=new BTNode<T>();//新建根结点
        p->child[0]=v;
        v->parent=p;
    }
    int r=p->key.search(v->key[s]);//寻找插入点
    p->key.insert(r+1,v->key.remove[s]);//删除中间关键值并插入父节点
    p->child.insert(r+2,u);//u作为右节点插入
    u->parent=p;
    solveOverflow(p);//继续向上检查是否溢出
}
template <typename T>
void BTree<T>::solveUnderflow(BTNode<T> *v) {
    if(v->child.size()>=(_order+1)/2) return;//未发生下溢
    BTNodePosi(T) p=v->parent;//记录父节点
    if(!p){
        //v为根结点时
        //因为根结点只要孩子数量大于等于2就可以，所以此时发生下溢，就说明根结点只有一个孩子，无关键码、
        if((!v->key.size())&&(v->child[0])){//该根结点不含有效关键码，但有孩子（虽然我觉得这是一定的啊）
            _root=v->child[0];//唯一的孩子成为根结点
            _root->parent=nullptr;
            v->child[0]= nullptr;//不是你管它呢？？？反正都要删了...
            delete  v;
            v= nullptr;
        }
        return;
    }
    int r=0;
    while (p->child[r]!=v) r++;//找到v在节点关键值中的位置（其实可以用search()
    //旋转
    //左兄弟有足够多的关键值和孩子
    if(0<r){//说明有左兄弟
        BTNodePosi(T) ls=p->child[r-1];//左兄弟
        if(ls->child.size()>=((_order+1)/2)+1){//左兄弟有足够多的孩子
            v->key.insert(0,p->key[r-1]);//先从父节点的关键码中借用前一个关键码
            v->child.insert(0,ls->child.remove(ls->child.size()-1));//再从左兄弟的孩子中取最右一个作为自己的孩子
            p->key[r-1]=ls->key.remove(ls->key.size()-1);//左兄弟的最后一个关键码补给父节点
            if(v->child[0]) v->child[0]->parent=v;//如果转移的孩子不是外部空节点
            return;
        }
    }
    //右兄弟有足够多的关键值和孩子
    if(r<p->key.size()-1){
        BTNodePosi(T) rs=p->parent[r+1];
        if(rs->child.size()>=((_order+1)/2)+1){
            v->key.insert(v->key.size(),p->key[r]);
            v->child.insert(v->child.size(),rs->child.remove(0));
            p->key[r]=rs->key.remove(0);
            if(v->child[v->child.size()-1]) v->child[v->child.siza()-1]->parent=v;
            return;
        }
    }
    //合并
    //和其左右兄弟以及两兄弟间的父节点的关键码组成一个节点
    //相当于两兄弟拽了一个关键码下来
    if(r>0){//存在左兄弟
        BTNodePosi(T) ls=p->child[r-1];//左兄弟
        ls->key.insert(ls->key.size(),p->key.remove(r-1));//父节点的关键码被下拉进入左兄弟
        p->child.remove(r);//删除父节点指向v的孩子节点
        //开始转移，把v中的关键码和孩子转移到左兄弟中
        ls->child.insert(ls->child.size(),v->child.remove(0));//先转移v的最左孩子（因为孩子数量和关键码数量不匹配）
        if(ls->child[ls->child.size()-1]) (ls->child[ls->child.size()-1])->parent=ls;//如果转移的孩子非空，重置父母
        while(v->key.size()){//依次成对转移其它孩子和关键码
            ls->key.insert(ls->key.size(),v->key.remove(0));
            ls->child.insert(ls->child.size(),v->child.remove(0));
            if(ls->child[ls->child.size()-1]) ls->child[ls->child.size()-1]->parent=ls;
        }
        delete v;
        v= nullptr;
    }
    else{//存在右兄弟
        BTNodePosi(T) rs=p->child[r+1];//右兄弟
        rs->key.insert(0,p->key.remove(r));
        p->child.remove(r);
        rs->child.insert(0,v->child.remove(v->child.size()-1));
        if(rs->child[0]) (rs->child[0])->parent=rs;
        while (v->key.size()){
            rs->key.insert(0,v->key.remove(v->key.size()-1));
            rs->child.insert(0,v->child.remove(v->child.size()-1));
            if(rs->child[0]) (rs->child[0])->parent=rs;
        }
        delete v;
        v= nullptr;
    }
}

#endif /* BTree_h */
