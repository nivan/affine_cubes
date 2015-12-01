#include "boundingvolume.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;


/**************
 * HyperPlane *
 **************/

HyperPlane::HyperPlane()
{}

HyperPlane::HyperPlane(Vector &normalVector, double &intercept):
    normalVector(normalVector), intercept(intercept)
{}

int HyperPlane::numDimensions(){
    return this->normalVector.getDimension();
}

/*****************************
 * AxisAlignedBoundingVolume *
 *****************************/

AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<std::pair<double, double> > &dimensionBounds):
    dimensionBounds(dimensionBounds)
{}

int AxisAlignedBoundingVolume::numPlanes(){
    int numDimensions = dimensionBounds.size();
    return 2 * numDimensions;
}



HyperPlane AxisAlignedBoundingVolume::getHyperPlane(int i){
    assert(0 <= i && i < this->numPlanes());
    int numDimensions = dimensionBounds.size();
    int dimensionIndex = i % numDimensions;
    bool leftPlane = i < numDimensions;
    //enumerate first the negative face planes
    double coeff = 1.0;
    if(leftPlane)
        coeff = -1.0;


    Vector normalVector(numDimensions);
    normalVector[dimensionIndex] = coeff;

    //
    double intercept;
    if(leftPlane)
        intercept = dimensionBounds.at(dimensionIndex).first;
    else
        intercept = dimensionBounds.at(dimensionIndex).second;

    //
    return HyperPlane(normalVector,intercept);
}

string AxisAlignedBoundingVolume::toString(){
    stringstream ss;
    int numPlanes = this->numPlanes();
    ss << "Number of bounding planes " <<  numPlanes << endl;
    for(int i = 0 ; i < numPlanes ; ++i){
        HyperPlane plane = this->getHyperPlane(i);
        ss << "Hyper plane " << i << endl;
        ss << "   " << plane.normalVector.toString() << endl;
        ss << "   " << plane.intercept << endl;

    }

    return ss.str();
}

AxisAlignedBoundingVolume* AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(std::vector<Vector>& points){
    int numPoints = points.size();
    assert(numPoints > 0);

    Vector& firstPoint = points.at(0);
    int numDimensions = firstPoint.getDimension();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
    vector<pair<double,double> > bounds(numDimensions,initialLimits);

    for(int i = 0 ; i < numPoints ; ++i){
        Vector& point = points.at(i);
        for(int d = 0 ; d < numDimensions ; ++d){
            pair<double,double> & dimBounds = bounds.at(d);
            double value = point[d];
            if(value < dimBounds.first)
                dimBounds.first = value;
            if(value > dimBounds.second)
                dimBounds.second = value;
        }
    }

    return new AxisAlignedBoundingVolume(bounds);
}

AxisAlignedBoundingVolume *AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(std::vector<Vector> &points, int startIndex, int endIndex){
    assert(startIndex < endIndex);
    Vector& firstPoint = points.at(startIndex);
    int numDimensions = firstPoint.getDimension();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
    vector<pair<double,double> > bounds(numDimensions,initialLimits);

    for(int i = startIndex ; i < endIndex ; ++i){
        Vector& point = points.at(i);
        for(int d = 0 ; d < numDimensions ; ++d){
            pair<double,double> & dimBounds = bounds.at(d);
            double value = point[d];
            if(value < dimBounds.first)
                dimBounds.first = value;
            if(value > dimBounds.second)
                dimBounds.second = value;
        }
    }

    return new AxisAlignedBoundingVolume(bounds);
}


void AxisAlignedBoundingVolume::getDotProductRangeInVolume(const Vector& direction,double& minDotProduct,double& maxDotProduct){
    int numDimensions = this->dimensionBounds.size();
    assert(numDimensions == direction.getDimension());

    minDotProduct = 0.0;
    maxDotProduct = 0.0;

    for(int i = 0 ; i < numDimensions ; ++i){
        pair<double,double> bounds = dimensionBounds.at(i);
        double value = direction[i];
        if(value > 0){
            minDotProduct += bounds.first * value;
            maxDotProduct += bounds.second * value;
        }
        else{
            minDotProduct += bounds.second * value;
            maxDotProduct += bounds.first * value;
        }
    }
}

Vector AxisAlignedBoundingVolume::getDirectionOfLargestVariance(){
    int numDimensions = dimensionBounds.size();
    double widestIntervalSize = 0;
    int dimIndex = -1;
    for(int i = 0 ; i < numDimensions ; ++i){
        pair<double,double> dimBounds = dimensionBounds.at(i);
        double intervalSize = dimBounds.second - dimBounds.first;
        if(intervalSize > widestIntervalSize){
            dimIndex = i;
            widestIntervalSize = intervalSize;
        }
    }

    Vector result(numDimensions);
    result[dimIndex] = 1;
    return result;
}

/*****************
 * Test Function *
 *****************/

void testBoundingVolume()
{
    //
    std::vector<std::pair<double,double> > bounds;
    bounds.push_back(make_pair(-3.0,5.0));
    bounds.push_back(make_pair(1.0,10.0));
    bounds.push_back(make_pair(2.5,12.9));

    AxisAlignedBoundingVolume bv(bounds);
    cout << bv.toString() << endl;

    //
    Vector v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Vector v2(3);
    v2[0] = -2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Vector v3(3);

    vector<Vector> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    bv = *AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(points);
    cout << bv.toString() << endl;

    //
    double minProduct;
    double maxProduct;
    bv.getDotProductRangeInVolume(v1,minProduct,maxProduct);
    cout << "min product = " << minProduct << " maxProduct = " << maxProduct << endl;
}
