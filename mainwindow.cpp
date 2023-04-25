#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <typeinfo>
#include <QMap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{

    int column = 3;

    // Set the number of Columns in ini_table(QTableWidget)
    ini_table->setColumnCount(column);

    // SaveBtn use to save the modifed table into ini file.
    saveBtn = new QPushButton(tr("&Change Save"));
    // Set the Horizontal Header Titles in QTableWidget
    ini_table->setHorizontalHeaderLabels(QStringList() << tr("Group") << tr("Key") << tr("Value"));
    // Connection between saveBtn and saveIniFile Function when user clicked on it.
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveIniFile()));

    ////connect(ini_table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveToIni(QTableWidgetItem*, QSettings )));


    // Create a new QWidget and store a pointer to it in centralWidget_Tabel
    QWidget* centralWidget_Tabel = new QWidget();

    // Create a new QGridLayout called iniFileLayout and set the parent widget to be centralWidget_Tabel
    QGridLayout *iniFileLayout = new QGridLayout(centralWidget_Tabel);

    // Create a new QHBoxLayout which is used to arrange widgets in either a horizontal or verticall box.
    QBoxLayout* btnlt = new QHBoxLayout();

    // Add a stretch to the left side of btnlt, which will push the save button to the right
    btnlt->addStretch(1);

    // Add the save button widget to btnlt
    btnlt->addWidget(saveBtn);

    // Add btnlt to the iniFileLayout at row 0, column 0
    iniFileLayout->addLayout(btnlt, 0, 0);

    // Add the ini_table widget to the iniFileLayout
    iniFileLayout->addWidget(ini_table, 1, 0, 1, 1);

    // Set the iniFileLayout as the layout for centralWidget_Tabel
    centralWidget_Tabel->setLayout(iniFileLayout);

    // Set centralWidget_Tabel as the central widget of the main window
    setCentralWidget(centralWidget_Tabel);

}

/* Load the ini file using QSettings which provides
 *  a platform-independent way of storing appplication settings
 */
void MainWindow::loadIniFile(QSettings &ini)
{

    // get a list of all the groups in the ini file
    QStringList keys = ini.allKeys();
    // Stores key value pairs
    QMap<QString, QStringList> groupKeyMap; // key=group, value=keyList

    // set the group and name in the groupKeyMap
    for (QString key : keys) {
        // key = BRAKES/TiltIDs
        QStringList splitList = key.split('/');
        // Take first vaule as group and Remaining as name
        QString group = splitList.takeFirst(); // BRAKES
        QString name = splitList.join('/');// TiltIDs

        // put the group and name as key value pair
        groupKeyMap[group].append(name);
    }

   // get the list of keys from groupKeyMap
   QStringList groupList  = groupKeyMap.keys();
    for (const QString &group : groupList) {
        ini.beginGroup(group);

        // get a key from list of all the key's group
        QStringList keyList = groupKeyMap[group];
        // Fill the table usig group and keyList
        fillTheTabel(ini, group, keyList);
        ini.endGroup();
    }

}

/*
 * Save the changed Table into iniFile
 */
void MainWindow::saveIniFile()
{
    // File path
    QString startDir = "C:/Users/PIVview/AppData/Roaming/ILA_5150/";

    // Open a  File using getOpenFileName method (an existing file selected by the user)
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file to open"), startDir);

    QSettings settings(filename, QSettings::IniFormat);
    //settings.setIniCodec("UTF-8");

    //get the group, Key, value from the tabel and convert it into Qstring
    for (int i =0; i < ini_table->rowCount(); ++i){

        QString group = ini_table->item(i, 0)->text();
        QString key = ini_table->item(i, 1)->text();
        QString value = ini_table->item(i, 2)->text();

        // Check current row contain StringList type
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

/*
 * Check which row contain QStringList
 */
bool MainWindow::isStringListType(int row) const
{
    return rowList.contains(row);

}


/*
 * Fill the tabel using Groups , keys and Values
 */
void MainWindow::fillTheTabel(QSettings &ini, const QString &group, QStringList keyList)
{

    // loop through each key in the group and add its value to the table widget
    int n = keyList.size();
    for (int i=0; i<n; ++i) {
        auto key = keyList[i];
        int row = ini_table->rowCount();

        qDebug()<< "Row count before insert:" << ini_table->rowCount();

        ini_table->insertRow(row);

        qDebug()<< "Row count after insert:" << ini_table->rowCount();

        QTableWidgetItem *groupItem = new QTableWidgetItem(group);
        ini_table->setItem(row, 0, groupItem);

        QTableWidgetItem *keyItem = new QTableWidgetItem(key);
        ini_table->setItem(row, 1, keyItem);


        QVariant values = ini.value(key);

        // check if values type is QStringList
        bool is_stringList = QString(values.typeName()) == "QStringList";

        if (is_stringList) {

            // Add row into rowList if it is QStringList
            rowList.append(row);

            auto val = ini.value(key).toStringList();
            auto joinVal = val.join(",");

            QTableWidgetItem *valueItem = new QTableWidgetItem(joinVal.trimmed());//
            ini_table->setItem(row, 2, valueItem);
        }
        else {
            QTableWidgetItem *valueItem = new QTableWidgetItem(ini.value(key).toString().trimmed());
            ini_table->setItem(row, 2, valueItem);
        }

    }
}



