#ifndef BVTREE_H
#define BVTREE_H

#include "boundingvolume.h"
#include "comparators.h"
#include "auxfunctions.h"
#include "histogram2dbin.h"

/**************
 * BVTreeNode *
 **************/

template<class TypeOfBV>
class BVTreeNode{
private:
    BVTreeNode*     leftChild;
    BVTreeNode*     rightChild;
    BoundingVolume* boundingVolume;
    int             beginIndex;
    int             endIndex;
    std::vector<Eigen::VectorXd> points;
public:
    BVTreeNode();
    BVTreeNode(std::vector<Eigen::VectorXd>& points);
    void init(std::vector<Eigen::VectorXd>& points);
    //void init(std::vector<Eigen::VectorXd>& points, int startIndex, int endIndex);
    ~BVTreeNode();
public:
    BoundingVolume* getBoundingVolume();
    void setLeftChild(BVTreeNode*);
    void setRightChild(BVTreeNode*);
    void setPoints(std::vector<Eigen::VectorXd>& points);
    bool isLeaf();

    int  getNumPoints();
    BVTreeNode* getLeftChild();
    BVTreeNode* getRightChild();
public:

    int countNumPointsWithDotProductBetween(double minValue,double maxValue, Eigen::VectorXd& probeDirection);
    void getDotProductRange(const Eigen::VectorXd& direction, double &minValue, double& maxValue);
};

template<class TypeOfBV>
BVTreeNode<TypeOfBV>::BVTreeNode():
    leftChild(NULL),
    rightChild(NULL),
    boundingVolume(NULL),
    beginIndex(-1),
    endIndex(0)
{}

template<class TypeOfBV>
BVTreeNode<TypeOfBV>::BVTreeNode(std::vector<Eigen::VectorXd>& points):
    leftChild(NULL),
    rightChild(NULL),
    boundingVolume(NULL){
    init(points);
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::init(std::vector<Eigen::VectorXd>& points){
    this->boundingVolume = new TypeOfBV(points);
    this->beginIndex = 0;
    this->endIndex = points.size();
    this->points.clear();
}

template<class TypeOfBV>
BVTreeNode<TypeOfBV>::~BVTreeNode(){
    if(leftChild)
        delete leftChild;
    if(rightChild)
        delete rightChild;
    if(boundingVolume)
        delete boundingVolume;
}

template<class TypeOfBV>
BoundingVolume *BVTreeNode<TypeOfBV>::getBoundingVolume(){
    return this->boundingVolume;
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::setLeftChild(BVTreeNode* n){
    this->leftChild = n;
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::setRightChild(BVTreeNode* n){
    this->rightChild = n;
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::setPoints(std::vector<Eigen::VectorXd> &points){
    this->points = points;
}

template<class TypeOfBV>
bool BVTreeNode<TypeOfBV>::isLeaf(){
    return (this->leftChild == NULL) && (this->rightChild == NULL);
}


template<class TypeOfBV>
int BVTreeNode<TypeOfBV>::getNumPoints(){
    //return this->numPoints;
    return this->endIndex - this->beginIndex;
}

template<class TypeOfBV>
BVTreeNode<TypeOfBV>* BVTreeNode<TypeOfBV>::getLeftChild(){
    return this->leftChild;
}

template<class TypeOfBV>
BVTreeNode<TypeOfBV>* BVTreeNode<TypeOfBV>::getRightChild(){
    return this->rightChild;
}

template<class TypeOfBV>
int BVTreeNode<TypeOfBV>::countNumPointsWithDotProductBetween(double minValue, double maxValue, Eigen::VectorXd &probeDirection){
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
        if(!this->isLeaf()){
            totalCount += this->leftChild->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
            totalCount += this->rightChild->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
        }
        else{
            //TODO: need to check the original points??
            //totalCount += this->numPoints;
            //assert(false);
            int numPoints = this->points.size();
            for(int i = 0 ; i < numPoints ; ++i){
                Eigen::VectorXd& v = this->points.at(i);
                double value = probeDirection.dot(v);
                if(minValue <= value && value <= maxValue)
                    totalCount += 1;
            }
        }
        return totalCount;
    }
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::getDotProductRange(const Eigen::VectorXd &direction, double &minValue, double &maxValue){
    this->boundingVolume->getDotProductRangeInVolume(direction,minValue,maxValue);
}

/**********
 * BVTree *
 **********/

template<class TypeOfBV>
class BVTree{
private:
    BVTreeNode<TypeOfBV>* root;
private:
    void buildTree(std::vector<Eigen::VectorXd> points, BVTreeNode<TypeOfBV>* currentNode,int &numNodes);
    void buildTree(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, BVTreeNode<TypeOfBV>* currentNode,int &numNodes);

public:
    BVTree();
    BVTree(std::vector<Eigen::VectorXd>& points);
    ~BVTree();
public:
    int countNumNodesWithDotProductBetween(double minValue,double maxValue, Eigen::VectorXd& probeDirection);
    void getUnbinnedHistogram(const Eigen::VectorXd& xAxis, const Eigen::VectorXd& yAxis, int maxDepth, std::vector<Histogram2DBin>& result);
    void getDotProductRange(const Eigen::VectorXd& direction, double &minValue, double& maxValue);
    int  getNumPoints();
};


template<class TypeOfBV>
void BVTree<TypeOfBV>::getDotProductRange(const Eigen::VectorXd &direction, double &minValue, double &maxValue){
    if(this->root)
        this->root->getDotProductRange(direction,minValue,maxValue);
}

#include <iostream>
#include <queue>

using namespace std;


template<class TypeOfBV>
BVTree<TypeOfBV>::BVTree():
    root(NULL)
{}

template<class TypeOfBV>
BVTree<TypeOfBV>::BVTree(std::vector<Eigen::VectorXd>& points):
    root(NULL){
    this->root = new BVTreeNode<TypeOfBV>(points);
    int numNodesGenerated = 0;
    this->buildTree(points,this->root,numNodesGenerated);
    cout << "Generated " << numNodesGenerated << " nodes." << endl;
}

template<class TypeOfBV>
BVTree<TypeOfBV>::~BVTree(){
    if(root)
        delete root;
}

template<class TypeOfBV>
int BVTree<TypeOfBV>::countNumNodesWithDotProductBetween(double minValue, double maxValue, Eigen::VectorXd &probeDirection){
    if(this->root)
        return this->root->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
    else
        return 0;
}

template<class TypeOfBV>
void BVTree<TypeOfBV>::getUnbinnedHistogram(const Eigen::VectorXd &xAxis, const Eigen::VectorXd &yAxis, int maxDepth, std::vector<Histogram2DBin> &result){
    std::queue<std::pair<int,BVTreeNode<TypeOfBV>*> > nodes;
    nodes.push(std::make_pair(0,this->root));
    while(!nodes.empty()){
        std::pair<int,BVTreeNode<TypeOfBV>*> current = nodes.front();
        nodes.pop();

        //
        int currentDepth = current.first;
        BVTreeNode<TypeOfBV>* currentNode = current.second;
        BVTreeNode<TypeOfBV>* leftChild = currentNode->getLeftChild();
        BVTreeNode<TypeOfBV>* rightChild = currentNode->getRightChild();
        if(currentDepth < maxDepth && leftChild && rightChild){
                //partition node
                nodes.push(std::make_pair(currentDepth+1,leftChild));
                nodes.push(std::make_pair(currentDepth+1,rightChild));
        }
        else{
            double minX;
            double maxX;
            currentNode->getDotProductRange(xAxis,minX,maxX);
            double minY;
            double maxY;
            currentNode->getDotProductRange(yAxis,minY,maxY);
            result.push_back(Histogram2DBin(minX,maxX,minY,maxY,currentNode->getNumPoints()));
        }
    }
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

template<class TypeOfBV>
int BVTree<TypeOfBV>::getNumPoints(){
  return this->root->getNumPoints();
}

template<class TypeOfBV>
void BVTree<TypeOfBV>::buildTree(std::vector<Eigen::VectorXd> points, BVTreeNode<TypeOfBV> *currentNode, int &numNodes){
    //init node information
    currentNode->init(points);
    numNodes += 1;

    int numPoints = points.size();
    bool needToSplit = numPoints > 2;

    if(needToSplit){
        //get direction to split
        Eigen::VectorXd direction = currentNode->getBoundingVolume()->getDirectionOfLargestVariance();
        sortByDotProduct(points,&direction);
        int midIndex = numPoints / 2;

        //
        std::vector<Eigen::VectorXd> leftPoints;
        std::vector<Eigen::VectorXd> rightPoints;
        partitionVector(points,midIndex,leftPoints,rightPoints);

        //
        BVTreeNode<TypeOfBV>* left = new BVTreeNode<TypeOfBV>();
        currentNode->setLeftChild(left);
        buildTree(leftPoints,left,numNodes);
        //
        BVTreeNode<TypeOfBV>* right = new BVTreeNode<TypeOfBV>();
        currentNode->setRightChild(right);
        buildTree(rightPoints,right,numNodes);
    }
    else{
        currentNode->setPoints(points);
    }
}

template<class TypeOfBV>
void BVTree<TypeOfBV>::buildTree(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, BVTreeNode<TypeOfBV>* currentNode,int &numNodes){
    this->boundingVolume = new TypeOfBV(points,beginIndex,endIndex);
    this->numPoints = endIndex - beginIndex;
    this->points.clear();
}

void testBVTree();


#endif // BVTREE_H
