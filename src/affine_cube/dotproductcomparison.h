#ifndef DOTPRODUCTCOMPARISON_H
#define DOTPRODUCTCOMPARISON_H

#include "Vector.h"

struct DotProductComparison {
private:
    Vector* v;
public:
  DotProductComparison(Vector* v);
  bool operator() (const Vector& v1,const Vector& v2);
};

#endif // DOTPRODUCTCOMPARISON_H
