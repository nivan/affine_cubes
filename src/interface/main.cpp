#include "widget.h"
#include <QApplication>

#include "../affine_cube/boundingvolume.h"
#include "../affine_cube/bvtree.h"
#include "glhistogramwidget.h"
#include "../affine_cube/generalbvtree.h"

#include <iostream>

using namespace std;

const int SIZE = 7;

int main(int argc, char *argv[])
{

#if 0
    int numPoints = 1000;
    double noisePortion = 0.05;
    std::vector<double> dataFractions{noisePortion, 1.0 - 2*noisePortion, noisePortion};

    bool needToSplit = numPoints >= 1 / noisePortion;

    if(needToSplit){

        double accumSum = 0;
        int prevStart = 0;
        int numChildren = 3;

        for(int i = 0 ; i < numChildren ; ++i){
            //
            int childBeginIndex = prevStart;
            accumSum += dataFractions.at(i) * numPoints;
            int childEndIndex   = floor(accumSum);
            if(i == (numChildren -1))
                childEndIndex = numPoints;
            prevStart = childEndIndex;

            cout << "node " << i << " : " << childBeginIndex << " , " << childEndIndex << endl;
        }

    }


    testGeneralTree();
    //    testBVTree();

    return 0;
#else
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
#endif
}
