#include "sidebarbutton.h"
#include <QPaintEvent>
#include <QPainter>
#include <QRect>

SideBarButton::SideBarButton(int y, QString &text, const QIcon &icon, QWidget *parent)
    :QPushButton (text, parent), text(text), yCoordinate(y), icon(icon), parent(parent)
{

}

void SideBarButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.fillRect(QRect(0,0,50,50), Qt::yellow);
    painter.setPen(Qt::black);
    // Rotate text

painter.rotate(-90);
    painter.drawText(10,10,event->rect().width(), fontMetrics().height(), Qt::AlignVCenter,"Button");
    painter.rotate(90);
}
