#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include "../affine_cube/bvtree.h"
#include "../affine_cube/generalbvtree.h"
#include "util/colorscale.h"

//#define BOUNDING_VOLUME_T Sphere
#define BOUNDING_VOLUME_T AxisAlignedBoundingVolume

void readCSVPoints(QString filename,std::vector<Vector>& points);

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = 0);
    ~HistogramWidget();

private:
    //BVTree<BOUNDING_VOLUME_T>* myTree;
    GeneralBVTree<BOUNDING_VOLUME_T>* myTree;
    int depth;
    double minX;
    double maxX;
    double minY;
    double maxY;
    ColorScale* scale;
private:
    Vector* xAxis;
    Vector* yAxis;
protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:
    void setDepth(int);
};

#endif // HISTOGRAMWIDGET_H
