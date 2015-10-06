#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <string.h>
#include <iostream>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::selectedPointTextInput(QString text)
{
    cout << text.toStdString() << "\n";
    flush(cout);
    char *textCon, *token;
    textCon = (char*) malloc(sizeof(char)*(text.size()+1));
    strcpy(textCon, text.toStdString().c_str());
    QVector3D point;
    int i = 0;

    token = strtok(textCon, " ");
    while (token != NULL && i < 3) {
        cout << "atoi " << atoi(token) << "\n";
        point[i] = atoi(token);
        i++;
        token = strtok(NULL, " ");
    }

    ui->glWidget->selectGridPoint(point);
}
