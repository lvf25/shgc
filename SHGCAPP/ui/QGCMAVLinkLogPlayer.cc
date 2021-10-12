#include <QStandardPaths>
#include <QtEndian>

#ifndef NO_SERIAL_LINK
#include "SerialLink.h"
#endif
#include "QGCMAVLinkLogPlayer.h"
#include "QGC.h"
#include "QGCApplication.h"
#include "SettingsManager.h"
#include "AppSettings.h"
#include "LinkManager.h"
#include "QGCQFileDialog.h"
#include "QGCMessageBox.h"
#include <QDate>
#include <QSqlQuery>

QGCMAVLinkLogPlayer::QGCMAVLinkLogPlayer(QObject *parent)
    : QObject           (parent)
    , _replayLink       (NULL)
    , _lastCurrentTime  (0)
{

}

QGCMAVLinkLogPlayer::~QGCMAVLinkLogPlayer()
{
}



void QGCMAVLinkLogPlayer::playPauseToggle(void)
{
    if (_replayLink->isPlaying()) {
        pause();
    } else {
        _replayLink->play();
    }
}

void QGCMAVLinkLogPlayer::pause(void)
{
    _replayLink->pause();
}


bool QGCMAVLinkLogPlayer::isreplayLink()
{
    if(_replayLink)
    {
        return  _replayLink->isLogReplay();
    }
    return false;
}

void QGCMAVLinkLogPlayer::selectLogFileForPlayback(void)
{
    // Disallow replay when any links are connected
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        QGCMessageBox::information(tr("Log Replay"), tr("You must close all connections prior to replaying a log."));
        return;
    }
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);//文件对话框为保存类型
    fd.setWindowTitle(tr("Load Telemetry Log File"));
    fd.setDirectory(qgcApp()->toolbox()->settingsManager()->appSettings()->telemetrySavePath());
    QStringList NameFilters;
    NameFilters << tr("MAVLink Log Files (*.tlog)") << tr("All Files (*)");
    fd.setNameFilters(NameFilters);
    QStringList logFilenames;
    if(fd.exec())
    {
        logFilenames = fd.selectedFiles();
        QString logFilename = logFilenames.first();
        // "C:/Users/Administrator/Desktop/2019-01-31 16-38-59.tlog"


        if (logFilename.isEmpty()) {
            return;
        }

        QSqlQuery query;

        if(query.exec(QString("select * from logfile where filedir = '%1'").arg(logFilename)))
        {
             if(query.next())
             {
                 QString logdir =  query.value(2).toString();
                 if(!logdir.isEmpty())
                 {
                     logfileforPlayback(logdir);
                     return;
                 }
             }
        }


        LogReplayLinkConfiguration* linkConfig = new LogReplayLinkConfiguration(QString("Log Replay"));
        linkConfig->setLogFilename(logFilename);
        linkConfig->setName(linkConfig->logFilenameShort());
        _logFilenameShort = linkConfig->logFilenameShort();
        emit logFilenameShortChanged();


        QStringList filenames = _logFilenameShort.split(".");
        QString filename = filenames[0];

        QDate date(QDate::currentDate());
        QString opentime=date.toString("yyyy-MM-dd");



        QString sql = QString("insert into logfile (filename ,opentime ,filedir )values(:filename,:opentime,:logFilename)");
        query.prepare(sql);
        query.bindValue(":filename",   filename);
        query.bindValue(":opentime",    opentime);
        query.bindValue(":logFilename",    logFilename);
        query.exec();

        emit updateSql();

        LinkManager* linkMgr = qgcApp()->toolbox()->linkManager();
        SharedLinkConfigurationPointer sharedConfig = linkMgr->addConfiguration(linkConfig);
        _replayLink = (LogReplayLink*)qgcApp()->toolbox()->linkManager()->createConnectedLink(sharedConfig);

        connect(_replayLink, &LogReplayLink::logFileStats,                      this, &QGCMAVLinkLogPlayer::_logFileStats);
        connect(_replayLink, &LogReplayLink::playbackStarted,                   this, &QGCMAVLinkLogPlayer::_playbackStarted);
        connect(_replayLink, &LogReplayLink::playbackPaused,                    this, &QGCMAVLinkLogPlayer::_playbackPaused);
        connect(_replayLink, &LogReplayLink::playbackPercentCompleteChanged,    this, &QGCMAVLinkLogPlayer::_playbackPercentCompleteChanged);
        connect(_replayLink, &LogReplayLink::currentLogTimeSecs,                this, &QGCMAVLinkLogPlayer::_setCurrentLogTime);
        connect(_replayLink, &LogReplayLink::disconnected,                      this, &QGCMAVLinkLogPlayer::_replayLinkDisconnected);

        _sliderValue = 0;
        emit sliderValueChanged();
    }
}


void QGCMAVLinkLogPlayer::selectfilename(QString str)
{
    qDebug() << "----------------------" << str << endl;
    QSqlQuery query;
    query.exec(QString("select * from logfile where filename like '%1'").arg(str));
    if(query.next())
    {
        emit updateSql();
    }
}



void QGCMAVLinkLogPlayer::logfileforPlayback(QString filedir)
{
    LogReplayLinkConfiguration* linkConfig = new LogReplayLinkConfiguration(QString("Log Replay"));
    linkConfig->setLogFilename(filedir);
    linkConfig->setName(linkConfig->logFilenameShort());
    _logFilenameShort = linkConfig->logFilenameShort();
    emit logFilenameShortChanged();

    LinkManager* linkMgr = qgcApp()->toolbox()->linkManager();
    SharedLinkConfigurationPointer sharedConfig = linkMgr->addConfiguration(linkConfig);
    _replayLink = (LogReplayLink*)qgcApp()->toolbox()->linkManager()->createConnectedLink(sharedConfig);

    connect(_replayLink, &LogReplayLink::logFileStats,                      this, &QGCMAVLinkLogPlayer::_logFileStats);
    connect(_replayLink, &LogReplayLink::playbackStarted,                   this, &QGCMAVLinkLogPlayer::_playbackStarted);
    connect(_replayLink, &LogReplayLink::playbackPaused,                    this, &QGCMAVLinkLogPlayer::_playbackPaused);
    connect(_replayLink, &LogReplayLink::playbackPercentCompleteChanged,    this, &QGCMAVLinkLogPlayer::_playbackPercentCompleteChanged);
    connect(_replayLink, &LogReplayLink::currentLogTimeSecs,                this, &QGCMAVLinkLogPlayer::_setCurrentLogTime);
    connect(_replayLink, &LogReplayLink::disconnected,                      this, &QGCMAVLinkLogPlayer::_replayLinkDisconnected);

    _sliderValue = 0;
    emit sliderValueChanged();
}


void QGCMAVLinkLogPlayer::logfileforplaybackByid(int id)
{
    // Disallow replay when any links are connected
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        QGCMessageBox::information(tr("Log Replay"), tr("You must close all connections prior to replaying a log."));
        return;
    }

    QSqlQuery query;

    if(query.exec(QString("select * from logfile where id = '%1'").arg(id+1)))
    {
        if(query.next())
        {
            QString logdir =  query.value(2).toString();
            if(!logdir.isEmpty())
            {
                logfileforPlayback(logdir);
            }
        }
    }
}


void QGCMAVLinkLogPlayer::_playbackError(void)
{
//    _ui->logFileNameLabel->setText("Error");
    _enablePlaybackControls(false);
}

QString QGCMAVLinkLogPlayer::_secondsToHMS(int seconds)
{
    int secondsPart  = seconds;
    int minutesPart  = secondsPart / 60;
    int hoursPart    = minutesPart / 60;
    secondsPart -= 60 * minutesPart;
    minutesPart -= 60 * hoursPart;

    return QString("%1:%2:%3").arg(hoursPart, 2,10,QLatin1Char('0')).arg(minutesPart, 2,10,QLatin1Char('0')).arg(secondsPart, 2,10,QLatin1Char('0'));
}

/// Signalled from LogReplayLink once log file information is known
void QGCMAVLinkLogPlayer::_logFileStats(bool    logTimestamped,         ///< true: timestamped log
                                        int     logDurationSeconds,     ///< Log duration
                                        int     binaryBaudRate)         ///< Baud rate for non-timestamped log
{
    Q_UNUSED(logTimestamped);
    Q_UNUSED(binaryBaudRate);

    qDebug() << "_logFileStats" << logDurationSeconds;

    _logDurationSeconds = logDurationSeconds;

    _logLengthTime = _secondsToHMS(logDurationSeconds);
    emit logLengthTimeChanged();
//    _ui->logLengthTime->setText(_secondsToHMS(logDurationSeconds));
}

/// Signalled from LogReplayLink when replay starts
void QGCMAVLinkLogPlayer::_playbackStarted(void)
{
//    _enablePlaybackControls(true);
//    _ui->playButton->setChecked(true);
//    _ui->playButton->setIcon(QIcon("qrc:/images/Pause.svg"));
//    _ui->positionSlider->setEnabled(false);
}

/// Signalled from LogReplayLink when replay is paused
void QGCMAVLinkLogPlayer::_playbackPaused(void)
{
//    _ui->playButton->setIcon(QIcon("qrc:/images/Play.svg"));
//    _ui->playButton->setChecked(false);
//    _ui->positionSlider->setEnabled(true);
}

void QGCMAVLinkLogPlayer::_playbackPercentCompleteChanged(int percentComplete)
{
//    _ui->positionSlider->blockSignals(true);
//    _ui->positionSlider->setValue(percentComplete);
//    _ui->positionSlider->blockSignals(false);
    _sliderValue = percentComplete;
    emit sliderValueChanged();
}

void QGCMAVLinkLogPlayer::setPlayheadFromSlider(int value)
{
    if (_replayLink) {
        _replayLink->movePlayhead(value);
    }
}

void QGCMAVLinkLogPlayer::_enablePlaybackControls(bool enabled)
{
//    _ui->playButton->setEnabled(enabled);
//#if 0
//    _ui->speedSlider->setEnabled(enabled);
//#endif
//    _ui->positionSlider->setEnabled(enabled);
}

#if 0
void QGCMAVLinkLogPlayer::_setAccelerationFromSlider(int value)
{
    //qDebug() << value;
    if (_replayLink) {
        _replayLink->setAccelerationFactor(value);
    }

    // Factor: -100: 0.01x, 0: 1.0x, 100: 100x

    float accelerationFactor;
    if (value < 0) {
        accelerationFactor = 0.01f;
        value -= -100;
        if (value > 0) {
            accelerationFactor *= (float)value;
        }
    } else if (value > 0) {
        accelerationFactor = 1.0f * (float)value;
    } else {
        accelerationFactor = 1.0f;
    }

    _ui->speedLabel->setText(QString("Speed: %1X").arg(accelerationFactor, 5, 'f', 2, '0'));
}
#endif

void QGCMAVLinkLogPlayer::_replayLinkDisconnected(void)
{
    _enablePlaybackControls(false);
    _replayLink = NULL;
}

void QGCMAVLinkLogPlayer::_setCurrentLogTime(int secs)
{
    if (secs != _lastCurrentTime) {
        _lastCurrentTime = secs;
        _logCurrentTime = _secondsToHMS(secs);
        emit logCurrentTimeChanged();
//        _ui->logCurrentTime->setText(_secondsToHMS(secs));
    }
}
