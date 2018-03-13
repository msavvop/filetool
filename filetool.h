#ifndef FILETOOL_H
#define FILETOOL_H

#include <QDirModel>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QMainWindow>
#include <QItemDelegate>
#include <QStyleOption>
#include <QUrl>
#include <QtWidgets>
#include "pastedialog.h"

class QModelIndex;
namespace Ui {
class filetool;
}

class filetool : public QMainWindow
{
    Q_OBJECT

public:
    explicit filetool(QWidget *parent = 0);
    ~filetool();
 QStringList selectedFiles();
 QModelIndex parentDir();
Qt::ItemFlags flags(const QModelIndex &index) const;
signals:
int combo_change(int);

private slots:
 void on_listView_activated(const QModelIndex &index);

 void on_treeView_activated(const QModelIndex &index);

 void on_iconView_activated(const QModelIndex &index);

 void on_listView_clicked(const QModelIndex &index);

 void on_treeView_clicked(const QModelIndex &index);

 void on_iconView_clicked(const QModelIndex &index);

 void on_actionList_View_triggered();

void closeEvent(QCloseEvent *event);

 void on_actionIcon_View_triggered();

 void on_comboBox_currentIndexChanged(const QString &arg1);


void setCurrentComboIndex(int);

 void on_comboBox_activated(const QString &arg1);

 void on_actionPrevious_triggered();

 void on_actionNext_triggered();

 void on_actionParentDir_triggered();

 void on_action_exit_triggered();

 void on_actionDirectory_triggered();



 void on_action_rename_triggered();



 void on_lineEdit_textChanged(const QString &arg1);

 void on_pushButton_clicked();

 void on_actionGo_home_triggered();

 void on_tableView_activated(const QModelIndex &index);

 void on_tableView_clicked(const QModelIndex &index);

 void on_actionList_Details_View_triggered();

 void on_action_delete_triggered();

 void on_action_cut_triggered();

 void on_actionC_opy_triggered();

 void on_action_paste_triggered();
 void setNOTBusy();

 void on_actionAbout_triggered();

protected:

void writeSettings();
void ReadSettings();


private:
    Ui::filetool *ui;

    QItemSelectionModel *selModel;
    QFileSystemModel *dirModel;
    QFileSystemModel *treeViewDirModel;
    QDirModel *dirModel2;

    QModelIndexList indexHistoryList;
    QModelIndexList indexForwardList;
    QModelIndex  previousIndex;
    QModelIndex parentDirIndex;
    QModelIndex nextIndex;
    QString newPath;
    QModelIndexList pasteList;
     QStringList List;
    bool copyIndicator;
    volatile bool busy;
};

#endif // FILETOOL_H
