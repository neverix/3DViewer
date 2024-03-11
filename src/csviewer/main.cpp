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

#include <commonhelper.h>
#include <cslogger.h>

#include "csapplication.h"
#include "cliwindow.h"
#include "app_version.h"

int main(int argc, char *argv[])
{ 
    QCoreApplication app(argc, argv);

    // initialize log
    CSLogger logger;
    logger.setLogRootDir(LOG_ROOT_DIR);
    logger.setLogPrefix(LOG_PREFIX);

    bool suc = QObject::connect(&app, &QApplication::aboutToQuit, &logger,
                                &CSLogger::onAboutToQuit, Qt::DirectConnection);
    Q_ASSERT(suc);
    CLIWindow w;

    logger.initialize();

    // application information
    qInfo() << "";
    qInfo() << "****************************************";
    qInfo() << "Applicaion Name : "     << APP_NAME;
    qInfo() << "Applicaion Version : "  << APP_VERSION;
    qInfo() << "****************************************";
    
    cs::CSApplication::getInstance()->start();
    return app.exec();
}
