#ifndef _CS_CLIWINDOW_H
#define _CS_CLIWINDOW_H

#include <cstypes.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <hpp/Processing.hpp>
#include <QtWebSockets/QWebSocketServer>

class CLIWindow: public QObject {
    Q_OBJECT
  public:
   CLIWindow(quint16 port);

  private:
   void initConnections();
   QWebSocketServer m_webSocketServer;

  private slots:
    void onCameraStateChanged(int state);
    void onRemovedCurrentCamera(QString serial);
    void onCameraStreamStarted();
    void onCameraStreamStopped();
    void onAboutToQuit();

    void onSocketConnected();
    void onSocketClosed();
};

#endif  // _CS_CLIWINDOW_H
