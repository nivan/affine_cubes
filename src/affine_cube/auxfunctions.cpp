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

void sortPortionByDotProduct(std::vector<Vector>& points, int beginIndex, int endIndex, Vector *direction){
    DotProductComparison comparator(direction);
    std::sort(points.begin() + beginIndex,points.begin() + endIndex,comparator);
}
