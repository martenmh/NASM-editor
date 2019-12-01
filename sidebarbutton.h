#ifndef SIDEBARBUTTON_H
#define SIDEBARBUTTON_H

#include <QPushButton>

class SideBarButton : public QPushButton
{
public:
    SideBarButton(int yCoordinate, QString &text, const QIcon &icon, QWidget *parent);
    /* Override paintEvent to make vertical buttons (like Clion) possible */
       QString text;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    int yCoordinate;

    const QIcon icon;
    QWidget *parent;
};

#endif // SIDEBARBUTTON_H
