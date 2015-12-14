#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include "../affine_cube/bvtree.h"
//#include "../affine_cube/generalbvtree.h"
#include "util/colorscale.h"
#include "../affine_cube/affinecube.h"

//#define BOUNDING_VOLUME_T Sphere
//#define BOUNDING_VOLUME_T AxisAlignedBoundingBox
//#define BOUNDING_VOLUME_T RotatedBoundingBox

void readCSVPoints(QString filename,std::vector<Eigen::VectorXd>& points);

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = 0);
    ~HistogramWidget();
private:
    std::vector<Eigen::VectorXd> points;

private:
    //BVTree<BOUNDING_VOLUME_T>* myTree;
    //GeneralBVTree<BOUNDING_VOLUME_T>* myTree;
    AffineCube* myTree;
    int depth;
    double minX;
    double maxX;
    double minY;
    double maxY;
    ColorScale* scale;
private:
    Eigen::VectorXd* xAxis;
    Eigen::VectorXd* yAxis;
private:
    bool showPoints;
    bool showBins;
    double pointsAlpha;
protected:
    void paintEvent(QPaintEvent * event);
signals:

public slots:
    void setDepth(int);
    void setShowPoints(bool);
    void setShowBins(bool);
    void setPtsAlpha(int);
};

#endif // HISTOGRAMWIDGET_H
