#ifndef PCA_H
#define PCA_H

#include <eigen3/Eigen/Dense>

class PCA
{
public:
    static bool performPCA(const Eigen::MatrixXd &points, Eigen::MatrixXd &rotationMatrix);
    static bool performPCA(std::vector<Eigen::VectorXd>& points, Eigen::MatrixXd& rotationMatrix);
    static bool performPCA(std::vector<Eigen::VectorXd>& points, int beginIndex, int endIndex, Eigen::MatrixXd& rotationMatrix);
};

void testPCA();

#endif // PCA_H
