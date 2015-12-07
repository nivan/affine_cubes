#ifndef AUXFUNCTIONS_H
#define AUXFUNCTIONS_H

#include <vector>
#include <eigen3/Eigen/Dense>

void sortByDotProduct(std::vector<Eigen::VectorXd>& points, Eigen::VectorXd *direction);
void sortPortionByDotProduct(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, Eigen::VectorXd *direction);

//
std::string vecToString(const Eigen::VectorXd&);

//
void testAuxFunctions();

#endif // AUXFUNCTIONS_H
