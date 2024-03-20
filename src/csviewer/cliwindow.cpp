#include "cliwindow.h"

#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QTranslator>
#include <QThread>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QBuffer>

#include <cstypes.h>
#include <icscamera.h>

#include "csapplication.h"
#include "./app_version.h"
#include "csprogressbar.h"
#include "csmessagebox.h"
#include "appconfig.h"
#include "csaction.h"

CLIWindow::CLIWindow(quint16 port) : m_webSocketServer(
    QStringLiteral("CSViewer"), QWebSocketServer::NonSecureMode, this
) {
    initConnections();
    qInfo() << "Opening WebSocket on port " << port;
    m_webSocketServer.listen(QHostAddress::Any, port);
}

void CLIWindow::initConnections() {
    bool suc = true;
    auto app = cs::CSApplication::getInstance();

    suc &= (bool)connect(app, &cs::CSApplication::cameraStateChanged, this,
                         &CLIWindow::onCameraStateChanged);
    suc &= (bool)connect(app, &cs::CSApplication::removedCurrentCamera, this,
                         &CLIWindow::onRemovedCurrentCamera);
    suc &= (bool)connect(this, &CLIWindow::connectCamera, app,
                         &cs::CSApplication::connectCamera);
    suc &= (bool)connect(this, &CLIWindow::disconnectCamera, app,
                         &cs::CSApplication::disconnectCamera);
    suc &= (bool)connect(app, &cs::CSApplication::cameraListUpdated, this,
                         &CLIWindow::onCameraListUpdated);
    suc &= (bool)connect(app, &cs::CSApplication::cameraStateChanged, this,
                         &CLIWindow::onCameraStateChanged);

    suc &= (bool)connect(qApp, &QApplication::aboutToQuit, this,
                         &CLIWindow::onAboutToQuit);

    suc &= (bool)connect(&m_webSocketServer, &QWebSocketServer::newConnection, this,
                         &CLIWindow::onSocketConnected);
    suc &= (bool)connect(&m_webSocketServer, &QWebSocketServer::closed, this,
                         &CLIWindow::onSocketClosed);

    Q_ASSERT(suc);
}

void CLIWindow::onCameraStateChanged(int state) {
    CAMERA_STATE cameraState = (CAMERA_STATE)state;
    switch (cameraState) {
        case CAMERA_CONNECTING:
        case CAMERA_RESTARTING_CAMERA:
            // m_ui->menuCamera->setEnabled(false);
        case CAMERA_STARTING_STREAM:
        case CAMERA_STOPPING_STREAM:
        case CAMERA_DISCONNECTING:
            // m_circleProgressBar->open();
            break;

        case CAMERA_CONNECTED: {
            // m_ui->menuCamera->setEnabled(true);

            // update camera info dialog
            CSCameraInfo info =
                cs::CSApplication::getInstance()->getCamera()->getCameraInfo();
            // m_cameraInfoDialog->updateCameraInfo(info);

            // m_circleProgressBar->close();
            break;
        }
        case CAMERA_DISCONNECTED:
        case CAMERA_DISCONNECTFAILED:
        case CAMERA_CONNECTFAILED:
            // m_circleProgressBar->close();
            // m_ui->menuCamera->setEnabled(false);
            // destoryRenderWindows();
            break;
        case CAMERA_START_STREAM_FAILED:
            // m_circleProgressBar->close();
            // destoryRenderWindows();
            break;
        case CAMERA_STARTED_STREAM:
            // onCameraStreamStarted();
            break;
        case CAMERA_STOPPED_STREAM:
        case CAMERA_PAUSED_STREAM:
            // onCameraStreamStopped();
            break;
        default:
            break;
    }

    // // update windows state
    // m_ui->menuWindows->setEnabled(cameraState == CAMERA_STARTED_STREAM);

    // updateStatusBar(state);
}

void CLIWindow::onRemovedCurrentCamera(QString serial) {
    // m_globalMessageBox->updateMessage(
    //     tr("The current camera has been disconnected"));
    // m_globalMessageBox->open();

    // connect(m_globalMessageBox, &CSMessageBox::accepted, this,
    //         &ViewerWindow::onConfirmCameraRemoved, Qt::UniqueConnection);
}

void CLIWindow::onCameraStreamStarted() {
    // get camera parameter
    auto app = cs::CSApplication::getInstance();
    auto camera = app->getCamera();

    QVariant hasRgbV, hasIrV, hasDepthV;
    camera->getCameraPara(cs::parameter::PARA_HAS_RGB, hasRgbV);
    camera->getCameraPara(cs::parameter::PARA_DEPTH_HAS_IR, hasIrV);
    camera->getCameraPara(cs::parameter::PARA_HAS_DEPTH, hasDepthV);

    // for (auto action : m_windowActions) {
    //     m_ui->menuWindows->removeAction(action);
    //     delete action;
    // }
    // m_windowActions.clear();

    // if (hasIrV.toBool()) {
    //     m_windowActions.push_back(new CSAction(CAMERA_DATA_L, "IR(L)"));
    //     m_windowActions.push_back(new CSAction(CAMERA_DATA_R, "IR(R)"));
    // }

    // if (hasDepthV.toBool()) {
    //     m_windowActions.push_back(new CSAction(CAMERA_DATA_DEPTH, "Depth"));
    //     m_windowActions.push_back(
    //         new CSAction(CAMERA_DATA_POINT_CLOUD, "Point Cloud"));
    // }

    // if (hasRgbV.toBool()) {
    //     m_windowActions.push_back(new CSAction(CAMERA_DATA_RGB, "RGB"));
    // }

    // m_ui->renderWindow->setShowTextureEnable(hasRgbV.toBool());

    // updateWindowActions();
    // onRenderWindowUpdated();

    bool suc = true;
    suc &= (bool)connect(app, &cs::CSApplication::output3DUpdated, this,
                         &CLIWindow::onOutput3DUpdated, Qt::QueuedConnection);
    suc &= (bool)connect(app, &cs::CSApplication::output2DUpdated, this,
                         &CLIWindow::onOutput2DUpdated, Qt::QueuedConnection);
    // suc &= (bool)connect(app, &cs::CSApplication::output2DUpdated,
    //                      m_ui->renderWindow, &RenderWindow::onOutput2DUpdated,
    //                      Qt::QueuedConnection);

    Q_ASSERT(suc);

    // m_circleProgressBar->close();
}

void CLIWindow::onCameraStreamStopped() {
    // m_circleProgressBar->close();
    auto app = cs::CSApplication::getInstance();
    // disconnect(app, &cs::CSApplication::output3DUpdated, m_ui->renderWindow,
    //            &RenderWindow::onOutput3DUpdated);
    // disconnect(app, &cs::CSApplication::output2DUpdated, m_ui->renderWindow,
    //            &RenderWindow::onOutput2DUpdated);
}

void CLIWindow::onAboutToQuit() {
    qInfo() << "About to quit";
    m_webSocketServer.close();
    cs::CSApplication::getInstance()->stop();
}

void CLIWindow::onSocketConnected() {
    qInfo() << "WebSocket connected";
    QWebSocket* pSocket = m_webSocketServer.nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this,
            &CLIWindow::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this,
            &CLIWindow::socketDisconnected);

    m_clients << pSocket;
}

void CLIWindow::processTextMessage(QString message) {
    QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    qInfo() << "Message received:" << message;
    if (!pClient) {
        qInfo() << "No client found";
        return;
    }
    // depending on the first character, do something
    if (message.startsWith("C")) {
        // connect to camera
        QString serial = message.mid(1);
        emit connectCamera(serial);
    } else if (message.startsWith("D")) {
        // disconnect from camera
        emit disconnectCamera();
    } else if (message.startsWith("L")) {
        // list cameras
        QJsonArray cameraList;
        for (const QString& camera : m_cameraList) {
            cameraList.append(camera);
        }
        QJsonDocument jsonDoc;
        jsonDoc.setArray(cameraList);
        pClient->sendTextMessage(jsonDoc.toJson());
    } else if (message.startsWith("S")) {
        if (!m_subscribers.contains(pClient)) {
            m_subscribers << pClient;
        } else {
            qWarning() << "Client already subscribed";
        }
    } else if (message.startsWith("U")) {
        if (m_subscribers.contains(pClient)) {
            m_subscribers.removeAll(pClient);
        } else {
            qWarning() << "Client not subscribed";
        }
    } else if (message.startsWith("G")) {
        // capture frame
        if(m_cachedImage.isNull()) {
            qWarning() << "No cached image";
        } else {
            sendImage({pClient}, m_cachedImage);
        }
    } else {
        qInfo() << "Pong";
        pClient->sendTextMessage(message);
    }
}

void CLIWindow::socketDisconnected() {
    QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    qInfo() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_subscribers.removeAll(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void CLIWindow::onSocketClosed() {
    qInfo() << "WebSocket closed";
}

void CLIWindow::onOutput2DUpdated(OutputData2D outputData) {
    if (outputData.image.isNull()) {
        qWarning() << "render image is null";
        return;
    }

    m_cachedImage = outputData.image;
    sendImage(m_subscribers, outputData.image);
}

void CLIWindow::sendImage(QList<QWebSocket*> clients, const QImage& image) {
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    for (QWebSocket* client : clients) {
        client->sendBinaryMessage(ba);
    }
}

void CLIWindow::onOutput3DUpdated(cs::Pointcloud pointCloud,
                                     const QImage& image) {
    // RenderWidget3D* widget =
    //     qobject_cast<RenderWidget3D*>(renderWidgets[CAMERA_DATA_POINT_CLOUD]);
    // if (widget) {
    //     if (!widget->isHidden()) {
    //         widget->onRenderDataUpdated(pointCloud, image);
    //     }
    // }
}

void CLIWindow::onCameraListUpdated(const QStringList infoList) {
    const int size = infoList.size();
    auto curCameraSerial = QString(cs::CSApplication::getInstance()
                                       ->getCamera()
                                       ->getCameraInfo()
                                       .cameraInfo.serial);

    // m_ui->cameraListWidget->clear();
    m_cameraList.clear();
    // int curSelect = -1;
    for (int i = 0; i < size; i++) {
        const QString& info = infoList.at(i);
        // addListWidgetItem(info);

        // if (!curCameraSerial.isEmpty() && info.contains(curCameraSerial)) {
        //     curSelect = i;
        // }
        m_cameraList.append(info);
    }

    // if (curSelect >= 0) {
    //     QListWidgetItem* item = m_ui->cameraListWidget->item(curSelect);
    //     QWidget* itemWidget = m_ui->cameraListWidget->itemWidget(item);
    //     CSListItem* csItem = qobject_cast<CSListItem*>(itemWidget);
    //     if (csItem) {
    //         csItem->setSelected(true);
    //     }
    // }
}
