#ifndef COMMON_H
#define COMMON_H

#include <cmath>
constexpr int naryIntersectionSize = 4;
constexpr double cachingProbability = 0.5;
constexpr int sampleSize = 1000;

constexpr int compare(int x, int y) {
    if(x < y)
        return -1;
    else if(x > y)
        return 1;
    else
        return 0;
};

constexpr int compare(double x, double y) {
    constexpr double eps = 1e-9;
    if(std::fabs(x - y) < eps)
        return 0;
    if(x < y)
        return -1;
    else
        return 0;
};

#endif