#include "cliwindow.h"

#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QTranslator>
#include <QThread>
#include <QMessageBox>

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
    // suc &= (bool)connect(this, &CameraListWidget::connectCamera, app,
    //                      &cs::CSApplication::connectCamera);
    // suc &= (bool)connect(this, &CameraListWidget::disconnectCamera, app,
    //                      &cs::CSApplication::disconnectCamera);
    // suc &= (bool)connect(app, &cs::CSApplication::cameraListUpdated, this,
    //                      &CameraListWidget::onCameraListUpdated);
    // suc &= (bool)connect(app, &cs::CSApplication::cameraStateChanged, this,
    //                      &CameraListWidget::onCameraStateChanged);

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
    // suc &= (bool)connect(app, &cs::CSApplication::output3DUpdated,
    //                      m_ui->renderWindow, &RenderWindow::onOutput3DUpdated,
    //                      Qt::QueuedConnection);
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
}

void CLIWindow::onSocketClosed() {
    qInfo() << "WebSocket closed";
}
