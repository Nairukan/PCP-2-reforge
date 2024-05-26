#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "objects.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Object *MainObject = nullptr;
    explicit MainWindow(QWidget *parent = nullptr, QRect Desctop = QRect(0, 0, 0, 0));
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void RebuildThis(Workspace *AnyWorkSpace);
    ~MainWindow();
    int h = 0;
    QImage IMenu;
    std::vector<QString> OpenSessions;
    Workspace *MainWorkSpace;
    Scrollable* MainWorkspace_father;
    //, ILeftMenu, IBeginBlock, IEndBlock, IDataInputBlock, IDataOutputBlock, IArithmeticBlock, IIfBlock, IWhileBlock;
public slots:
    void Tick();

private:
    Ui::MainWindow *ui;
    QTimer *T = nullptr;
    QPoint TempArea;
    QMouseEvent *event;
};

#endif // MAINWINDOW_H
