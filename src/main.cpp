#include "test.h"

using namespace std;

#ifndef TEST_DATA_NUM
#define TEST_DATA_NUM 5595
#endif


int main(int argc, char *argv[]) {

    testInsertAndDelete(TEST_DATA_NUM);
    return 0;
}