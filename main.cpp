#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString org = QString("iLA_5150");
    QString app = QString("sample");
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, org, app);


    MainWindow Mwindow(settings);
    Mwindow.resize(350,400);


    Mwindow.loadIniFile(settings);
    Mwindow.show();
    //Mwindow.saveIniFile(settings);

    return a.exec();
}
