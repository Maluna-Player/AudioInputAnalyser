#include "audioinputanalyzer.h"

#include <QDebug>

AudioInputAnalyzer::AudioInputAnalyzer()
    :   m_sensibility(0.8),
        m_audioInfo(),
        m_audioInput(0),
        m_device(QAudioDeviceInfo::defaultInputDevice()) {

}

AudioInputAnalyzer::~AudioInputAnalyzer() {

}

void AudioInputAnalyzer::setSensibility(qreal newSensibility) {
    m_sensibility = newSensibility;
}

qreal AudioInputAnalyzer::getSensibility() const {
    return m_sensibility;
}

void AudioInputAnalyzer::initializeAudio() {
    initializeFormat();
    checkIfAudioDeviceSupported();

    m_audioInfo = new AudioInfo(m_format, this);
    connect(m_audioInfo, SIGNAL(update()), this, SLOT(onAudioInputUpdate()));

    createAudioInput();
}

void AudioInputAnalyzer::onAudioInputUpdate() {
    if (m_audioInfo->level() >= getSensibility()) {
      emit soundDetected();
    }
}

void AudioInputAnalyzer::start() {
    // TODO
    initializeAudio();
}

void AudioInputAnalyzer::stop() {
    //TODO
}

void AudioInputAnalyzer::initializeFormat() {
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");
}

void AudioInputAnalyzer::checkIfAudioDeviceSupported() {
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }
}

void AudioInputAnalyzer::createAudioInput() {
    m_audioInput = new QAudioInput(m_device, m_format, this);
    m_audioInfo->start();
    m_audioInput->start(m_audioInfo);
}
