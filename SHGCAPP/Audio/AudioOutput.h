﻿/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QStringList>
#include <QTextToSpeech>

#include "QGCToolbox.h"

class QGCApplication;

class AudioOutput : public QGCTool
{
    Q_OBJECT
public:
    AudioOutput(QGCApplication* app, QGCToolbox* toolbox);

    static bool     getMillisecondString    (const QString& string, QString& match, int& number);
    static QString  fixTextMessageForAudio  (const QString& string);

public slots:
    bool            say                     (const QString& text);

private slots:
    void            _stateChanged           (QTextToSpeech::State state);

protected:
    QTextToSpeech*  _tts;
    QStringList     _texts;
};

