#include "boundingvolume.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <limits>
#include "../miniball/miniballcomputation.h"
#include "auxfunctions.h"
#include "../pca/pca.h"

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

string HyperPlane::toString(){
    stringstream ss;
    ss << "Normal Vector: " << endl << this->normalVector << endl;
    ss << " intercep " << this->intercept;
    return ss.str();
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

/**************************
 * AxisAlignedBoundingBox *
 **************************/

AxisAlignedBoundingBox::AxisAlignedBoundingBox(std::vector<std::pair<double, double> > &dimensionBounds):
    dimensionBounds(dimensionBounds)
{}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(std::vector<Eigen::VectorXd> &points){
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


AxisAlignedBoundingBox::AxisAlignedBoundingBox(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex){
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

AxisAlignedBoundingBox::~AxisAlignedBoundingBox(){

}

int AxisAlignedBoundingBox::numPlanes(){
    int numDimensions = dimensionBounds.size();
    return 2 * numDimensions;
}



HyperPlane AxisAlignedBoundingBox::getHyperPlane(int i){
    assert(0 <= i && i < this->numPlanes());
    int numDimensions = dimensionBounds.size();
    int dimensionIndex = i % numDimensions;
    bool leftPlane = i < numDimensions;
    //enumerate first the negative face planes
    double coeff = 1.0;

    Eigen::VectorXd normalVector = Eigen::VectorXd::Zero(numDimensions);
    normalVector[dimensionIndex] = coeff;

    //
    double intercept;
    if(leftPlane)
        intercept = -dimensionBounds.at(dimensionIndex).first;
    else
        intercept = -dimensionBounds.at(dimensionIndex).second;

    //
    return HyperPlane(normalVector,intercept);
}

string AxisAlignedBoundingBox::toString(){
    stringstream ss;
    int numDimensions = this->dimensionBounds.size();
    ss << "Axis Aligned BV dim = " << numDimensions << " : ";
    for(int i = 0 ; i < numDimensions ; ++i){
        pair<double,double> myLimits = this->dimensionBounds.at(i);
        ss << "[" << myLimits.first << ", " << myLimits.second << "] X ";

    }

    return ss.str();
}

AxisAlignedBoundingBox* AxisAlignedBoundingBox::getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points){
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

    return new AxisAlignedBoundingBox(bounds);
}

AxisAlignedBoundingBox *AxisAlignedBoundingBox::getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd> &points, int startIndex, int endIndex){
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

    return new AxisAlignedBoundingBox(bounds);
}


void AxisAlignedBoundingBox::getDotProductRangeInVolume(const Eigen::VectorXd& direction,double& minDotProduct,double& maxDotProduct){
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

Eigen::VectorXd AxisAlignedBoundingBox::getDirectionOfLargestVariance(){
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

/**********************
 * RotatedBoundingBox *
 ***********************/


RotatedBoundingBox::RotatedBoundingBox(std::vector<Eigen::VectorXd>& points){
    init(points,0,points.size());
}

RotatedBoundingBox::RotatedBoundingBox(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex){
    init(points,beginIndex,endIndex);
}

RotatedBoundingBox::~RotatedBoundingBox(){

}

void RotatedBoundingBox::init(std::vector<Eigen::VectorXd> &points, int beginIndex, int endIndex){
    //
    int numPoints = endIndex - beginIndex;
    assert(numPoints > 0);

    Eigen::VectorXd& firstPoint = points.at(beginIndex);
    int numDimensions = firstPoint.size();

    //
    bool success = PCA::performPCA(points,beginIndex,endIndex,this->rotationMatrix);
    if(!success){
        this->rotationMatrix = Eigen::MatrixXd::Identity(numDimensions,numDimensions);
    }

    pair<double,double> initialLimits = make_pair(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    //
    this->dimensionBounds = vector<pair<double,double> >(numDimensions,initialLimits);

    for(int i = beginIndex ; i < endIndex ; ++i){
        Eigen::VectorXd& initialpoint = points.at(i);
        Eigen::VectorXd  point =  initialpoint.transpose() * rotationMatrix; // change to the box basis
        //cout << "Intial Point " << endl << initialpoint << endl;
        //cout << " Transformed "<< endl << point << endl;
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


std::string RotatedBoundingBox::toString(){
    //TODO
    stringstream ss;
    int numPlanes = this->numPlanes();
    ss << "rotated box: num planes " << numPlanes << endl;
    ss << "rotation Matrix " << endl;
    ss << this->rotationMatrix << endl << endl;
    for(int i = 0 ; i < numPlanes ; ++i){
        HyperPlane plane = this->getHyperPlane(i);
        ss << "   Plane " << i << " : " << endl;
        ss << plane.toString() << endl;
    }
    return ss.str();
}

int RotatedBoundingBox::numPlanes(){
    int numDimensions = dimensionBounds.size();
    return 2 * numDimensions;
}

HyperPlane RotatedBoundingBox::getHyperPlane(int i){
    //TODO
    assert(0 <= i && i < this->numPlanes());
    int numDimensions = dimensionBounds.size();
    int dimensionIndex = i % numDimensions;
    bool leftPlane = i < numDimensions;

    //enumerate first the negative face planes
    Eigen::VectorXd normalVector = this->rotationMatrix.col(dimensionIndex);

    //
    double intercept;
    if(leftPlane)
        intercept = -dimensionBounds.at(dimensionIndex).first;
    else
        intercept = -dimensionBounds.at(dimensionIndex).second;

    //
    return HyperPlane(normalVector,intercept);
}

void RotatedBoundingBox::getDotProductRangeInVolume(const Eigen::VectorXd& direction,double& minDotProduct,double& maxDotProduct){

    //
    int numDimensions = this->dimensionBounds.size();
    assert(numDimensions == direction.size());
    Eigen::VectorXd  directionInTheBoxCoords =  direction.transpose() * rotationMatrix;

    minDotProduct = 0.0;
    maxDotProduct = 0.0;

    for(int i = 0 ; i < numDimensions ; ++i){
        pair<double,double> bounds = dimensionBounds.at(i);
        double value = directionInTheBoxCoords[i];
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

Eigen::VectorXd RotatedBoundingBox::getDirectionOfLargestVariance(){
    return this->rotationMatrix.col(0);
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

    AxisAlignedBoundingBox bv(bounds);
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
    bv = *AxisAlignedBoundingBox::getAxisAlignedBoundingVolume(points);
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

    Eigen::VectorXd v3 = Eigen::VectorXd::Zero(3);

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

void testRoatedBox(){
    Eigen::VectorXd v1(2);
    v1[0] = -1.0;
    v1[1] = -1.0;

    Eigen::VectorXd v2(2);
    v2[0] = 1.0;
    v2[1] = 1.0;

    Eigen::VectorXd v3(2);
    v3[0] = -0.5;
    v3[1] = 0.5;

    Eigen::VectorXd v4(2);
    v4[0] = 0.5;
    v4[1] = -0.5;

    vector<Eigen::VectorXd> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    points.push_back(v4);

    Eigen::VectorXd probeDirection(2);
    probeDirection[0] = -50.0;
    probeDirection[1] =  50.0;
    double minV,maxV;

    AxisAlignedBoundingBox axisBBox(points);
    axisBBox.getDotProductRangeInVolume(probeDirection,minV,maxV);
    cout << "Axis Aligned BBOX" << endl;
    cout << axisBBox.toString();
    cout << "inner product bounds [" << minV << " , " << maxV << "]" << endl;

    RotatedBoundingBox bbox(points);
    bbox.getDotProductRangeInVolume(probeDirection,minV,maxV);
    cout << "Rotated BBOX" << endl;
    cout << bbox.toString() << endl;
    cout << "inner product bounds [" << minV << " , " << maxV << "]" << endl;


}
