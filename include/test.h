#ifndef BPLUSTREE_TEST_H
#define BPLUSTREE_TEST_H

#include "BPlusTree.hpp"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <mutex>

void testInsert(int testSetNum);

void testDelete(int testSetNum);

void testSerialization(int testSetNum);

void testUnSerialization();

void testMutex();

void testNewAndDelete(int testSetNum);



#endif //BPLUSTREE_TEST_H
