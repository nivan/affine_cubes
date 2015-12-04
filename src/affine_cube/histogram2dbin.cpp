#include "histogram2dbin.h"
#include <sstream>

/******************
 * Histogram2DBin *
 ******************/

Histogram2DBin::Histogram2DBin():
    minX(0),
    maxX(-1),
    minY(0),
    maxY(0),
    count(0)
{}

Histogram2DBin::Histogram2DBin(double minX, double maxX, double minY, double maxY, uint64_t count):
    minX(minX),
    maxX(maxX),
    minY(minY),
    maxY(maxY),
    count(count)
{}

std::string Histogram2DBin::toString(){
    std::stringstream ss;
    ss << "(" << minX << "," << maxX << ") X (" << minY << "," << maxY << ") : " << count;
    return ss.str();
}
