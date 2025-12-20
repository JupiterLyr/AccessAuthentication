#include "mainwindow.h"
#include <iostream>
#include <QThread>
#include <QWidget>
#include "ui.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}
