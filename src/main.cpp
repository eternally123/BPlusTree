#include "test.h"

#include <climits>

using namespace std;

#ifndef TEST_DATA_NUM
#define TEST_DATA_NUM 50000000
#endif


int main(int argc, char *argv[]) {

//    cout << INT_MAX << " " << INT_MIN << endl;



//    testInsert(TEST_DATA_NUM);
//    testDelete(TEST_DATA_NUM);
//    testSerialization(TEST_DATA_NUM);
//    testUnSerialization();
//    testMutex();
    testNewAndDelete(TEST_DATA_NUM);
    return 0;


}