#include "BPlusTreeTest.h"


void testInsert() {
    BPlusTree<int, int> *t = new BPlusTree<int, int>(4);
    t->insert(15, 15);
    t->insert(12, 12);
    t->insert(13, 13);
    t->insert(14, 14);
    t->insert(16, 16);
    t->insert(11, 11);
    t->insert(17, 17);
    t->insert(18, 18);
    t->insert(19, 19);
    t->insert(20, 20);


    t->insert(21, 21);
    t->insert(22, 22);
    t->insert(23, 23);
    t->printTree();
    t->printList();

}

void testInsertBulk() {


}


void testDelete() {
    BPlusTree<int, int> *t = new BPlusTree<int, int>(5);
    t->insert(15, 15);
    t->insert(12, 12);
    t->insert(13, 13);
    t->insert(14, 14);
    t->insert(16, 16);
    t->insert(11, 11);
    t->insert(17, 17);
    t->insert(18, 18);
    t->insert(19, 19);
    t->insert(20, 20);
    t->insert(21, 21);
    t->insert(22, 22);
    t->insert(23, 23);
    t->insert(24, 24);

    t->remove(11);
    t->remove(12);
    t->remove(24);
    t->remove(23);
    t->remove(22);
    t->remove(21);
    t->remove(20);
    t->remove(17);
    t->remove(18);
    t->remove(19);
    t->remove(16);
    t->remove(15);
    t->remove(14);
    t->remove(13);
    t->printTree();
    t->printList();
}
