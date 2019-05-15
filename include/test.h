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

void testInsertAndDelete(int testSet);

void testSerialization(int testSet);

void testUnSerialization();

void testMutex();

void testMutexThreadOne(BPlusTree<double, double> *pBp, std::mutex *mut);

void testMutexThreadTwo(BPlusTree<double, double> *pBp, std::mutex *mut);

#endif //BPLUSTREE_TEST_H
