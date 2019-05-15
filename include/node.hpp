#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

#include <iostream>
#include <cstdio>


template<typename Tkey, typename Tdata>
class Node;


template<typename Tkey, typename Tdata>
class KeyUnit {
    //有一个指针为空
protected:
    Tkey key;
    Tdata data;
    Node<Tkey, Tdata> *pLeftNode;
    Node<Tkey, Tdata> *pRightNode;

public:
    Tkey getKey();

    Tdata getData();

    Node<Tkey, Tdata> *getLeftNode();

    Node<Tkey, Tdata> *getRightNode();

    bool setKey(Tkey key);

    bool setData(Tdata data);

    bool setLeftNode(Node<Tkey, Tdata> *pNode);

    bool setRightNode(Node<Tkey, Tdata> *pNode);

    void print();
};


template<typename Tkey, typename Tdata>
class Node {
    //节点的度为索引节点最多的子孩子个数
    //为了便于分裂合并，节点的key值pKey、data数据pData、children指针pChildren的数组大小均比实际值大1

    //右子树大于等于节点值
    //左子树小于节点值

    //分裂时左少右多

    //keyNum个数的范围[maxKeyNum/2 maxKeyNum]
public:
    bool isLeafNode;
    int maxKeyNum;
    int currentKeyNum;
    int degree;
    Tkey *pKey;
    Tdata *pData;
    Node<Tkey, Tdata> **pChildren;
    Node<Tkey, Tdata> *pParent;
    Node<Tkey, Tdata> *pNextLeafNode;
    Node<Tkey, Tdata> *pPreLeafNode;


public://构造函数与析构函数
    Node(bool isLeaf, int degree);

    Node(bool isLeaf, int degree, Tkey *pKeys, Tdata *pDatas, Node<Tkey, Tdata> **pNodes, int num);

    ~Node();

public://打印类信息
    void print();

public://get、set方法
    bool getIsLeaf();

    int getMaxKeyNum();

    int getCurrentKeyNum();

    int getDegree();

    Tkey *getPKey();

    Tdata *getPData();

    Node<Tkey, Tdata> **getPChildren();

    Node<Tkey, Tdata> *getPParent();

    Node<Tkey, Tdata> *getPNextLeafNode();

    Node<Tkey, Tdata> *getPPreLeafNode();

    bool setPParent(Node<Tkey, Tdata> *pParent);

    bool setPNext(Node<Tkey, Tdata> *pNext);

    bool setPPre(Node<Tkey, Tdata> *pPre);

public://主要功能

    /*
     * 查找相关功能
     */

    //判断key是否存在此节点中
    int getKeyIndex(Tkey key);

    //根据key获取子树指针
    Node<Tkey, Tdata> *getChildTreeByKey(Tkey key);

    //根据key获取孩子节点的index
    int getPChildrenIndexByKey(Tkey key);

    //获取父节点childrenIndex
    int getPChildrenIndexInParent();

    //找到节点的左兄弟节点
    Node<Tkey, Tdata> *getLeftBrotherNode();

    //找到节点的右兄弟节点
    Node<Tkey, Tdata> *getRightBrotherNode();

    /*
     * 更新相关功能
     */

    //根据index更新key
    bool updateKeyByIndex(int index, Tkey key);

    bool updateThePParentInChildrenNode(Node<Tkey, Tdata> *selfNode);

    /*
     * 插入相关功能
     */

    //在叶子节点插入key值与data
    bool insertInLeafNode(Tkey key, Tdata data);

    //索引节点插入key值与两个子树
    bool insertInIndexNode(Tkey key, Node<Tkey, Tdata> *pNewChildIndex, Node<Tkey, Tdata> *pNewChildIndex2);

    //判断是否需要进行节点分裂
    bool ifNeedToSplitNode();

    //分裂当前叶子节点。但不会将父节点值赋给分裂后节点。也不会将两个节点连起来。
    KeyUnit<Tkey, Tdata> *splitLeafNode();

    //分裂当前索引节点。但不会将父节点值赋给分裂后节点。也不会将两个节点连起来。
    KeyUnit<Tkey, Tdata> *splitIndexNode();

    /*
     * 删除相关功能
     */

    //在叶子节点中删除key值
    bool deleteInLeafNodeByKey(Tkey key);

    //在叶子节点中删除key值
    bool deleteInLeafNodeByKeyIndex(int keyIndex);

    //在索引节点中删除key值
    bool deleteInIndexNodeByKey(Tkey key, Node<Tkey, Tdata> *pRetainenNode);

    //在索引节点中删除key值
    bool deleteInIndexNodeByKeyIndex(int keyIndex, Node<Tkey, Tdata> *pRetainenNode);

    //判断节点是否需要合并或借
    bool ifNeedToMergeOrBorrow();

    //当前节点是否可以借给其他叶子节点数据
    bool ifCanLendKeyToOtherLeafNode();

    //当前节点是否可以借给其他索引节点数据
    bool ifCanLendKeyToOtherIndexNode();

    //叶子节点借出最小值
    KeyUnit<Tkey, Tdata> *lendTheMinKeyInLeafNode();

    //叶子节点借出最大值
    KeyUnit<Tkey, Tdata> *lendTheMaxKeyInLeafNode();

    //索引节点借出最小值
    KeyUnit<Tkey, Tdata> *lendTheMinKeyInIndexNode();

    //索引节点借出最大值
    KeyUnit<Tkey, Tdata> *lendTheMaxKeyInIndexNode();

    //当前节点是否可以和其他叶子节点合并
    bool ifCanMergeWithOtherLeafNode();

    //当前节点是否可以和其他索引节点合并
    bool ifCanMergeWithOtherIndexNode();

    //和左叶子节点合并
    bool mergeWithLeftBrotherLeafNode(Node<Tkey, Tdata> *pLeftBrotherNode);

    //和右叶子节点合并
    bool mergeWithRightBrotherLeafNode(Node<Tkey, Tdata> *pRightBrotherNode);

    //和左索引节点合并
    bool mergeWithLeftBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pLeftBrotherNode, Tkey parentValue);

    //和右索引节点合并
    bool mergeWithRightBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pRightBrotherNode, Tkey parentValue);
};


/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * implement of class
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


/*
 * implement of class KeyUnit
 */
template<typename Tkey, typename Tdata>
Tkey KeyUnit<Tkey, Tdata>::getKey() {
    return key;
}

template<typename Tkey, typename Tdata>
Tdata KeyUnit<Tkey, Tdata>::getData() {
    return data;
}


template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *KeyUnit<Tkey, Tdata>::getLeftNode() {
    return pLeftNode;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *KeyUnit<Tkey, Tdata>::getRightNode() {
    return pRightNode;
}

template<typename Tkey, typename Tdata>
bool KeyUnit<Tkey, Tdata>::setKey(Tkey key) {
    this->key = key;
    return true;
}

template<typename Tkey, typename Tdata>
bool KeyUnit<Tkey, Tdata>::setData(Tdata data) {
    this->data = data;
    return true;
}

template<typename Tkey, typename Tdata>
bool KeyUnit<Tkey, Tdata>::setLeftNode(Node<Tkey, Tdata> *pNode) {
    pLeftNode = pNode;
    return true;
}

template<typename Tkey, typename Tdata>
bool KeyUnit<Tkey, Tdata>::setRightNode(Node<Tkey, Tdata> *pNode) {
    pRightNode = pNode;
    return true;
}

template<typename Tkey, typename Tdata>
void KeyUnit<Tkey, Tdata>::print() {
    std::cout << "key: " << key << std::endl;
    std::cout << "data: " << data << std::endl;
    if (pLeftNode != NULL)
        pLeftNode->print();
    if (pRightNode != NULL)
        pRightNode->print();
}


/*
 *
 *
 *
 *
 *
 *
 * implement of class Node
 *
 *
 *
 *
 *
 */

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata>::Node(bool isLeaf, int degree) {
    this->isLeafNode = isLeaf;
    maxKeyNum = degree - 1;
    this->degree = degree;
    currentKeyNum = 0;
    pKey = new Tkey[degree];
    pParent = NULL;
    pNextLeafNode = NULL;
    pPreLeafNode = NULL;
    for (int i = 0; i < degree; ++i) {
        pKey[i] = -1;
    }

    if (this->isLeafNode == true) {
        pData = new Tdata[degree];
    } else {
        pChildren = new Node<Tkey, Tdata> *[degree + 1];
        for (int i = 0; i < degree + 1; ++i) {
            pChildren[i] = NULL;
        }
    }


}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata>::Node(bool isLeaf, int degree, Tkey *pKeys, Tdata *pDatas, Node<Tkey, Tdata> **pNodes, int num) {
    this->isLeafNode = isLeaf;
    maxKeyNum = degree - 1;
    this->degree = degree;
    currentKeyNum = num;
    pKey = new Tkey[degree];
    pParent = NULL;
    pNextLeafNode = NULL;
    pPreLeafNode = NULL;
    for (int i = 0; i < num; ++i) {
        pKey[i] = pKeys[i];
    }
    for (int i = num; i < degree; i++) {
        pKey[i] = -1;
    }

    if (this->isLeafNode == true) {
        pData = new Tdata[degree];
        for (int i = 0; i < num; ++i) {
            pData[i] = pDatas[i];
        }
    } else {
        pChildren = new Node<Tkey, Tdata> *[degree + 1];
        for (int i = 0; i < num + 1; ++i) {
            pChildren[i] = pNodes[i];
        }
    }


}


template<typename Tkey, typename Tdata>
Node<Tkey, Tdata>::~Node() {
//    std::cout << "调用析构函数..." << std::endl;
    delete[] pKey;
    if (isLeafNode == true) {
        delete[] pData;
    } else {
        delete[] pChildren;
    }


}

template<typename Tkey, typename Tdata>
void Node<Tkey, Tdata>::print() {
    std::cout << std::endl;
    std::cout << "是否是叶子节点:" << isLeafNode << std::endl;
    std::cout << "节点的度:" << degree << std::endl;
    std::cout << "最大key num:" << maxKeyNum << std::endl;
    std::cout << "当前key num:" << currentKeyNum << std::endl;

    std::cout << "key数组:" << pKey << std::endl;
    for (int i = 0; i < maxKeyNum + 1; ++i) {
        if (i == currentKeyNum)
            std::cout << "|";
        std::cout << pKey[i] << " ";
    }
    std::cout << std::endl;

    if (isLeafNode == false) {
        std::cout << "索引节点孩子:" << std::endl;
        for (int i = 0; i < maxKeyNum + 2; ++i) {
            if (i == currentKeyNum + 1)
                std::cout << "|";
            std::cout << pChildren[i] << " ";
        }
        std::cout << std::endl;
    }


    std::cout << "父节点:" << pParent << std::endl;

    if (isLeafNode == true) {
        std::cout << "叶子节点数据:" << std::endl;
        for (int i = 0; i < maxKeyNum + 1; ++i) {
            if (i == currentKeyNum)
                std::cout << "|";
            std::cout << pData[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getCurrentKeyNum() {
    return currentKeyNum;
}

template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getDegree(){
    return degree;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::getIsLeaf() {
    return isLeafNode;
}

template<typename Tkey, typename Tdata>
Tkey *Node<Tkey, Tdata>::getPKey() {
    return pKey;
}

template<typename Tkey, typename Tdata>
Tdata *Node<Tkey, Tdata>::getPData() {
    if (isLeafNode == false) {
//        错误处理
        std::cout << "isIndexNode getPData()" << std::endl;
    }
    return pData;
}

template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getMaxKeyNum() {
    return maxKeyNum;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> **Node<Tkey, Tdata>::getPChildren() {
    if (isLeafNode == true) {
        //错误处理
        std::cout << "isLeafNode getPChildren()" << std::endl;
    }
    return pChildren;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getPNextLeafNode() {
    return pNextLeafNode;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getPPreLeafNode() {
    return pPreLeafNode;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getPParent() {
    return pParent;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::setPParent(Node<Tkey, Tdata> *pParent) {
    this->pParent = pParent;
    return true;
}


template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::setPNext(Node<Tkey, Tdata> *pNext) {
    if (isLeafNode == false) {
        std::cout << "setPNext():node isn't leafNode" << std::endl;
    }
    this->pNextLeafNode = pNext;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::setPPre(Node<Tkey, Tdata> *pPre) {
    if (isLeafNode == false) {
        std::cout << "setPPre():node isn't leafNode" << std::endl;
    }
    this->pPreLeafNode = pPre;
    return true;
}


template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getKeyIndex(Tkey key) {
    for (int i = 0; i < currentKeyNum; ++i) {
        if (pKey[i] == key)
            return i;
    }
    return -1;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getChildTreeByKey(Tkey key) {
    if (currentKeyNum == 0) {
        std::cout << "getChildrenTreeByKey():currentKeyNum is 0" << std::endl;
        return NULL;
    }
    int indexOfChildrenTree = 0;
    for (; indexOfChildrenTree < currentKeyNum; ++indexOfChildrenTree) {
        if (pKey[indexOfChildrenTree] > key)
            return pChildren[indexOfChildrenTree];
    }
    return pChildren[currentKeyNum];
}

template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getPChildrenIndexByKey(Tkey key) {
    if (currentKeyNum == 0) {
        std::cout << "getChildrenIndexInPkeyByKey():currentKeyNum is 0" << std::endl;
        return -1;
    }
    int indexOfChildrenTree = 0;
    for (; indexOfChildrenTree < currentKeyNum; ++indexOfChildrenTree) {
        if (pKey[indexOfChildrenTree] > key)
            return indexOfChildrenTree;
    }
    return currentKeyNum;

}


template<typename Tkey, typename Tdata>
int Node<Tkey, Tdata>::getPChildrenIndexInParent() {
    if (pParent == NULL) {
        std::cout << "getIndexInParentPChildren():no parent" << std::endl;
        return -1;
    }
    return pParent->getPChildrenIndexByKey(pKey[0]);

}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getLeftBrotherNode() {
    if (pParent == NULL) {
        std::cout << "getLeftBrotherNode():no parent" << std::endl;
        return NULL;
    }
    int indexInParentPChildren = getPChildrenIndexInParent();
    if (indexInParentPChildren > 0) {
        //有左子树
        return pParent->getPChildren()[indexInParentPChildren - 1];
    } else {
        //无左子树
        return NULL;
    }

}


template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *Node<Tkey, Tdata>::getRightBrotherNode() {
    if (pParent == NULL) {
        std::cout << "getRightBrotherNode():no parent" << std::endl;
        return NULL;
    }
    int indexInParentPChildren = getPChildrenIndexInParent();
//    std::cout << "getRightBrotherNode():indexInParentPChildren:" << indexInParentPChildren << std::endl;
    if (indexInParentPChildren < currentKeyNum ) {
        //有右子树
//        pParent->getPChildren()[indexInParentPChildren + 1];
//            std::cout << "getRightBrotherNode():indexInParentPChildren:" << indexInParentPChildren << std::endl;
        return pParent->getPChildren()[indexInParentPChildren + 1];
    } else {
        //无右子树
        return NULL;
    }

}


template<typename Tkey, typename Tdata>
//f
bool Node<Tkey, Tdata>::insertInLeafNode(Tkey key, Tdata data) {
    if (currentKeyNum == maxKeyNum + 1) {
        //错误处理
        std::cout << "insertInLeafNode():currentKeyNum is " << currentKeyNum << ",can't insert" << std::endl;
        return false;
    }
    if (getKeyIndex(key) != -1) {
        std::cout << "insertInLeafNode():key is already exists " << ",can't insert" << std::endl;
        return false;
    }
    if (currentKeyNum == 0) {
        //当前无节点
        pKey[0] = key;
        pData[0] = data;
        currentKeyNum++;
    } else {
        int i;
        for (i = currentKeyNum - 1; i >= 0; --i) {
            if (pKey[i] < key)
                break;
            pKey[i + 1] = pKey[i];
            pData[i + 1] = pData[i];
        }
        pKey[i + 1] = key;
        pData[i + 1] = data;
        currentKeyNum++;
    }
    return true;
}

template<typename Tkey, typename Tdata>
//f
bool Node<Tkey, Tdata>::insertInIndexNode(Tkey key, Node<Tkey, Tdata> *pNewChildIndex,
                                          Node<Tkey, Tdata> *pNewChildIndex2) {
    if (currentKeyNum == maxKeyNum + 1) {
        //错误处理
        std::cout << "insertInLeafNode():currentKeyNum is " << currentKeyNum << ",can't insert" << std::endl;
        return false;
    }
    if (getKeyIndex(key) != -1) {
        std::cout << "insertInIndexNode():key(" << key << ")is already exists " << ",can't insert" << std::endl;
        print();
        return false;
    }
    if (currentKeyNum == 0) {
        //当前无节点
        pKey[0] = key;
        pChildren[0] = pNewChildIndex;
        pChildren[1] = pNewChildIndex2;
        currentKeyNum++;
    } else {
        int i;
        for (i = currentKeyNum - 1; i >= 0; --i) {
            if (pKey[i] < key)
                break;
            pKey[i + 1] = pKey[i];
            pChildren[i + 2] = pChildren[i + 1];
        }
        pKey[i + 1] = key;
        pChildren[i + 1] = pNewChildIndex;
        pChildren[i + 2] = pNewChildIndex2;
        currentKeyNum++;
    }
    return true;
}

template<typename Tkey, typename Tdata>
//f
bool Node<Tkey, Tdata>::ifNeedToSplitNode() {
    if (currentKeyNum != maxKeyNum + 1) {
        return false;
    }
    return true;
}


template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::splitLeafNode() {
    if (ifNeedToSplitNode() == false) {
        //错误处理
        std::cout << "splitLeafNode():currentKeyNum is " << currentKeyNum << ",can't split" << std::endl;
        return NULL;
    }

    int mid = currentKeyNum / 2;
    Node<Tkey, Tdata> *pNewNode = new Node<Tkey, Tdata>(true, degree, &pKey[mid], &pData[mid], NULL,
                                                        currentKeyNum - mid);
    currentKeyNum = mid;

    KeyUnit<Tkey, Tdata> *pKeyUnit = new KeyUnit<Tkey, Tdata>();
    pKeyUnit->setKey(pKey[mid]);
    pKeyUnit->setLeftNode(NULL);
    pKeyUnit->setRightNode(pNewNode);
    return pKeyUnit;
}

template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::splitIndexNode() {
    ifNeedToSplitNode();
    int mid = currentKeyNum / 2;
    Node<Tkey, Tdata> *pNewNode = new Node<Tkey, Tdata>(false, degree, &pKey[mid + 1], NULL, &pChildren[mid + 1],
                                                        currentKeyNum - mid - 1);
    currentKeyNum = mid;
    //设置孩子的父节点
    //    for (int i = 0; i < pNewNode->getCurrentKeyNum() + 1; ++i) {
//        pNewNode->getPChildren()[i]->setPParent(pNewNode);
//    }

    KeyUnit<Tkey, Tdata> *pKeyUnit = new KeyUnit<Tkey, Tdata>();
    pKeyUnit->setKey(pKey[mid]);
    pKeyUnit->setLeftNode(NULL);
    pKeyUnit->setRightNode(pNewNode);
    return pKeyUnit;
}


template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::deleteInLeafNodeByKey(Tkey key) {
    int index = getKeyIndex(key);
    if (index == -1) {
        //错误处理
        std::cout << "deleteInLeafNodeByKey():key isn't exist " << std::endl;
        return false;
    }
    for (int i = index; i < currentKeyNum; ++i) {
        pKey[i] = pKey[i + 1];
        pData[i] = pData[i + 1];
    }
    --currentKeyNum;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::deleteInLeafNodeByKeyIndex(int keyIndex) {
    if (keyIndex >= currentKeyNum || keyIndex < 0) {
        //错误处理
        std::cout << "deleteInLeafNodeByKey():key isn't exist " << std::endl;
        return false;
    }
    for (int i = keyIndex; i < currentKeyNum - 2; ++i) {
        pKey[i] = pKey[i + 1];
        pData[i] = pData[i + 1];
    }
    --currentKeyNum;
    return true;
}


template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::deleteInIndexNodeByKeyIndex(int keyIndex, Node<Tkey, Tdata> *pRetainenNode) {
    if (keyIndex >= currentKeyNum) {
        //错误处理
        std::cout << "deleteInIndexNodeByKeyIndex():key isn't exist " << std::endl;
        return false;
    }
    for (int i = keyIndex; i < currentKeyNum - 1; ++i) {
        pKey[i] = pKey[i + 1];
        pChildren[i + 1] = pChildren[i + 2];
    }
    pChildren[keyIndex] = pRetainenNode;
    --currentKeyNum;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::deleteInIndexNodeByKey(Tkey key, Node<Tkey, Tdata> *pRetainenNode) {
    int index = getKeyIndex(key);
    if (index == -1) {
        //错误处理
        std::cout << "deleteInIndexNodeByKey():key isn't exist " << std::endl;
        return false;
    }
    for (int i = index; i < currentKeyNum - 1; ++i) {
        pKey[i] = pKey[i + 1];
        pChildren[i + 1] = pChildren[i + 2];
    }
    pChildren[index] = pRetainenNode;
    --currentKeyNum;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::ifNeedToMergeOrBorrow() {
    if (currentKeyNum < maxKeyNum / 2)
        return true;
    return false;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::ifCanLendKeyToOtherLeafNode() {
    if (currentKeyNum > maxKeyNum / 2)
        return true;
    return false;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::ifCanLendKeyToOtherIndexNode() {
    if (currentKeyNum > maxKeyNum / 2)
        return true;
    return false;
}

template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::lendTheMinKeyInLeafNode() {
    KeyUnit<Tkey, Tdata> *nsp = new KeyUnit<Tkey, Tdata>();
    nsp->setKey(pKey[0]);
    nsp->setData(pData[0]);
    for (int i = 1; i < currentKeyNum; ++i) {
        pKey[i - 1] = pKey[i];
        pData[i - 1] = pData[i];
    }
    currentKeyNum--;
    return nsp;
}

template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::lendTheMaxKeyInLeafNode() {
    KeyUnit<Tkey, Tdata> *nsp = new KeyUnit<Tkey, Tdata>();
    nsp->setKey(pKey[currentKeyNum - 1]);
    nsp->setData(pData[currentKeyNum - 1]);

    currentKeyNum--;
    return nsp;
}

template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::lendTheMinKeyInIndexNode() {
    KeyUnit<Tkey, Tdata> *nsp = new KeyUnit<Tkey, Tdata>();
    nsp->setKey(pKey[0]);
    nsp->setLeftNode(pChildren[0]);
    for (int i = 1; i < currentKeyNum; ++i) {
        pKey[i - 1] = pKey[i];
        pChildren[i - 1] = pChildren[i];
    }
    pChildren[currentKeyNum - 1] = pChildren[currentKeyNum];
    currentKeyNum--;
    return nsp;
}

template<typename Tkey, typename Tdata>
KeyUnit<Tkey, Tdata> *Node<Tkey, Tdata>::lendTheMaxKeyInIndexNode() {
    KeyUnit<Tkey, Tdata> *nsp = new KeyUnit<Tkey, Tdata>();
    nsp->setKey(pKey[currentKeyNum - 1]);
    nsp->setRightNode(pChildren[currentKeyNum]);

    currentKeyNum--;
    return nsp;
}


template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::ifCanMergeWithOtherLeafNode() {
    if (currentKeyNum < maxKeyNum / 2)
        return true;
    return false;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::ifCanMergeWithOtherIndexNode() {
    if (currentKeyNum < maxKeyNum / 2)
        return true;
    return false;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::mergeWithLeftBrotherLeafNode(Node<Tkey, Tdata> *pLeftBrotherNode) {
    if (pLeftBrotherNode->getCurrentKeyNum() + currentKeyNum > maxKeyNum) {
        //错误处理
        std::cout << "mergeWithLeftBrotherLeafNode():can't merge .too many keys" << std::endl;
        return false;
    }


    int count = pLeftBrotherNode->getCurrentKeyNum();
//    std::cout << "finish..." << std::endl;

    for (int i = currentKeyNum - 1; i >= 0; --i) {
        pKey[i + count] = pKey[i];
        pData[i + count] = pData[i];
    }
//    std::cout << "finish..." << std::endl;
    for (int i = 0; i < count; ++i) {
        pKey[i] = pLeftBrotherNode->getPKey()[i];
        pData[i] = pLeftBrotherNode->getPData()[i];
    }
    currentKeyNum = currentKeyNum + count;
    delete pLeftBrotherNode;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::mergeWithRightBrotherLeafNode(Node<Tkey, Tdata> *pRightBrotherNode) {
    if (pRightBrotherNode->getCurrentKeyNum() + currentKeyNum > maxKeyNum) {
        //错误处理
        std::cout << "mergeWithLeftBrotherLeafNode():can't merge .too many keys" << std::endl;
        return false;
    }

    int count = pRightBrotherNode->getCurrentKeyNum();
//    std::cout << "count:" << count << std::endl;
    for (int i = 0; i < count; ++i) {
        pKey[i + currentKeyNum] = pRightBrotherNode->pKey[i];
        pData[i + currentKeyNum] = pRightBrotherNode->pData[i];
    }
    currentKeyNum = currentKeyNum + count;
//    print();
    delete pRightBrotherNode;
    return true;
}

template<typename Tkey, typename Tdata>
bool
Node<Tkey, Tdata>::mergeWithLeftBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pLeftBrotherNode, Tkey parentValue) {
    if (pLeftBrotherNode->getCurrentKeyNum() + currentKeyNum > maxKeyNum) {
        //错误处理
        std::cout << "mergeWithLeftBrotherIndexNode():can't merge .too many keys" << std::endl;
        return false;
    }

    int count = pLeftBrotherNode->getCurrentKeyNum();
    for (int i = currentKeyNum - 1; i >= 0; --i) {
        pKey[i + (count + 1)] = pKey[i];
        pChildren[i + 1 + (count + 1)] = pChildren[i + 1];
    }
    pChildren[count + 1] = pChildren[0];

    for (int i = 0; i < count; ++i) {
        pKey[i] = pLeftBrotherNode->getPKey()[i];
        pChildren[i] = pLeftBrotherNode->getPChildren()[i];
    }
    pKey[count] = parentValue;
    pChildren[count] = pLeftBrotherNode->getPChildren()[count];

    currentKeyNum = currentKeyNum + count + 1;


    delete pLeftBrotherNode;
    return true;
}

template<typename Tkey, typename Tdata>
bool
Node<Tkey, Tdata>::mergeWithRightBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pRightBrotherNode, Tkey parentValue) {
    if (pRightBrotherNode->getCurrentKeyNum() + currentKeyNum > maxKeyNum) {
        //错误处理
        std::cout << "mergeWithRightBrotherIndexNode():can't merge .too many keys" << std::endl;
        return false;
    }
    int count = pRightBrotherNode->getCurrentKeyNum();
    pKey[currentKeyNum] = parentValue;
    for (int i = 0; i < count; ++i) {
        pKey[currentKeyNum + 1 + i] = pRightBrotherNode->getPKey()[i];
        pChildren[currentKeyNum + 1 + i] = pRightBrotherNode->getPChildren()[i];
    }
    pChildren[currentKeyNum + 1 + count] = pRightBrotherNode->getPChildren()[count];
    currentKeyNum = currentKeyNum + count + 1;
    delete pRightBrotherNode;
    return true;
}


template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::updateKeyByIndex(int index, Tkey key) {
    if (index > currentKeyNum - 1 || index < 0) {
        //错误处理
        std::cout << "updateKeyByIndex():index out:" << index << "  currentkeyNum:" << currentKeyNum << std::endl;
        return false;
    }
    pKey[index] = key;
    return true;
}

template<typename Tkey, typename Tdata>
bool Node<Tkey, Tdata>::updateThePParentInChildrenNode(Node<Tkey, Tdata> *selfNode) {
    if (currentKeyNum == 0)
        return true;

    for (int i = 0; i < currentKeyNum + 1; ++i) {
        pChildren[i]->setPParent(selfNode);
    }
    return true;
}

#endif