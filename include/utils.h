#ifndef BPLUSTREE_DATAGENERATE_H
#define BPLUSTREE_DATAGENERATE_H

#include "node.hpp"

#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdio>

double *generateRandomDoubuleNumber(int num, double start, double gap);

template<typename Tkey, typename Tdata>
bool serialization(char *filePath, Node<Tkey, Tdata> *pNode);

template<typename Tkey, typename Tdata>
bool unSerialization(char *filePath, Node<Tkey, Tdata> *pNode);


template<typename Tkey, typename Tdata>
bool serialization(char *filePath, Node<Tkey, Tdata> *pNode) {
    /*
     *
     */



    std::FILE *fp;
    fp = std::fopen(filePath, "w");
    if (fp == NULL) {
        return false;
    } else {
        //打开文件成功，写入数据
        bool isLeafNode = pNode->getIsLeaf();
        int degree = pNode->getDegree();
        int maxKeyNum = pNode->getMaxKeyNum();
        int currentKeyNum = pNode->getCurrentKeyNum();

        std::fwrite(&isLeafNode, sizeof(isLeafNode), 1, fp);
        std::fwrite(&degree, sizeof(degree), 1, fp);
        std::fwrite(&maxKeyNum, sizeof(maxKeyNum), 1, fp);
        std::fwrite(&currentKeyNum, sizeof(currentKeyNum), 1, fp);
        std::fwrite(pNode->getPKey(), sizeof(Tkey), pNode->getCurrentKeyNum(), fp);
        if (pNode->getIsLeaf() == true) {
            std::fwrite(pNode->getPData(), sizeof(Tdata), pNode->getCurrentKeyNum() + 1, fp);
        }
    }
    std::fclose(fp);
    return true;
}

template<typename Tkey, typename Tdata>
bool unSerialization(char *filePath, Node<Tkey, Tdata> *pNode) {
//    int *arr=new int[16];

    FILE *fp;
    fp = std::fopen(filePath, "r");
    if (fp == NULL) {
        return false;
    } else {
        //打开文件成功，写入数据

        bool isLeafNode;
        int degree;

        int maxKeyNum;
        int currentKeyNum;

        std::fread(&isLeafNode, sizeof(isLeafNode), 1, fp);
        std::fread(&degree, sizeof(degree), 1, fp);

        pNode = new Node<Tkey, Tdata>(isLeafNode, degree);


        std::fread(&maxKeyNum, sizeof(maxKeyNum), 1, fp);
        std::fread(&currentKeyNum, sizeof(currentKeyNum), 1, fp);
//        std::fread(pNode->getPKey(), sizeof(Tkey), pNode->getCurrentKeyNum(), fp);
//        if (pNode->getIsLeaf() == true) {
//            std::fread(pNode->getPData(), sizeof(Tdata), pNode->getCurrentKeyNum() + 1, fp);
//        }
        std::cout << isLeafNode << " " << degree << " " << maxKeyNum << " " << currentKeyNum;
        pNode->print();
    }

    std::fclose(fp);

    return true;
}

#endif
