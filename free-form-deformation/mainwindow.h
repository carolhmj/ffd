#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QVector4D>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    //void changeNumGridPointsSignal(int ns, int nt, int nu);

public slots:
    void changeNumGridPoints(int nNum);
    void populatePointSelector(std::vector<QVector4D> pointList);
};

#endif // MAINWINDOW_H
