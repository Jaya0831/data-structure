//
//  BinNode.h
//  bst
//
//  Created by 刘佳翊 on 2020/2/29.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef BinNode_h
#define BinNode_h
//BinNode类
#include <stack>
#include <queue>
using namespace std;

#define BinNodePosi(T) BinNode<T>*
template <typename T> struct BinNode{
    T data;
    BinNodePosi(T) parent, lChild, rChild;//d父亲，左右孩子
    int height;//高度
    BinNode():data(NULL),lChild(nullptr),rChild(nullptr),height(0){}
    BinNode(T e, BinNode* p= nullptr, BinNode* lChild= nullptr,BinNode* rChild= nullptr,int height=0):
    data(e),parent(p),lChild(lChild),rChild(rChild),height(height){}
    int size();//子树规模
    bool HasLChild(BinNodePosi(T));
    bool HasRChild(BinNodePosi(T));
    BinNodePosi(T) insertAsLC(T const &);//const & 引用参数在函数内作为常量不可变；作为左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &);//作为右孩子插入新节点
    BinNodePosi(T) succ();//（中序遍历一yi xia意义下）当前节点的直接后继
    template <typename VST> void travLevel(VST &);//子树层次遍历
    template <typename VST> void travPre(VST &);//子树的先序遍历
    template <typename VST> void travIn(VST &);//子树的中序遍历
    template <typename VST> void travPost(VST &);//子树的后序遍历
};
//BinNode接口
//
template <typename T> 
bool BinNode<T>::HasLChild(BinNodePosi(T) e){
    if (e->lChild) return true;
    else return false;
}
//
template <typename T> 
bool BinNode<T>::HasRChild(BinNodePosi(T) e){
    if (e->rChild) return true;
    else return false;
}
//
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const &e){
    return lChild=new BinNode(e,this);//this 父节点
    //???????????????????????
}
//
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const &e){
    return rChild=new BinNode(e,this);
}
//
template <typename T>
int BinNode<T>::size(){//后代的总数，即以其为根的子树的规模
    int s=1;//计入本身
    if (lChild) {
        s+=lChild->size();
    }
    if (rChild) {
        s+=rChild->size();
    }
    return s;
}
//先序遍历
//迭代实现1
//template <typename T,typename VST>
//void travPre_I1(BinNodePosi(T) x, VST & visit){
//    stack<BinNodePosi(T)> s;//辅助栈
//    if (x) {
//        s.push(x);//进栈
//    }
//    while (!s.empty()) {
//        x=s.pop();//弹出当前节点
//        visit(x->data);//访问
//        if (x->rChild) {
//            s.push(x->rChild);//右孩子先进后出
//        }
//        if (x->lChild) {
//            s.push(x->lChild);//左孩子先进先出
//        }
//    }
//}
//
//template <typename T>
//template <typename VST>
//void BinNode<T>::travPre(VST & visit){//？？？？？？？？？？？？？？
//    stack<BinNodePosi(T)> s;
//    if (this) {
//        s.push(this);
//    }
//    while (!s.empty()) {
//        this=s.pop();
//        visit(this->data);
//        if (this->rChild) {
//            s.push(this->rChild);
//        }
//        if (this->lChild) {
//            s.push(this->lChild);
//        }
//    }
//}

//迭代实现2
template <typename T,typename VST>
void visitAlongLeftBranch(BinNodePosi(T) x, VST & visit, stack <BinNodePosi(T)> &s){
    while (x) {
        visit(x->data);
        s.push(x->rChild);//右孩子（右子树）入栈（将来逆序出栈）
        x=x->lChild;//沿左侧链下行
    }
}
template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST & visit){
    stack<BinNodePosi(T)> s;
    while (true) {//以右子树为单位，逐披访问节点
        visitAlongLeftBranch(this, visit, s);//访问子树的左侧链。右子树入栈缓冲
        if (s.empty()) {
            break;
        }//栈空时退出
        this=s.pop();
    }
}

//中序遍历
template <typename T>
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &s){
    while (x) {
        s.push(x);
        x=x->lChild;
    }
}
template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST & visit){
    stack<BinNodePosi(T)> s;
    while (true) {
        goAlongLeftBranch(this, s);//从当前节点出发，逐批入栈
        if (s.empty()) {
            break;
        }//直至所有节点处理完成
        this=s.pop();//x的左子树或为空，或已遍历
        visit(this->data);
        this=this->rChild;
    }
}
//层次遍历
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST & visit){
    queue<BinNodePosi(T)> q;
    q.push(this);
    while (q) {
        BinNodePosi(T) x=q.pop();
        visit(x->data);
        if (x->lChild) {
            q.push(x->lChild);
        }
        if (x->rChild) {
            q.push(x->rChild);
        }
    }
}
//查找中序遍历时的直接后继
//进入右孩子，再沿左一直往下
template <typename T> 
BinNodePosi(T) BinNode<T>::succ(){
    BinNodePosi(T) s=this;//记录后继的临时变量
    if (HasRChild(*s))
    {
        s=s->rChild;
        while (HasLChild(*s)) s=s->lChild;
    }
    else{//否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”
        while (s->parent&&(s->parent->lc)!=s) s=s->parent;//当找到或发现没有后继时停止
        s=s->parent;
    }
    return s;
}


#endif /* BinNode_h */
