#include "histogramwidget.h"
#include <QPainter>
#include <cassert>


HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent)
{

    //
    this->scale = ColorScaleFactory::getInstance(SEQUENTIAL_SINGLE_HUE_RED);
    this->depth = 0;
    std::vector<Vector> points;

#if 1
    //random data
    int numDimensions = 4;
    xAxis = new Vector(numDimensions);
    yAxis = new Vector(numDimensions);
    //
    (*xAxis)[0] = 1.0;
    //
    (*yAxis)[1] = 1.0;

    int numPoints = 1000;
    for(int i = 0 ; i < numPoints ; ++i){
        Vector point(numDimensions);
        for(int d = 0 ; d < numDimensions ; ++d){
            point[d] = (rand() * 1.0) / RAND_MAX;
        }
        points.push_back(point);
    }
#elif 1
    //test data from file
    double weights1[] = {-0.25504687, 0.18069224, 0.1844671 , 0.17998339, 0.1802003 , 0.18041489, 0.18067747, 0.18090149, 0.18115207, 0.17661623, 0.16695162, 0.15727139, 0.14759242, 0.13790725, 0.12803831, 0.11821229, 0.10829935, 0.09838501, 0.08841584, 0.07843253, 0.06835597, 0.05828226, 0.04821307, 0.03788746, 0.02672559, 0.01472539, 0.00423005, -0.0062836 , -0.01702462, -0.02799306, -0.03904066, -0.05036745, -0.0611769 , -0.07142132, -0.08215804, -0.09285149, -0.10352378, -0.11420251, -0.12470873, -0.13543912, -0.14567545, -0.15577893, -0.16604223, -0.17625254, -0.18710215, -0.19956397, -0.21119001, -0.22317638, -0.22693173, -0.24007948};
    double weights2[] = {2.52039455e-01, 1.85265745e-01, 2.06818400e-01, 1.61659806e-01, 1.53324752e-01, 1.43764320e-01, 1.33276201e-01, 1.21136035e-01, 1.09050964e-01, 9.47932428e-02, 7.96899821e-02, 6.40513832e-02, 4.76863094e-02, 3.08504044e-02, 1.41712468e-02, -3.10729848e-03, -2.03631152e-02, -3.75688530e-02, -5.44987525e-02, -7.12202229e-02, -8.70528505e-02, -1.02861702e-01, -1.18168552e-01, -1.29366199e-01, -1.41220889e-01, -2.11526469e-01, -3.20735274e-01, -4.30085572e-01, -1.52883711e-01, -1.47983904e-01, -1.42151341e-01, -1.41987480e-01, -1.31315340e-01, -1.09592714e-01, -9.64478384e-02, -8.17477040e-02, -6.55620128e-02, -5.08524983e-02, -3.43266722e-02, -1.78404853e-02, 2.66086889e-04, 2.22458641e-02, 4.37592185e-02, 6.92606391e-02, 9.61266730e-02, 1.33096475e-01, 1.65681949e-01, 1.97696977e-01, 1.72731007e-01, 2.02024311e-01};

    for(int i = 0 ; i < 50 ; ++i){
        (*xAxis)[i] = weights1[i];
        (*yAxis)[i] = weights2[i];
    }

    readCSVPoints("../../data/i_band.csv",points);
#endif


    //
    qDebug() << "building tree";
    myTree = new BVTree(points);
    qDebug() << "done building tree";
    myTree->getDotProductRange(*xAxis,minX,maxX);
    myTree->getDotProductRange(*yAxis,minY,maxY);
}

HistogramWidget::~HistogramWidget()
{
    if(this->myTree)
        delete myTree;
    if(xAxis)
        delete xAxis;
    if(yAxis)
        delete yAxis;
}

void HistogramWidget::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,width(),height());


    std::vector<Histogram2DBin> result;
    myTree->getUnbinnedHistogram(*xAxis,*yAxis,depth,result);
    int numbins = result.size();

    //
    QTransform m;
    m.scale(width() / (maxX - minX),height() / (maxY - minY));
    m.translate(-minX,-minY);

    //
    float maxCount = 0;
    for(int i = 0 ; i < numbins ; ++i){
        Histogram2DBin bin = result.at(i);
        if(maxCount < bin.count){
            maxCount = bin.count;
        }
    }


    //qDebug() << "Num Bins " << numbins << result.at(0).count;
    for(int i = 0 ; i < numbins ; ++i){
        Histogram2DBin bin = result.at(i);
        QColor color = this->scale->getColor(bin.count / maxCount);
        painter.setBrush(color);
        QPolygonF screenRect = m.map(QRectF(QPointF(bin.minX,bin.minY),QPointF(bin.maxX,bin.maxY)));
        painter.drawPolygon(screenRect);
    }

}

void HistogramWidget::setDepth(int v){
    this->depth = v;
    this->repaint();
}



void readCSVPoints(QString filename,std::vector<Vector>& points){
    points.clear();

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open file " << filename;
        assert(false);
    }

    qDebug() << "reading points";
    while(!file.atEnd()){
        QString line = file.readLine();
        QStringList tokens = line.split(",");
        int numDimensions = tokens.size();
        Vector point(numDimensions);
        for(int i = 0 ; i < numDimensions ; ++i){
            bool ok = false;
            point[i] = tokens[i].toDouble(&ok);
            assert(ok);
        }
        points.push_back(point);
    }
    qDebug() << "done reading points " << points.size();
}
