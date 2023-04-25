#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <typeinfo>
#include <QMap>

MainWindow::MainWindow(QSettings &settings, QWidget *parent) :
    QMainWindow(parent)

{

    int column = 3;
    ini_table->setColumnCount(column);

    ini_table->setColumnCount(column);
    saveBtn = new QPushButton(tr("&Change Save"));
    ini_table->setHorizontalHeaderLabels(QStringList() << tr("Group") << tr("Key") << tr("Value"));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveIniFile()));
    //connect(ini_table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveToIni(QTableWidgetItem*, QSettings )));

    QWidget* centralWidget_Tabel = new QWidget();
    QGridLayout *iniFileLayout = new QGridLayout(this);
    QBoxLayout* btnlt = new QHBoxLayout();
    btnlt->addStretch(1);
    btnlt->addWidget(saveBtn);
    iniFileLayout->addLayout(btnlt, 0, 0);
    iniFileLayout->addWidget(ini_table, 1, 0, 1, 1);
    centralWidget_Tabel->setLayout(iniFileLayout);
    setCentralWidget(centralWidget_Tabel);

   //loadIniFile(settings);

}
void MainWindow::loadIniFile(QSettings &ini)
{
    ini.setIniCodec("UTF-8");

    // get a list of all the groups in the ini file
    QStringList keys = ini.allKeys();
    QMap<QString, QStringList> groupKeyMap; // key=group, value=keyList

    for (auto key : keys) {
        // key = BRAKES/TiltIDs
        auto splitList = key.split('/');
        auto group = splitList.takeFirst(); // BRAKES
        auto name = splitList.join('/');// TiltIDs
        groupKeyMap[group].append(name);


        //qDebug() << splitList;
        //qDebug() << name;
    }

    //QStringList groupList = ini.childGroups();
    auto groupList  = groupKeyMap.keys();
    for (const QString &group : groupList) {
        ini.beginGroup(group);

        // get a  list of all the keys in this group
        //QStringList keyList = ini.childKeys();
        auto keyList = groupKeyMap[group];

        fillTheTabel(ini, group, keyList);
        ini.endGroup();
    }

      // show the table widget
      //ini_table->show();



//    Q_FOREACH(QString group, ini.childGroups()){
//        ini.beginGroup(group);
//        const QStringList childKeys = ini.childKeys();
//        QStringList Keys;
//        QStringList values;

//        foreach(const QString &childKey, childKeys){
//            Keys << childKey;
//            values << ini.value(childKey).toString();


//        }
//        ini.endGroup();

//        //fillTheTabel(group, Keys, values);


//    }


}

void MainWindow::saveIniFile()
{
    qDebug()<< "no of rows :" << ini_table->rowCount();
    QString startDir = "C:/Users/PIVview/AppData/Roaming/ILA_5150/";
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file to open"), startDir);
    QSettings settings(filename, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    for (int i =0; i < ini_table->rowCount(); ++i){

        QString group = ini_table->item(i, 0)->text();
        QString key = ini_table->item(i, 1)->text();
        QString value = ini_table->item(i, 2)->text();

        if (isStringListType(i)) {
            QStringList valueList = value.split(',');
            settings.beginGroup(group);
            settings.setValue(key, valueList);
            settings.endGroup();
        }
        else {
            settings.beginGroup(group);
            settings.setValue(key, value);
            settings.endGroup();
        }

    }


}

bool MainWindow::isStringListType(int row) const
{
    return rowList.contains(row);

}

void MainWindow::fillTheTabel(QSettings &ini, const QString &group, QStringList keyList)
{

    // loop through each key in the group and add its value to the table widget
    for (const QString &key : keyList) {
        int row = ini_table->rowCount();
        ini_table->insertRow(row);

        QTableWidgetItem *groupItem = new QTableWidgetItem(group);
        ini_table->setItem(row, 0, groupItem);

        QTableWidgetItem *keyItem = new QTableWidgetItem(key);
       // qDebug()<< "Key:" << keyItem;

        ini_table->setItem(row, 1, keyItem);


        auto values = ini.value(key);


        bool is_stringList = QString(values.typeName()) == "QStringList";

        //qDebug()<< "String Type:" << values.typeName();

        if (is_stringList) {
            rowList.append(row);
            auto val = ini.value(key).toStringList();
            auto joinVal = val.join(",");

            //qDebug()<< "joinVal:" << joinVal;


            QTableWidgetItem *valueItem = new QTableWidgetItem(joinVal.trimmed());//
            ini_table->setItem(row, 2, valueItem);
        }
        else {
            QTableWidgetItem *valueItem = new QTableWidgetItem(ini.value(key).toString().trimmed());//

            ini_table->setItem(row, 2, valueItem);
        }


    }
}



