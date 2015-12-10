#include "comparators.h"

/************************
 * DotProductComparison *
 ************************/

DotProductComparison::DotProductComparison(Eigen::VectorXd* v):
    v(v)
{}

bool DotProductComparison::operator() (const Eigen::VectorXd& v1,const Eigen::VectorXd& v2){
    return v->dot(v1) < v->dot(v2);
}
