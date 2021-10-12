#pragma once

#include <QWidget>
#include <QFile>

#include "MAVLinkProtocol.h"
#include "LinkInterface.h"
#include "LogReplayLink.h"

namespace Ui
{
class QGCMAVLinkLogPlayer;
}

/**
 * @brief Replays MAVLink log files
 *
 * This class allows to replay MAVLink logs at varying speeds.
 * captured flights can be replayed, shown to others and analyzed
 * in-depth later on.
 */
class QGCMAVLinkLogPlayer : public QObject
{
    Q_OBJECT

public:
    explicit QGCMAVLinkLogPlayer(QObject *parent = 0);
    ~QGCMAVLinkLogPlayer();


    Q_INVOKABLE void   selectLogFileForPlayback(void);
    Q_INVOKABLE void   playPauseToggle(void);
    Q_INVOKABLE void   setPlayheadFromSlider(int value);
    Q_INVOKABLE void   pause(void);
    Q_INVOKABLE void   logfileforPlayback(QString logdir);
    Q_INVOKABLE void   logfileforplaybackByid(int id);
    Q_INVOKABLE void   selectfilename(QString str);

    Q_PROPERTY(QString          logFilenameShort     READ logFilenameShort      NOTIFY logFilenameShortChanged);
    Q_PROPERTY(int              sliderValue          READ sliderValue           NOTIFY sliderValueChanged);
    Q_PROPERTY(QString          logLengthTime        READ logLengthTime         NOTIFY logLengthTimeChanged);
    Q_PROPERTY(QString          logCurrentTime       READ logCurrentTime        NOTIFY logCurrentTimeChanged);
    Q_PROPERTY(bool             isreplayLink         READ isreplayLink          CONSTANT);


    QString         logFilenameShort    (void)  const { return _logFilenameShort; }
    int             sliderValue         (void)  const { return _sliderValue; }
    QString         logLengthTime       (void)  const { return _logLengthTime;}
    QString         logCurrentTime      (void)  const { return _logCurrentTime;}
    bool            isreplayLink        (void);

private slots:
#if 0
    void _setAccelerationFromSlider(int value);
#endif
    void _logFileStats(bool logTimestamped, int logDurationSeconds, int binaryBaudRate);
    void _playbackStarted(void);
    void _playbackPaused(void);
    void _playbackPercentCompleteChanged(int percentComplete);
    void _playbackError(void);
    void _replayLinkDisconnected(void);
    void _setCurrentLogTime(int secs);

private:
    void _finishPlayback(void);
    QString _secondsToHMS(int seconds);
    void _enablePlaybackControls(bool enabled);

    LogReplayLink*  _replayLink;
    int             _logDurationSeconds;
    int             _lastCurrentTime;

    QString         _logFilenameShort;
    int             _sliderValue;
    QString         _logLengthTime;
    QString         _logCurrentTime;



signals:
    void logFilenameShortChanged(void);
    void sliderValueChanged(void);
    void logLengthTimeChanged(void);
    void logCurrentTimeChanged(void);
    void replayLinkChanged(void);
    void updateSql(void);
};

