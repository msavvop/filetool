#include "overwritedialog.h"
#include "ui_overwritedialog.h"
#include <iostream>

OverwriteDialog::OverwriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverwriteDialog)
{

    ui->setupUi(this);


}

OverwriteDialog::~OverwriteDialog()
{
    delete ui;
}


void OverwriteDialog::setFiles(QString source,QString target)
{
    SourceFile=source;
    TargetFile=target;
    ui->label->setText(tr("File %2 exists, should I overwrite it \n with file %1").arg(SourceFile).arg(TargetFile));


}
void OverwriteDialog::on_pushButton_4_clicked()
{
  emit sendAnswer("Yes");
    std::cerr<<"answer Yes"<<std::endl;
  event->accept();


}

void OverwriteDialog::on_pushButton_3_clicked()
{
   emit sendAnswer("YesToAll");
    std::cerr<<"answer YesToAll"<<std::endl;
    event->accept();

}

void OverwriteDialog::on_pushButton_2_clicked()
{
   emit sendAnswer("No");
    std::cerr<<"answer No"<<std::endl;
 event->accept();

}

void OverwriteDialog::on_pushButton_clicked()
{
   emit sendAnswer("NoToAll");
    std::cerr<<"answer NoToAll"<<std::endl;
  event->accept();

}

