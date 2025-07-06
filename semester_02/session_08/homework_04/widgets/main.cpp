#include "widgets.h"
#include <qapplication.h>
#include <qnamespace.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    widgets window;
    window.show();
    return app.exec();
}
