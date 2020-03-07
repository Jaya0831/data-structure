//
//  main.cpp
//  数据结构-数
//
//  Created by 刘佳翊 on 2020/2/18.
//  Copyright © 2020 刘佳翊. All rights reserved.
//
//接口
//    root() 根结点
//    parent() 父节点
//    firstChild() 长子
//    nextSibling() 兄弟
//    insert(i,e) 将e作为dii个孩子插入
//    remove(i) 删除第i个孩子及其后代
//    traverse() 遍历

//线性存储（二维数组：rank，data，parent）
//    空间性能：O(n)
//    时间性能：
//        parent():
//        root():O(n)或O(1)
//        firstChild():O(n)
//        nextSibling():O(n)

//线性存储改进（rank，data，parent，children(链表存所有孩子))

//长子+兄弟表示法
    //每个节点均设两个引用
        //纵：firstChild()
        //横：nextSibling()


//二叉树(binary tree)
    //同一节点的孩子和子树，均以左，右区分
        //IChild() ~ ISubtree()
        //rChild() ~ rSubtree()

//描述多叉树
    //长子兄弟表示法左旋45度
    //左子右兄弟
//BinNode类
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

#define BinNodePosi(T) BinNode<T>*
template <typename T> struct BinNode{
    BinNodePosi(T) parent, lChild, rChild;//d父亲，左右孩子
    T data;
    int height;//高度
    int size();//子树规模
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
