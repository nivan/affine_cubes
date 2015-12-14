#ifndef AUXFUNCTIONS_H
#define AUXFUNCTIONS_H

#include <vector>
#include <eigen3/Eigen/Dense>

//
void sortByDotProduct(std::vector<Eigen::VectorXd>& points, Eigen::VectorXd *direction);
void sortPortionByDotProduct(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, Eigen::VectorXd *direction);
void partitionVectorByClustering(std::vector<double>& sortedValues,int minNumClusters, int maxNumCluster,std::vector<std::pair<int,int> >& childrenPartition);

//
void addPointToInterval(double& minInterval, double& maxInterval, double v);

//
std::string vecToString(const Eigen::VectorXd&);

//
void testAuxFunctions();

#endif // AUXFUNCTIONS_H
