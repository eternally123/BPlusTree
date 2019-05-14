#include "utils.h"

#include <algorithm>
#include <vector>
#include <iostream>

double *generateRandomDoubuleNumber(int num, double start, double gap) {
    using namespace std;

    vector<double> temp;
    for (int i = 0; i < num; ++i) {
        temp.push_back(start + i * gap);
    }
    random_shuffle(temp.begin(), temp.end());

    double *arr = new double[temp.size()];
    for (int i = 0; i < temp.size(); i++) {
        arr[i] = temp[i];
    }
    return arr;
}