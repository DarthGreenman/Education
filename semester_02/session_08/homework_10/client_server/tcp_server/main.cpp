#include "tcpserver.h"
#include <qcoreapplication.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TcpServer server(nullptr);

    return a.exec();
}
