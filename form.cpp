#include "form.h"
#include "ui_form.h"

#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this); 

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Form::~Form()
{
    delete ui;
}

void Form::on_tableWidget_cellClicked(int row, int column)
{
    extern QString playURL;
    QTableWidgetItem *item = ui->tableWidget->itemAt(row,column);
    playURL = item->text();
    qDebug() << playURL;
}
