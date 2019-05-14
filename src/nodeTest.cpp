#include "node.h"


void testinsertInLeafNodeAndsplitLeafNode() {
    using namespace std;

    Node<int, int> *n = new Node<int, int>(true, 5);
    n->print();
    n->insertInLeafNode(5, 5);
    n->insertInLeafNode(4, 4);
    n->insertInLeafNode(2, 2);
    n->insertInLeafNode(6, 6);
    cout << n->ifNeedToSplitNode() << endl;
    n->insertInLeafNode(7, 7);
    cout << n->ifNeedToSplitNode() << endl;
    n->print();
    KeyUnit<int, int> *nsp = n->splitLeafNode();
    n->print();
    nsp->print();

    cout << endl;
    delete n;
}

void testinsertInIndexNodeAndsplitIndexNode() {
    //未测试children部分是否正确
    using namespace std;

    Node<int, int> *n = new Node<int, int>(false, 5);
    n->print();
    n->insertInIndexNode(5, NULL, NULL);
    n->insertInIndexNode(4, NULL, NULL);
    n->insertInIndexNode(2, NULL, NULL);
    n->insertInIndexNode(6, NULL, NULL);
    cout << n->ifNeedToSplitNode() << endl;
    n->insertInIndexNode(7, NULL, NULL);
    cout << n->ifNeedToSplitNode() << endl;
    n->print();
    KeyUnit<int, int> *nsp = n->splitIndexNode();
    n->print();
    nsp->print();

    cout << endl;
    delete n;
}


void testmerge() {
    Node<int, int> *n = new Node<int, int>(true, 7);//[3,6]
    n->deleteInLeafNodeByKey(4);
    n->print();

    n->insertInLeafNode(5, 5);
    n->insertInLeafNode(4, 4);
    n->insertInLeafNode(6, 6);
    n->insertInLeafNode(1, 1);
    n->print();
    n->deleteInLeafNodeByKey(5);
    n->deleteInLeafNodeByKey(4);
    n->deleteInLeafNodeByKey(1);
    n->deleteInLeafNodeByKey(6);
    n->print();


    delete n;


}

