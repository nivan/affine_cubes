#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>

class Interval
{
public:
    enum IntersectionType{ DISJOINT, CONTAINS, IS_CONTAINED, OVERLAPS};
public:
    double minValue;
    double maxValue;
public:
    Interval();
    Interval(double minV, double maxV);
public:
    inline bool contains(double value) const{
        return (minValue <= value && value <= maxValue);
    }
    inline void addPoint(double v){
        if(v < minValue)
            minValue = v;
        if(v > maxValue)
            maxValue = v;
    }
    inline IntersectionType testIntersection(Interval& myInterval){
        if(this->minValue > myInterval.maxValue || this->maxValue < myInterval.minValue)
            return DISJOINT;
        else if(this->minValue <= myInterval.minValue && myInterval.maxValue <= this->maxValue)
            return CONTAINS;
        else if(myInterval.minValue <= this->minValue && this->maxValue <= myInterval.maxValue)
            return IS_CONTAINED;
        else
            return OVERLAPS;
    }
public:
    std::string toString();
};

void testInterval();

#endif // INTERVAL_H
