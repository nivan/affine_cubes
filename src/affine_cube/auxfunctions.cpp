#include "auxfunctions.h"
#include "comparators.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "../ckmeans/Ckmeans.1d.dp.h"

using namespace std;

/*****************
 * Aux Functions *
 *****************/

void sortByDotProduct(std::vector<Eigen::VectorXd> &points, Eigen::VectorXd* direction){
    DotProductComparison comparator(direction);
    std::sort(points.begin(),points.end(),comparator);
}

void sortPortionByDotProduct(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, Eigen::VectorXd *direction){
    DotProductComparison comparator(direction);
    std::sort(points.begin() + beginIndex,points.begin() + endIndex,comparator);
}

void partitionVectorByClustering(std::vector<double>& sortedValues,int minNumClusters, int maxNumCluster,std::vector<std::pair<int,int> >& childrenPartition){
    std::vector<double> clusteringData;
    int numPoints = sortedValues.size();
    clusteringData.push_back(0);//clustering code ignores the first element in the vector;
    for(int i = 0 ; i < numPoints ; ++i){
        clusteringData.push_back(sortedValues.at(i));
    }
    ClusterResult result = kmeans_1d_dp(clusteringData,minNumClusters,maxNumCluster);
//    cout << "Num Clusters " << result.nClusters << endl;
//    for(int i = 0 ; i <= numPoints ; ++i){
//        cout << result.cluster.at(i) << "," ;
//    }
//    cout << endl;

    //find not partitions
    int beginIndex = 0;
    int prevCluster = 1;
    childrenPartition.clear();
    for(int i = 1 ; i <= numPoints ; ++i){
        int currentCluster = result.cluster.at(i);
        if(currentCluster != prevCluster){
            childrenPartition.push_back(make_pair(beginIndex,i-1));
            beginIndex = i-1;
            prevCluster = currentCluster;
        }
    }
    childrenPartition.push_back(make_pair(beginIndex,numPoints));
}


void testAuxFunctions(){
    //
    Eigen::VectorXd v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Eigen::VectorXd v2(3);
    v2[0] = 2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Eigen::VectorXd v3(3);
    v3[0] = -5.5;
    v3[1] = 12.0;
    v3[2] = 39.0;

    Eigen::VectorXd v4(3);
    v4[0] = -29.0;
    v4[1] = -15.0;
    v4[2] = -45.0;

    Eigen::VectorXd v5(3);
    v5[0] = 19.0;
    v5[1] = -35.0;
    v5[2] = 15.0;

    //
    std::vector<Eigen::VectorXd> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    points.push_back(v4);
    points.push_back(v5);

    //points
    cout << ">>>>>>>>>>>>" << endl;
    int numPoints = points.size();
    for(int i = 0 ; i < numPoints ; ++i){
        cout << "point " << i << " " << vecToString(points.at(i)) << endl;
    }
    cout << "===========" << endl;
    Eigen::VectorXd probeDir = Eigen::VectorXd::Zero(3);
    probeDir[0] = 1.0;

    //
    sortPortionByDotProduct(points,1,3,&probeDir);
    //points
    for(int i = 0 ; i < numPoints ; ++i){
        cout << "point " << i << " " << vecToString(points.at(i)) << endl;
    }
    cout << "<<<<<<<<<<<<" << endl;

}


string vecToString(const Eigen::VectorXd &v){
    int dimension = v.size();

    stringstream ss;
    ss << "(";
    for(int i = 0 ; i < dimension - 1 ; ++i){
        ss << v[i] << ", ";
    }
    ss << v[dimension - 1] << ")";

    return ss.str();
}
