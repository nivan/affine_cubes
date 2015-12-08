#include "widget.h"
#include <QApplication>

#include "../affine_cube/boundingvolume.h"
#include "../affine_cube/bvtree.h"
#include "glhistogramwidget.h"
#include "../affine_cube/generalbvtree.h"
#include <iostream>
#include "../pca/pca.h"

#include <eigen3/Eigen/Dense>
using namespace Eigen;
using namespace std;

const int SIZE = 7;

int main(int argc, char *argv[])
{
#if 0
    //testAuxFunctions();
    testBoundingVolume();
    //    testGeneralTree();
    //    testBVTree();

#elif 0
    //testPCA();
    testRoatedBox();
#elif 0
    int numPoints = 20;
    double noisePortion = 0.05;
    std::vector<double> dataFractions{noisePortion, 0.5 - noisePortion,0.5 - noisePortion, noisePortion};

    bool needToSplit = true;//numPoints >= 1 / noisePortion;

    int beginIndex = 0;
    int endIndex = beginIndex + numPoints;

    if(needToSplit){

        double accumSum = 0;
        int prevStart = 0;
        int numChildren = 4;

        for(int i = 0 ; i < numChildren ; ++i){
            //
            int childBeginIndex = prevStart;
            accumSum += dataFractions.at(i) * numPoints;
            int childEndIndex   = floor(accumSum);
            if(i == (numChildren -1))
                childEndIndex = numPoints;
            prevStart = childEndIndex;

            cout << "node " << i << " : " << (childBeginIndex + beginIndex) << " , " << (childEndIndex+beginIndex) << endl;
        }

    }

    return 0;
#elif 1
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
#elif 1
    QApplication a(argc, argv);
    GLHistogramWidget w;
    w.show();

    return a.exec();
#endif
}
