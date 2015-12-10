#include "widget.h"
#include <QApplication>

#include "../affine_cube/boundingvolume.h"
#include "../affine_cube/bvtree.h"
#include "glhistogramwidget.h"
#include "../affine_cube/generalbvtree.h"
#include <iostream>
#include "../pca/pca.h"
#include "../affine_cube/auxfunctions.h"
#include "../ckmeans/Ckmeans.1d.dp.h"
#include <eigen3/Eigen/Dense>
using namespace Eigen;
using namespace std;

const int SIZE = 7;

int main(int argc, char *argv[])
{
#if 0
    int minNumClusters = 2;
    int maxNumCluster  = 10;

    std::default_random_engine generator;
    std::vector<std::normal_distribution<double> > distributions;
    //std::normal_distribution<double> distribution1(-2.0,0.5);
    std::normal_distribution<double> distribution2(6.0,2);
    std::normal_distribution<double> distribution3(0.0,0.2);
    std::normal_distribution<double> distribution4(-6.0,0.2);
    //distributions.push_back(distribution1);
    distributions.push_back(distribution2);
    distributions.push_back(distribution3);
    distributions.push_back(distribution4);

    int numDistributions = distributions.size();

    std::vector<double> data;
    int numPoints = 10;
    cout << "Data" << endl;
    data.push_back(0.0);
    for(int i = 0; i < numPoints ; ++i){
        std::normal_distribution<double>& distribution = distributions.at(rand() % numDistributions);
        double number = distribution(generator);
        data.push_back(number);
        cout << number << ", ";
    }
    ClusterResult result = kmeans_1d_dp(data,2,4);
    cout << "Num Clusters " << result.nClusters << endl;
    for(int i = 0 ; i <= numPoints ; ++i){
        cout << result.cluster.at(i) << "," ;
    }
    cout << endl;

#elif 0
    int minNumClusters = 2;
    int maxNumCluster  = 10;

    std::default_random_engine generator;
    std::vector<std::normal_distribution<double> > distributions;
    std::normal_distribution<double> distribution1(-2.0,0.5);
    std::normal_distribution<double> distribution2(6.0,2);
    std::normal_distribution<double> distribution3(0.0,0.2);
    std::normal_distribution<double> distribution4(-6.0,0.2);
    distributions.push_back(distribution1);
    distributions.push_back(distribution2);
    distributions.push_back(distribution3);
    distributions.push_back(distribution4);

    int numDistributions = distributions.size();

    std::vector<double> data;
    int numPoints = 10;
    cout << "Data" << endl;
    for(int i = 0; i < numPoints ; ++i){
        std::normal_distribution<double>& distribution = distributions.at(rand() % numDistributions);
        double number = distribution(generator);
        data.push_back(number);
        cout << number << ", ";
    }
    cout << endl;

    sort(data.begin(),data.end());
    cout << "Sorted Data" << endl;
    for(int i = 0; i < numPoints ; ++i){
        cout << data.at(i) << ", ";
    }


    std::vector<std::pair<int,int> > childrenPartition;
    partitionVectorByClustering(data,minNumClusters,maxNumCluster,childrenPartition);
    int numPartitions = childrenPartition.size();
    for(int i = 0 ; i < numPartitions ; ++i){
        cout << "Partition " << i << " = " << childrenPartition.at(i).first << " , " << childrenPartition.at(i).second << endl;
    }
#elif 0
    //testAuxFunctions();
    //testBoundingVolume();
    testRoatedBox();
    //    testGeneralTree();
    //    testBVTree();

#elif 0
    //testPCA();
    testRoatedBox();
#elif 0
    int numPoints = 20;
    double noisePortion = 0.05;
    std::vector<double> dataFractions{noisePortion, 0.5 - noisePortion,0.5 - noisePortion, noisePortion};

    bool needToSplit = true;//numPoints >= 1 / noisePortion;

    int beginIndex = 0;
    int endIndex = beginIndex + numPoints;

    if(needToSplit){

        double accumSum = 0;
        int prevStart = 0;
        int numChildren = 4;

        for(int i = 0 ; i < numChildren ; ++i){
            //
            int childBeginIndex = prevStart;
            accumSum += dataFractions.at(i) * numPoints;
            int childEndIndex   = floor(accumSum);
            if(i == (numChildren -1))
                childEndIndex = numPoints;
            prevStart = childEndIndex;

            cout << "node " << i << " : " << (childBeginIndex + beginIndex) << " , " << (childEndIndex+beginIndex) << endl;
        }

    }

    return 0;
#elif 1
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
#elif 1
    QApplication a(argc, argv);
    GLHistogramWidget w;
    w.show();

    return a.exec();
#endif
}
