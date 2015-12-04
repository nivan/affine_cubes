#include "bvtree.h"
#include <cassert>
#include <algorithm>
#include <queue>
#include <sstream>
#include <iostream>

using namespace std;

/**************
 * BVTreeNode *
 **************/
/*template<class TypeOfBV>
BVTreeNode<TypeOfBV>::BVTreeNode():
    leftChild(NULL),
    rightChild(NULL){

}

template<class TypeOfBV>
BVTreeNode<TypeOfBV>::BVTreeNode(std::vector<Vector>& points):
    leftChild(NULL),
    rightChild(NULL){
    init(points);
}

template<class TypeOfBV>
void BVTreeNode<TypeOfBV>::init(std::vector<Vector>& points){
    this->boundingVolume = AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(points);
    this->numPoints = points.size();
    this->points.clear();
}
*/

//void BVTreeNode::init(std::vector<Vector> &points, int startIndex, int endIndex){
//    this->boundingVolume = AxisAlignedBoundingVolume::getAxisAlignedBoundingVolume(points,startIndex,endIndex);
//    this->numPoints = endIndex - startIndex;
//}


//template<class TypeOfBV>
//BVTreeNode<TypeOfBV>::~BVTreeNode(){
//    if(leftChild)
//        delete leftChild;
//    if(rightChild)
//        delete rightChild;
//    if(boundingVolume)
//        delete boundingVolume;
//}

//template<class TypeOfBV>
//BoundingVolume *BVTreeNode<TypeOfBV>::getBoundingVolume(){
//    return this->boundingVolume;
//}

//template<class TypeOfBV>
//void BVTreeNode<TypeOfBV>::setLeftChild(BVTreeNode* n){
//    this->leftChild = n;
//}

//template<class TypeOfBV>
//void BVTreeNode<TypeOfBV>::setRightChild(BVTreeNode* n){
//    this->rightChild = n;
//}

//template<class TypeOfBV>
//void BVTreeNode<TypeOfBV>::setPoints(std::vector<Vector> &points){
//    this->points = points;
//}

//template<class TypeOfBV>
//int BVTreeNode<TypeOfBV>::getNumPoints(){
//    return this->numPoints;
//}

//template<class TypeOfBV>
//BVTreeNode<TypeOfBV>* BVTreeNode<TypeOfBV>::getLeftChild(){
//    return this->leftChild;
//}

//template<class TypeOfBV>
//BVTreeNode<TypeOfBV>* BVTreeNode<TypeOfBV>::getRightChild(){
//    return this->rightChild;
//}

//template<class TypeOfBV>
//int BVTreeNode<TypeOfBV>::countNumPointsWithDotProductBetween(double minValue, double maxValue, Vector &probeDirection){
//    double minDotProductInBV;
//    double maxDotProductInBV;
//    this->boundingVolume->getDotProductRangeInVolume(probeDirection,minDotProductInBV,maxDotProductInBV);

//    if(maxDotProductInBV < minValue || minDotProductInBV > maxValue){
//        //no intersection
//        return 0;
//    }
//    else if(minValue <= minDotProductInBV && maxDotProductInBV <= maxValue){
//        //bin contained in bv
//        return this->numPoints;
//    }
//    else{
//        //partial overlap
//        int totalCount = 0;
//        if(this->leftChild && this->rightChild){
//            totalCount += this->leftChild->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
//            totalCount += this->rightChild->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
//        }
//        else{
//            //TODO: need to check the original points??
//            //totalCount += this->numPoints;
//            //assert(false);
//            int numPoints = this->points.size();
//            for(int i = 0 ; i < numPoints ; ++i){
//                Vector& v = this->points.at(i);
//                double value = probeDirection.dot(v);
//                if(minValue <= value && value <= maxValue)
//                    totalCount += 1;
//            }
//        }
//        return totalCount;
//    }
//}

//template<class TypeOfBV>
//void BVTreeNode<TypeOfBV>::getDotProductRange(const Vector &direction, double &minValue, double &maxValue){
//    this->boundingVolume->getDotProductRangeInVolume(direction,minValue,maxValue);
//}


///**********
// * BVTree *
// **********/

//template<class TypeOfBV>
//BVTree<TypeOfBV>::BVTree():
//    root(NULL)
//{}

//template<class TypeOfBV>
//BVTree<TypeOfBV>::BVTree(std::vector<Vector>& points):
//    root(NULL){
//    this->root = new BVTreeNode<TypeOfBV>(points);
//    int numNodesGenerated = 0;
//    this->buildTree(points,this->root,numNodesGenerated);
//    cout << "Generated " << numNodesGenerated << " nodes." << endl;
//}

//template<class TypeOfBV>
//BVTree<TypeOfBV>::~BVTree(){
//    if(root)
//        delete root;
//}

//template<class TypeOfBV>
//int BVTree<TypeOfBV>::countNumNodesWithDotProductBetween(double minValue, double maxValue, Vector &probeDirection){
//    if(this->root)
//        return this->root->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
//    else
//        return 0;
//}

//template<class TypeOfBV>
//void BVTree<TypeOfBV>::getUnbinnedHistogram(const Vector &xAxis, const Vector &yAxis, int maxDepth, std::vector<Histogram2DBin> &result){
//    std::queue<std::pair<int,BVTreeNode<TypeOfBV>*> > nodes;
//    nodes.push(std::make_pair(0,this->root));
//    while(!nodes.empty()){
//        std::pair<int,BVTreeNode<TypeOfBV>*> current = nodes.front();
//        nodes.pop();

//        //
//        int currentDepth = current.first;
//        BVTreeNode<TypeOfBV>* currentNode = current.second;
//        BVTreeNode<TypeOfBV>* leftChild = currentNode->getLeftChild();
//        BVTreeNode<TypeOfBV>* rightChild = currentNode->getRightChild();
//        if(currentDepth < maxDepth && leftChild && rightChild){
//                //partition node
//                nodes.push(std::make_pair(currentDepth+1,leftChild));
//                nodes.push(std::make_pair(currentDepth+1,rightChild));
//        }
//        else{
//            double minX;
//            double maxX;
//            currentNode->getDotProductRange(xAxis,minX,maxX);
//            double minY;
//            double maxY;
//            currentNode->getDotProductRange(yAxis,minY,maxY);
//            result.push_back(Histogram2DBin(minX,maxX,minY,maxY,currentNode->getNumPoints()));
//        }
//    }
//}


//template<typename T>
//void partitionVector(std::vector<T> &container, int partitionIndex,std::vector<T> &leftPoints,std::vector<T> &rightPoints){
//    leftPoints.clear();
//    rightPoints.clear();

//    for(int i = 0 ; i < partitionIndex ; ++i){
//        leftPoints.push_back(container.at(i));
//    }
//    for(int i = partitionIndex ; i < container.size() ; ++i){
//        rightPoints.push_back(container.at(i));
//    }
//}

//template<class TypeOfBV>
//int BVTree<TypeOfBV>::getNumPoints(){
//  return this->root->getNumPoints();
//}

//template<class TypeOfBV>
//void BVTree<TypeOfBV>::buildTree(std::vector<Vector> points, BVTreeNode<TypeOfBV> *currentNode, int &numNodes){
//    //init node information
//    currentNode->init(points);
//    numNodes += 1;

//    int numPoints = points.size();
//    bool needToSplit = numPoints > 2;

//    if(needToSplit){
//        //get direction to split
//        Vector direction = currentNode->getBoundingVolume()->getDirectionOfLargestVariance();
//        sortByDotProduct(points,&direction);
//        int midIndex = numPoints / 2;

//        //
//        std::vector<Vector> leftPoints;
//        std::vector<Vector> rightPoints;
//        partitionVector(points,midIndex,leftPoints,rightPoints);

//        //
//        BVTreeNode<TypeOfBV>* left = new BVTreeNode<TypeOfBV>();
//        currentNode->setLeftChild(left);
//        buildTree(leftPoints,left,numNodes);
//        //
//        BVTreeNode<TypeOfBV>* right = new BVTreeNode<TypeOfBV>();
//        currentNode->setRightChild(right);
//        buildTree(rightPoints,right,numNodes);
//    }
//    else{
//        currentNode->setPoints(points);
//    }
//}

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
    int pointsSize = points.size();
    for(int i = 0 ; i < pointsSize ; ++i){
        cout << "Vector " << i << " : " << points.at(i).toString().c_str() << endl;
    }

    //
    points.clear();
    int numPoints = 1000;
    int numDimensions = 4;
    for(int i = 0 ; i < numPoints ; ++i){
        Vector point(numDimensions);
        for(int d = 0 ; d < numDimensions ; ++d){
            point[d] = (rand() * 1.0) / RAND_MAX;
        }
        points.push_back(point);
    }

    //
    BVTree<AxisAlignedBoundingVolume> myTree(points);
    //
    Vector queryVector(4);
    queryVector[0] = 1.0;
    queryVector[2] = 2.0;
    queryVector[3] = -5.0;
    //brute force
    double minInterval = 0.25;
    double maxInterval = 0.75;
    int count = 0;
    for(int i = 0 ; i < numPoints ; ++i){
        double value = points.at(i)[0] + 2.0 * points.at(i)[2] - 5*points.at(i)[3];
        if(minInterval <= value && value <= maxInterval)
            count++;
    }


    //
    Vector xAxis(4);
    xAxis[0] = 1.0;
    Vector yAxis(4);
    yAxis[1] = 1.0;
    std::vector<Histogram2DBin> result;
    myTree.getUnbinnedHistogram(xAxis,yAxis,100,result);
    int numbins = result.size();
    cout << "Num Bins " << numbins << endl;
    for(int i = 0 ; i < numbins ; ++i){
        Histogram2DBin bin = result.at(i);
        cout << "    Bin " << i << " " << bin.toString() << endl;
    }
}
