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
#include <QString>
#include<iostream>
#include <utility>
#include<overwritedialog.h>
#include <QMutex>
#include <thread.h>





using std::reference_wrapper;



 class UndoRedo {
// This class is made for objects that keep information nessesary to undo and redo commands



private:
    QString UndoRedoCommand;
    QString UndoRedoSourceDir;
    QString UndoRedoTargetDir;
    QStringList UndoRedoList;


public:
    UndoRedo()
    {
        UndoRedoCommand="";
        UndoRedoSourceDir="";
        UndoRedoTargetDir="";
        UndoRedoList.push_back("");
     }

    QString get_UndoRedoCommand(){return UndoRedoCommand;}
    QString get_UndoRedoSourceDir(){return UndoRedoSourceDir;}
    QString get_UndoRedoTargetDir(){return UndoRedoTargetDir;}
    QStringList get_UndoRedoList(){return UndoRedoList;}

    void set_UndoRedoCommand(QString command1){UndoRedoCommand=command1;}
    void set_UndoRedoSourceDir(QString Dir){UndoRedoSourceDir=Dir;}
    void set_UndoRedoTargetDir(QString Dir){UndoRedoTargetDir=Dir;}
    void set_UndoRedoList(QStringList list)
    {
        UndoRedoList=list;
    }
    bool operator ==(UndoRedo& object1) const
    {

        if (UndoRedoCommand!=object1.get_UndoRedoCommand()) return false;
          if  (UndoRedoSourceDir!=object1.get_UndoRedoSourceDir()) return false;
           if (UndoRedoTargetDir!=object1.get_UndoRedoTargetDir())return false;
           if ( UndoRedoList.size()!=object1.get_UndoRedoList().size()) return false;

            for (int i; i<UndoRedoList.size();i++)
            {
                if (UndoRedoList[i]!=object1.get_UndoRedoList()[i]) return false;
            }
            return true;

    }
    bool operator !=(UndoRedo& object1) const
    {
        return !(*this==object1);
    }


};



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
    void Delete();
    void Copy();
    void Cut();
    Qt::ItemFlags flags(const QModelIndex &index) const;



signals:
int combo_change(int);

bool answerSet(QString);

public slots:

void fileDialog(QString,QString);
void setAnswer(QString);
QString getAnswer();

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

 void on_actionRe_do_triggered();

 void on_action_undo_triggered();

 void createContextMenu();

 void on_actionrecycle_triggered();

protected:

void writeSettings();
void ReadSettings();



private:
    Ui::filetool *ui;
    QString Answer;
    QItemSelectionModel *selModel;
    QFileSystemModel *dirModel;
    QFileSystemModel *treeViewDirModel;
    QDirModel *dirModel2; // I used QDirModel which is deprived because some commands and especially
    //                       index(j,0,dirModel2->index(path)) were not not working properly
    //                       in QFileSystemModel when I was writing the code


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

    UndoRedo PreviousObject;
    UndoRedo NextObject;
    UndoRedo command;

    OverwriteDialog *pDialog;
    PasteDialog *pasteDialog;
    QVector<UndoRedo> undovector;
    QVector<UndoRedo> redovector;
    QMutex mutex2;
};

#endif // FILETOOL_H
