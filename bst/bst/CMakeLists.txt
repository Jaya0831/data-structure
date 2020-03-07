cmake_minimum_required(VERSION 3.15)
project(bst)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bst
        AVL.h
        BinNode.h
        BinTree.h
        BST.h
        Entry.h
        main.cpp Splay.h BTNode.h BTree.h RedBlack.h)
