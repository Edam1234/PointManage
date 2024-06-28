#include "PointManage.h"
SkipList skipList(4, 0.5);
PointManage::PointManage(QWidget* parent)
    : QWidget(parent)
{
    
    ui.setupUi(this);
    resize(800, 600);
    srand(time(nullptr));
    QString filePath = "D:\\download\\Point.txt";
    loadDataFromFile(filePath);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &PointManage::search);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &PointManage::del);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &PointManage::modify);
    connect(ui.pushButton, &QPushButton::clicked, this, &PointManage::add);
    connect(ui.pushButton_5, &QPushButton::clicked, this, &PointManage::save);
    connect(ui.pushButton_6, &QPushButton::clicked, this, &PointManage::queding);

}
PointManage::~PointManage()
{}

void PointManage::loadDataFromFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading: " << file.errorString();
        return;
    }
    QString itemText = QString("Longitude     Latitude");
    ui.listWidget->addItem(itemText);
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');
        if (fields.size() == 2) {
            bool ok1, ok2;
            double longitude = fields.at(0).toDouble(&ok1);
            double latitude = fields.at(1).toDouble(&ok2);

            if (ok1 && ok2) {
                QString itemText = QString("%1, %2").arg(QString::number(longitude, 'f', 6)).arg(QString::number(latitude, 'f', 6));
                ui.listWidget->addItem(itemText);
                skipList.insert(longitude, latitude); // 插入跳表
            }
        }
    }
    file.close();
}

void PointManage::search() {
    QString text = ui.lineEdit->text();
    QStringList coordinates = text.split(' ');
    if (coordinates.size() == 2) {
        bool ok1, ok2;
        double longitude = coordinates.at(0).trimmed().toDouble(&ok1);
        double latitude = coordinates.at(1).trimmed().toDouble(&ok2);

        if (ok1 && ok2) {
            bool found = skipList.search(longitude, latitude);
            if (found) {
                QString targetText = QString("%1, %2").arg(QString::number(longitude, 'f', 6)).arg(QString::number(latitude, 'f', 6));

                for (int i = 0; i < ui.listWidget->count(); ++i) {
                    QListWidgetItem* item = ui.listWidget->item(i);
                    if (item->text() == targetText) {
                        ui.listWidget->setCurrentItem(item);
                        item->setSelected(true);
                        item->setBackground(Qt::yellow);
                        QMessageBox::information(this, "Search Result", "Found: " + targetText);
                        return;
                    }
                }
            }
            else {
                QMessageBox::warning(this, "Search Result", "Not Found: " + QString::number(longitude, 'f', 6) + " " + QString::number(latitude, 'f', 6));
            }
        }
        else {
            QMessageBox::critical(this, "Error", "Invalid input. Please enter valid longitude and latitude.");
        }
    }
    else {
        QMessageBox::critical(this, "Error", "Invalid input format. Please enter in 'longitude latitude' format.");
    }
}

void PointManage::del() {
    QListWidgetItem* item = ui.listWidget->currentItem();
    if (item) {
        QString text = item->text();
        QStringList coordinates = text.split(", ");
        if (coordinates.size() == 2) {
            bool ok1, ok2;
            double longitude = coordinates.at(0).trimmed().toDouble(&ok1);
            double latitude = coordinates.at(1).trimmed().toDouble(&ok2);
            QString Longitude = QString::number(longitude, 'f', 6);
            QString Latitude = QString::number(latitude, 'f', 6);
            if (ok1 && ok2) {
                skipList.SKdelete(longitude, latitude);
                delete item; // 删除列表项
                QMessageBox::information(this, "Delete Result", "Deleted: " + Longitude + ", " + Latitude);
            }
            else {
                QMessageBox::critical(this, "Error", "Invalid item. Cannot convert to longitude and latitude.");
            }
        }
        else {
            QMessageBox::critical(this, "Error", "Invalid item format.");
        }
    }
    else {
        QMessageBox::warning(this, "Delete Result", "No item selected.");
    }
}
void PointManage::add()
{
    QString text = ui.lineEdit->text();
    QStringList coordinates = text.split(' ');
    if (coordinates.size() == 2) {
        bool ok1, ok2;
        double longitude = coordinates.at(0).trimmed().toDouble(&ok1);
        double latitude = coordinates.at(1).trimmed().toDouble(&ok2);

        if (ok1 && ok2) {
            skipList.insert(longitude, latitude);
            QString itemText = QString("%1, %2").arg(QString::number(longitude, 'f', 6)).arg(QString::number(latitude, 'f', 6));
            ui.listWidget->addItem(itemText);
            QMessageBox::information(this, "Add Result", "Added: " + itemText);
        }
        else {
            QMessageBox::critical(this, "Error", "Invalid input. Please enter valid longitude and latitude.");
        }
    }
    else {
        QMessageBox::critical(this, "Error", "Invalid input format. Please enter in 'longitude latitude' format.");
    }
}

void PointManage::save() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    auto nodes = skipList.getAllNodes();
    for (const auto& node : nodes) {
        out << QString::number(node.first, 'f', 6) << '\t' << QString::number(node.second, 'f', 6) << '\n';
    }

    file.close();
    QMessageBox::information(this, "Save Result", "Data saved to " + fileName);
}

void PointManage::modify() {
    QListWidgetItem* item = ui.listWidget->currentItem();
    if (item) {
        QString text = item->text();
        QStringList oldCoordinates = text.split(", ");
        if (oldCoordinates.size() == 2) {
            bool ok1, ok2;
            double oldLongitude = oldCoordinates.at(0).trimmed().toDouble(&ok1);
            double oldLatitude = oldCoordinates.at(1).trimmed().toDouble(&ok2);
            ui.lineEdit->setText(text);
            skipList.insert(oldLongitude, oldLatitude);
        }
    }
}

void PointManage::queding()
{
    QString text = ui.lineEdit->text();
    QStringList coordinates = text.split(',');
    if (coordinates.size() == 2) {
        bool ok1, ok2;
        double longitude = coordinates.at(0).trimmed().toDouble(&ok1);
        double latitude = coordinates.at(1).trimmed().toDouble(&ok2);
        if (ok1 && ok2) {
            skipList.insert(longitude, latitude);
            QString itemText = QString("%1, %2").arg(QString::number(longitude, 'f', 6)).arg(QString::number(latitude, 'f', 6));
            ui.listWidget->insertItem(1, itemText);  // Insert at the first position
            QMessageBox::information(this, "Modify Result", "Modify: " + itemText);
        }
    }

}