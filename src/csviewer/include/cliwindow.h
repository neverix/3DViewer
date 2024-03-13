#ifndef _CS_CLIWINDOW_H
#define _CS_CLIWINDOW_H

#include <cstypes.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <hpp/Processing.hpp>
#include <QtWebSockets/QWebSocket>

class CLIWindow: public QObject {
    Q_OBJECT
  public:
   CLIWindow(QUrl url);

  private:
   void initConnections();
   QWebSocket m_webSocket;
   QUrl m_url;

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
