#include "interval.h"
#include <limits>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

Interval::Interval():
    minValue(std::numeric_limits<double>::max()),
    maxValue(-std::numeric_limits<double>::max())
{}

Interval::Interval(double minV, double maxV):
    minValue(minV),
    maxValue(maxV)
{}

std::string Interval::toString(){
    std::stringstream ss;
    ss << "[" << this->minValue << " , " << this->maxValue << "]";
    return ss.str();
}

std::string intersectionToString(Interval::IntersectionType type){
    switch(type){
    case Interval::DISJOINT:
        return "DISJOINT";
        break;
    case Interval::CONTAINS:
        return "CONTAINS";
        break;
    case Interval::IS_CONTAINED:
        return "IS_CONTAINED";
        break;
    case Interval::OVERLAPS:
        return "OVERLAPS";
        break;
    }

    assert(false);
}

void testInterval(){
    cout << "**** TEST CONTAINS" << endl;

    Interval int1(0,1);
    double values[] = {0.5,-1,0,1,0.3,0.75,1,1.1};
    for(int i = 0 ; i < 8 ; ++i){
        double v = values[i];
        cout << "Interval " << int1.toString() << " contains value " << v << " = " << int1.contains(v) << endl;

    }

    cout << "**** TEST ADD POINT" << endl;

    cout << "Interval " << int1.toString() << endl;
    for(int i = 0 ; i < 8 ; ++i){
        double v = values[i];
        int1.addPoint(v);
        cout << "After adding value " << v << " Interval " << int1.toString() << endl;

    }

    cout << "**** TEST INTERSECTION" << endl;

    vector<Interval> intervals;
    intervals.push_back(Interval(0,1));
    intervals.push_back(Interval(0.5,1));
    intervals.push_back(Interval(2,3));
    intervals.push_back(Interval(-1,1));
    intervals.push_back(Interval(-2,-1));

    int numIntervals = intervals.size();
    for(int i = 0 ; i < numIntervals ; ++i){
        Interval inti = intervals.at(i);
        for(int j = i ; j < numIntervals ; ++j){
            Interval intj = intervals.at(j);
            cout << "Intersection of " << inti.toString() << " and " << intj.toString() << " is " << intersectionToString(inti.testIntersection(intj)) << endl;
        }
    }
}
