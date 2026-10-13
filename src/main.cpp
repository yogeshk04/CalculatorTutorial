// Phase 2: First Qt Window.
// This replaces Phase 1's console loop with a graphical window (Phase 1's
// code is still safe in git history - see commit "Project phase 1").
// Every Qt Widgets app needs exactly one QApplication (it runs the
// "event loop": the program waits for things like button clicks instead
// of running top-to-bottom like the console app did) and at least one
// visible widget.
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    // app.exec() starts the event loop and doesn't return until the
    // window is closed.
    return app.exec();
}
