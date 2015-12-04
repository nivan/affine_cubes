#ifndef MINIBALLCOMPUTATION_H
#define MINIBALLCOMPUTATION_H

#include <vector>
#include "../affine_cube/Vector.h"

class MiniBallComputation
{
public:
    MiniBallComputation();
    ~MiniBallComputation();
public:
    static void computeMiniBall(std::vector<Vector>& points,Vector& center, double& radius);
};

#endif // MINIBALLCOMPUTATION_H
