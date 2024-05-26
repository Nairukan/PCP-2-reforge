#include "objects.h"

N_ScrollWidget::N_ScrollWidget(QWidget *parent, QWidget *LinkedWidget)
    : QWidget(parent)
{
    this->setGeometry(parent->width() - 12, 0, 12, parent->height());
    this->LinkedWidget = LinkedWidget;
    ScrollBody = {0, 0, 10, this->height() * this->height() / LinkedWidget->height()};
    T1 = new QTimer(this);
    connect(T1, SIGNAL(timeout()), this, SLOT(Tick()));
}

N_MenuBLOCK::~N_MenuBLOCK()
{
    for (auto now : EDITS) {
        if (now!=nullptr){

            delete now;
        }
    }
    for (auto now : BUTTONS) {
        if (now!=nullptr){
            now->LinkedWidget=nullptr;
            delete now;
        }
    }
    for (auto now : LABELS) {
        if (now!=nullptr)
            delete now;
    }
    for (auto now : COMBOS) {
        if (now!=nullptr)
            delete now;
    }
    //delete N_Parent;
}

Scrollable::Scrollable(double width, double heigth, double left, double top, Object *parent, Widget* CONTENT)
    : Widget(width, heigth, left, top, parent, QColor(0, 0, 0))
{
    TypeELEM=("Scrollable");
    this->CONTENT=CONTENT;
    addChildren(CONTENT);
    bool ParentContainCONTENT=false;
    CONTENT->parent->removeChildren(CONTENT);
    CONTENT->parent=this;
    CONTENT->moveToHARD(this->x()+0, this->y()+0);
    CONTENT->setSizeHARD(this->w() - 12, this->h());
    SCROLL = new ScrollBar(this, CONTENT);
    addChildren(SCROLL);


    //SCROLL->moveToHARD(this->w() - 12, 0);
    //SCROLL->setSizeHARD(12, this->h());
}

ScrollBar::ScrollBar(Widget *parent, Widget *LinkedWidget)
    : Widget(0,0,0,0, parent)
{
    TypeELEM="ScrollBar";
    this->moveToHARD(parent->x()+parent->w() - 12, parent->y()+0);
    this->setSizeHARD(12, parent->h());
    this->LinkedWidget = LinkedWidget;
    ScrollBody = {parent->x(), parent->y(), 10, this->h() * this->h() / LinkedWidget->h()};
    //T1 = new QTimer(this);
    //connect(T1, SIGNAL(timeout()), this, SLOT(Tick()));
}



void ScrollBar::Tick() {}

void ScrollBar::mousePressEvent(QMouseEvent *event) {}

void ScrollBar::mouseReleaseEvent(QMouseEvent *event) {}


void N_ScrollWidget::paintEvent(QPaintEvent *event)
{

    QPainter p(this);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, this->width(), this->height());
    p.setBrush(QBrush(Qt::darkGray));
    p.drawRect(ScrollBody);
}

void N_ScrollWidget::Tick() {}

void N_ScrollWidget::mousePressEvent(QMouseEvent *event) {}

void N_ScrollWidget::mouseReleaseEvent(QMouseEvent *event) {}


WidgetM::WidgetM(QWidget *parent, BlockMenu *MenuFather)
    : QWidget(parent)
{
    //CONTENT->TypeELEM
    CONTENT = new N_MenuBLOCK(this);
    this->MenuFather = MenuFather;
    CONTENT->setGeometry(0, 0, this->width() - 12, this->height());
    SCROLL = new N_ScrollWidget(this, CONTENT);
    SCROLL->setGeometry(this->width() - 12, 0, 12, this->height());
}

WidgetM::~WidgetM()
{
    delete CONTENT;
    delete SCROLL;
}

void WidgetM::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(QBrush(Qt::lightGray));
    p.drawRect(0, 0, this->width(), this->height());
    this->SCROLL->ScrollBody = {0,
                                (0 - this->CONTENT->y()) * this->height() / this->CONTENT->height(),
                                10,
                                this->height() * this->height() / CONTENT->height()};
    if (this->SCROLL->ScrollBody.height() >= CONTENT->height()) {
        SCROLL->hide();
    } else
        SCROLL->show();
}

void WidgetM::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        CONTENT->move(0, CONTENT->y() + CONTENT->OTS * 2);
        if (CONTENT->y() >= 0) {
            CONTENT->move(0, 0);
        }
        repaint();
    } else {
        CONTENT->move(0, CONTENT->y() - CONTENT->OTS * 2);
        if (CONTENT->height() + CONTENT->y() <= this->height()) {
            CONTENT->move(0, this->height() - CONTENT->height());
        }
        repaint();
    }
}

void WidgetM::setGeometry(int x, int y, int w, int h)
{
    this->move(x, y);
    this->resize(w, h);
    CONTENT->move(0, 0);
    SCROLL->setGeometry(this->width() - 12, 0, 12, this->height());
    CONTENT->OTS = this->height() * 0.0823;
}
