#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <vector>
//#include "Vector.h"
#include <eigen3/Eigen/Dense>

/**************
 * HyperPlane *
 **************/

class HyperPlane{
public:
    Eigen::VectorXd normalVector;
    double intercept;
public:
    HyperPlane();
    HyperPlane(Eigen::VectorXd& normalVector, double& intercept);
    int numDimensions();
};

/******************
 * BoundingVolume *
 ******************/

class BoundingVolume{
public:
    virtual ~BoundingVolume() {};
    virtual void getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV) = 0;
    virtual Eigen::VectorXd getDirectionOfLargestVariance() = 0;
};

/**********
 * Sphere *
 **********/

class Sphere: public BoundingVolume{
private:
    Eigen::VectorXd* center;
    double  radius;
    Eigen::VectorXd* directionOfLargestVariance;
public:
    Sphere();
    Sphere(Eigen::VectorXd&,double);
    Sphere(std::vector<Eigen::VectorXd>&);
    Sphere(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex);
    template<class iterator_type>
    Sphere(iterator_type beginIterator, iterator_type endIterator);
    ~Sphere();
     std::string toString();
public:
    Eigen::VectorXd getCenter();
    double gerRadius();
    bool contains(const Eigen::VectorXd& point);
public:
    void getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV);
    Eigen::VectorXd getDirectionOfLargestVariance();
};

/************
 * Polytope *
 ************/

class Polytope: public BoundingVolume {
public:
    virtual int numPlanes() {return 0;};
    virtual HyperPlane getHyperPlane(int i) = 0;
};

/*****************************
 * AxisAlignedBoundingVolume *
 *****************************/

class AxisAlignedBoundingVolume: public Polytope{
private:
    std::vector<std::pair<double,double> > dimensionBounds;
public:
    AxisAlignedBoundingVolume(std::vector<std::pair<double,double> >& dimensionBounds);
    AxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points);
    AxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex);
    std::string toString();
public:
    int numPlanes();
    HyperPlane getHyperPlane(int i);
    void getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV);
    Eigen::VectorXd getDirectionOfLargestVariance();
public:
    static AxisAlignedBoundingVolume* getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points);
    static AxisAlignedBoundingVolume* getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points,int startIndex, int endIndex);
};

/*****************************
 * AxisAlignedBoundingVolume *
 *****************************/

/*****************
 * Aux Functions *
 *****************/

void testBoundingVolume();
void testSphere();

#endif // BOUNDINGVOLUME_H
