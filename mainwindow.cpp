#include "mainwindow.h"
#include "objects.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QRect Desctop)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setGeometry(0, 28, this->, Desctop.height()-30);
    QImage i("MenuPCP.png");
    IMenu = i;
    QFont f = QFont("times", 14);
    //f.setPointSize(14);
    //f.setBold(1);
    int wid, he;
    QFontMetrics FM(f);
    for (;;) {
        wid = FM.horizontalAdvance("Project");
        he = FM.height();
        if ((38 * ui->menuokok->width() / 91 >= wid && 12 * ui->menuokok->height() / 17 >= he)
            || f.pointSize() == 1) {
            break;
        }
        f.setPointSize(f.pointSize() - 1);
        QFontMetrics tmp(f);
        FM = tmp;
    }
    this->ui->menuBar->setFont(f);
    T = new QTimer(this);
    connect(T, SIGNAL(timeout()), this, SLOT(Tick()));
    MainObject = new Menu(this->width(),
                          this->height() - ui->menuBar->height(),
                          0,
                          ui->menuBar->height());
    MainObject->addChildren(new LeftMenu(0.089, 0.74, 0, 0.015, MainObject));
    MainObject->addChildren(new TranslateButton(0.07, 0.16, 0.006, 0.785, MainObject));
    MainObject->child[0]->addChildren(
        new BeginBlock(0.85, 0.065, 0.06, 0.06, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new EndBlock(0.85, 0.065, 0.06, 0.145, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new DataInputBlock(0.85, 0.065, 0.06, 0.29, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new DataOutputBlock(0.85, 0.065, 0.06, 0.375, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new ArithmeticBlock(0.85, 0.065, 0.06, 0.46, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new IfBlock(0.85, 0.065, 0.06, 0.605, MainObject->child[0], this));
    MainObject->child[0]->addChildren(
        new WhileBlock(0.85, 0.065, 0.06, 0.69, MainObject->child[0], this));
    MainObject->addChildren(new FuncPanel(0.1645, 0.377, 0.8355, 0.035, MainObject, this));
    MainObject->addChildren(new VariablePanel(0.1645, 0.56, 0.8355, 0.412, MainObject, this));
    MainWorkSpace = new Workspace(0.748, 0.5, 0.087, 0.035, MainObject, nullptr, this);

    MainWorkspace_father =new Scrollable(0.748, 0.94, 0.087, 0.035, MainObject, MainWorkSpace);

    //MainWorkSpace->addChildren(new WidgetM());
    MainObject->addChildren(MainWorkspace_father);
}

QCursor c;

int FukingActiveSlot = 0;
int prev_x, prev_y;
Object *SelectObject = nullptr;

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777216) {
        this->close();
    }
}

void MainWindow::Tick()
{
    if (FukingActiveSlot) {
        if (1) { //std::abs(c.pos().x()-prev_x)>0
            SelectObject->moveHARD(c.pos().x() - prev_x, c.pos().y() - prev_y);
            if (SelectObject->LinkedItem != nullptr) {
                //SelectObject->LinkedItem->moveHARD(c.pos().x()-prev_x, c.pos().y()-prev_y);
            }
            prev_x = c.pos().x();
            prev_y = c.pos().y();
        }

        repaint();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!FukingActiveSlot) { //Если кнопка до этого не была нажата или была отжата
        //Получение точки мыши в локальных кординатах окна
        this->TempArea.setX(c.pos().x() - this->geometry().left());
        this->TempArea.setY(c.pos().y() - this->geometry().top());
        //if (SelectObject!=nullptr &&)
        if (SelectObject != nullptr && SelectObject->IsBlock
            && SelectObject->child[0]->whoIsDaddy(&TempArea) == nullptr && SelectObject->activate
            && SelectObject->child[1]->whoIsDaddy(&TempArea) != nullptr) { //Если какой-то элемент активирован(Например открыто меню блока) и
        } else {
            if (SelectObject != nullptr && SelectObject->IsBlock && SelectObject->activate
                && SelectObject->whoIsDaddy(&TempArea) == nullptr) {
                SelectObject->activate = 0;
                SelectObject->ClickBut();
            }
            SelectObject = MainObject->whoIsDaddy(&TempArea);
            if (SelectObject != nullptr) {
                if (SelectObject->IsBlock) {
                    FukingActiveSlot++;
                    prev_x = c.pos().x();
                    prev_y = c.pos().y();
                    this->event = event;
                    T->start(10);
                } else if (SelectObject->IsButton) {
                    if (SelectObject->parent->IsBlock) {
                        SelectObject->parent->activate = !(SelectObject->parent->activate);
                        SelectObject->parent->ClickBut();
                        SelectObject = SelectObject->parent;
                    }
                }
            }
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    MainObject->setSizeHARD(this->width(), this->height());
    repaint();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (FukingActiveSlot) {
        T->stop();
        this->TempArea.setX(c.pos().x() - this->geometry().left());
        this->TempArea.setY(c.pos().y() - this->geometry().top());
        if (SelectObject != MainObject) {
            int w = SelectObject->w(), h = SelectObject->h();
            if (SelectObject->IsBlock && SelectObject->parent->TypeELEM == "LeftMenu") {

                //QMessageBox::information(this, "Info", "SelectedItem.Old_Parent==LeftMenu");
                if (SelectObject->TypeELEM == "Begin") {
                    SelectObject->parent->addChildren(
                        new BeginBlock(0.85, 0.065, 0.06, 0.06, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "End") {
                    MainObject->child[0]->addChildren(
                        new EndBlock(0.85, 0.065, 0.06, 0.145, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "DataInput") {
                    MainObject->child[0]->addChildren(
                        new DataInputBlock(0.85, 0.065, 0.06, 0.29, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "DataOutput") {
                    MainObject->child[0]->addChildren(
                        new DataOutputBlock(0.85, 0.065, 0.06, 0.375, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "Match") {
                    MainObject->child[0]->addChildren(
                        new ArithmeticBlock(0.85, 0.065, 0.06, 0.46, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "If") {
                    MainObject->child[0]->addChildren(
                        new IfBlock(0.85, 0.065, 0.06, 0.605, MainObject->child[0], this));
                } else if (SelectObject->TypeELEM == "While") {
                    MainObject->child[0]->addChildren(
                        new WhileBlock(0.85, 0.065, 0.06, 0.69, MainObject->child[0], this));
                }
            }
            else{
                QMessageBox::information(this, "Info", "SelectedItem.Old_Parent!=LeftMenu\n"+QString("OLD_parent=="+SelectObject->parent->TypeELEM));

            }
            SelectObject->parent->child.erase(std::find(SelectObject->parent->child.begin(),
                                                        SelectObject->parent->child.end(),
                                                        SelectObject));

            Object *fatherOrMother = MainObject->whoIsDaddy(&TempArea);
            SelectObject->parent = fatherOrMother;
            fatherOrMother->addChildren(SelectObject);
            SelectObject->setSizeHARD(SelectObject->w(), SelectObject->h());
            SelectObject->moveToHARD(SelectObject->x(), SelectObject->y());
            //QMessageBox::critical(this, QString::number(SelectObject->parent->child.size()), "Release");
        }
        FukingActiveSlot--;
    }
    RebuildThis(MainWorkSpace);
}

void MainWindow::wheelEvent(QWheelEvent *event){
    this->TempArea.setX(c.pos().x() - this->geometry().left());
    this->TempArea.setY(c.pos().y() - this->geometry().top());
    Object* selectedItem=MainObject->whoIsDaddy(&TempArea);
    while (selectedItem!=nullptr && ((Widget*)(selectedItem))->TypeELEM!="Scrollable")
        selectedItem=selectedItem->parent;
    if (selectedItem==nullptr) return;
    Scrollable* ActiveScrollArea=(Scrollable*)selectedItem;
    if (ActiveScrollArea->CONTENT->kofHeigth<=1){//QMessageBox::information(this, "Info", QString::number(ActiveScrollArea->h()/double(ActiveScrollArea->CONTENT->h())));
        return;
    }
    if (event->angleDelta().y() > 0) {
        ActiveScrollArea->CONTENT->moveHARD(0, 15 * 2);
        if (ActiveScrollArea->CONTENT->kofTop >= 0) {
            ActiveScrollArea->CONTENT->kofTop=0;
        }
        ActiveScrollArea->updateGeometry();
        repaint();
    } else {
        ActiveScrollArea->CONTENT->moveHARD(0, -15 * 2);
        if (ActiveScrollArea->CONTENT->h() + ActiveScrollArea->CONTENT->y() <= ActiveScrollArea->h()+ActiveScrollArea->y()) {
            ActiveScrollArea->CONTENT->moveHARD(0, ActiveScrollArea->h()+ActiveScrollArea->y() - (ActiveScrollArea->CONTENT->h() + ActiveScrollArea->CONTENT->y()));
        }
        ActiveScrollArea->updateGeometry();
        repaint();
    }
}

MainWindow::~MainWindow()
{
    delete T;
    //MainObject->~Object();
    delete MainObject;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    h = 0;
    QPainter p(this);
    //p.drawImage(QRect(0, ui->mainToolBar->height(), this->width(), this->height()-ui->mainToolBar->height()), QImage("MenuPCP.png"), QRect(0,0,1280,644));
    QPen o = p.pen();
    QBrush l = p.brush();
    p.setPen(QPen(Qt::blue, 3, Qt::SolidLine));
    MainObject->paint(this, &p, IMenu);
    MainObject->paintChild(this, &p, IMenu);
    if (SelectObject != nullptr) {
        SelectObject->paint(this, &p, IMenu);
        SelectObject->paintChild(this, &p, IMenu);
    }
    //if (FukingActiveSlot)
    //p.drawRect(this->TempArea);
    //this->setWindowTitle(QString::number(h));
    //p.drawText(QPoint(100, 100), "Hello, sunny)");
    //QImage i("Viktoria.png");
    p.setPen(o);
    p.setBrush(l);
    //p.drawImage(QRect(4, ui->mainToolBar->height()+35, 273/3, 97/3), QImage("MenuPCP.png"), QRect(1280,0,273,97));
    //p.drawText(QRect(21, ui->mainToolBar->height()+44, 200/3, 50/3), "Begin func");
    //QMatrix q;
}

void MainWindow::RebuildThis(Workspace *AnyWorkSpace)
{
    // QMessageBox::critical(this, "Way of Process", "Begin RebuildThis");
    int SumHight = 0;
    double PointY = 15.0/AnyWorkSpace->h();
    if (AnyWorkSpace != nullptr){
        //QMessageBox::critical(this, "Way of Process", "RebuildThis: "+QString::number(AnyWorkSpace->heigth));
        for (int i = 0; i < AnyWorkSpace->child.size(); i++) {
            AnyWorkSpace->child[i]
                ->moveToHARD((double) ((AnyWorkSpace->width - AnyWorkSpace->child[i]->width) / 2
                                       + AnyWorkSpace->left),
                             PointY * AnyWorkSpace->heigth + AnyWorkSpace->top);
            PointY += AnyWorkSpace->child[i]->kofHeigth + 10.0/AnyWorkSpace->h();
        }
        PointY += 15.0/AnyWorkSpace->h();
        if (PointY > 1.0){

            for(auto child: AnyWorkSpace->child){
                child->kofTop=(child->y()-AnyWorkSpace->y())/double(PointY*AnyWorkSpace->h());
                child->kofHeigth=(child->h())/double(PointY*AnyWorkSpace->h());
            }

            AnyWorkSpace->kofHeigth=PointY*AnyWorkSpace->h()/AnyWorkSpace->parent->h();
            //AnyWorkSpace->setSize(AnyWorkSpace->kofWidth, PointY);
            QMessageBox::information(this, "Info", QString::number(AnyWorkSpace->child[0]->h())+"="+QString::number(AnyWorkSpace->h()*PointY*AnyWorkSpace->child[0]->kofHeigth));
            if (AnyWorkSpace->parent!=nullptr && AnyWorkSpace->parent->TypeELEM=="Scrollable"){
                AnyWorkSpace->parent->updateGeometry();
            }
        }
    }
    //QMessageBox::critical(this, "Way of Process", "End RebuildThis");
    repaint();
}
