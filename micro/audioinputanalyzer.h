#ifndef AUDIOINPUTANALYZER_H
#define AUDIOINPUTANALYZER_H

#include "audioinfo.h"

#include <QAudioInput>

class AudioInputAnalyzer : public QObject
{
    Q_OBJECT

public:
    AudioInputAnalyzer();
    virtual ~AudioInputAnalyzer();

    virtual void setSensibility(qreal newSensibility);
    virtual qreal getSensibility() const;

public slots:
    virtual void start();
    virtual void stop();

private:
    AudioInfo *m_audioInfo;

    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioDeviceInfo m_device;

    qreal m_sensibility;

    virtual void initializeAudio();
    virtual void createAudioInput();

    virtual void initializeFormat();

    virtual void checkIfAudioDeviceSupported();

private slots:
    virtual void onAudioInputUpdate();

signals:
    void soundDetected();
};

#endif // AUDIOINPUTANALYZER_H
