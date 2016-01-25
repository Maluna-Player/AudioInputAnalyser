#include <QVBoxLayout>
#include <QDebug>
#include <iostream>

#include "inputtest.h"

#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend recording"
#define RESUME_LABEL    "Resume recording"

const int BufferSize = 4096;

InputTest::InputTest()
    :   m_canvas(0)
    ,   m_modeButton(0)
    ,   m_suspendResumeButton(0)
    ,   m_deviceBox(0)
    ,   m_device(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInfo(0)
    ,   m_audioInput(0)
    ,   m_input(0)
    ,   m_pullMode(false)
    ,   m_buffer(BufferSize, 0)
{
    initializeWindow();
    initializeAudio();
}


InputTest::~InputTest() {}

void InputTest::initializeWindow()
{
    QScopedPointer<QWidget> window(new QWidget);
    QScopedPointer<QVBoxLayout> layout(new QVBoxLayout);

    m_canvas = new RenderArea(this);
    layout->addWidget(m_canvas);

    m_deviceBox = new QComboBox(this);
    const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    m_deviceBox->addItem(defaultDeviceInfo.deviceName(), qVariantFromValue(defaultDeviceInfo));
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (deviceInfo != defaultDeviceInfo)
            m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }

    connect(m_deviceBox, SIGNAL(activated(int)), SLOT(deviceChanged(int)));
    layout->addWidget(m_deviceBox);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(100);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SLOT(sliderChanged(int)));
    layout->addWidget(m_volumeSlider);

    m_modeButton = new QPushButton(this);
    m_modeButton->setText(tr(PUSH_MODE_LABEL));
    connect(m_modeButton, SIGNAL(clicked()), SLOT(toggleMode()));
    layout->addWidget(m_modeButton);

    m_suspendResumeButton = new QPushButton(this);
    m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    connect(m_suspendResumeButton, SIGNAL(clicked()), SLOT(toggleSuspend()));
    layout->addWidget(m_suspendResumeButton);

    window->setLayout(layout.data());
    layout.take(); // ownership transferred

    setCentralWidget(window.data());
    QWidget *const windowPtr = window.take(); // ownership transferred
    windowPtr->show();
}

void InputTest::initializeAudio()
{
    m_pullMode = true;

    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    m_audioInfo  = new AudioInfo(m_format, this);
    connect(m_audioInfo, SIGNAL(update()), SLOT(refreshDisplay()));

    createAudioInput();
}

void InputTest::createAudioInput()
{
    m_audioInput = new QAudioInput(m_device, m_format, this);
    m_volumeSlider->setValue(m_audioInput->volume() * 100);
    m_audioInfo->start();
    m_audioInput->start(m_audioInfo);
}

void InputTest::readMore()
{
    if (!m_audioInput)
        return;
    qint64 len = m_audioInput->bytesReady();
    if (len > BufferSize)
        len = BufferSize;
    qint64 l = m_input->read(m_buffer.data(), len);
    if (l > 0)
        m_audioInfo->write(m_buffer.constData(), l);
}

void InputTest::toggleMode()
{
    // Change bewteen pull and push modes
    m_audioInput->stop();

    if (m_pullMode) {
        m_modeButton->setText(tr(PULL_MODE_LABEL));
        m_input = m_audioInput->start();
        connect(m_input, SIGNAL(readyRead()), SLOT(readMore()));
        m_pullMode = false;
    } else {
        m_modeButton->setText(tr(PUSH_MODE_LABEL));
        m_pullMode = true;
        m_audioInput->start(m_audioInfo);
    }

    m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
}

void InputTest::toggleSuspend()
{
    // toggle suspend/resume
    if (m_audioInput->state() == QAudio::SuspendedState) {
        m_audioInput->resume();
        m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    } else if (m_audioInput->state() == QAudio::ActiveState) {
        m_audioInput->suspend();
        m_suspendResumeButton->setText(tr(RESUME_LABEL));
    } else if (m_audioInput->state() == QAudio::StoppedState) {
        m_audioInput->resume();
        m_suspendResumeButton->setText(tr(SUSPEND_LABEL));
    } else if (m_audioInput->state() == QAudio::IdleState) {
        // no-op
    }
}

void InputTest::refreshDisplay()
{
    qreal level = m_audioInfo->level();
    if (level > 0.5) {
        m_canvas->setLevel(level);
    } else {
        m_canvas->setLevel(0);
    }
}

void InputTest::deviceChanged(int index)
{
    m_audioInfo->stop();
    m_audioInput->stop();
    m_audioInput->disconnect(this);
    delete m_audioInput;

    m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    createAudioInput();
}

void InputTest::sliderChanged(int value)
{
    if (m_audioInput)
        m_audioInput->setVolume(qreal(value) / 100);
}
