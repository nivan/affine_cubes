#include "bvtree.h"
#include <cassert>
#include <algorithm>
#include <queue>
#include <sstream>
#include <iostream>

using namespace std;


/*****************
 * Test Function *
 *****************/

void testBVTree(){

    //
    Eigen::VectorXd v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Eigen::VectorXd v2(3);
    v2[0] = -2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Eigen::VectorXd v3(3);
    std::vector<Eigen::VectorXd> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);

    //
    Eigen::VectorXd probeVector = Eigen::VectorXd::Zero(3);
    probeVector[0] = 1.0;

    //
    sortByDotProduct(points,&probeVector);
    cout << "Sorted Vector" << endl;
    int pointsSize = points.size();
    for(int i = 0 ; i < pointsSize ; ++i){
        cout << "Vector " << i << " : " << vecToString(points.at(i)).c_str() << endl;
    }

    //
    points.clear();
    int numPoints = 1000;
    int numDimensions = 4;
    for(int i = 0 ; i < numPoints ; ++i){
        Eigen::VectorXd point = Eigen::VectorXd::Zero(numDimensions);
        for(int d = 0 ; d < numDimensions ; ++d){
            point[d] = (rand() * 1.0) / RAND_MAX;
        }
        points.push_back(point);
    }

    //
    BVTree<AxisAlignedBoundingVolume> myTree(points);
    //
    Eigen::VectorXd queryVector(4);
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
    Eigen::VectorXd xAxis = Eigen::VectorXd::Zero(4);
    xAxis[0] = 1.0;
    Eigen::VectorXd yAxis = Eigen::VectorXd::Zero(4);
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
