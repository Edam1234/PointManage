#pragma once
#include <QtWidgets/QWidget>
#include "ui_PointManage.h"
#include<qfile.h>
#include<qstring.h>
#include<QTextStream>
#include<qlist.h>
#include<qdebug.h>
#include"SkipList.h"
#include<qmessagebox.h>
#include<QFileDialog>
class PointManage : public QWidget
{
    Q_OBJECT

public:
    PointManage(QWidget *parent = nullptr);
    ~PointManage();
    void loadDataFromFile(QString fileName);
    void search();
    void del();
    void add();
    void save();
    void modify();
    void queding();
private:
    Ui::PointManageClass ui;


};


