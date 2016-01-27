#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioinputanalyzer.h"
#include "renderarea.h"
#include <QLabel>
#include <QMainWindow>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

private:
    AudioInputAnalyzer *m_audioInputAnalyzer;
    QLabel *m_audioInput;

    virtual void initializeWindow();

private slots:
    void signalSoundDetected();

};

#endif // MAINWINDOW_H
