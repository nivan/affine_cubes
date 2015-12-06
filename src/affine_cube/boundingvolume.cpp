#include "boundingvolume.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <limits>
#include "../miniball/miniballcomputation.h"

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

/**********
 * Sphere *
 **********/

Sphere::Sphere():
    center(NULL),
    radius(-1),
    directionOfLargestVariance(NULL)
{}

Sphere::Sphere(Vector& c,double r):
    directionOfLargestVariance(NULL){
    this->center = new Vector(c);
    this->radius = r;
}

Sphere::Sphere(std::vector<Vector>& points):
    directionOfLargestVariance(NULL){
    if(points.size() > 0){
        int numDimensions = points.at(0).getDimension();
        this->center = new Vector(numDimensions);
        MiniBallComputation::computeMiniBall(points,*center,this->radius);
        //TODO: implement and set the direction of largest variance from points
    }
}

Sphere::Sphere(std::vector<Vector>& points, int beginIndex, int endIndex):
    directionOfLargestVariance(NULL){
    int numPoints = endIndex - beginIndex;
    if(numPoints > 0){
        int numDimensions = points.at(beginIndex).getDimension();
        this->center = new Vector(numDimensions);
        std::vector<Vector> usedPoints(points.begin() + beginIndex, points.begin() + endIndex);
        MiniBallComputation::computeMiniBall(points,*center,this->radius);
        //TODO: implement and set the direction of largest variance from points
    }
}


Sphere::~Sphere(){
    if(this->center)
        delete this->center;
}

string Sphere::toString(){
    stringstream ss;
    if(this->center)
        ss << "Sphere Center " << this->center->toString() << " radius " << this->radius;
    else
        ss << "Sphere Center NULL radius "  << this->radius;

    return ss.str();
}

Vector Sphere::getCenter(){
    return *this->center;
}

double Sphere::gerRadius(){
    return this->radius;
}

bool Sphere::contains(const Vector& point){
    if(!this->center)
        return false;

    Vector x = (*center) - point;
    return ( x.length2() <= (radius * radius) );
}

void Sphere::getDotProductRangeInVolume(const Vector& direction, double& minV, double& maxV){
    double centerProduct = this->center->dot(direction);
    double vNorm = direction.length();
    //
    minV = centerProduct - this->radius * vNorm;
    maxV = centerProduct + this->radius * vNorm;
}

Vector Sphere::getDirectionOfLargestVariance(){
    if(this->directionOfLargestVariance){
        return *directionOfLargestVariance;
    }
    else{
        //return random axis aligned direction
        assert(this->center);
        int numDimensions = this->center->getDimension();
        int axis = rand() % numDimensions;
        Vector dir(numDimensions);
        dir[axis] = 1.0;
        return dir;
    }
}

/*****************************
 * AxisAlignedBoundingVolume *
 *****************************/

AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<std::pair<double, double> > &dimensionBounds):
    dimensionBounds(dimensionBounds)
{}

AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<Vector> &points){
    int numPoints = points.size();
    assert(numPoints > 0);

    Vector& firstPoint = points.at(0);
    int numDimensions = firstPoint.getDimension();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
    //
    this->dimensionBounds = vector<pair<double,double> >(numDimensions,initialLimits);

    for(int i = 0 ; i < numPoints ; ++i){
        Vector& point = points.at(i);
        for(int d = 0 ; d < numDimensions ; ++d){
            pair<double,double> & dimBounds = this->dimensionBounds.at(d);
            double value = point[d];
            if(value < dimBounds.first)
                dimBounds.first = value;
            if(value > dimBounds.second)
                dimBounds.second = value;
        }
    }
}


AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<Vector>& points, int beginIndex, int endIndex){
    int numPoints = endIndex - beginIndex;
    assert(numPoints > 0);

    Vector& firstPoint = points.at(beginIndex);
    int numDimensions = firstPoint.getDimension();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),std::numeric_limits<double>::min());
    //
    this->dimensionBounds = vector<pair<double,double> >(numDimensions,initialLimits);

    for(int i = beginIndex ; i < endIndex ; ++i){
        Vector& point = points.at(i);
        for(int d = 0 ; d < numDimensions ; ++d){
            pair<double,double> & dimBounds = this->dimensionBounds.at(d);
            double value = point[d];
            if(value < dimBounds.first)
                dimBounds.first = value;
            if(value > dimBounds.second)
                dimBounds.second = value;
        }
    }
}

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
    int numDimensions = this->dimensionBounds.size();
    ss << "Axis Aligned BV dim = " << numDimensions << " : ";
    for(int i = 0 ; i < numDimensions ; ++i){
        pair<double,double> myLimits = this->dimensionBounds.at(i);
        ss << "[" << myLimits.first << ", " << myLimits.second << "] X ";

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


void testSphere(){

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

    Vector center(3);
    double radius;
    MiniBallComputation::computeMiniBall(points,center,radius);

    cout << "Result of MiniBall" << endl;
    cout << "    Center " << center.toString() << endl;
    cout << "    radius " << radius << endl;
}


