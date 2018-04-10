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
    ui->label->setText(tr("File \n%2 \nexists, should I overwrite it  with file \n %1").arg(SourceFile).arg(TargetFile));


}
void OverwriteDialog::on_pushButton_4_clicked()
{

//     std::cerr<<"answer Yes"<<std::endl;
     close();
     emit sendAnswer("Yes");


}

void OverwriteDialog::on_pushButton_3_clicked()
{

//     std::cerr<<"answer YesToAll"<<std::endl;
     close();

   emit sendAnswer("YesToAll");


}

void OverwriteDialog::on_pushButton_2_clicked()
{

//     std::cerr<<"answer No"<<std::endl;
      close();
   emit sendAnswer("No");



}

void OverwriteDialog::on_pushButton_clicked()
{

//     std::cerr<<"answer NoToAll"<<std::endl;
     close();

   emit sendAnswer("NoToAll");


}
void OverwriteDialog::closedialog(bool answer)
{

    if (answer)
    {
        close();
//        event->accept();
    }
    else
    {
//        event->ignore();
    }



}
