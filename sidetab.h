#ifndef SIDETAB_H
#define SIDETAB_H

#include <QWidget>
#include <QPoint>
#include <QString>
#include <QIcon>
#include <QPushButton>
#include "sidebarbutton.h"
#include "filetree.h"
class SideTab : public QWidget
{
public:
    SideTab(QWidget *parent, FileTree *filetree);
    SideTab(QWidget *parent);
    SideBarButton* addAction(int yCoordinate, QString &text, const QIcon &icon = QIcon());

    QSize minimumSizeHint() const;
    /* Possible to show / hide */
    void toggleShow();
    FileTree *tree();
public slots:


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    bool show;

    QList<SideBarButton *> mActions;

    FileTree *filetree;
};

#endif // SIDETAB_H
