#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QRect r = QApplication::desktop()->availableGeometry();
  MainWindow w(nullptr, r);
  w.showMaximized();
  //w.ui->menuBar->setGeometry(0, 0, this->width(), 40*this->height()/662);
  //w.MainObject->addChildren(new Window(0.5, 0.5, 0.5, 0.5, w.MainObject));
  //w.MainObject->addChildren(new Window(0.5, 0.5, 0.5, 0.5, w.MainObject));
  w.repaint();
  return a.exec();
}
