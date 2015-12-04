#include "widget.h"
#include <QApplication>

#include "../affine_cube/boundingvolume.h"
#include "../affine_cube/bvtree.h"
#include "glhistogramwidget.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
