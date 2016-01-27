#include "mainwindow.h"

#include <QHBoxLayout>
#include <QObject>

MainWindow::MainWindow() {
    initializeWindow();
    connect(m_audioInputAnalyzer, SIGNAL(soundDetected()),
                          this, SLOT(signalSoundDetected()));

}

MainWindow::~MainWindow() {

}

void MainWindow::initializeWindow() {
    m_audioInputAnalyzer = new AudioInputAnalyzer;
    m_audioInputAnalyzer->start();
    m_audioInput = new QLabel("wait for a sound");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_audioInput);

    setLayout(layout);
}

void MainWindow::signalSoundDetected() {
    m_audioInput->setText("SOUND DETECTED");
}
