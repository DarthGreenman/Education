#include <QCoreApplication>
#include <QSqlDatabase>
#include <QTcpServer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QTcpServer tcp_srv{};

    auto db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("bookmarket");
    db.setUserName("postgres");
    db.setPassword("489aMARy");

    if (const auto is_open = db.open(); is_open)
        db.close();

    return a.exec();
}

// Set up code that uses the Qt event loop here.
// Call a.quit() or a.exit() to quit the application.
// A not very useful example would be including
// #include <QTimer>
// near the top of the file and calling
// QTimer::singleShot(5000, &a, &QCoreApplication::quit);
// which quits the application after 5 seconds.

// If you do not need a running Qt event loop, remove the call
// to a.exec() or use the Non-Qt Plain C++ Application template.
