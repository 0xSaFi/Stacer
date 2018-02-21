#ifndef APTSourceRepositoryItem_H
#define APTSourceRepositoryItem_H

#include <QWidget>
#include <QFileInfo>
#include "Managers/tool_manager.h"

namespace Ui {
class APTSourceRepositoryItem;
}

class APTSourceRepositoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit APTSourceRepositoryItem(APTSourcePtr aptSource,
                               QWidget *parent = 0);

    ~APTSourceRepositoryItem();

private slots:
    void on_deleteAptSourceBtn_clicked();
    void on_aptSourceCheck_clicked(bool checked);

private:
    void init();

private:
    Ui::APTSourceRepositoryItem *ui;

    APTSourcePtr mAptSource;

};

#endif
