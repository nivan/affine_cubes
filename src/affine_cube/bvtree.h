#ifndef BVTREE_H
#define BVTREE_H

#include "boundingvolume.h"
#include <stdint.h>

//
struct DotProductComparison {
private:
    Vector* v;
public:
  DotProductComparison(Vector* v);
  bool operator() (const Vector& v1,const Vector& v2);
};

void sortByDotProduct(std::vector<Vector>& points, Vector *direction);

class Histogram2DBin{
public:
    double minX;
    double maxX;
    double minY;
    double maxY;
    uint64_t count;
public:
    Histogram2DBin();
    Histogram2DBin(double minX, double maxX, double minY, double maxY, uint64_t count);
    std::string toString();
};

//
class BVTreeNode{
private:
    BVTreeNode*     leftChild;
    BVTreeNode*     rightChild;
    BoundingVolume* boundingVolume;
    int             numPoints;
    std::vector<Vector> points;
public:
    BVTreeNode();
    BVTreeNode(std::vector<Vector>& points);
    void init(std::vector<Vector>& points);
    void init(std::vector<Vector>& points, int startIndex, int endIndex);
    ~BVTreeNode();
public:
    BoundingVolume* getBoundingVolume();
    void setLeftChild(BVTreeNode*);
    void setRightChild(BVTreeNode*);
    void setPoints(std::vector<Vector>& points);

    int  getNumPoints();
    BVTreeNode* getLeftChild();
    BVTreeNode* getRightChild();
public:

    int countNumPointsWithDotProductBetween(double minValue,double maxValue, Vector& probeDirection);
    void getDotProductRange(const Vector& direction, double &minValue, double& maxValue);
};

//

class BVTree
{
private:
    BVTreeNode* root;
private:
    void buildTree(std::vector<Vector> points, BVTreeNode* currentNode,int &numNodes);

public:
    BVTree();
    BVTree(std::vector<Vector>& points);
    ~BVTree();
public:
    int countNumNodesWithDotProductBetween(double minValue,double maxValue, Vector& probeDirection);
    void getUnbinnedHistogram(const Vector& xAxis, const Vector& yAxis, int maxDepth, std::vector<Histogram2DBin>& result);
    void getDotProductRange(const Vector& direction, double &minValue, double& maxValue);
    int  getNumPoints();
};

void testBVTree();

#endif // BVTREE_H
