//
//  RedBlack.h
//  _bst
//
//  Created by 刘佳翊 on 2020/3/12.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef RedBlack_h
#define RedBlack_h
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lc))
#define FromParentTo(x) (IsRoot(x)?this->_root:(IsLChild(x)?(x).parent->lChild:(x).parent->rChild))//来自父亲的引用
#define BlackHeightUpdated(x) (\
(stature((x).lChild)==stature((x).rChild))&&\
((x).height==(((x).color==RB_RED)?stature((x).lChild):stature((x).lChild)+1))\
)//x节点是否处于平衡状态
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
template <typename T>
void RedBlack<T>::solveDoubleRed(BinNode<T> *x) {
    if(IsRoot(*x)){//若插入节点为根结点，直接改变颜色
        this->_root->color=RB_BLACK;
        this->_root->height++;
        return;
    }
    BinNodePosi(T) p=x->parent;
    if(p->color==RB_BLACK) return;//是黑节点时不会有吧双红缺陷直接返回
    //因为p为红，所以p必不可能为根结点，祖父g存在
    BinNodePosi(T) g=p->parent;
    BinNodePosi(T) u=(g->lChild==p)?g->rChild:g->lChild;//叔父节点
    if(u->color==RB_BLACK){//当叔父节点为黑（外部节点也为黑）（外部节点是实际存在的还是null？）
//        if(IsLChild(*x)){
//            if(IsLChild(*p)){
//                this->connect34(x,p,g,x->lChild,x->rChild,p->rChild,u);//旋转
//                p->color=RB_BLACK;
//                g->color=RB_RED;//改变颜色
//            }
//        }诶我啰嗦的代码...
          //1-1：p和x同侧，这时p是中间的节点，将变成黑的，两边的x和p是红的
          // 交换p和x颜色，3+4重构
          if(IsLChild(*x)==IsLChild(*p)) p->color=RB_BLACK;
          //1-2：p和x不同侧，这时x是中间的节点，将变成黑的，两边的p和g是红的
          //交换g和x颜色，3+4重构
          else x->color=RB_BLACK;
          //两种情况下g都将变成红色
          g->color=RB_RED;
          BinNodePosi(T) gg=g->parent;//可能为null
          BinNodePosi(T) r=FromParentTo(*g)=rotateAt(x);//这个宏定义真的妙！！！
          // FromParentTo（x）得到的是
            //x为根结点：红黑树的根结点引用：_root
            //x不为根结点：x的父节点的左孩子/右孩子（指向x）的引用
          r->parent=gg;//重连父节点
    }
    //2：叔父节点为红时，相当于发生上溢（B—树），只需改变颜色，但这种上溢可能会迭代，
     //让唯一的黑节点（在中间2或3位置）变红向上，左右剩下的部分各将一个节点(p,u)变成黑节点，相当于拆分
    else{
        p->color=RB_BLACK;p->height++;
        u->color=RB_BLACK;p->height++;
        if(!IsLChild(*g)) g->color=RB_RED;
        solveDoubleRed(g);
    }
}
template <typename T>
bool RedBlack<T>::remove(const T &e) {
    BinNodePosi(T)*  x=search(e);
    if(!x) return false;
    BinNodePosi(T) r=removeAt(x,this->_hot);//r为接替被删除节点位置的节点
    this->_size--;
    if(this->_size==0) return true;//如果被删除后树为空不用调整直接结束
    if(!this->_hot){//说明红黑树只剩r一个节点（根结点）
        this->_root->color=RB_BLACK;//根结点染黑
        updateHeight(this->_root);
        return true;
    }
    //1.1:相当于没有双黑缺陷的(a)
    //可不可以把x的颜色记录下来，下面的if语句等效于x为红，r我饿黑
    if(BlackHeightUpdated(this->_hot)){//看r的父节点是否处于平衡状态
        return true;
    }
    //1.2:没有双黑缺陷的(b)
    if(r->color==RB_RED){
        r->color=RB_BLACK;
        updateHeight(r);
        return true;
    }
    //2:其余为有双黑缺陷的情况
    solveDoubleBlack(r);
    return true;
}
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNode<T> *r) {
    BinNodePosi(T) p=r?r->parent:this->_hot;//????????没有太懂
    if(!p) return;//如果r为根结点直接返回
    BinNodePosi(T) s=(r==p->lChild)?p->rChild:p->lChild;//r的兄弟
    if(s->color==RB_BLACK){
        if((s->lChild)->color==RB_RED||(s->rChild)->color==RB_RED){//s有红孩子
            BinNodePosi(T) t= (s->lChild->color==RB_RED)?s ->lChild:s->rChild;
            Color originalColor=p->color;
            BinNodePosi(T) b=FromParentTo(*r)=this->rotateAt(t);//旋转，解释同插入节点相同操作
            //a和c节点（即b的左右孩子）染黑
            if(b->lChild){
                b->lChild->color=RB_BLACK;
                updateHeight(b->lChild);
            }
            if(b->rChild){
                b->rChild.color=RB_BLACK;
                updateHeight(b->rChild);
            }
            //b继承p的颜色
            b->color=originalColor;
            updateHeight(b);
        }
        else{//s没有红孩子
            s->color=RB_RED;
            updateHeight(s);
            //p为红
            if(p->color==RB_RED){
                p->color=RB_BLACK;//相当于p和s交换颜色，不用更新高度
            }
            else {
                updateHeight(p);//p保持黑，但高度--
                solveDoubleBlack(p);//一定迭代，因为p和它的两个孩子（x和s）都为黑，一定发生下溢
            }
        }
    }
    else{//s为红
        s->color=RB_BLACK;
        p->color=RB_RED;
        BinNodePosi(T) t=IsLChild(*s)?s->lChild:s->rChild;//取t与s同侧作为旋转的孙子节点
        this->_hot=p;//x的父亲作为_hot?????????????????????
        FromParentTo(*p)=this->rotateAt(t);
        solveDoubleBlack(r);//转化为BB-1或BB-2R
    }
}

#endif /* RedBlack_h */
