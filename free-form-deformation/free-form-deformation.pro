#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T21:14:43
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = free-form-deformation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    ffdgrid.cpp \
    gridpoints.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    ffdgrid.h \
    gridpoints.h

FORMS    += mainwindow.ui

LIBS     += -L $$PWD/trimesh2/lib.Linux64/ -ltrimesh

INCLUDEPATH += $$PWD/trimesh2/include/

QMAKE_CXXFLAGS += -fopenmp -ggdb -std=c++11
QMAKE_LFLAGS +=  -fopenmp -ggdb
