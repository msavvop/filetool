#ifndef PASTEDIALOG_H
#define PASTEDIALOG_H


#include "thread.h"
#include <QDialog>
#include<QEvent>
#include <QCloseEvent>
#include <QFileSystemModel>
#include<undoredo.h>


class QCloseEvent;
class QModelIndex;
namespace Ui {
class PasteDialog;
}

class PasteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasteDialog(QWidget *parent );
    ~PasteDialog();

//    void setAction(QString str, QString Source, QStringList List);
    void setAction(UndoRedo);

private:
    Ui::PasteDialog *ui;
    Thread *pathread;
    QString actionString;
   QStringList list;
   QStringList list1;
   QString Answer;
   UndoRedo ReturnObject;


  public slots:

    void on_Cancel_clicked();
    void ReturnObjectAndCode(bool,QStringList);


void FileExists(QString Source,QString Target);
void setAnswer(QString);
QString getAnswer();

    private slots:

    void closeEvent(QCloseEvent *event);

signals:

void dialogComplete(bool);
void file_Exists(QString Source,QString Target);
void sendAnswer(QString);
void CloseOverWriteDialog(bool);
void ReturnTheseValues(bool,QStringList,UndoRedo);
};

#endif // PASTEDIALOG_H
