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

    void setAction(QString str, QStringList List);
private:
    Ui::PasteDialog *ui;
    Thread *pathread;
    QString actionString;
   QStringList list;


  public slots:

    void on_Cancel_clicked();

    private slots:

    void closeEvent(QCloseEvent *event);

signals:

void dialogComplete(bool);
};

#endif // PASTEDIALOG_H
