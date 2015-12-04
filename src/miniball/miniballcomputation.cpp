#include "miniballcomputation.h"
#include "Seb.h"
#include <vector>

//
typedef double FT;
typedef Seb::Point<FT> Point;
typedef std::vector<Point> PointVector;
typedef Seb::Smallest_enclosing_ball<FT> Miniball;

//
using namespace std;

MiniBallComputation::MiniBallComputation()
{

}

MiniBallComputation::~MiniBallComputation()
{

}

void MiniBallComputation::computeMiniBall(std::vector<Vector> &points, Vector &center, double& radius){
    //
    int numPoints = points.size();

    assert(points.size() > 0);

    int dimension = points.at(0).getDimension();

    assert(center.getDimension() == dimension);

    // Construct n random points in dimension d
    PointVector S;
    for (int i=0; i<numPoints; ++i) {
        Vector& vec = points.at(i);
        Point pt(dimension);
        for(int d = 0 ; d < dimension ; ++d){
            pt[d] = vec[d];
        }
        S.push_back(pt);
    }
    // Compute the miniball by inserting each value
    Miniball mb(dimension, S);

    // Output
    radius = mb.radius();

    //
    Miniball::Coordinate_iterator center_it = mb.center_begin();
    for (int j=0; j<dimension; ++j)
        center[j] = center_it[j];
}



