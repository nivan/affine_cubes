#ifndef DOTPRODUCTCOMPARISON_H
#define DOTPRODUCTCOMPARISON_H

#include <eigen3/Eigen/Dense>
//#include "Vector.h"

struct DotProductComparison {
private:
    Eigen::VectorXd* v;
public:
  DotProductComparison(Eigen::VectorXd* v);
  bool operator() (const Eigen::VectorXd& v1,const Eigen::VectorXd& v2);
};

#endif // DOTPRODUCTCOMPARISON_H
