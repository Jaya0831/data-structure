//
// Created by 刘佳翊 on 2020/3/3.
//

#ifndef BST_SPLAY_H
#define BST_SPLAY_H
#include "BST.h"
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lc))

template <typename T> inline void attachAsLChild(BinNodePosi(T) p, BinNodePosi(T) lc){
    p->lChild=lc;
    if(lc) lc->parent=p;
}
template <typename T> inline void attachAsRChild(BinNodePosi(T) p, BinNodePosi(T) rc){
    p->rChild=rc;
    if(rc) rc->parent=p;
}


template <typename T>
class Splay:public BST<T>{
protected:
    BinNodePosi(T) splay(BinNodePosi(T) v);//将v伸展至树根
public:
    BinNodePosi(T) & search(const T & e);
    BinNodePosi(T) insert(const T & e);
    bool remove(const T & e);
    //重写查找，插入，删除
};
template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNode<T> *v) {
    if(!v) return NULL;
    BinNodePosi(T) p;//父亲
    BinNodePosi(T) g;//祖父
    while ((p=v->parent)&&(g=p->parent)){//自上而下，反复双层伸展
        BinNodePosi(T) gg=g->parent;//每轮之后，v都将以原曾祖父为父
        if(IsLChild(*v)){
            if (IsLChild(*p)){//zig-zig
                attachAsLChild(g,p->rChild);
                attachAsLChild(p,v->rChild);
                attachAsRChild(p,g);
                attachAsRChild(v,p);
            }
            else{//zig-zag
                attachAsLChild(p,v->rChild);
                attachAsRChild(g,v->lChild);
                attachAsLChild(v,g);
                attachAsRChild(v,p);
            }
        }
        else{
            if (IsLChild(*p)){//zag-zig
                attachAsRChild(p,v->lChild);
                attachAsLChild(g,v->rChild);
                attachAsLChild(v,p);
                attachAsRChild(v,g);
            }
            else{//zag-zag
                attachAsRChild(g,p->lChild);
                attachAsRChild(p,v->lChild);
                attachAsLChild(p,g);
                attachAsLChild(v,p);
            }
        }
        if(!gg) v->parent=NULL;//若无曾祖父gg，则此时v是树根，否则gg之后应以v为左或右儿子
        else (g==gg->lChild)? attachAsLChild(gg,v):attachAsRChild(gg,v);
        updateHeight(g);updateHeight(p);updateHeight(v);
    }//双层伸展结束后，必有g==NULL，但p可能为空
    if(p=v->parent){//若p是根，还需额外再旋转一次
        v->parent=NULL;
        return v;//伸展完成，v抵达树根
    }
}
template  <typename T>
BinNodePosi(T) & Splay<T>::search(const T &e) {
    //调用标准BST内部接口定位目标节点
    BinNodePosi(T) p=searchIn(this->_root,e,this->_hot=NULL);
    //无论成功与否，最后被访问的节点都要伸展至根
    this->_root=splay(p?p:this->_hot);
    return  this->_root;//总是返回根结点：成功时是该节点，失败时是最后以个访问的节点
}
template <typename T>
BinNodePosi(T) Splay<T>::insert(const T &e) {
    if(!this->_root) {
        this->_size++;
        return this->_root = new BinNode<T>(e);
    }
    if(search(e)->data!=e) {
        this->_size++;
        BinNodePosi(T)t = this->_root;
        if (e < this->_hot) {
            t->parent = this->_root = new BinNode<T>(e, nullptr, t->lChild, t);
            if (t->lChild) {
                t->lChild->parent = this->_root;
                t->lChild = nullptr;
            }
        } else {
            t->parent = this->_root = new BinNode<T>(e, nullptr, t, t->rChild);
            if (t->rChild) {
                t->rChild->parent = this->_root;
                t->rChild = nullptr;
            }
        }
        updateHeightAbove(t);
        return this->_root;
    }
}
template <typename T>
bool Splay<T>::remove(const T &e) {
    if(!this->_root||(search(e)->data!=e)) return false;//空树或未找到结束
    BinNodePosi(T) w=this->_root;//待删除节点存在时，_root为待删除节点
    if(!(w->lChild)){//无左孩子时，直接删除即可
        this->_root=w->rChild;
        if(this->_root) this->_root->parent=nullptr;//若不是只有根结点
    }
    else if(!(w->rChild)){//无右孩子时，直接删除即可
        this->_root=w->lChild;
        if(this->_root) this->_root->parent=nullptr;
    }
    else{//有两个孩子
        BinNodePosi(T) lt=w->lChild;//缓存左孩子
        lt->parent=nullptr;
        this->_root->lChild=nullptr;//孤立出待删除节点（此时的根结点）
        this->_root=w->rChild;
        this->_root->parent= nullptr;//更换根结点，原来的右子树成为一棵单独的树
        this->_root=search(e);//搜索右子树，伸展w的直接后继
        //w的右子树均比w大，最后会停到右子树最小的节点上
        this->_root->lChild=lt;//因为是右子树中最小的一个，所以它的左子树为空
        lt->parent=this->_root;//连接上
    }
    delete w;
    this->_size--;
    if(this->_root) updateHeight(this->_root);
    return true;
}
#endif //BST_SPLAY_H
