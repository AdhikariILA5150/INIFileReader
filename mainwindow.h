#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    void loadIniFile(QSettings &ini);

    void fillTheTabel(QSettings &settings, const QString &group, QStringList value);
    bool isStringListType(int row) const;

protected Q_SLOTS:
    void saveIniFile();

private:
    Ui::MainWindow *ui;

    QTableWidget* ini_table = new QTableWidget(this);

    QPushButton *saveBtn;
    QList <int>  rowList;




};

#endif // MAINWINDOW_H
