//
//  GraphMatrix.h
//  数据结构-图
//
//  Created by 刘佳翊 on 2020/2/28.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef GraphMatrix_h
#define GraphMatrix_h
#include <vector>
#include "Graph.h"

//图顶点类的实现
template <typename Tv>
struct Vertex{//顶点对象（并未严格封装）
    Tv data;
    int inDegree, outDegree;//入度，出度
    VStatus status;//状态（三种）
    int dTime, fTime;//顶点被发现 和被访问的时刻
    int parent;//在遍历树中的父节点
    int priority;//在遍历树中的优先级（最短通路 ，极端跨边等）
    Vertex(Tv const & d):
    data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),dTime(-1),fTime(-1),priority(INT_MAX){}
};

//图边类的实现
template <typename Te>
struct  Edge{//边对象
    Te data;//数据
    int weight;//权重
    EStatus status;//类型
    Edge( Te const &d, int w)://构造新边
    data(d),weight(w),status(UNDETERMINED){}
};

template <typename Tv,typename Te>
class GraphMatrix: public Graph<Tv, Te>{
public:
    vector<Vertex<Tv>> V;//顶点集
    vector<vector<Edge<Te>*>> E;//边集
public:
    GraphMatrix(){n=e=0;}
    ~GraphMatrix(){//析构
        for (int j=0; j<n; j++) {
            for (int i=0; i<n; i++) {
                delete E[j][k];//清除所有动态申请的边记录
            }
        }
    }
     virtual VStatus status(Vertex<Tv> v){
         return v.status;
     }
     virtual int dTime(Vertex<Tv> v){
         return v.dTime;
     }
     virtual int fTime(Vertex<Tv> v){
         return v.fTime;
     }
     virtual int parent(Vertex<Tv> v){
         return v.parent;
     }
     virtual int priority(Vertex<Tv> v){
         return v.priority;
     }
     virtual bool exists(int i, int j){
         return (0<=i)&&(i<n)&&(0<=j)&&(j<n)&&E[i][j]!=NULL;
     }
     virtual int nextNbr(int i, int j){//j表示当前在i的所有邻接顶点中最近一次查找到的邻居
         while ((-i<j)&&!exists((i,--j)));//往前找直到找到下一邻居或超出范围
         return j;
     }
     virtual int firstNbr(int i){//寻找首个邻居
         return nextNbr(i, n)//假象存在下标为i的顶点
     }
     virtual void insert(Te const& edge, int w, int i, int j){//插入到(i,j)，权重为w
         if (exits(i,j)) return;//若两点间已有边，不进行更新
         E[i][j]=new Edge<Te>(edge,w);//创建新边
         e++;//更新边计数
         V[i].outDegree++;
         V[i].inDegree++;//更新入度和出度
     }
     virtual Te remove(int i, int j){//删除顶点i和j之间的
         Te eBak = edge(i, j);//备份边(i,j)的信息，作为返回值
         delete E[i][j];
         E[i][j]=NULL;//删除边(i,j)
         e--;//更新边计数
         V[i].outDegree--;
         V[j],inDegree--;//e更新出入度
         return eBak;
     }
     virtual int insert(Tv const & vertex){
         for (int j=0; j<n; j++) E[j].insert(NULL);n++;//1
         E.insert(vector<Edge<Te>*>(n,n,NULL));//23,上一步已经更新过n
         return V.insert(Vert<Tv>(vertex));//4
     }
}
#endif /* GraphMatrix_h */

