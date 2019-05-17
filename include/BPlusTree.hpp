#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include "node.hpp"

#include <vector>
#include <unistd.h>
#include <mutex>


template<typename Tkey, typename Tdata>
class BPlusTree {
    //右子树大于等于父节点 左子树小于父节点
    //degree至少为4
protected:
    int degree;
    Node<Tkey, Tdata> *pTreeRoot;
    Node<Tkey, Tdata> *pListStart;
    std::mutex *mut;

public://构造函数与析构函数
    BPlusTree(int degree);

    ~BPlusTree();

public://get、set方法
    Node<Tkey, Tdata> *getPTreeRoot();

    Node<Tkey, Tdata> *getPListStart();

public://主要功能：增删改查
    //根据key搜索，返回叶子节点，若无返回空
    Node<Tkey, Tdata> *search(Tkey key);//f

    //插入key和data
    bool insert(Tkey key, Tdata data);//f

    //删除key
    bool remove(Tkey key);//f

    //打印树信息
    void printTree();

    //打印root节点子树信息
    void printTree(Node<Tkey, Tdata> *pRoot);

    void printList();

    //查看父节点是否正确
    bool checkPParent(Node<Tkey, Tdata> *pRoot);

    //查看是否能找到所有数据
    bool ifCanFindAllData();

public:
    //序列化到磁盘
    bool serialization(char *filePath);

    //从磁盘反序列化
    bool unSerialization(char *filePath);

protected://内部方法
    Node<Tkey, Tdata> *search(Tkey key, Node<Tkey, Tdata> *pRoot);

    Node<Tkey, Tdata> *searchDonNotCareIfExist(Tkey key, Node<Tkey, Tdata> *pRoot);

    bool insertInIndexNode(Node<Tkey, Tdata> *pParent, Tkey key, Node<Tkey, Tdata> *leftChildren,
                           Node<Tkey, Tdata> *rightChildren);

    bool deleteInIndexNode(int index, Node<Tkey, Tdata> *pIndexNode, Node<Tkey, Tdata> *pRetainChildren);

    bool updateChildrenPParentofPIndexNode(Node<Tkey, Tdata> *pIndexNode);

    bool deleteSubTree(Node<Tkey, Tdata> *pNode);

//    bool checkKey(Node<Tkey, Tdata> *pRoot, Tkey min, Tkey max);
};


/*
 *
 *
 * implement of class BPlusTree
 *
 *
 */

template<typename Tkey, typename Tdata>
BPlusTree<Tkey, Tdata>::BPlusTree(int degree) {
    this->degree = degree;
    pTreeRoot = NULL;
    pListStart = NULL;
    mut = new std::mutex();
}

template<typename Tkey, typename Tdata>
BPlusTree<Tkey, Tdata>::~BPlusTree() {
    delete mut;
    if (pTreeRoot == NULL) {
        //空树
        return;
    } else {
        if (pTreeRoot->getIsLeaf() == true) {
            //无孩子
            delete pTreeRoot;
            return;
        } else {
            //有孩子
            for (int i = 0; i < pTreeRoot->getCurrentKeyNum() + 1; ++i) {
                deleteSubTree(pTreeRoot->getPChildren()[i]);
            }
            delete pTreeRoot;
        }
    }
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *BPlusTree<Tkey, Tdata>::getPTreeRoot() {
    return pTreeRoot;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *BPlusTree<Tkey, Tdata>::getPListStart() {
    return pListStart;
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *BPlusTree<Tkey, Tdata>::search(Tkey key) {
//    mut->lock();
    Node<Tkey, Tdata> *pResult = search(key, pTreeRoot);
//    mut->unlock();
    return pResult;
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::insert(Tkey key, Tdata data) {
    /*
     * 1、若是空树，创建树
     * 2、若不是空树，根据key值查找叶子节点。找到后，插入。
     *      2.1、若插入后不超过叶子节点key值存储范围，结束
     *      2.2、若插入后超过了叶子节点key值存储范围：
     *              节点分裂：
     */
    mut->lock();
    if (pTreeRoot == NULL) {//树为空
        Node<Tkey, Tdata> *pNode = new Node<Tkey, Tdata>(true, degree);
        pNode->insertInLeafNode(key, data);
        pTreeRoot = pNode;
        pListStart = pNode;
        mut->unlock();
        return true;
    } else {//树非空,先查找在哪个叶子节点插入
        Node<Tkey, Tdata> *pNode = searchDonNotCareIfExist(key, pTreeRoot);
        if (pNode->getKeyIndex(key) != -1) {
            //已存在
            mut->unlock();
            return false;
        }
        pNode->insertInLeafNode(key, data);
        if (pNode->ifNeedToSplitNode() == false) {
            //插入后无需分裂
            mut->unlock();
            return true;
        } else {
            //插入后需要分裂,采用右分裂 pParent是父节点。nsp有value和右子树。pNode是左子树
            KeyUnit<Tkey, Tdata> *nsp = pNode->splitLeafNode();
            Node<Tkey, Tdata> *pNewNode = nsp->getRightNode();
            pNewNode->setPNext(pNode->getPNextLeafNode());
            pNewNode->setPPre(pNode);
            pNode->setPNext(pNewNode);
            if (pNewNode->getPNextLeafNode() != NULL)
                pNewNode->getPNextLeafNode()->setPPre(pNewNode);

            //递归调用在索引节点插入
            bool result = insertInIndexNode(pNode->getPParent(), nsp->getKey(), pNode, pNewNode);
            delete nsp;
            mut->unlock();
            return result;
        }
    }
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::remove(Tkey key) {
    /*
     * 1.找到节点，删除。若无则返回false
     * 2.keyNum满足，返回
     * 3.不满足则：
     *  3.1借兄弟，更改，返回
     *  3.2合并兄弟，更改，删父节点，返回
     */
    using namespace std;
    mut->lock();
    if (search(key) == NULL) {
        std::cout << "key不存在:" << key << std::endl;
        mut->unlock();
        return false;
    } else {
        //key存在时,删除数据
        Node<Tkey, Tdata> *pNode = search(key);
        pNode->deleteInLeafNodeByKey(key);
        if (pNode->ifNeedToMergeOrBorrow() == false) {
            //如果删除后符合树定义，结束
            mut->unlock();
            return true;
        } else {
            //可能需要借或者合并
            if (pNode == pTreeRoot) {
                //根节点，无需合并或借
                mut->unlock();
                return true;
            } else {
                //非根节点，需要借或合并
                Node<Tkey, Tdata> *pLeftNode = pNode->getLeftBrotherNode();
                Node<Tkey, Tdata> *pRightNode = pNode->getRightBrotherNode();
                if ((pLeftNode != NULL) && (pLeftNode->ifCanLendKeyToOtherLeafNode() == true)) {
                    //向左兄弟借
                    int parentKeyIndexToUpdate = pLeftNode->getPChildrenIndexInParent();
                    KeyUnit<Tkey, Tdata> *nsp = pLeftNode->lendTheMaxKeyInLeafNode();
                    //此节点插入
                    pNode->insertInLeafNode(nsp->getKey(), nsp->getData());
                    pNode->getPParent()->updateKeyByIndex(parentKeyIndexToUpdate, nsp->getKey());
                    delete nsp;
                    mut->unlock();
                    return true;
                } else if ((pRightNode != NULL) && (pRightNode->ifCanLendKeyToOtherLeafNode() == true)) {
                    //向右兄弟借
                    int parentKeyIndexToUpdate = pNode->getPChildrenIndexInParent();
                    KeyUnit<Tkey, Tdata> *nsp = pRightNode->lendTheMinKeyInLeafNode();
                    //此节点插入
                    pNode->insertInLeafNode(nsp->getKey(), nsp->getData());
                    pNode->getPParent()->updateKeyByIndex(parentKeyIndexToUpdate, pRightNode->getPKey()[0]);
                    delete nsp;
                    mut->unlock();
                    return true;
                } else {
                    //合并
                    if (pLeftNode != NULL) {
                        //和左子树merge，先更改链表和链表起始地址
                        if (pLeftNode == pListStart)
                            pListStart = pNode;
                        //然后改链表连接关系
                        pNode->setPPre(pLeftNode->getPPreLeafNode());
                        if (pLeftNode->getPPreLeafNode() != NULL)
                            pLeftNode->getPPreLeafNode()->setPNext(pNode);
                        //最后合并，更改父节点
                        int index = pLeftNode->getPChildrenIndexInParent();
                        pNode->mergeWithLeftBrotherLeafNode(pLeftNode);
                        if (pNode->getPParent()->getCurrentKeyNum() == 1) {
                            //父节点只剩一个元素，delete并设置树根
                            pTreeRoot = pNode;
                            delete pNode->getPParent();
                            pNode->setPParent(NULL);
                            mut->unlock();
                            return true;
                        } else {
                            //父节点还有多个元素，根在父节点及上
                            bool result = deleteInIndexNode(index, pNode->getPParent(), pNode);
                            mut->unlock();
                            return result;
                        }
                    } else {
                        //和右子树merge，先更改链表
                        pNode->setPNext(pRightNode->getPNextLeafNode());
                        if (pRightNode->getPNextLeafNode() != NULL)
                            pRightNode->getPNextLeafNode()->setPPre(pNode);
                        //最后合并，更改父节点
                        int index = pNode->getPChildrenIndexInParent();
                        pNode->mergeWithRightBrotherLeafNode(pRightNode);
                        if (pNode->getPParent()->getCurrentKeyNum() == 1) {
                            //父节点只剩一个元素，delete
                            pTreeRoot = pNode;
                            delete pNode->getPParent();
                            pNode->setPParent(NULL);
                            mut->unlock();
                            return true;
                        } else {
                            //父节点还有多个元素，根在父节点及上
                            bool result = deleteInIndexNode(index, pNode->getPParent(), pNode);
                            mut->unlock();
                            return result;
                        }
                    }
                }
            }
        }
    }
}

template<typename Tkey, typename Tdata>
void BPlusTree<Tkey, Tdata>::printTree() {
    printTree(pTreeRoot);
}

template<typename Tkey, typename Tdata>
void BPlusTree<Tkey, Tdata>::printTree(Node<Tkey, Tdata> *pRoot) {
    if (pRoot == NULL) {
        //空节点，返回
        return;
    } else {
        //非空，打印
        pRoot->print();
        if (pRoot->getIsLeaf() == true) {
            //根节点，返回
            return;
        } else {
            //非根节点，打印子树
            if (pRoot->getCurrentKeyNum() == 0) {
                //无子树，阶树
                return;
            } else {
                //有子树，打印
                for (int i = 0; i < pRoot->getCurrentKeyNum() + 1; ++i) {
                    printTree(pRoot->getPChildren()[i]);
                }
            }

        }
    }


}

template<typename Tkey, typename Tdata>
void BPlusTree<Tkey, Tdata>::printList() {
    while (pListStart != NULL) {
        pListStart->print();
        pListStart = pListStart->getPNextLeafNode();
    }
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::checkPParent(Node<Tkey, Tdata> *pRoot) {
    //检查pRoot的孩子。看其父节点是否正确
    bool result = true;
    if (pRoot->getIsLeaf() == true) {
        //树的根节点
        return true;
    } else {
        //非树的根节点
        //检查子树的父节点是否正确
        Node<Tkey, Tdata> *pChild;
        for (int i = 0; i < pRoot->getCurrentKeyNum() + 1; ++i) {
            if (pRoot->getPChildren()[i]->getPParent() != pRoot) {
                result = false;
                break;
            }
        }
        for (int i = 0; i < pRoot->getCurrentKeyNum() + 1; ++i) {
            if (checkPParent(pRoot->getPChildren()[i]) == false)
                result = false;
            break;
        }
        //检查子树
    }
    return result;
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::ifCanFindAllData() {
    Node<Tkey, Tdata> *p = pListStart;
    while (p != NULL) {
        for (int i = 0; i < p->getCurrentKeyNum(); ++i) {
            if (search(p->getPKey()[i]) == NULL)
                return false;
        }
        p = p->getPNextLeafNode();
    }
    return true;
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::serialization(char *filePath) {
    /*
     *层次遍历写入文件
     */
    if (pTreeRoot == NULL) {
        //树空，无需序列化
        return true;
    } else {
        //序列化
        FILE *fp;
        fp = std::fopen(filePath, "w");
        if (fp == NULL) {
            //打开文件失败
            return false;
        } else {
            //打开文件成功，写入数据
            using namespace std;

            bool isLeafNode;
            int degree;
            int maxKeyNum;
            int currentKeyNum;

            /*
             * 层次遍历序列化
             */
            vector<Node<Tkey, Tdata> *> *v = new vector<Node<Tkey, Tdata> *>();
            Node<Tkey, Tdata> *pNodeTempt;
            Node<Tkey, Tdata> *pChildNodeTempt;

            v->push_back(pTreeRoot);
            while (v->empty() == false) {
                pNodeTempt = v->at(0);
                v->erase(v->begin());
                //写入pNodeTempt
                {
//                    cout<<"开始序列化节点:"<<endl;
//                    sleep(1);
//                    pNodeTempt->print();
                    isLeafNode = pNodeTempt->getIsLeaf();
                    degree = pNodeTempt->getDegree();
                    maxKeyNum = pNodeTempt->getMaxKeyNum();
                    currentKeyNum = pNodeTempt->getCurrentKeyNum();

                    fwrite(&isLeafNode, sizeof(isLeafNode), 1, fp);
                    fwrite(&degree, sizeof(degree), 1, fp);
                    fwrite(&maxKeyNum, sizeof(maxKeyNum), 1, fp);
                    fwrite(&currentKeyNum, sizeof(currentKeyNum), 1, fp);
                    fwrite(pNodeTempt->getPKey(), sizeof(Tkey), pNodeTempt->getCurrentKeyNum(), fp);
                    if (pNodeTempt->getIsLeaf() == true) {
                        //叶子节点
                        fwrite(pNodeTempt->getPData(), sizeof(Tdata), pNodeTempt->getCurrentKeyNum(), fp);
                    }
                }
                if (pNodeTempt->getIsLeaf() == false) {
                    //非叶子节点，有孩子，孩子入队列
                    for (int i = 0; i < pNodeTempt->getCurrentKeyNum() + 1; ++i) {
                        pChildNodeTempt = pNodeTempt->getPChildren()[i];
                        //孩子入vector
                        v->push_back(pChildNodeTempt);
                    }
                }
            }
            fclose(fp);
            return true;
        }
    }
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::unSerialization(char *filePath) {
    /*
    *层次遍历读出文件
    */
    //反序列化
    FILE *fp;
    fp = std::fopen(filePath, "r");
    if (fp == NULL) {
        //打开文件失败
        return false;
    } else {
        //打开文件成功，读出数据
        using namespace std;

        bool isLeafNode;
        int degree;
        int maxKeyNum;
        int currentKeyNum;

        /*
         * 层次遍历反序列化
         */
        vector<Node<Tkey, Tdata> *> *v = new vector<Node<Tkey, Tdata> *>();
        Node<Tkey, Tdata> *pNodeTempt;
        Node<Tkey, Tdata> *pParentNodeTempt;
        Node<Tkey, Tdata> *pPreNodeTempt;

        {//读出根节点
            fread(&isLeafNode, sizeof(isLeafNode), 1, fp);
            fread(&degree, sizeof(degree), 1, fp);
            fread(&maxKeyNum, sizeof(maxKeyNum), 1, fp);
            fread(&currentKeyNum, sizeof(currentKeyNum), 1, fp);

            pNodeTempt = new Node<Tkey, Tdata>(isLeafNode, degree);
            pTreeRoot = pNodeTempt;
            pNodeTempt->setCurrentKeyNum(currentKeyNum);//set

            fread(pNodeTempt->getPKey(), sizeof(Tkey), currentKeyNum, fp);//set

            if (pNodeTempt->getIsLeaf() == true) {
                //叶子节点
                fread(pNodeTempt->getPData(), sizeof(Tdata), pNodeTempt->getCurrentKeyNum(), fp);//set
            }
            v->push_back(pNodeTempt);
        }

        while (v->empty() == false) {
            pParentNodeTempt = v->at(0);//父节点
//            pParentNodeTempt->print();
//            cout << endl;
            v->erase(v->begin());
            if (pParentNodeTempt->getIsLeaf() == false) {
                //非叶子节点，有孩子
                for (int i = 0; i < pParentNodeTempt->getCurrentKeyNum() + 1; ++i) {
                    {//读出节点数据到pNodeTempt中
                        fread(&isLeafNode, sizeof(isLeafNode), 1, fp);
                        fread(&degree, sizeof(degree), 1, fp);
                        fread(&maxKeyNum, sizeof(maxKeyNum), 1, fp);
                        fread(&currentKeyNum, sizeof(currentKeyNum), 1, fp);

                        pNodeTempt = new Node<Tkey, Tdata>(isLeafNode, degree);//isLeaf degree maxKeyNum
                        pNodeTempt->setCurrentKeyNum(currentKeyNum);//currentKeyNum
                        fread(pNodeTempt->getPKey(), sizeof(Tkey), currentKeyNum, fp);
                        if (pNodeTempt->getIsLeaf() == true) {
                            //叶子节点
                            fread(pNodeTempt->getPData(), sizeof(Tdata), pNodeTempt->getCurrentKeyNum(), fp);//data
                        }
                    }

//                    pParentNodeTempt->pChildren[i]=pNodeTempt;
                    pParentNodeTempt->setPChild(pNodeTempt, i);


                    pNodeTempt->setPParent(pParentNodeTempt);
                    v->push_back(pNodeTempt);
                }
            } else if (pParentNodeTempt->getIsLeaf() == true) {
                //无孩子，一定是叶子节点
                if (pListStart == NULL) {
                    //链表根节点
                    pListStart = pParentNodeTempt;
                    pParentNodeTempt->setPPre(NULL);
                    pParentNodeTempt->setPNext(NULL);
                    pPreNodeTempt = pParentNodeTempt;
                } else {
                    pPreNodeTempt->setPNext(pParentNodeTempt);
                    pParentNodeTempt->setPNext(NULL);
                    pParentNodeTempt->setPPre(pPreNodeTempt);
                    pPreNodeTempt = pParentNodeTempt;
                }
            } else {
                cout << "unSerialization():error" << endl;
            }
        }
        fclose(fp);
        return true;
    }
}


template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *BPlusTree<Tkey, Tdata>::search(Tkey key, Node<Tkey, Tdata> *pRoot) {
    /*
     * 1、若果根为null，返回null
     * 2、如果根不空，且根为叶子节点
     *      查找是否存在key，存在返回叶子节点地址，否则返回null
     * 3、如果根不空，且根不为叶子节点
     *      确定key在哪棵子树中，递归查找此树
     */
    if (pRoot == NULL) {
        return NULL;
    } else {
        if (pRoot->getIsLeaf() == true) {
            int keyIndex = pRoot->getKeyIndex(key);
            if (keyIndex == -1)
                return NULL;
            else {
                return pRoot;
            }
        } else {
            Node<Tkey, Tdata> *pChildrenTree = pRoot->getChildTreeByKey(key);
            return search(key, pChildrenTree);
        }
    }
}

template<typename Tkey, typename Tdata>
Node<Tkey, Tdata> *BPlusTree<Tkey, Tdata>::searchDonNotCareIfExist(Tkey key, Node<Tkey, Tdata> *pRoot) {
    /*
     * 1、若果树为null，返回null
     * 2、如果树不空，且根为叶子节点
     *      直接返回叶子节点
     * 3、如果根不空，且根不为叶子节点
     *      哪棵子树存储key范围包含此key值，递归查找此树
     */

    if (pRoot == NULL) {
        return NULL;
    } else {
        if (pRoot->getIsLeaf() == true) {
            return pRoot;
        } else {
            Node<Tkey, Tdata> *pChildrenTree = pRoot->getChildTreeByKey(key);
            return searchDonNotCareIfExist(key, pChildrenTree);
        }
    }
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::insertInIndexNode(Node<Tkey, Tdata> *pParent, Tkey key, Node<Tkey, Tdata> *leftChildren,
                                               Node<Tkey, Tdata> *rightChildren) {
//    std::cout << "插入的是" << key << std::endl << std::endl;
    if (pParent == NULL) {
        //父节点空，创建，并修改树的根节点
        pParent = new Node<Tkey, Tdata>(false, degree);
        pTreeRoot = pParent;
        pParent->insertInIndexNode(key, leftChildren, rightChildren);
        leftChildren->setPParent(pParent);
        rightChildren->setPParent(pParent);
        return true;
    } else {
        //父节点不空，插入
        leftChildren->setPParent(pParent);
        rightChildren->setPParent(pParent);
        pParent->insertInIndexNode(key, leftChildren, rightChildren);
        if (pParent->ifNeedToSplitNode() == false) {
            //插入后无需分裂。结束
            return true;
        } else {
            //插入后需要分裂,分裂后会影响子节点信息
            KeyUnit<Tkey, Tdata> *nsp = pParent->splitIndexNode();
            updateChildrenPParentofPIndexNode(nsp->getRightNode());
            return insertInIndexNode(pParent->getPParent(), nsp->getKey(), pParent, nsp->getRightNode());
        }
    }


}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::deleteInIndexNode(int index, Node<Tkey, Tdata> *pIndexNode,
                                               Node<Tkey, Tdata> *pRetainChildren) {
    if (pIndexNode == NULL) {
        //错误处理
        std::cout << "deleteInIndexNode():pIndexNode为空" << std::endl;
        return false;
    }
    pIndexNode->deleteInIndexNodeByKeyIndex(index, pRetainChildren);
    //是否需要合并之类的。。。。。。
    if ((pIndexNode == pTreeRoot) || (pIndexNode->ifNeedToMergeOrBorrow() == false)) {
        //树根或不需合并。符合结束
        return true;
    } else {
        //节点key数量不符合，借或合并
        Node<Tkey, Tdata> *pLeftNode = pIndexNode->getLeftBrotherNode();

        Node<Tkey, Tdata> *pRightNode = pIndexNode->getRightBrotherNode();

        if (pLeftNode != NULL && pLeftNode->ifCanLendKeyToOtherIndexNode() == true) {
            //借左节点
            //左节点借出
            int parentKeyIndex = pIndexNode->getPChildrenIndexInParent() - 1;
            Tkey parentKey = pIndexNode->getPParent()->getPKey()[parentKeyIndex];
            KeyUnit<Tkey, Tdata> *nsp = pLeftNode->lendTheMaxKeyInIndexNode();
            //此结点插入
            pIndexNode->insertInIndexNode(parentKey, nsp->getRightNode(), pIndexNode->getPChildren()[0]);
            //此节点插入值的孩子节点更新其父节点
            nsp->getRightNode()->setPParent(pIndexNode);
            //父节点更新
            pIndexNode->getPParent()->updateKeyByIndex(parentKeyIndex, nsp->getKey());
            return true;
        } else if (pRightNode != NULL && pRightNode->ifCanLendKeyToOtherIndexNode() == true) {
            //借右节点
            //右节点借出
            int parentKeyIndex = pIndexNode->getPChildrenIndexInParent();
            Tkey parentKey = pIndexNode->getPParent()->getPKey()[parentKeyIndex];
            KeyUnit<Tkey, Tdata> *nsp = pRightNode->lendTheMinKeyInIndexNode();
            //此结点插入
            pIndexNode->insertInIndexNode(parentKey, pIndexNode->getPChildren()[pIndexNode->getCurrentKeyNum()],
                                          nsp->getLeftNode());
            //此节点插入值的孩子节点更新其父节点
            nsp->getLeftNode()->setPParent(pIndexNode);
            //父节点更新
            pIndexNode->getPParent()->updateKeyByIndex(parentKeyIndex, nsp->getKey());
            return true;
        } else {
            //合并
            if (pLeftNode != NULL) {
                //和左节点合并
                int parentIndexRemove = pIndexNode->getPChildrenIndexInParent() - 1;
                Tkey parentKey = pIndexNode->getPParent()->getPKey()[parentIndexRemove];
                //更新左节点子孩子
                pLeftNode->updateThePParentInChildrenNode(pIndexNode);
                pIndexNode->mergeWithLeftBrotherIndexNodeAndParentKey(pLeftNode, parentKey);
                if (pIndexNode->getPParent()->getCurrentKeyNum() == 1) {
                    //合并后，父节点无元素，直接释放
                    pTreeRoot = pIndexNode;
                    delete pIndexNode->getPParent();
                    pIndexNode->setPParent(NULL);
                    return true;
                } else {
                    return deleteInIndexNode(parentIndexRemove, pIndexNode->getPParent(), pIndexNode);
                }
            } else if (pRightNode != NULL) {
                //和右节点合并.需更新右节点子孩子的父节点为当前节点
                int parentIndexRemove = pIndexNode->getPChildrenIndexInParent();
                Tkey parentKey = pIndexNode->getPParent()->getPKey()[parentIndexRemove];
                //更新右节点子孩子
                pRightNode->updateThePParentInChildrenNode(pIndexNode);
                pIndexNode->mergeWithRightBrotherIndexNodeAndParentKey(pRightNode, parentKey);
                if (pIndexNode->getPParent()->getCurrentKeyNum() == 1) {
                    //合并后，父节点无元素，直接释放
                    pTreeRoot = pIndexNode;
                    delete pIndexNode->getPParent();
                    pIndexNode->setPParent(NULL);
                    return true;
                } else {
                    return deleteInIndexNode(parentIndexRemove, pIndexNode->getPParent(), pIndexNode);
                }
            } else {
                std::cout << "deleteInIndexNode():error" << std::endl;
                return false;
            }
        }
    }
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::updateChildrenPParentofPIndexNode(Node<Tkey, Tdata> *pIndexNode) {
    if (pIndexNode->getCurrentKeyNum() == 0) {
        return true;
    } else {
        for (int i = 0; i < pIndexNode->getCurrentKeyNum() + 1; ++i) {
            pIndexNode->getPChildren()[i]->setPParent(pIndexNode);
        }
    }
    return true;
}

template<typename Tkey, typename Tdata>
bool BPlusTree<Tkey, Tdata>::deleteSubTree(Node<Tkey, Tdata> *pNode) {
    if (pTreeRoot->getIsLeaf() == true) {
        //无孩子
        delete pTreeRoot;
        return true;
    } else {
        //有孩子
        for (int i = 0; i < pTreeRoot->getCurrentKeyNum() + 1; ++i) {
            deleteSubTree(pTreeRoot->getPChildren()[i]);
        }
        delete pTreeRoot;
        return true;
    }
}

#endif