#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    value++;
    ui->widget->setValue(value);
    ui->widget_2->setvalue(value);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(value > 0)
    {
        value--;
        ui->widget->setValue(value);
        ui->widget_2->setvalue(value);
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    this->value = value;
    ui->widget->setValue(value);
    ui->widget_2->setvalue(value);
}
