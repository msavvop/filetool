#include "pastedialog.h"
#include "ui_pastedialog.h"
#include <iostream>
#include <QMessageBox>
PasteDialog::PasteDialog(QWidget *parent = 0) :
    QDialog(parent),
    ui(new Ui::PasteDialog)
{

    ui->setupUi(this);
    pathread= new Thread;
    connect(pathread, SIGNAL(valueChanged(int)), ui->progressBar, SLOT(setValue(int )));
    connect(pathread, SIGNAL(dialogComplete(bool)), this, SIGNAL(dialogComplete(bool)));


}

PasteDialog::~PasteDialog()
{

    delete ui;
}



void PasteDialog::setAction(QString str, QStringList List)
{
    int Size=0;
    actionString=str;
    list=List;

    ui->label->setText(tr("%1").arg(actionString));



    pathread->setthread(actionString,list);
    Size=pathread->showSize();
     std::cerr <<"In dialog Size= \t"<<Size<<std::endl;
     ui->progressBar->setRange(0,Size );
    pathread->start();


}

void PasteDialog::closeEvent(QCloseEvent *event)
{
    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm Canceling Current Action"),tr("Are you sure that you want to Stop Current Action?"),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {


        on_Cancel_clicked();
        event->accept();
    }
    else
    {
        event->ignore();
    }


}



void PasteDialog::on_Cancel_clicked()
{
    pathread->stop();

}

