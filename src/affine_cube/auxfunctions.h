#ifndef AUXFUNCTIONS_H
#define AUXFUNCTIONS_H

#include <vector>
#include "Vector.h"

void sortByDotProduct(std::vector<Vector>& points, Vector *direction);
void sortPortionByDotProduct(std::vector<Vector>& points, int beginIndex, int endIndex, Vector *direction);

//
void testAuxFunctions();

#endif // AUXFUNCTIONS_H
