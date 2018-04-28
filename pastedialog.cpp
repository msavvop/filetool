#include "pastedialog.h"
#include "ui_pastedialog.h"
#include <iostream>
#include <QMessageBox>


extern QMutex global_mutex;
extern QWaitCondition global_var_not_set;
extern QWaitCondition global_var_set;

extern QMutex global_mutex2;
extern QWaitCondition global_Return_var_not_set;
extern QWaitCondition global_Return_var_set;

extern int HowManyThreads;

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
//    connect(pathread,SIGNAL(ReturnThisValue(bool)),this,SIGNAL(ReturnThisValue(bool)));
    connect(pathread,SIGNAL(ReturnThisValue(bool)),this,SLOT(ReturnObjectAndCode(bool)));

//    connect(pathread,SIGNAL(fileexists( QString,QString)),this,SIGNAL(file_Exists( QString,QString)));
//    connect(pathread, SIGNAL(CloseOverWriteDialog(bool)), this, SIGNAL(CloseOverWriteDialog(bool)));


}

PasteDialog::~PasteDialog()
{

    emit CloseOverWriteDialog(true);

    delete ui;
}



void PasteDialog::setAction(UndoRedo command)
{
    actionString=command.get_UndoRedoCommand();
    QString Source=command.get_UndoRedoSourceDir();
    list=command.get_UndoRedoList();
    ReturnObject=command;
    int Size=0;


    for(int i=0;i<list.size()-1;i++)
    {
        if(actionString=="delete")
        {
            ui->label->setText(tr("%1\n  %2").arg(actionString).arg(list.at(i)));

        }
        else
        {
            ui->label->setText(tr("%1\n  %2 \n to %3").arg(actionString).arg(list.at(i)).arg(list.last()));

        }


    }




    pathread->setthread(actionString,Source,list);
    Size=pathread->showSize();
     std::cerr <<"In dialog Size= \t"<<Size<<std::endl;
     ui->progressBar->setRange(0,Size );


//    connect(this,SIGNAL(sendAnswer(QString)),this,SLOT(setAnswer(QString)));
//    connect(this,SIGNAL(sendAnswer(QString)),pathread,SIGNAL(send_Answer(QString)));


    pathread->start();



}

void PasteDialog::ReturnObjectAndCode(bool ReturnValue)
{
    emit ReturnTheseValues(ReturnValue,ReturnObject);
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
        pathread->requestInterruption();

//        pathread->stop();


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
