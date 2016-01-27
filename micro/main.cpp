
#include <QApplication>

#include "mainwindow.h"

int main(int argv, char **args)
{
    QApplication app(argv, args);
    app.setApplicationName("Audio Input Test");

    MainWindow window;
    window.show();

    return app.exec();
}
