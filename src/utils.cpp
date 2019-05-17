#include "utils.h"



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

int *generateRandomIntNumber(int num, int start, int gap){
    using namespace std;

    vector<int> temp;
    for (int i = 0; i < num; ++i) {
        temp.push_back(start + i * gap);
    }
    random_shuffle(temp.begin(), temp.end());

    int *arr = new int[temp.size()];
    for (int i = 0; i < temp.size(); i++) {
        arr[i] = temp[i];
    }
    return arr;
}
