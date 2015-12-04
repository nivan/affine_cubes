#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <vector>
#include "Vector.h"

class HyperPlane{
public:
    Vector normalVector;
    double intercept;
public:
    HyperPlane();
    HyperPlane(Vector& normalVector, double& intercept);
    int numDimensions();
};

class BoundingVolume{
public:
    virtual ~BoundingVolume() {};
    virtual void getDotProductRangeInVolume(const Vector& direction, double& minV, double& maxV) = 0;
    virtual Vector getDirectionOfLargestVariance() = 0;
};

//
class Sphere: public BoundingVolume{
private:
    Vector* center;
    double  radius;
    Vector* directionOfLargestVariance;
public:
    Sphere();
    Sphere(Vector&,double);
    Sphere(std::vector<Vector>&);
    Sphere(std::vector<Vector>& points, int beginIndex, int endIndex);
    template<class iterator_type>
    Sphere(iterator_type beginIterator, iterator_type endIterator);
    ~Sphere();
     std::string toString();
public:
    Vector getCenter();
    double gerRadius();
    bool contains(const Vector& point);
public:
    void getDotProductRangeInVolume(const Vector& direction, double& minV, double& maxV);
    Vector getDirectionOfLargestVariance();
};

//
class Polytope: public BoundingVolume {
public:
    virtual int numPlanes() {return 0;};
    virtual HyperPlane getHyperPlane(int i) = 0;
};

class AxisAlignedBoundingVolume: public Polytope{
private:
    std::vector<std::pair<double,double> > dimensionBounds;
public:
    AxisAlignedBoundingVolume(std::vector<std::pair<double,double> >& dimensionBounds);
    AxisAlignedBoundingVolume(std::vector<Vector>& points);
    AxisAlignedBoundingVolume(std::vector<Vector>& points, int beginIndex, int endIndex);
    std::string toString();
public:
    int numPlanes();
    HyperPlane getHyperPlane(int i);
    void getDotProductRangeInVolume(const Vector& direction, double& minV, double& maxV);
    Vector getDirectionOfLargestVariance();
public:
    static AxisAlignedBoundingVolume* getAxisAlignedBoundingVolume(std::vector<Vector>& points);
    static AxisAlignedBoundingVolume* getAxisAlignedBoundingVolume(std::vector<Vector>& points,int startIndex, int endIndex);
};

void testBoundingVolume();
void testSphere();

#endif // BOUNDINGVOLUME_H
