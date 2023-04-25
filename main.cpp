#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString org = QString("iLA_5150");
    QString app = QString("sample");


    QSettings settings(QSettings::IniFormat, QSettings::UserScope, org, app); // UserScope FOLDERID_RoamingAppData


    MainWindow Mwindow;
    Mwindow.resize(350,1400);

    // call a loadInifile where passed setting as a parameter.
    Mwindow.loadIniFile(settings);
    Mwindow.show();

    return a.exec();
}
