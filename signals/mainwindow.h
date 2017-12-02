#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "transferdataservice_as.h"
#include "discretservice.h"
#include "sensorsservice.h"
#include "analogsignalsservice.h"
#include <vector>

#include <QMainWindow>

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QLCDNumber>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>


QT_CHARTS_USE_NAMESPACE


namespace Ui {
class MainWindow;
}

//-------------
struct IndicatorGroup_forDiscretSignals {
    QLabel* label;
    QLCDNumber* lcdNumber;
    IndicatorGroup_forDiscretSignals(QLabel* l, QLCDNumber* n) : label(l), lcdNumber(n) {}
};

//---------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateSignalsView();
    void updateDiscretSignalsView();
    void updateSensorsAndAnalogSignalsView();
    void initIndicators();
    void mainControllerOnTimer();
    void initChart();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    QBarSet *barSet;
    QChart *chart;
    QBarSeries *series;
    QBarCategoryAxis *axis;

    TransferDataService_AS transferService_AS;
    TransferDataService_DS transferService_DS;

    SensorsService sensorsService;
    AnalogSignalsService analogSignalsService;

    SensorsData sensorsData;
    DiscretSignalsSet discretSignalsSet;
    AnalogSignalsSet analogSignalsSet;
    std::vector<IndicatorGroup_forDiscretSignals> listViewIndicators_forDiscretSignals;
};

#endif // MAINWINDOW_H
