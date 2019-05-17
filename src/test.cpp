#include "test.h"

#include <sys/time.h>

#ifndef DATA_GAP_OF_CACULATE_TIME
#define DATA_GAP_OF_CACULATE_TIME 1000000
#endif


void testMutexThreadOne(BPlusTree<int, int> *pBp);

void testMutexThreadTwo(BPlusTree<int, int> *pBp);


void testInsert(int testSetNum) {
    using namespace std;

    int *arr = generateRandomIntNumber(testSetNum, 10, 1);


    struct timeval start, tempt, end;
    int timeUse;
    //开始计时
    gettimeofday(&start, NULL);
    gettimeofday(&tempt, NULL);


    cout << "begin to insert" << endl;
    BPlusTree<int, int> *bp = new BPlusTree<int, int>(50);
    //插入数据并统计时间
    for (int i = 1; i <= testSetNum; ++i) {
        bp->insert(arr[i - 1], arr[i - 1]);
        if (i % DATA_GAP_OF_CACULATE_TIME == 0) {
            //每100w条数据统计一次时间
            gettimeofday(&end, NULL);
            timeUse = 1000000 * (end.tv_sec - tempt.tv_sec) + end.tv_usec - tempt.tv_usec;
            gettimeofday(&tempt, NULL);
            cout << "time of insert " << DATA_GAP_OF_CACULATE_TIME / 10000 << "W pieces of data(exists:"
                 << (i - DATA_GAP_OF_CACULATE_TIME) / 10000 << "W):" << timeUse << " us" << endl;
        }
    }
    int totalCost = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "total time of insert " << testSetNum / 1000000 << "W data " << totalCost << " us" << endl;
    //验证插入正确性
    cout << endl;
    bool ifCanFindAllData = true;
    for (int i = 0; i < testSetNum; ++i) {
        if (bp->search(arr[i]) == NULL) {
            ifCanFindAllData = false;
            break;
        }
    }
    cout << "if can find all data: " << ifCanFindAllData << endl;

    delete arr;
}

void testDelete(int testSetNum) {
    using namespace std;


    int *arr = generateRandomIntNumber(testSetNum, 10, 1);
    BPlusTree<int, int> *bp = new BPlusTree<int, int>(50);
    cout << "begin to insert" << endl;
    for (int i = 1; i <= testSetNum; ++i) {
        bp->insert(arr[i - 1], arr[i - 1]);
    }
    cout << "finish insert" << endl;
    cout << "begin to delete" << endl;

    //开始删除
    struct timeval start, tempt, end;
    int timeUse;
    //开始计时
    gettimeofday(&start, NULL);
    gettimeofday(&tempt, NULL);


    //删除数据并统计时间
    for (int i = 1; i <= testSetNum; ++i) {
        bp->remove(arr[i - 1]);
        if (i % DATA_GAP_OF_CACULATE_TIME == 0) {
            //每100w条数据统计一次时间
            gettimeofday(&end, NULL);
            timeUse = 1000000 * (end.tv_sec - tempt.tv_sec) + end.tv_usec - tempt.tv_usec;
            gettimeofday(&tempt, NULL);
            cout << "time of delete " << DATA_GAP_OF_CACULATE_TIME / 10000 << "W pieces of data(exists:"
                 << testSetNum / 10000 - (i - DATA_GAP_OF_CACULATE_TIME) / 10000 << "W):" << timeUse << " us" << endl;
        }
    }
    int totalCost = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "total time of delete " << testSetNum / 1000000 << "W data " << totalCost << " us" << endl;


    delete[] arr;
    bp->printTree();
    bp->printList();
}


void testSerialization(int testSetNum) {
    using namespace std;
    char *filePath = "serializationData";

    int number = testSetNum;
    int *arr = generateRandomIntNumber(number, 10, 1);
    BPlusTree<int, int> *bp = new BPlusTree<int, int>(50);
    cout << "begin to build tree(total data: " << testSetNum / 10000 << "W pieces)" << endl;
    for (int i = 0; i < testSetNum; ++i) {
        bp->insert(arr[i], arr[i]);
    }
    cout << "finish building" << endl;
    cout << "begin to serialize" << endl;


    struct timeval start, end;
    int timeUse;
    //开始计时
    gettimeofday(&start, NULL);
    bp->serialization(filePath);
    gettimeofday(&end, NULL);
    timeUse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "finish serializing,time:" << timeUse << "us" << endl;
    delete[] arr;
//    bp->printTree();
}

void testUnSerialization() {
    using namespace std;
    char *filePath = "/home/shiliangw/user/code/BPlusTree/bin/serializationData";
    BPlusTree<int, int> *bp = new BPlusTree<int, int>(50);
    struct timeval start, end;
    int timeUse;
    //开始计时
    gettimeofday(&start, NULL);
    bp->unSerialization(filePath);
    gettimeofday(&end, NULL);
    timeUse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "finish unSerializing,time:" << timeUse << "us" << endl;
}


void testMutex() {
    using namespace std;
    BPlusTree<int, int> *pBp = new BPlusTree<int, int>(50);
    thread t1(testMutexThreadOne, pBp);
    thread t2(testMutexThreadTwo, pBp);
    t1.join();
    t2.join();


//    Node<int, int> *p = pBp->getPListStart();
//    while (p != NULL) {
//        for (int i = 0; i < p->getCurrentKeyNum(); ++i) {
//            cout << p->getPKey()[i] << " ";
//        }
//        p = p->getPNextLeafNode();
//    }
    delete pBp;
}


void testMutexThreadOne(BPlusTree<int, int> *pBp) {
    using namespace std;
    int testDataNum = 5000000;
    cout << "thread 1 begin to insert" << endl;
    int *arr = generateRandomIntNumber(testDataNum, 10, 1);
    for (int i = 1; i <= testDataNum; ++i) {
        pBp->insert(arr[i - 1], arr[i - 1]);
    }
    cout << "thread 1 finish inserting" << endl;
    delete[] arr;
}


void testMutexThreadTwo(BPlusTree<int, int> *pBp) {
    using namespace std;
    int testDataNum = 5000000;
    cout << "thread 2 begin to insert" << endl;
    int *arr = generateRandomIntNumber(testDataNum, 10, 1);
    for (int i = 1; i <= testDataNum; ++i) {
        pBp->insert(arr[i - 1], arr[i - 1]);
    }
    cout << "thread 2 finish inserting" << endl;
    delete[] arr;

}


void testNewAndDelete(int testSetNum) {
    using namespace std;

    int *arr = generateRandomIntNumber(testSetNum, 10, 1);
    BPlusTree<int, int> *bp = new BPlusTree<int, int>(50);

    cout << "begin to insert" << endl;
    //插入数据并统计时间
    for (int i = 1; i <= testSetNum; ++i) {
        bp->insert(arr[i - 1], arr[i - 1]);
    }
    cout << "finish inserting" << endl;

    sleep(10);

    cout << "begin to delete" << endl;
    for (int i = 1; i <= testSetNum; ++i) {
        bp->remove(arr[i - 1]);
    }
    cout << "finish deleting" << endl;

    sleep(10);


    cout << "begin to insert" << endl;
    //插入数据并统计时间
    for (int i = 1; i <= testSetNum; ++i) {
        bp->insert(arr[i - 1], arr[i - 1]);
    }
    cout << "finish inserting" << endl;


    sleep(10);

    delete arr;

}
