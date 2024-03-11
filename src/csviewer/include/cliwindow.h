#ifndef _CS_CLIWINDOW_H
#define _CS_CLIWINDOW_H

#include <cstypes.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <QMap>
#include <QVector>
#include <hpp/Processing.hpp>


class CLIWindow: public QObject {
    Q_OBJECT
  public:
   CLIWindow();

  private:
   void initConnections();

  private slots:
    void onCameraStateChanged(int state);
    void onRemovedCurrentCamera(QString serial);
    void onCameraStreamStarted();
    void onCameraStreamStopped();
    void onAboutToQuit();
};

#endif  // _CS_CLIWINDOW_H
