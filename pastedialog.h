#ifndef PASTEDIALOG_H
#define PASTEDIALOG_H


#include "thread.h"
#include <QDialog>
#include<QEvent>
#include <QCloseEvent>
#include <QFileSystemModel>



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

    void setAction(QString str, QString Source, QStringList List);
private:
    Ui::PasteDialog *ui;
    Thread *pathread;
    QString actionString;
   QStringList list;
   QString Answer;
   QString ReturnValue;


  public slots:

    void on_Cancel_clicked();


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
void ReturnThisValue(bool);
};

#endif // PASTEDIALOG_H
