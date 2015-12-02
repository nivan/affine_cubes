#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include "../affine_cube/bvtree.h"
#include "util/colorscale.h"

void readCSVPoints(QString filename,std::vector<Vector>& points);

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = 0);
    ~HistogramWidget();

private:
    BVTree* myTree;
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
