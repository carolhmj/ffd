#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->glWidget, SIGNAL(displayPoints(std::vector<QVector4D>)), this, SLOT(populatePointSelector(std::vector<QVector4D>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeNumGridPoints(int nNum)
{
     //changeNumGridPointsSignal(ui->nsSelector->value(), ui->ntSelector->value(), ui->nuSelector->value());
    ui->glWidget->changeNumGridPoints(ui->nsSelector->value(), ui->ntSelector->value(), ui->nuSelector->value());
}

void MainWindow::populatePointSelector(std::vector<QVector4D> pointList)
{
    for (unsigned int i = 0; i < pointList.size(); i++){
        QString tmp;
        tmp.push_back("Point ");
        tmp.setNum(i);
        ui->pointSelector->addItem(tmp);
    }
}
