#include "pastedialog.h"
#include "ui_pastedialog.h"
#include <iostream>
#include <QMessageBox>


extern QMutex global_mutex;
extern QWaitCondition global_var_not_set;
extern QWaitCondition global_var_set;

PasteDialog::PasteDialog(QWidget *parent = 0) :
    QDialog(parent),
    ui(new Ui::PasteDialog)
{

    ui->setupUi(this);
    pathread= new Thread;
    Answer="";
    connect(pathread, SIGNAL(valueChanged(int)), ui->progressBar, SLOT(setValue(int )));
    connect(pathread, SIGNAL(dialogComplete(bool)), this, SIGNAL(dialogComplete(bool)));
    connect(pathread,SIGNAL(fileexists( QString,QString)),this,SLOT(FileExists(QString,QString)));
//    connect(pathread,SIGNAL(fileexists( QString,QString)),this,SIGNAL(file_Exists( QString,QString)));
//    connect(pathread, SIGNAL(CloseOverWriteDialog(bool)), this, SIGNAL(CloseOverWriteDialog(bool)));


}

PasteDialog::~PasteDialog()
{
   pathread->NoToAll=false;
    pathread->YesToAll=false;
    emit CloseOverWriteDialog(true);
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

//    connect(this,SIGNAL(sendAnswer(QString)),this,SLOT(setAnswer(QString)));
//    connect(this,SIGNAL(sendAnswer(QString)),pathread,SIGNAL(send_Answer(QString)));



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

void PasteDialog::FileExists(QString Source,QString Target)
{
//      std::cerr <<"IN PasteDialog::FileExists \t"<<std::endl;

//    std::cerr <<"PasteDialog::FileExists Copying Source \t"<<qPrintable( Source)<<std::endl;
//    std::cerr <<"PasteDialog::FileExists to Target \t"<<qPrintable(Target)<<std::endl;

    emit file_Exists(Source,Target);


}
void PasteDialog::setAnswer(QString answer)
{

    Answer=answer;
//     std::cerr<<"in PasteDialog setAnswer Answer is ="<<qPrintable(Answer)<<std::endl;
     pathread->set_Answer(Answer);
     Answer="";


}
QString PasteDialog::getAnswer()
{
    return Answer;
}
