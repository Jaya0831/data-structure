//
//  Graph.h
//  数据结构-图
//
//  Created by 刘佳翊 on 2020/2/28.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include <stack>
#include <queue>
using namespace std;

typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;
typedef enum {UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD} EStatus;
template <typename Tv, typename Te>
class Graph{
public:
    int n,e;//顶点和边的数量
    virtual VStatus status(Tv v)=0;
    virtual int dTime(Tv v)=0;
    virtual int fTime(Tv v)=0;
    virtual int parent(Tv v)=0;
    virtual int priority(Tv v)=0;
    virtual bool exists(int i, int j)=0;
    virtual int firstNbr(int i, int j)=0;
    virtual int NextNbr(int i, int j)=0;
    virtual void insert(Te const& edge, int w, int i, int j)=0;
    virtual Te remove(int i, int j)=0;
    virtual int insert(Tv const & vertex)=0;
private:
    void reset();
    void BFS(int v, int &clock);
    void bfs(int s);
    void DFS(int v, int & clock);
    void dfs(int s);
    
};
template<typename Tv, typename Te> void Graph<Tv, Te>::reset(){//所有顶点，边的辅助信息复位
    for (int i=0; i<n; i++) {//顶点
        status(i)=UNDISCOVERED;
        dTime(i)=fTime(i)=-1;
        parent(i)=-1;
        priority(i)=INT_MAX;
        for (int j=0; j<n; j++) {//d边
            if (exists(i, j)) {
                status(i,j)=UNDETERMINED;
            }
        }
    }
}
template<typename Tv, typename Te> void Graph<Tv, Te>::BFS(int v, int &clock){
    queue<int> Q;
        status(v)=DISCOVERED;
        Q.push(v);//初始化
        while (!Q.empty())
        {
            int v=Q.front();
            dTime(v)=++clock;//取出队首顶点v
            for (int i = firstNbr(v); i > -1; i=NextNbr(v, u))
            {
                if (status(i)==UNDISCOVERED)//顶点尚未被发现
                {
                    status(i)=DISCOVERED;
                    Q.push(i);//发现该顶点
                    status(v,i)=TREE;//???????????????????????
                    parent(i)=v;//引入树和父子关系
                }
                else
                {
                    status(v,u)=CROSS;//将(i,v)归类于跨边
                }
            }
            status(v)=VISITED;//返回的不是只读吗？？？？？？？？
        }
}        
template<typename Tv, typename Te> void Graph<Tv, Te>::bfs(int s){
    reset();
        int clock=0;
        int v=s;//初始化
        do{//注意遍历顶点，一旦遇到undiscovered的顶点
            if (status(v)==UNDISCOVERED)
            {
                BFS(v,clock);//就从该点出发进行一次BFS
            }
        }while(s!=(v=(++v%n)));//按序号访问，刚好从s->n->0->n-1
}
template<typename Tv, typename Te>void Graph<Tv, Te>::DFS(int v, int & clock){
    dTime(v)=++clock;//开始访问时间
    status(v)=DISCOVERED;
    for (int i = firstNbr(v); i > -1; i=NextNbr(v,i))
    {
        switch (status(i))
        {
           //无向图只可能是前两种情况，因为已经visited的节点不可能再被访问到
        case UNDISCOVERED:
            status(v,i)=TREE;
            parent(i)=i;
            DFS(i,clock);
            break;
        case DISCOVERED:
            status(v,i)=BACKWARD;//若i已经被发现但尚未访问完毕，状态设为回向边
            break;
        default://只有visited一种可能
            status(v,i)=dTime(v)<dTime(i)?FORWARD:CROSS;//如果是v更早被发现，为跨边，反之为前向边
            break;
        }
    }
    status(v)=VISITED;
    fTime(v)=++clock;
}
template<typename Tv, typename Te>void Graph<Tv, Te>::dfs(int s){
    reset();
    int clock=0;
    int v=s;
    do{
        if (status(v)==UNDISCOVERED)
        {
            DFS(v,clock);
        }
    }while ((v=(++v%n)!=s));
}
#endif /* Graph_h */
