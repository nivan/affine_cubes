#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <eigen3/Eigen/Dense>

/************************
 * DotProductComparison *
 ************************/

struct DotProductComparison {
private:
    Eigen::VectorXd* v;
public:
  DotProductComparison(Eigen::VectorXd* v);
  bool operator() (const Eigen::VectorXd& v1,const Eigen::VectorXd& v2);
};

#endif // COMPARATOR_H
