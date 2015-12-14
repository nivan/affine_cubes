#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <vector>
//#include "Vector.h"
#include <eigen3/Eigen/Dense>

enum BoundingVolumeType {AXIS_ALIGNED_BBOX, ROTATED_BBOX, SPHERE};

/**************
 * HyperPlane *
 **************/

class HyperPlane{
public:
    //{x | <normalVector,x> + intecept = 0}
    Eigen::VectorXd normalVector;
    double intercept;
public:
    HyperPlane();
    HyperPlane(Eigen::VectorXd& normalVector, double& intercept);
    int numDimensions();
    std::string toString();
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

class AxisAlignedBoundingBox: public Polytope{
private:
    std::vector<std::pair<double,double> > dimensionBounds;
public:
    AxisAlignedBoundingBox(std::vector<std::pair<double,double> >& dimensionBounds);
    AxisAlignedBoundingBox(std::vector<Eigen::VectorXd>& points);
    AxisAlignedBoundingBox(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex);
    ~AxisAlignedBoundingBox();
    std::string toString();
public:
    int numPlanes();
    HyperPlane getHyperPlane(int i);
    void getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV);
    Eigen::VectorXd getDirectionOfLargestVariance();
public:
    static AxisAlignedBoundingBox* getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points);
    static AxisAlignedBoundingBox* getAxisAlignedBoundingVolume(std::vector<Eigen::VectorXd>& points,int startIndex, int endIndex);
};

/**********************
 * RotatedBoundingBox *
 ***********************/

class RotatedBoundingBox: public Polytope{
private:
    Eigen::MatrixXd                        rotationMatrix; //left multiplication transforms the canonical basis in the basis for this bounding box
    std::vector<std::pair<double,double> > dimensionBounds; //limits of the bounding box in the rotated coordinates
private:
    void init(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex);
public:
    RotatedBoundingBox(std::vector<Eigen::VectorXd>& points);
    RotatedBoundingBox(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex);
    ~RotatedBoundingBox();
    std::string toString();
public:
    int numPlanes();
    HyperPlane getHyperPlane(int i);
    void getDotProductRangeInVolume(const Eigen::VectorXd& direction, double& minV, double& maxV);
    Eigen::VectorXd getDirectionOfLargestVariance();
};

/*****************
 * Aux Functions *
 *****************/

void testBoundingVolume();
void testSphere();
void testRoatedBox();

#endif // BOUNDINGVOLUME_H
