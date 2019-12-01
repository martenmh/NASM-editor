#include "filetree.h"
#include <QtDebug>

FileTree::FileTree()
{
    model.setRootPath("");
    //this->setModel(&model);
    setAnimated(false);
    setIndentation(20);
    setSortingEnabled(true);
    const QSize as = QApplication::desktop()->availableGeometry(this).size();
    resize(as / 2);
    setColumnWidth(0, width() / 3);

    setWindowTitle(QObject::tr("Dir view"));
}

void FileTree::toggleFileTree(){
    if(this->isHidden()){
        this->show();
        return;
    }
    this->hide();
}

void FileTree::setCurDir(QString &dir){
    // Set root path for file model
    model.setRootPath(dir);

    // Initialize File model
    this->setModel(&model);
}

