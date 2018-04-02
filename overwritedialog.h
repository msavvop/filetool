#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class OverwriteDialog;
}

class OverwriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OverwriteDialog(QWidget *parent = 0);
    ~OverwriteDialog();

    void setFiles(QString,QString);

private:
    Ui::OverwriteDialog *ui;
    QString SourceFile;
    QString TargetFile;
    QCloseEvent *event;
signals:

  void sendAnswer(QString);







private slots:
     void on_pushButton_4_clicked();
     void on_pushButton_3_clicked();
     void on_pushButton_2_clicked();
     void on_pushButton_clicked();

};

#endif // OVERWRITEDIALOG_H
