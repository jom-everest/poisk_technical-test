#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QChartView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initIndicators();
    initChart();

    // запуск таймера с периодом 250 мс и привязка к слоту "updateStateFirstSetOnTimer()"
    // метод "updateStateFirstSetOnTimer()" будет вызываться каждый раз по сигналу таймера, каждые 250 мс
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mainControllerOnTimer()));
    timer->start(TIMER_DELAY_MILLISEC);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainControllerOnTimer()
{
    // снятие данных с датчиков (2 функции sin)
    sensorsData = sensorsService.getData();

    // Вычисление Аналоговых Сигналов
    analogSignalsSet = analogSignalsService.compute(sensorsData, discretSignalsSet);

    // передача данных, в нашем случае - запись на файл
    transferService_AS.sendData(analogSignalsSet);

    // получение Дискретных Сигналов - чтение из файла
    transferService_DS.recieveData(discretSignalsSet);

    // обновление отображения данных на форме приложения
    updateSignalsView();
}

void MainWindow::updateSignalsView()
{
    updateDiscretSignalsView();
    updateSensorsAndAnalogSignalsView();
}

void MainWindow::initIndicators()
{
    listViewIndicators_forDiscretSignals.push_back({ui->label1, ui->lcdNumber1});
    listViewIndicators_forDiscretSignals.push_back({ui->label2, ui->lcdNumber2});
    listViewIndicators_forDiscretSignals.push_back({ui->label3, ui->lcdNumber3});
    listViewIndicators_forDiscretSignals.push_back({ui->label4, ui->lcdNumber4});

    for (auto i: listViewIndicators_forDiscretSignals) {
        i.label->setText("");
        i.label->setFixedSize(6, 6);
        i.label->setStyleSheet("border-radius: 30px; background-color: blue;");
        i.lcdNumber->display(0);
    }

    ui->horizontalSlider->setValue(50);
    ui->horizontalSlider_2->setValue(50);
    ui->label->setText("");
    ui->label_2->setText("");
}


void MainWindow::initChart() {
    barSet = new QBarSet("");
    *barSet << 0 << 0<< 0 << 0 << 0 << 0;
    series = new QBarSeries();
    series->append(barSet);
    chart = new QChart();
    chart->addSeries(series);
//    chart->setTitle("Simple barchart example");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "1-st function"<< "2-nd function" <<"1-st Signal" << "2-nd Signal" << "3-rd Signal" << "4-th Signal";
    axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->axisY()->setRange(-2, 2);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout_2->addWidget(chartView, 0, 0);
    chart->setMargins(QMargins(5, 5, 5, 5));
}

void MainWindow::updateDiscretSignalsView()
{
    for (int i = 0; i < DISCRETSIGNALSSET_SIZE; i++) {
        if (discretSignalsSet.values[i] == 0) {
            listViewIndicators_forDiscretSignals[i].label->setStyleSheet("background-color: blue;");
            listViewIndicators_forDiscretSignals[i].lcdNumber->display(0);
        }
        else {
            listViewIndicators_forDiscretSignals[i].label->setStyleSheet("background-color: red;");
            listViewIndicators_forDiscretSignals[i].lcdNumber->display(1);
        }
    }
}

void MainWindow::updateSensorsAndAnalogSignalsView()
{
    barSet->replace(0, sensorsData.first);
    barSet->replace(1, sensorsData.second);

    barSet->replace(2, analogSignalsSet.values[0]);
    barSet->replace(3, analogSignalsSet.values[1]);
    barSet->replace(4, analogSignalsSet.values[2]);
    barSet->replace(5, analogSignalsSet.values[3]);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->label->setText(QString::number(position) + " %");
    sensorsService.setPh(M_PI*position/100);
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->label_2->setText(QString::number((double)position/50, 'g', 4) + " times");
    sensorsService.setAm((double)position/50);
    chart->axisY()->setRange(-(position+25)/25, (position)/25+1);
}
