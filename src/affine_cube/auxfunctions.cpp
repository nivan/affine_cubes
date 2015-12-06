#include "auxfunctions.h"
#include "dotproductcomparison.h"
#include <algorithm>
#include <iostream>

using namespace std;

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


void testAuxFunctions(){
    //
    Vector v1(3);
    v1[0] = 1.0;
    v1[1] = -1.0;
    v1[2] = 3.0;

    Vector v2(3);
    v2[0] = 2.0;
    v2[1] = 10.0;
    v2[2] = -9.0;

    Vector v3(3);
    v3[0] = -5.5;
    v3[1] = 12.0;
    v3[2] = 39.0;

    Vector v4(3);
    v4[0] = -29.0;
    v4[1] = -15.0;
    v4[2] = -45.0;

    Vector v5(3);
    v5[0] = 19.0;
    v5[1] = -35.0;
    v5[2] = 15.0;

    //
    std::vector<Vector> points;
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    points.push_back(v4);
    points.push_back(v5);

    //points
    cout << ">>>>>>>>>>>>" << endl;
    int numPoints = points.size();
    for(int i = 0 ; i < numPoints ; ++i){
        cout << "point " << i << " " << points.at(i).toString() << endl;
    }
    cout << "===========" << endl;
    Vector probeDir(3);
    probeDir[0] = 1.0;

    //
    sortPortionByDotProduct(points,1,3,&probeDir);
    //points
    for(int i = 0 ; i < numPoints ; ++i){
        cout << "point " << i << " " << points.at(i).toString() << endl;
    }
    cout << "<<<<<<<<<<<<" << endl;

}
