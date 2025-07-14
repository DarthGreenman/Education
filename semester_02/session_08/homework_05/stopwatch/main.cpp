
#include "stopwatch_window.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    watch::stopwatch_window window{};
    window.show();
    return app.exec();
}
