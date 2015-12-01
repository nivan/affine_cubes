#include "bvtree.h"
#include <cassert>
#include <algorithm>
#include <iostream>

using namespace std;

void sortByDotProduct(std::vector<Vector> &points, Vector* direction){
    DotProductComparison comparator(direction);
    std::sort(points.begin(),points.end(),comparator);
}


/************************
 * DotProductComparison *
 ************************/

DotProductComparison::DotProductComparison(Vector* v):
    v(v)
{}

bool DotProductComparison::operator() (const Vector& v1,const Vector& v2){
    return v->dot(v1) < v->dot(v2);
}

/**************
 * BVTreeNode *
 **************/

BVTreeNode::BVTreeNode():
    leftChild(NULL),
    rightChild(NULL){

}

BVTreeNode::BVTreeNode(std::vector<Vector>& points):
    leftChild(NULL),
    rightChild(NULL){
    init(points);
}

void BVTreeNode::init(std::vector<Vector>& points){
    this->boundingVolume = AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(points);
    this->numPoints = points.size();
    this->points.clear();
}

void BVTreeNode::init(std::vector<Vector> &points, int startIndex, int endIndex){
    this->boundingVolume = AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(points,startIndex,endIndex);
    this->numPoints = endIndex - startIndex;
}

BVTreeNode::~BVTreeNode(){
    if(leftChild)
        delete leftChild;
    if(rightChild)
        delete rightChild;
    if(boundingVolume)
        delete boundingVolume;
}

BoundingVolume *BVTreeNode::getBoundingVolume(){
    return this->boundingVolume;
}

void BVTreeNode::setLeftChild(BVTreeNode* n){
    this->leftChild = n;
}

void BVTreeNode::setRightChild(BVTreeNode* n){
    this->rightChild = n;
}

void BVTreeNode::setPoints(std::vector<Vector> &points){
    this->points = points;
}

int BVTreeNode::countNumNodesWithDotProductBetween(double minValue, double maxValue, Vector &probeDirection){
    double minDotProductInBV;
    double maxDotProductInBV;
    this->boundingVolume->getDotProductRangeInVolume(probeDirection,minDotProductInBV,maxDotProductInBV);

    if(maxDotProductInBV < minValue || minDotProductInBV > maxValue){
        //no intersection
        return 0;
    }
    else if(minValue <= minDotProductInBV && maxDotProductInBV <= maxValue){
        //bin contained in bv
        return this->numPoints;
    }
    else{
        //partial overlap
        int totalCount = 0;
        if(this->leftChild && this->rightChild){
            totalCount += this->leftChild->countNumNodesWithDotProductBetween(minValue,maxValue,probeDirection);
            totalCount += this->rightChild->countNumNodesWithDotProductBetween(minValue,maxValue,probeDirection);
        }
        else{
            //TODO: need to check the original points??
            //totalCount += this->numPoints;
            //assert(false);
            int numPoints = this->points.size();
            for(int i = 0 ; i < numPoints ; ++i){
                Vector& v = this->points.at(i);
                double value = probeDirection.dot(v);
                if(minValue <= value && value <= maxValue)
                    totalCount += 1;
            }
        }
        return totalCount;
    }
}


/**********
 * BVTree *
 **********/

BVTree::BVTree():
    root(NULL)
{}

BVTree::BVTree(std::vector<Vector>& points):
    root(NULL){
    this->root = new BVTreeNode(points);
    int numNodesGenerated = 0;
    this->buildTree(points,this->root,numNodesGenerated);
    cout << "Generated " << numNodesGenerated << " nodes." << endl;
}

BVTree::~BVTree(){
    if(root)
        delete root;
}

int BVTree::countNumNodesWithDotProductBetween(double minValue, double maxValue, Vector &probeDirection){
    if(this->root)
        return this->root->countNumNodesWithDotProductBetween(minValue,maxValue,probeDirection);
    else
        return 0;
}

template<typename T>
void partitionVector(std::vector<T> &container, int partitionIndex,std::vector<T> &leftPoints,std::vector<T> &rightPoints){
    leftPoints.clear();
    rightPoints.clear();

    for(int i = 0 ; i < partitionIndex ; ++i){
        leftPoints.push_back(container.at(i));
    }
    for(int i = partitionIndex ; i < container.size() ; ++i){
        rightPoints.push_back(container.at(i));
    }
}

void BVTree::buildTree(std::vector<Vector> points, BVTreeNode *currentNode, int &numNodes){
    //init node information
    currentNode->init(points);
    numNodes += 1;

    int numPoints = points.size();
    bool needToSplit = numPoints > 2;

    if(needToSplit){
        //get direction to split
        Vector direction = currentNode->getBoundingVolume()->getDirectionOfLargestVariance();
        sortByDotProduct(points,&direction);
        int midIndex = numPoints / 2;
        double median = direction.dot(points[midIndex]);

        //
        while(midIndex < numPoints){
            double currentValue = direction.dot(points[midIndex]);
            if(currentValue > median)
                break;
            else
                ++midIndex;
        }

        //
        std::vector<Vector> leftPoints;
        std::vector<Vector> rightPoints;
        partitionVector(points,midIndex,leftPoints,rightPoints);

        //
        BVTreeNode* left = new BVTreeNode();
        currentNode->setLeftChild(left);
        buildTree(leftPoints,left,numNodes);
        //
        BVTreeNode* right = new BVTreeNode();
        currentNode->setRightChild(right);
        buildTree(rightPoints,right,numNodes);
    }
    else{
        currentNode->setPoints(points);
    }
}

/*****************
 * Test Function *
 *****************/

void testBVTree(){
  
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
    std::vector<Vector> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);

    //
    Vector probeVector(3);
    probeVector[0] = 1.0;

    //
    sortByDotProduct(points,&probeVector);
    cout << "Sorted Vector" << endl;
    for(int i = 0 ; i < points.size() ; ++i){
      cout << "Vector " << i << " : " << points.at(i).toString() << endl;
    }

    //
    points.clear();
    int numPoints = 100000;
    int numDimensions = 4;
    for(int i = 0 ; i < numPoints ; ++i){
        Vector point(numDimensions);
        for(int d = 0 ; d < numDimensions ; ++d){
            point[d] = (rand() * 1.0) / RAND_MAX;
        }
        points.push_back(point);
        //cout << "Point " << i << " : " << point.toString() << endl;
    }

    //
    BVTree myTree(points);
    //
    Vector queryVector(4);
    queryVector[0] = 1.0;
    queryVector[2] = 2.0;
    //brute force
    double minInterval = 0.25;
    double maxInterval = 0.75;
    int count = 0;
    for(int i = 0 ; i < numPoints ; ++i){
        double value = points.at(i)[0] + 2.0 * points.at(i)[2];
        if(minInterval <= value && value <= maxInterval)
            count++;
    }
    cout << "Brute Force " << count << endl;
    //
    cout << myTree.countNumNodesWithDotProductBetween(minInterval,maxInterval,queryVector) << endl;
}
