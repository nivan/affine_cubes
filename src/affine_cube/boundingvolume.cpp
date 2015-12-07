#include "boundingvolume.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <limits>
#include "../miniball/miniballcomputation.h"
#include "auxfunctions.h"

using namespace std;


/**************
 * HyperPlane *
 **************/

HyperPlane::HyperPlane()
{}

HyperPlane::HyperPlane(Eigen::VectorXd &normalVector, double &intercept):
    normalVector(normalVector), intercept(intercept)
{}

int HyperPlane::numDimensions(){
    return this->normalVector.size();
}

/**********
 * Sphere *
 **********/

Sphere::Sphere():
    center(NULL),
    radius(-1),
    directionOfLargestVariance(NULL)
{}

Sphere::Sphere(Eigen::VectorXd& c,double r):
    directionOfLargestVariance(NULL){
    this->center = new Eigen::VectorXd(c);
    this->radius = r;
}

Sphere::Sphere(std::vector<Eigen::VectorXd>& points):
    directionOfLargestVariance(NULL){
    if(points.size() > 0){
        int numDimensions = points.at(0).size();
        this->center = new Eigen::VectorXd(Eigen::VectorXd::Zero(numDimensions));
        MiniBallComputation::computeMiniBall(points,*center,this->radius);
        //TODO: implement and set the direction of largest variance from points
    }
}

Sphere::Sphere(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex):
    directionOfLargestVariance(NULL){
    int numPoints = endIndex - beginIndex;
    if(numPoints > 0){
        int numDimensions = points.at(beginIndex).size();//getDimension();
        this->center = new Eigen::VectorXd(Eigen::VectorXd::Zero(numDimensions));
        std::vector<Eigen::VectorXd> usedPoints(points.begin() + beginIndex, points.begin() + endIndex);
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
        ss << "Sphere Center " << vecToString(*center) << " radius " << this->radius;
    else
        ss << "Sphere Center NULL radius "  << this->radius;

    return ss.str();
}

Eigen::VectorXd Sphere::getCenter(){
    return *this->center;
}

double Sphere::gerRadius(){
    return this->radius;
}

bool Sphere::contains(const Eigen::VectorXd& point){
    if(!this->center)
        return false;

    Eigen::VectorXd x = (*center) - point;
    return ( x.squaredNorm() <= (radius * radius) );
}

void Sphere::getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV){
    double centerProduct = this->center->dot(direction);
    double vNorm = direction.norm();
    //
    minV = centerProduct - this->radius * vNorm;
    maxV = centerProduct + this->radius * vNorm;
}

Eigen::VectorXd Sphere::getDirectionOfLargestVariance(){
    if(this->directionOfLargestVariance){
        return *directionOfLargestVariance;
    }
    else{
        //return random axis aligned direction
        assert(this->center);
        int numDimensions = this->center->size();
        int axis = rand() % numDimensions;
        Eigen::VectorXd dir(numDimensions);
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

AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<Eigen::VectorXd> &points){
    int numPoints = points.size();
    assert(numPoints > 0);

    Eigen::VectorXd& firstPoint = points.at(0);
    int numDimensions = firstPoint.size();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    //
    this->dimensionBounds = vector<pair<double,double> >(numDimensions,initialLimits);

    for(int i = 0 ; i < numPoints ; ++i){
        Eigen::VectorXd& point = points.at(i);
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


AxisAlignedBoundingVolume::AxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex){
    int numPoints = endIndex - beginIndex;
    assert(numPoints > 0);

    Eigen::VectorXd& firstPoint = points.at(beginIndex);
    int numDimensions = firstPoint.size();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    //
    this->dimensionBounds = vector<pair<double,double> >(numDimensions,initialLimits);

    for(int i = beginIndex ; i < endIndex ; ++i){
        Eigen::VectorXd& point = points.at(i);
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


    Eigen::VectorXd normalVector = Eigen::VectorXd::Zero(numDimensions);
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

AxisAlignedBoundingVolume* AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points){
    int numPoints = points.size();
    assert(numPoints > 0);

    Eigen::VectorXd& firstPoint = points.at(0);
    int numDimensions = firstPoint.size();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    vector<pair<double,double> > bounds(numDimensions,initialLimits);

    for(int i = 0 ; i < numPoints ; ++i){
        Eigen::VectorXd& point = points.at(i);
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

AxisAlignedBoundingVolume *AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd> &points, int startIndex, int endIndex){
    assert(startIndex < endIndex);
    Eigen::VectorXd& firstPoint = points.at(startIndex);
    int numDimensions = firstPoint.size();
    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    vector<pair<double,double> > bounds(numDimensions,initialLimits);

    for(int i = startIndex ; i < endIndex ; ++i){
        Eigen::VectorXd& point = points.at(i);
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


void AxisAlignedBoundingVolume::getDotProductRangeInVolume(const Eigen::VectorXd& direction,double& minDotProduct,double& maxDotProduct){
    int numDimensions = this->dimensionBounds.size();
    assert(numDimensions == direction.size());

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

Eigen::VectorXd AxisAlignedBoundingVolume::getDirectionOfLargestVariance(){
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

    Eigen::VectorXd result = Eigen::VectorXd::Zero(numDimensions);
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
    Eigen::VectorXd v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Eigen::VectorXd v2(3);
    v2[0] = -2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Eigen::VectorXd v3(3);

    vector<Eigen::VectorXd> points;
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

    //
    Eigen::VectorXd vLV = bv.getDirectionOfLargestVariance();
    cout << "Direction " << vecToString(vLV) << endl;
}


void testSphere(){

    Eigen::VectorXd v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Eigen::VectorXd v2(3);
    v2[0] = -2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Eigen::VectorXd v3(3);

    vector<Eigen::VectorXd> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);

    Eigen::VectorXd center(3);
    double radius;
    MiniBallComputation::computeMiniBall(points,center,radius);

    cout << "Result of MiniBall" << endl;
    cout << "    Center " << vecToString(center) << endl;
    cout << "    radius " << radius << endl;
}


