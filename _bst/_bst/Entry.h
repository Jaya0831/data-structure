//
//  Entry.h
//  _bst
//
//  Created by 刘佳翊 on 2020/3/12.
//  Copyright © 2020 刘佳翊. All rights reserved.
//

#ifndef Entry_h
#define Entry_h
template<typename K, typename V>
struct Entry{
    K key;//关键码
    V value;//数值
    Entry(K k=K(), V v=V()):key(k), value(v) {};//默认构造函数
    Entry(Entry<K,V> const &e):key(e,key), value(e.value){};//克隆
    //比较器，判等器（从此，不必严格区分词条及其所对应的关键码）

    bool operator < (Entry<K,V> const &e){return key < e.key;}//重载小于运算符
    bool operator > (Entry<K,V> const &e){return key > e.key;}//大于
    bool operator == (Entry<K,V> const &e){return key ==e.key;}//等于
    bool operator != (Entry<K,V> const &e){return key != e.key;}//不等
};

#endif /* Entry_h */
