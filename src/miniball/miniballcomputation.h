#ifndef MINIBALLCOMPUTATION_H
#define MINIBALLCOMPUTATION_H

#include <vector>
#include <eigen3/Eigen/Dense>


class MiniBallComputation
{
public:
    MiniBallComputation();
    ~MiniBallComputation();
public:
    static void computeMiniBall(std::vector<Eigen::VectorXd>& points,Eigen::VectorXd& center, double& radius);
};

#endif // MINIBALLCOMPUTATION_H
