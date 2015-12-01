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
    virtual int numPlanes() {return 0;};
    virtual HyperPlane getHyperPlane(int i) = 0;
    virtual void getDotProductRangeInVolume(const Vector& direction, double& minV, double& maxV) = 0;
    virtual Vector getDirectionOfLargestVariance() = 0;
};

class AxisAlignedBoundingVolume: public BoundingVolume{
private:
    std::vector<std::pair<double,double> > dimensionBounds;
public:
    AxisAlignedBoundingVolume(std::vector<std::pair<double,double> >& dimensionBounds);
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

#endif // BOUNDINGVOLUME_H
