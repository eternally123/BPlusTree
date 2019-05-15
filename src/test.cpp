#include "test.h"


void testInsertAndDelete(int testSet) {
    using namespace std;

    int number = testSet;
    double *arr = generateRandomDoubuleNumber(number, 10, 0.1);
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

void testSerialization(int testSet) {
    using namespace std;
    char *filePath = "/home/shiliangw/user/code/BPlusTree/bin/serializationData";

    int number = testSet;
    double *arr = generateRandomDoubuleNumber(number, 10, 0.1);
    BPlusTree<double, double> *bp = new BPlusTree<double, double>(5);
    for (int i = 0; i < testSet; ++i) {
        bp->insert(arr[i], arr[i]);
    }
    bp->printTree();
    bp->serialization(filePath);

    delete[] arr;
}

void testUnSerialization() {
    using namespace std;
    char *filePath = "/home/shiliangw/user/code/BPlusTree/bin/serializationData";

    BPlusTree<double, double> *bp = new BPlusTree<double, double>(5);
    bp->unSerialization(filePath);
    cout << endl << endl;
    bp->printTree();
    cout << endl << endl;
    cout << endl << endl;
    cout << endl << endl;
    cout << endl << endl;

    bp->printList();
}


void testMutex() {
    using namespace std;
    BPlusTree<double, double> *pBp = new BPlusTree<double, double>(5);
    mutex *mut=new mutex;


    thread t1(testMutexThreadOne, pBp, mut);
    thread t2(testMutexThreadTwo, pBp, mut);

    t1.join();
    t2.join();

    delete pBp;
    delete mut;

}



void testMutexThreadOne(BPlusTree<double,double> *pBp, std::mutex *mut) {




}


void testMutexThreadTwo(BPlusTree<double,double> *pBp, std::mutex *mut) {


}