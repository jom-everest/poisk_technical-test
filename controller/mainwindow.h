#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "analogsignalsservice.h"
#include "analyseranalogsignals.h"
#include "discretservice.h"
#include "discretsignalsproviderservice.h"

#include <set>

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QLCDNumber>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QTextStream>


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

//-------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateSignalsView();
    void updateDiscretSignalsView();
    void updateAnalogSignalsView();
    void initIndicators();
    void mainControllerOnTimer();
    void initChart();

    void on_setSignalButton_pressed();
    void on_setSignalButton_released();

    void on_verticalSlider_sliderMoved(int position);

    void on_verticalSlider_2_sliderMoved(int position);

    void on_verticalSlider_3_sliderMoved(int position);

    void on_verticalSlider_4_sliderMoved(int position);

    void on_verticalSlider_5_sliderMoved(int position);

    void on_verticalSlider_6_sliderMoved(int position);

    void on_verticalSlider_7_sliderMoved(int position);

    void on_verticalSlider_8_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QBarSet *barSet;
    QChart *chart;
    QBarSeries *series;
    QBarCategoryAxis *axis;

    std::set<int> indexOutrangedSignals;

    TransferDataService_AS transferService_AS;
    TransferDataService_DS transferService_DS;
    DiscretSignalsProviderService discretSignalsProviderService;
    AnalyserAnalogSignals analyserAnalogSignals;

    DiscretSignalsSet discretSignalsSet;
    AnalogSignalsSet analogSignalsSet;
    QFile* myFile;
    QTextStream* logger;
    std::vector<IndicatorGroup_forDiscretSignals> listViewIndicators_forDiscretSignals;
};

#endif // MAINWINDOW_H
