#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T15:06:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = affine_cubes
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    #../affine_cube/Vector.cpp \
    ../affine_cube/boundingvolume.cpp \
    ../affine_cube/bvtree.cpp \
    histogramwidget.cpp \
    util/yellowtobrownscale.cpp \
    util/sequentialred.cpp \
    util/scalar.cpp \
    util/rainbowscale.cpp \
    util/pseudorainbowscale.cpp \
    util/locsscale.cpp \
    util/lineargrayscale.cpp \
    util/heatedobjectscale.cpp \
    util/greentowhitescale.cpp \
    util/grayscale.cpp \
    util/divergent.cpp \
    util/colorscale.cpp \
    util/colorbar.cpp \
    util/bluetoyellowscale.cpp \
    util/bluetocyanscale.cpp \
    glhistogramwidget.cpp \
    ../miniball/Seb_debug.C \
    ../miniball/miniballcomputation.cpp \
    ../affine_cube/comparators.cpp \
    ../affine_cube/auxfunctions.cpp \
    ../affine_cube/histogram2dbin.cpp \
    ../affine_cube/generalbvtree.cpp \
    scatterplotwidget.cpp \
    ../pca/pca.cpp \
    ../ckmeans/Ckmeans.1d.dp.cpp

HEADERS  += widget.h \
    #../affine_cube/Vector.h \
    ../affine_cube/boundingvolume.h \
    ../affine_cube/bvtree.h \
    histogramwidget.h \
    util/yellowtobrownscale.h \
    util/sequentialred.h \
    util/scalar.h \
    util/rainbowscale.h \
    util/pseudorainbowscale.h \
    util/locsscale.h \
    util/lineargrayscale.h \
    util/heatedobjectscale.h \
    util/greentowhitescale.h \
    util/grayscale.h \
    util/divergent.h \
    util/colorscale.h \
    util/colorbar.h \
    util/bluetoyellowscale.h \
    util/bluetocyanscale.h \
    glhistogramwidget.h \
    ../miniball/Subspan.h \
    ../miniball/Subspan-inl.h \
    ../miniball/Seb_point.h \
    ../miniball/Seb_debug.h \
    ../miniball/Seb_configure.h \
    ../miniball/Seb.h \
    ../miniball/Seb-inl.h \
    ../miniball/miniballcomputation.h \
    ../affine_cube/comparators.h \
    ../affine_cube/auxfunctions.h \
    ../affine_cube/histogram2dbin.h \
    ../affine_cube/generalbvtree.h \
    scatterplotwidget.h \
    ../pca/pca.h \
    ../ckmeans/Ckmeans.1d.dp.h


FORMS    += widget.ui

INCLUDEPATH += "/home/nivan/src/crow/include"

QMAKE_CXXFLAGS += -std=c++0x

LIBS += -lboost_system
