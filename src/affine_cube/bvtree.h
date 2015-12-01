#ifndef BVTREE_H
#define BVTREE_H

#include "boundingvolume.h"

//
struct DotProductComparison {
private:
    Vector* v;
public:
  DotProductComparison(Vector* v);
  bool operator() (const Vector& v1,const Vector& v2);
};

void sortByDotProduct(std::vector<Vector>& points, Vector *direction);

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
public:
    int countNumNodesWithDotProductBetween(double minValue,double maxValue, Vector& probeDirection);
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
};

void testBVTree();

#endif // BVTREE_H
