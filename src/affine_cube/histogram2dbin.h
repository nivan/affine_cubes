#ifndef HISTOGRAM2DBIN_H
#define HISTOGRAM2DBIN_H

#include <stdint.h>
#include <string>

class   Histogram2DBin{
public:
    double minX;
    double maxX;
    double minY;
    double maxY;
    uint64_t count;
public:
    Histogram2DBin();
    Histogram2DBin(double minX, double maxX, double minY, double maxY, uint64_t count);
    std::string toString();
public:
    double getDataDensity();
};

#endif // HISTOGRAM2DBIN_H
