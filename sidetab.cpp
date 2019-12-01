#include "sidetab.h"
#include <QPainter>
#include <QPaintEvent>
#include <QAction>
#include "sidebarbutton.h"

SideTab::SideTab(QWidget *parent, FileTree *ft): QWidget(parent), show(true), filetree(ft)
{
setMouseTracking(true);
}

SideTab::SideTab(QWidget *parent): QWidget(parent), show(true)
{
setMouseTracking(true);
}

SideBarButton* SideTab::addAction(int yCoordinate, QString &text, const QIcon &icon){
    SideBarButton *button = new SideBarButton(yCoordinate, text, icon, this);
    button->setFlat(true);

    //button->setStyleSheet("border-radius: 0; background-color: black; color: white;");
    connect(button, &QPushButton::clicked, filetree, &FileTree::toggleFileTree);
    this->mActions.push_back(button);
    update();
    return button;
    //explicit QAction(const QIcon &icon, const QString &text, QObject *parent = nullptr);

}

QSize SideTab::minimumSizeHint() const{

}

void SideTab::toggleShow(){
}


void SideTab::paintEvent(QPaintEvent *event){
    if(show){
        QPainter painter(this);
        painter.fillRect(event->rect(), Qt::gray);
    }
}

void SideTab::mousePressEvent(QMouseEvent *event){

}

FileTree *SideTab::tree(){
    return this->filetree;
}
