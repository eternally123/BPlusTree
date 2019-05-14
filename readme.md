- Node类的数据结构
```
template<typename Tkey, typename Tdata>
class Node {
    //节点的度为索引节点最多的子孩子个数
    //为了便于分裂合并，节点的key值pKey、data数据pData、children指针pChildren的数组大小均比实际值大1

    //右子树大于等于节点值
    //左子树小于节点值

    //分裂时左少右多

    //keyNum个数的范围[maxKeyNum/2 maxKeyNum]
protected:
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
}
```
- Node类的方法
```
//构造函数与析构函数
public:
    Node(bool isLeaf, int degree);
    Node(bool isLeaf, int degree, Tkey *pKeys, Tdata *pDatas, Node<Tkey, Tdata> **pNodes, int num);
    ~Node();
```
```
//get、set方法
public:
    bool getIsLeaf();
    int getMaxKeyNum();
    int getCurrentKeyNum();
    Tkey *getPKey();
    Tdata *getPData();
    Node<Tkey, Tdata> **getPChildren();
    Node<Tkey, Tdata> *getPParent();
    Node<Tkey, Tdata> *getPNextLeafNode();
    Node<Tkey, Tdata> *getPPreLeafNode();
    bool setIsLeaf(bool isLeaf);
    bool setPParent(Node<Tkey, Tdata> *pParent);
    bool setPNext(Node<Tkey, Tdata> *pNext);
    bool setPPre(Node<Tkey, Tdata> *pPre);
```
```
//与查找相关的方法
public:
    int getKeyIndex(Tkey key);
    int getPChildrenIndexByKey(Tkey key);
    Node<Tkey, Tdata> *getChildTreeByKey(Tkey key);
    int getPChildrenIndexInParent();
    Node<Tkey, Tdata> *getLeftBrotherNode();
    Node<Tkey, Tdata> *getRightBrotherNode();
```
```
//更新相关的方法
public:
    bool updateKeyByIndex(int index, Tkey key);
    <!-- bool updateThePParentInChildrenNode(Node<Tkey, Tdata> *selfNode); -->//todo
```
```
//与插入相关的方法
public:
    bool insertInLeafNode(Tkey key, Tdata data);
    bool insertInIndexNode(Tkey key, Node<Tkey, Tdata> *pNewChildIndex, Node<Tkey, Tdata> *pNewChildIndex2);

    bool ifNeedToSplitNode();

    KeyUnit<Tkey, Tdata> *splitLeafNode();
    KeyUnit<Tkey, Tdata> *splitIndexNode();
```
```
//删除相关的方法
public:
    bool deleteInLeafNodeByKey(Tkey key);
    bool deleteInLeafNodeByKeyIndex(int keyIndex);
    bool deleteInIndexNodeByKey(Tkey key, Node<Tkey, Tdata> *pRetainenNode);
    bool deleteInIndexNodeByKeyIndex(int keyIndex, Node<Tkey, Tdata> *pRetainenNode);

    bool ifNeedToMergeOrBorrow();

    bool ifCanLendKeyToOtherLeafNode();
    bool ifCanLendKeyToOtherIndexNode();

    KeyUnit<Tkey, Tdata> *lendTheMinKeyInLeafNode();
    KeyUnit<Tkey, Tdata> *lendTheMaxKeyInLeafNode();
    KeyUnit<Tkey, Tdata> *lendTheMinKeyInIndexNode();
    KeyUnit<Tkey, Tdata> *lendTheMaxKeyInIndexNode();

    bool ifCanMergeWithOtherLeafNode();
    bool ifCanMergeWithOtherIndexNode();

    bool mergeWithLeftBrotherLeafNode(Node<Tkey, Tdata> *pLeftBrotherNode);
    bool mergeWithRightBrotherLeafNode(Node<Tkey, Tdata> *pRightBrotherNode);
    bool mergeWithLeftBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pLeftBrotherNode, Tkey parentValue);
    bool mergeWithRightBrotherIndexNodeAndParentKey(Node<Tkey, Tdata> *pRightBrotherNode, Tkey parentValue);
```


- BPlusTree类的数据结构
```
template<typename Tkey, typename Tdata>
class BPlusTree {
    //右子树大于等于父节点 左子树小于父节点
protected:
    int degree;
    Node<Tkey, Tdata> *pTreeRoot;
    Node<Tkey, Tdata> *pListStart;
}
```
- BPlusTree类的方法
```
public:
    Node<Tkey, Tdata> *search(Tkey key);
    bool insert(Tkey key, Tdata data);
    bool remove(Tkey key);
    void printTree();
    void printTree(Node<Tkey, Tdata> *pRoot);
    void printLeafNode();
    void printLeafNode(Node<Tkey, Tdata> *pLeafNode);

protected:
    Node<Tkey, Tdata> *search(Tkey key, Node<Tkey, Tdata> *pRoot);
    Node<Tkey, Tdata> *searchDonNotCareIfExist(Tkey key, Node<Tkey, Tdata> *pRoot);
    bool insertInIndexNode(Node<Tkey, Tdata> *pParent, Tkey key, Node<Tkey, Tdata> *leftChildren,Node<Tkey, Tdata> *rightChildren);
    bool deleteInIndexNode(int index, Node<Tkey, Tdata> *pIndexNode, Node<Tkey, Tdata> *pRetainChildren);
```