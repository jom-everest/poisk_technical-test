#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QTime>

#include <QtCharts/QChartView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myFile = new QFile("log_controller.txt");
    myFile->open(QIODevice::WriteOnly);
    logger = new QTextStream(myFile);

    // первоначальная настройка индикаторов группы "Дискретных состояний".
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
    delete logger;
    delete myFile;
    delete ui;
}

//------------------------------------------------------------------------------------------
// главная логика работы первой программы здесь
// эта функция вызывается каждые 250 мс
void MainWindow::mainControllerOnTimer()
{
    // получение состояния Дискретных Сигналов
    discretSignalsSet = discretSignalsProviderService.getData();

    // получение состояния Аналоговых Сигналов и в ответ отправление состояния Дискретных Сигналов
    if (transferService_AS.recieveData(analogSignalsSet))
       transferService_DS.sendData(discretSignalsSet);

    // проверка величин Аналоговых Сигналов на допустимый диапазон
    if (analyserAnalogSignals.isOutrange(analogSignalsSet, indexOutrangedSignals)) {
        // запись в лог
        static QTime time;
        static std::set<int> indexes_old;
        if ((abs(time.second() - QTime::currentTime().second()) > 1) || (indexes_old != indexOutrangedSignals)) {
            time = QTime::currentTime();
            indexes_old = indexOutrangedSignals;
            QString warnMessage = time.toString() + ": [";
            for (auto i = begin(indexes_old); i != end(indexes_old); ++i) {
                warnMessage += QString::number(*i) + ": value='" +
                        QString::number(analogSignalsSet.values[*i], 'g', 4) + "'";
                if (i != end(indexes_old)) warnMessage += ", ";
            }
            warnMessage += "] - 'выход за допустимый диапазон'\n" ;
            *logger << warnMessage;
        }
    }

    // обновить отображение сигналов в окне приложения
    updateSignalsView();
}
//------------------------------------------------------------------------------------------

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

    ui->groupBox_2->setTitle("1 Signal");
    ui->verticalSlider->setRange(-100, 100);
    ui->verticalSlider->setValue(100);
    ui->verticalSlider_2->setRange(-100, 100);
    ui->verticalSlider_2->setValue(-100);
    ui->label->setText("2");
    ui->label_2->setText("-2");

    ui->groupBox_3->setTitle("2 Signal");
    ui->verticalSlider_3->setRange(-100, 100);
    ui->verticalSlider_3->setValue(100);
    ui->verticalSlider_4->setRange(-100, 100);
    ui->verticalSlider_4->setValue(-100);
    ui->label_3->setText("2");
    ui->label_4->setText("-2");

    ui->groupBox_4->setTitle("3 Signal");
    ui->verticalSlider_5->setRange(-100, 100);
    ui->verticalSlider_5->setValue(100);
    ui->verticalSlider_6->setRange(-100, 100);
    ui->verticalSlider_6->setValue(-100);
    ui->label_5->setText("2");
    ui->label_6->setText("-2");

    ui->groupBox_5->setTitle("4 Signal");
    ui->verticalSlider_7->setRange(-100, 100);
    ui->verticalSlider_7->setValue(100);
    ui->verticalSlider_8->setRange(-100, 100);
    ui->verticalSlider_8->setValue(-100);
    ui->label_7->setText("2");
    ui->label_8->setText("-2");

}


void MainWindow::initChart() {
    barSet = new QBarSet("");
    *barSet << 0 << 0<< 0 << 0;
    series = new QBarSeries();

    series->append(barSet);
    chart = new QChart();
    chart->addSeries(series);
//    chart->setTitle("Simple barchart example");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "1-st Signal" << "2-nd Signal" << "3-rd Signal" << "4-th Signal";
    axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->axisY()->setRange(-2, 2);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setMargins(QMargins(5,5,5,5));
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget(chartView, 0, 0);
}

void MainWindow::updateAnalogSignalsView()
{
    int max = 0;
    for (int i = 0; i < ANALOGSIGNALSSET_SIZE; i++) {
        if (abs(analogSignalsSet.values[i]) > max)
            max = abs(analogSignalsSet.values[i]);
        barSet->replace(i, analogSignalsSet.values[i]);
    }
    if (++max < 2) max = 2;
    chart->axisY()->setRange(-max, max);

    // оповещение о выходе уровня сигналов за указанный уровень
    static bool isAlarm = false;
    if (!indexOutrangedSignals.empty()) {
        static int ticks = 0;
        ticks++;

        isAlarm = true;
        if ((ticks % 8) < 4)
            ui->centralWidget->setStyleSheet("#centralWidget{border: 5px solid red;}");
        else
            ui->centralWidget->setStyleSheet("");
    }
    else if (isAlarm) {
        ui->centralWidget->setStyleSheet("");
        isAlarm = false;
    }
}

void MainWindow::updateSignalsView()
{
    updateDiscretSignalsView();
    updateAnalogSignalsView();
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

void MainWindow::on_setSignalButton_pressed()
{
    discretSignalsProviderService.setControlSignal(true);
    ui->setSignalButton->setText("UnSet");
}

void MainWindow::on_setSignalButton_released()
{
    discretSignalsProviderService.setControlSignal(false);
    ui->setSignalButton->setText("Set");
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    if (position < 0) {
        ui->verticalSlider->setValue(0);
        position = 0;
    }

    ui->label->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMax(0, (double)position/50);
}

void MainWindow::on_verticalSlider_2_sliderMoved(int position)
{
    if (position > 0) {
        ui->verticalSlider_2->setValue(0);
        position = 0;
    }

    ui->label_2->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMin(0, (double)position/50);
}

void MainWindow::on_verticalSlider_3_sliderMoved(int position)
{
    if (position < 0) {
        ui->verticalSlider_3->setValue(0);
        position = 0;
    }

    ui->label_3->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMax(1, (double)position/50);
}

void MainWindow::on_verticalSlider_4_sliderMoved(int position)
{
    if (position > 0) {
        ui->verticalSlider_4->setValue(0);
        position = 0;
    }

    ui->label_4->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMin(1, (double)position/50);
}

void MainWindow::on_verticalSlider_5_sliderMoved(int position)
{
    if (position < 0) {
        ui->verticalSlider_5->setValue(0);
        position = 0;
    }

    ui->label_5->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMax(2, (double)position/50);
}

void MainWindow::on_verticalSlider_6_sliderMoved(int position)
{
    if (position > 0) {
        ui->verticalSlider_6->setValue(0);
        position = 0;
    }

    ui->label_6->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMin(2, (double)position/50);
}

void MainWindow::on_verticalSlider_7_sliderMoved(int position)
{
    if (position < 0) {
        ui->verticalSlider_7->setValue(0);
        position = 0;
    }

    ui->label_7->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMax(3, (double)position/50);
}

void MainWindow::on_verticalSlider_8_sliderMoved(int position)
{
    if (position > 0) {
        ui->verticalSlider_8->setValue(0);
        position = 0;
    }

    ui->label_8->setText(QString::number((double)position/50, 'g', 4));
    analyserAnalogSignals.setRestrictionMin(3, (double)position/50);
}
