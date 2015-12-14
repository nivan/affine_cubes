#include "affinecube.h"
#include <limits>
#include <cassert>
#include <tuple>
#include <queue>
#include "auxfunctions.h"
#include <iostream>

using namespace std;

/**************
 * AffineCube *
 **************/

AffineCube::AffineCube():
    beginIndex(-1),
    endIndex(-2)
{}

AffineCube::AffineCube(std::vector<Eigen::VectorXd>& ){
}

AffineCube::~AffineCube(){
    this->boundingVolumes.clear();
    this->children.clear();
}

long AffineCube::countNumPointsWithDotProductBetween(double minValue,double maxValue, Eigen::VectorXd& probeDirection){
    Interval targetInterval(minValue,maxValue);
    Interval productRange = getDotProductRange(probeDirection);
    
    Interval::IntersectionType iType = targetInterval.testIntersection(productRange);

    //
    if(iType == Interval::CONTAINS)
        return this->numPoints();
    else if(iType == Interval::DISJOINT)
        return 0;
    else{
        if(this->isLeaf()){
            int numElements = this->points.size();
            long count = 0;
            for(int i = 0 ; i < numElements ; ++i){
                Eigen::VectorXd& point = this->points.at(i);
                double value = probeDirection.dot(point);
                if(minValue <= value && value <= maxValue)
                    ++count;
            }
            return count;
        }
        else{
            int numChildren = this->numChildren();
            long total = 0;
            for(int i = 0 ; i < numChildren ; ++i){
                AffineCube* child = this->getChild(i);
                total += child->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
            }
            return total;
        }
    }
}

long AffineCube::countNumPointsWithDotProductBetween(std::vector<DirectionSearchConstraint> &searchConstraints){
    //    double minProduct;
    //    double maxProduct;

    //    getDotProductRange(probeDirection, minProduct, maxProduct);

    //    //
    //    if(minValue <= minProduct && maxProduct <= maxValue)
    //        return this->numPoints();
    //    else if(maxProduct < minValue || minProduct > maxValue)
    //        return 0;
    //    else{
    //        if(this->isLeaf()){
    //            Eigen::VectorXd dotProducts = points * probeDirection;
    //            int numElements = dotProducts.size();
    //            long count = 0;
    //            for(int i = 0 ; i < numElements ; ++i){
    //                double value = dotProducts[i];
    //                if(minValue <= value && value <= maxValue)
    //                    ++count;
    //            }
    //        }
    //        else{
    //            int numChildren = this->numChildren();
    //            long total = 0;
    //            for(int i = 0 ; i < numChildren ; ++i){
    //                AffineCube* child = this->getChild(i);
    //                total += child->countNumPointsWithDotProductBetween(minValue,maxValue,probeDirection);
    //            }
    //            return total;
    //        }
    //    }
    //TODO
    assert(false);
}

Interval AffineCube::getDotProductRange(const Eigen::VectorXd& direction){
    Interval result(std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());

    if(this->isLeaf()){
        int numElements = this->points.size();
        for(int i = 0 ; i < numElements ; ++i){
            Eigen::VectorXd& point = this->points.at(i);
            double value = direction.dot(point);
            result.addPoint(value);
        }
    }
    else{
        for(auto it = boundingVolumes.begin() ; it != boundingVolumes.end() ; ++it){
            BoundingVolume* bv = it->second;
            double minV;
            double maxV;
            bv->getDotProductRangeInVolume(direction,minV,maxV);
            //
            result.addPoint(minV);
            result.addPoint(maxV);
        }
    }
    return result;
}

AffineCube* AffineCube::buildTree(std::vector<Eigen::VectorXd>& data, std::vector<BoundingVolumeType>& bvTypes){
    AffineCube* root = new AffineCube();
    std::queue<std::tuple<AffineCube*,long,long> > nodesToProcess;
    nodesToProcess.push(make_tuple(root,0,data.size()));

    while(!nodesToProcess.empty()){

        //get data
        tuple<AffineCube*,int,int> currentTuple = nodesToProcess.front();
        nodesToProcess.pop();
        AffineCube* currentNode = std::get<0>(currentTuple);
        long bIndex = std::get<1>(currentTuple);
        long eIndex = std::get<2>(currentTuple);

        //cout << "Processing Node " << Interval(bIndex,eIndex).toString() << endl;

        //
        currentNode->beginIndex = bIndex;
        currentNode->endIndex = eIndex;
        int numNodes = eIndex - bIndex;
        currentNode->boundingVolumes.clear();
        currentNode->children.clear();
        currentNode->points.clear();

        //create bounding volumes
        int numBoundingVolumes = bvTypes.size();
        //cout << "    Num BVs " << numBoundingVolumes << endl;
        for(int i = 0 ; i < numBoundingVolumes ; ++i){
            BoundingVolumeType bvType = bvTypes.at(i);
            switch (bvType) {
            case AXIS_ALIGNED_BBOX:
                currentNode->boundingVolumes[bvType] = new AxisAlignedBoundingBox(data,bIndex,eIndex);
                break;
            case ROTATED_BBOX:
                currentNode->boundingVolumes[bvType] = new RotatedBoundingBox(data,bIndex,eIndex);
                break;
            case SPHERE:
                currentNode->boundingVolumes[bvType] = new Sphere(data,bIndex,eIndex);
                break;
            default:
                assert(false);
                break;
            }
        }

        //
        bool needToSplit = numNodes > 10; // we should be able to use some sort of volume measure here to decide if we need to split

        if(needToSplit){
            //cout << "   Split" << endl;
            int minNumClusters = 2;
            int maxNumClusters = 10;

            AxisAlignedBoundingBox* bv = AxisAlignedBoundingBox::getAxisAlignedBoundingVolume(data,bIndex,eIndex);
            Eigen::VectorXd direction = bv->getDirectionOfLargestVariance();
            currentNode->setPartitionDirection(direction);
            sortPortionByDotProduct(data,bIndex,eIndex,&direction);
            std::vector<double> dotProducts;
            for(long i = bIndex; i < eIndex ; ++i){
                dotProducts.push_back(direction.dot(data.at(i)));
            }

            std::vector<std::pair<int,int> > childrenPartition;//TODO: make those be long values
            partitionVectorByClustering(dotProducts,minNumClusters,maxNumClusters,childrenPartition);

            int numChildren = childrenPartition.size();
            for(int i = 0 ; i < numChildren ; ++i){
                std::pair<int,int> indices = childrenPartition.at(i);
                //cout << "    Child " << i << " = [" << (bIndex + indices.first) << " , " << (bIndex + indices.second) << "]" << endl;
                AffineCube* newChild = new AffineCube();
                currentNode->addChild(newChild);
                nodesToProcess.push(make_tuple(newChild,(bIndex + indices.first),(bIndex + indices.second)));
            }

            delete bv;
        }
        else{
            //cout << "   LEAF" << endl;
            for(long i = bIndex ; i < eIndex ; ++i){
                currentNode->points.push_back(data.at(i));
            }
        }
    }
    return root;
}

void AffineCube::getUnbinnedHistogram(const Eigen::VectorXd &xAxis, const Eigen::VectorXd &yAxis, int maxDepth, std::vector<Histogram2DBin> &result){
    std::queue<std::pair<int,AffineCube*> > nodes;
    nodes.push(std::make_pair(0,this));
    while(!nodes.empty()){
        std::pair<int,AffineCube*> current = nodes.front();
        nodes.pop();

        //
        int currentDepth = current.first;
        AffineCube* currentNode = current.second;
        if(currentDepth < maxDepth && !currentNode->isLeaf()){
            //partition node
            int numChildren = currentNode->numChildren();
            for(int i = 0 ; i < numChildren ; ++i)
                nodes.push(std::make_pair(currentDepth+1,currentNode->getChild(i)));
        }
        else{
            Interval rangeX = currentNode->getDotProductRange(xAxis);
            Interval rangeY = currentNode->getDotProductRange(yAxis);
            result.push_back(Histogram2DBin(rangeX.minValue,rangeX.maxValue,rangeY.minValue,rangeY.maxValue,currentNode->numPoints()));
        }
    }
}


void testAffineCube(){
    vector<BoundingVolumeType> types;
    types.push_back(AXIS_ALIGNED_BBOX);

    //
    int numDimensions = 2;
    std::vector<Eigen::VectorXd> points;
    int numPoints = 100;
    for(int i = 0 ; i < numPoints ; ++i){
        Eigen::VectorXd point(numDimensions);
        for(int d = 0 ; d < numDimensions ; ++d){
            point[d] = (rand() * 1.0) / RAND_MAX;
        }
        points.push_back(point);
    }

    AxisAlignedBoundingBox bboxAxisAlignedBoundingBox(points);


    //
    AffineCube* root = AffineCube::buildTree(points,types);

    //
    Eigen::VectorXd xAxis = Eigen::VectorXd::Zero(2);
    xAxis[0] = 1.0;
    Eigen::VectorXd yAxis = Eigen::VectorXd::Zero(2);
    yAxis[1] = 1.0;

    cout << "bounding box " << bboxAxisAlignedBoundingBox.toString() << endl;


    for(int maxDepth = 0; maxDepth < 3 ; maxDepth++){
        cout << "Max depth " << maxDepth << endl;
        std::vector<Histogram2DBin> result;
        root->getUnbinnedHistogram(xAxis,yAxis,maxDepth,result);
        int numbins = result.size();
        cout << "   Num Bins " << numbins << endl;
        for(int i = 0 ; i < numbins ; ++i){
            Histogram2DBin bin = result.at(i);
            cout << "    Bin " << i << " " << bin.toString() << endl;
        }
    }

}
