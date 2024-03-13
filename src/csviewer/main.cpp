/*******************************************************************************
* This file is part of the 3DViewer                                            *     
*                                                                              *
* Copyright (C) 2022 Revopoint3D Company Ltd.                                  *
* All rights reserved.                                                         *
*                                                                              *
* This program is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation, either version 3 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* This program is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)             *
* for more details.                                                            *
*                                                                              *
********************************************************************************/

#include <QApplication>
#include <QFontDatabase>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include <commonhelper.h>
#include <cslogger.h>

#include "csapplication.h"
#define CLI_ONLY 1
#include "cliwindow.h"
#if CLI_ONLY
#else
#include "viewerwindow.h"
#endif
#include "app_version.h"

int main(int argc, char *argv[])
{ 
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("3D camera viewer");
    parser.addHelpOption();

    QCommandLineOption wsOption(
        QStringList() << "w"
                      << "websocket",
        QCoreApplication::translate("main", "Set up web socket server at a URL instead of running a GUI."),
        "url", "*");
    parser.addOption(wsOption);
    parser.process(app);
    bool useWs = parser.isSet(wsOption);

    // initialize log
    CSLogger logger;
    logger.setLogRootDir(LOG_ROOT_DIR);
    logger.setLogPrefix(LOG_PREFIX);

    bool suc = QObject::connect(&app, &QApplication::aboutToQuit, &logger,
                                &CSLogger::onAboutToQuit, Qt::DirectConnection);
    Q_ASSERT(suc);

    logger.initialize();

    // application information
    qInfo() << "";
    qInfo() << "****************************************";
    qInfo() << "Applicaion Name : " << APP_NAME;
    qInfo() << "Applicaion Version : " << APP_VERSION;
    qInfo() << "****************************************";

    cs::CSApplication::getInstance()->start();

    if (useWs) {
        QString url = parser.value(wsOption);
        QUrl wsUrl(url);
        CLIWindow *w = new CLIWindow(wsUrl);
    } else {
        #if CLI_ONLY
        qInfo() << "GUI is not supported in this build.";
        return 1;
        #else
        ViewerWindow *w = new ViewerWindow();
        w->show();

        // set style and font
        QString stylePath = QString("%1/themes/global.css").arg(APP_PATH);
        QString fontPath =
            QString("%1/fonts/SourceHanSansCN-Regular.ttf").arg(APP_PATH);

        qInfo() << "set style and font";
        CommonHelper::setStyle(stylePath);
        CommonHelper::setFont(fontPath);
        #endif
    }

    return app.exec();
}
