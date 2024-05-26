#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen *src = QGuiApplication::screens().at(0);
    QRect r = src->availableGeometry();
    MainWindow w(nullptr, r);
    w.showMaximized();
    QMessageBox::information(nullptr, "INFO", QString::number(w.MainWorkspace_father->SCROLL->kofHeigth));
    //w.ui->menuBar->setGeometry(0, 0, this->width(), 40*this->height()/662);
    //w.MainObject->addChildren(new Window(0.5, 0.5, 0.5, 0.5, w.MainObject));
    //w.MainObject->addChildren(new Window(0.5, 0.5, 0.5, 0.5, w.MainObject));
    w.repaint();
    return a.exec();
}
