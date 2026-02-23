#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 12345);
    timer = new QTimer(this);

    // 데이터가 들어오면 progressPendingdatagrams 함수 실행하자!
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processPendingDatagrams);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);

    std::string url = "udp://192.168.137.1:5000";

    if(cap.open(url, cv::CAP_FFMPEG)){
        cap.set(cv::CAP_PROP_BUFFERSIZE, 1);
        timer->start(10);
    }

    // Chart Setting
    actualSeries = new QLineSeries();
    targetSeries = new QLineSeries();
    actualSeries->setName("Actual RPM");
    targetSeries->setName("Target RPM");

    // 목표값은 점선으로 표기
    QPen pen = targetSeries->pen();
    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::red);
    targetSeries->setPen(pen);

    QChart *chart= new QChart();
    chart->addSeries(actualSeries);
    chart->addSeries(targetSeries);
    chart->setTitle("PID Control Mornitoring");

    axisX = new QValueAxis();
    axisX->setRange(0,10);

    chart->addAxis(axisX, Qt::AlignBottom);

    axisX->setTitleText("Time (s)");
    axisX->setTitleVisible(true);

    axisY = new QValueAxis();
    axisY-> setRange(0,200);
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY-> setTitleText("RPM");
    // axisX->setTitleVisible(true);
    // chart->addAxis(axisX, Qt::AlignBottom);

    actualSeries->attachAxis(axisX);
    actualSeries->attachAxis(axisY);
    targetSeries->attachAxis(axisX);
    targetSeries->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
    layout->addWidget(chartView);
    layout->setContentsMargins(0,0,0,20);

    plotTimer.start();

    // ui->RPM_gauge->setSource(QUrl::fromLocalFile("C:/Qt_Project_soo/sdv_control/gauge.qml"));
    //
    ui->RPM_gauge->setSource(QUrl(QStringLiteral("qrc:/gauge.qml")));
    ui->RPM_gauge->setResizeMode(QQuickWidget::SizeRootObjectToView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AccelBtn_pressed()
{
    QByteArray message = "ACCEL_ON\n";
    udpSocket->writeDatagram(message, QHostAddress(piAddress), 8888);
    qDebug() << "Tx : Accel_Btn!!";
}

void MainWindow::on_BrakeBtn_pressed()
{
    QByteArray message = "BRAKE_ON\n";
    udpSocket->writeDatagram(message, QHostAddress(piAddress), 8888);
    qDebug() << "Tx : Brake_Btn";
}


void MainWindow::on_BrakeBtn_released()
{
    QByteArray message = "BRAKE_OFF\n";
    udpSocket->writeDatagram(message, QHostAddress(piAddress), 8888);
    qDebug() << "손을 뗐습니다!";
}


void MainWindow::on_AccelBtn_released()
{
    QByteArray message = "ACCEL_OFF\n";
    udpSocket->writeDatagram(message, QHostAddress(piAddress), 8888);
    qDebug() << "손을 뗐습니다!";
}

void MainWindow::updateFrame()
{
    cv::Mat frame;
    if(!cap.read(frame)){
        qDebug() << "Read Frame Fail!";
        return;
    }

    if(!frame.empty()){
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage qImg((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        ui->videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(ui->videoLabel->size(), Qt::KeepAspectRatio));
    }
}

// UDP를 통해 파이로 부터 데이터를 받아오는 것
void MainWindow::processPendingDatagrams(){
    while(udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString data = QString::fromUtf8(datagram);

        QStringList list = data.split(",");
        if(list.size() < 2) return;

        float current_rpm = list[0].split(":")[1].toFloat();
        float target_rpm = list[1].split(":")[1].toFloat();

        // Send to RPM_Gauge
        QObject *rootObject = ui->RPM_gauge->rootObject();
        if(rootObject){
            rootObject->setProperty("rpmValue", current_rpm);
        }
        // Graph 그
        updateCharts(current_rpm, target_rpm);
    }
}

// 그래프 그리기!
void MainWindow::updateCharts(float current, float target){
    // qDebug() << "RPM Tx Complete -> Current : "<<current <<"Target : " << target << "Error: " << error;

    qreal time = plotTimer.elapsed() / 1000.0;

    actualSeries->append(time, current);
    targetSeries->append(time, target);
    // errorSeries->append(time, target - current);

    if(time>10){
        axisX->setRange(time-10, time);
    }

    float error_rate = 0.0f;
    if(qAbs(target) > 0.1f){
        error_rate = (qAbs(target-current)/target)*100.0f;
    }

    ui->lblErrorRate->setText(QString("Error : %1 %").arg(error_rate, 0 ,'f',2));
}
