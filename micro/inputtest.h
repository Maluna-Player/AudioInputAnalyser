#ifndef INPUTTEST_H
#define INPUTTEST_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include "audioinfo.h"
#include "renderarea.h"

class InputTest : public QMainWindow
{
    Q_OBJECT

public:
    InputTest();
    ~InputTest();

private:
    void initializeWindow();
    void initializeAudio();
    void createAudioInput();

private slots:
    void refreshDisplay();
    void readMore();
    void toggleMode();
    void toggleSuspend();
    void deviceChanged(int index);
    void sliderChanged(int value);

private:
    // Owned by layout
    RenderArea *m_canvas;
    QPushButton *m_modeButton;
    QPushButton *m_suspendResumeButton;
    QComboBox *m_deviceBox;
    QSlider *m_volumeSlider;

    QAudioDeviceInfo m_device;
    AudioInfo *m_audioInfo;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QIODevice *m_input;
    bool m_pullMode;
    QByteArray m_buffer;
};

#endif // INPUTTEST_H
