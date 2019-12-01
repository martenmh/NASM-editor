#ifndef FILETREE_H
#define FILETREE_H

#include <QSessionManager>
#include <QtWidgets>
#include <QFileSystemModel>
#include <QTreeView>
#include <QString>

class FileTree : public QTreeView
{
public:
    FileTree();
    void setCurDir(QString &dir);

public slots:
    void toggleFileTree();

private:
    QFileSystemModel model;
};

#endif // FILETREE_H
