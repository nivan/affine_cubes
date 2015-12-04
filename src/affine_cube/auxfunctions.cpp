#include "auxfunctions.h"
#include "dotproductcomparison.h"
#include <algorithm>

/*****************
 * Aux Functions *
 *****************/

void sortByDotProduct(std::vector<Vector> &points, Vector* direction){
    DotProductComparison comparator(direction);
    std::sort(points.begin(),points.end(),comparator);
}
