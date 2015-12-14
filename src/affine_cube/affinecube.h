#ifndef AFFINECUBE_H
#define AFFINECUBE_H

#include "boundingvolume.h"
#include <map>
#include <eigen3/Eigen/Dense>
#include "interval.h"
#include "histogram2dbin.h"

class DirectionSearchConstraint {
public:
    Eigen::VectorXd direction;
    Interval range;
public:
    DirectionSearchConstraint();
public:
    inline bool satisfiesContraint(Eigen::VectorXd& point){
        double coeff = point.dot(direction);
        return range.contains(coeff);
    }
};

class AffineCube
{
private:
    long beginIndex;
    long endIndex;
    std::vector<Eigen::VectorXd> points; //stores points if leaf
    Eigen::VectorXd partitionDirection;  //for non-leaves
    std::map<BoundingVolumeType,BoundingVolume*> boundingVolumes;
    std::vector<AffineCube*> children;
private:

public:
    AffineCube();
    AffineCube(std::vector<Eigen::VectorXd>& data);
    ~AffineCube();
public:
    inline bool isLeaf() const {
        return (numChildren() == 0);
    }
    inline int numChildren() const{
        return children.size();
    }
    inline AffineCube* getChild(int index){
        if(index < 0 || index >= this->numChildren())
            return NULL;
        return this->children.at(index);
    }
    inline void addChild(AffineCube* newChild){
        this->children.push_back(newChild);
    }
    inline void addBoundingVolume(BoundingVolume* bv, BoundingVolumeType type){
        boundingVolumes[type] = bv;
    }
    inline long numPoints(){
        return endIndex - beginIndex;
    }
    inline void setPartitionDirection(Eigen::VectorXd v){
        this->partitionDirection = v;
    }
    inline Eigen::VectorXd getPartitionDirection(){
        return this->partitionDirection;
    }

public:
    long  countNumPointsWithDotProductBetween(double minValue,double maxValue, Eigen::VectorXd& probeDirection);
    long  countNumPointsWithDotProductBetween(std::vector<DirectionSearchConstraint>& searchConstraints);

    Interval getDotProductRange(const Eigen::VectorXd& direction);
    void getUnbinnedHistogram(const Eigen::VectorXd &xAxis, const Eigen::VectorXd &yAxis, int maxDepth, std::vector<Histogram2DBin> &result);

public:
    static AffineCube* buildTree(std::vector<Eigen::VectorXd>& data, std::vector<BoundingVolumeType>& bvTypes);
};

void testAffineCube();

#endif // AFFINECUBE_H
