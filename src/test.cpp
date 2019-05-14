#include "test.h"

void testInsertAndDelete(int testSet){
    using namespace std;

    int number = testSet;
    double *arr = generateRandomDoubuleNumber(number,10,0.1);
    BPlusTree<double, double> *bp = new BPlusTree<double, double>(5);
    for (int i = 0; i < testSet; ++i) {
        bp->insert(arr[i], arr[i]);
    }

    for (int i = 0; i < testSet; ++i) {
        bool result = bp->remove(arr[i]);
        if (result == false) {
            cout << "删除:" << arr[i] << "时出错" << endl;
        }
    }
    delete[] arr;
    bp->printTree();
    bp->printList();
}
