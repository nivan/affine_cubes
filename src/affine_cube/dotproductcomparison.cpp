#include "dotproductcomparison.h"

/************************
 * DotProductComparison *
 ************************/

DotProductComparison::DotProductComparison(Vector* v):
    v(v)
{}

bool DotProductComparison::operator() (const Vector& v1,const Vector& v2){
    return v->dot(v1) < v->dot(v2);
}


