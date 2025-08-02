#include "mainwindow.h"

#include <chrono>
#include <qapplication.h>
#include <qhostaddress.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    constexpr unsigned short bindPort{ 12345 };
    const std::chrono::milliseconds timeout { 1000 };
    
    MainWindow w{nullptr, QHostAddress::LocalHost, bindPort, timeout};
    w.show();
    
    return a.exec();
}
