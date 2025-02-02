#ifndef _CS_CLIWINDOW_H
#define _CS_CLIWINDOW_H

#include <cstypes.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <hpp/Processing.hpp>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

class CLIWindow: public QObject {
    Q_OBJECT
  public:
   CLIWindow(quint16 port);

  private:
   void initConnections();
   void sendImage(QList<QWebSocket*> clients, const QImage& image);
   QWebSocketServer m_webSocketServer;
   QStringList m_cameraList;
   QList<QWebSocket*> m_clients;
   QList<QWebSocket*> m_subscribers;
   QImage m_cachedImage;

  private slots:
   void onCameraListUpdated(const QStringList infoList);
   void onCameraStateChanged(int state);
   void onRemovedCurrentCamera(QString serial);
   void onCameraStreamStarted();
   void onCameraStreamStopped();
   void onAboutToQuit();
   void onSocketConnected();
   void onSocketClosed();
   void onOutput2DUpdated(OutputData2D outputData);
   void onOutput3DUpdated(cs::Pointcloud pointCloud, const QImage& image);
   void processTextMessage(QString message);
   void socketDisconnected();

  signals:
   void connectCamera(QString serial);
   void disconnectCamera();
};

#endif  // _CS_CLIWINDOW_H
