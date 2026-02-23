#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QImage>
#include <QUdpSocket>
#include <QQuickItem>
#include <QLayout>
#include <QBoxLayout>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void on_AccelBtn_pressed();

    void on_BrakeBtn_pressed();

    void on_BrakeBtn_released();

    void on_AccelBtn_released();

    void updateFrame();

    void processPendingDatagrams(); // UDP 신호 오는것 처리

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
    QTimer *timer;
    QUdpSocket *udpSocket;
    QString piAddress = "192.168.137.2";

    QLineSeries *actualSeries;
    QLineSeries *targetSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QElapsedTimer plotTimer;

    void updateCharts(float current, float target);

};
#endif // MAINWINDOW_H
