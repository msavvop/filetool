#include "filetool.h"

#include "ui_filetool.h"


#include <QMessageBox>
#include <string>


filetool::filetool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::filetool)
{
    busy=false;
    ui->setupUi(this);


    dirModel=new QFileSystemModel;
    treeViewDirModel=new QFileSystemModel;
    selModel=new QItemSelectionModel(dirModel);
    dirModel2=new QDirModel;
    connect(this, SIGNAL(combo_change(int)),this,SLOT(setCurrentComboIndex(int)));

//    dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    treeViewDirModel->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);


/*    if(dirModel->canFetchMore(QModelIndex()))
    {

       dirModel->fetchMore(QModelIndex());
    }
*/




 // ui->treeView->setStyleSheet(" { background-color: blue; }");

    ui->listView->setModel(dirModel);
    ui->treeView->setModel(treeViewDirModel);
    ui->iconView->setModel(dirModel);
    ui->tableView->setModel(dirModel);


    ui->comboBox->setModel(dirModel);



    ui->listView->setSelectionModel(selModel);
//    ui->treeView->setSelectionModel(selModel);
    ui->iconView->setSelectionModel(selModel);
    ui->tableView->setSelectionModel(selModel);



    QModelIndex cwdIndex=dirModel->setRootPath(QDir::rootPath());
    QModelIndex cwdIndex1=treeViewDirModel->setRootPath(QDir::rootPath());
    Q_UNUSED(cwdIndex1);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView->horizontalHeader()->setSectionsClickable(true);



    on_listView_clicked(cwdIndex);
    on_listView_activated(cwdIndex);
    on_actionGo_home_triggered();
    ReadSettings();



}

filetool::~filetool()
{
     writeSettings();
    delete ui;
}

void filetool::writeSettings()
{
    QSettings ftsettings;


    ftsettings.setValue("filetool/size",size());
    ftsettings.setValue("filetool/Properties",saveState());
}

void filetool::ReadSettings()
{
  QSettings ftsettings;

  resize(ftsettings.value("filetool/size",sizeHint()).toSize());
  restoreState(ftsettings.value("filetool/Properties").toByteArray());

}



void filetool::on_listView_activated(const QModelIndex &index)
{




    dirModel->setReadOnly(true);

    if (dirModel->isDir(index))
    {
        QString path=dirModel->filePath(index);

/*        QRegExp sep ("[:]");
        QString disk=path.section(sep,0,1);
        QString comboIndexexpr="^.*"+disk+".*$";
        QRegExp RegExpr(comboIndexexpr);




        int comboIndex=;
    QMessageBox *box2;
     box2->information(this,tr("comboIndex"),tr("%1").arg(comboIndex),tr("ok"));

        emit combo_change(comboIndex);
*/
        ui->lineEdit->setText(path);

        ui->listView->setRootIndex(index);
        ui->iconView->setRootIndex(index);

        dirModel->fetchMore(index);



/*
           int column=dirModel->columnCount(index);;
           QMessageBox *box;
            box->information(this,tr("arg1=%1\t row=%2").arg(column),tr("%1").arg(column),tr("ok"));
*/


          ui->tableView->setSortingEnabled(true);
//          dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

        ui->tableView->setRootIndex(index);



//        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
        ui->tableView->horizontalHeader()->setSectionsClickable(true);
        dirModel->sort(0,Qt::AscendingOrder);



//        ui->tableView->scrollTo(index);

         ui->tableView->resizeColumnsToContents();


        QModelIndex cwdIndex1= treeViewDirModel->index(path);
//        ui->treeView->setRootIndex(cwdIndex1);

        ui->treeView->scrollTo(cwdIndex1);
        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);
        ui->treeView->header()->hide();




        if(indexHistoryList.isEmpty())
        {
             indexHistoryList.push_back(index);
             return;
        }
        if((indexHistoryList.last())==index) return;
        indexHistoryList.push_back(index);



    }
}

void filetool::on_listView_clicked(const QModelIndex &index)
{
    dirModel->setReadOnly(true);
    ui->listView->setCurrentIndex(index);

    ui->iconView->setCurrentIndex(index);
    ui->tableView->setCurrentIndex(index);

    QString path=dirModel->filePath(index);
    QModelIndex cwdIndex1= treeViewDirModel->index(path);
    ui->treeView->setCurrentIndex(cwdIndex1);


}

void filetool::on_treeView_activated(const QModelIndex &index)
{


     QString path=treeViewDirModel->filePath(index);
    on_listView_activated(dirModel->index(path));

}





void filetool::on_treeView_clicked(const QModelIndex &index)
{
     QString path=treeViewDirModel->filePath(index);
    on_listView_clicked(dirModel->index(path));
}

void filetool::on_iconView_activated(const QModelIndex &index)
{
    on_listView_activated(index);
}


void filetool::on_iconView_clicked(const QModelIndex &index)
{
    on_listView_clicked(index);
}

void filetool::on_actionList_View_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void filetool::on_actionList_Details_View_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void filetool::on_actionIcon_View_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void filetool::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QRegExp sep ("[()]");
    QString index=arg1.section(sep,1,1);




    QModelIndex Index=dirModel->index(index);
    on_listView_activated(Index);
    on_listView_clicked(Index);


}

void filetool::on_comboBox_activated(const QString &arg1)
{
    QRegExp sep ("[()]");
    QString index=arg1.section(sep,1,1);




     QModelIndex Index=dirModel->index(index);
    on_listView_clicked(Index);
    on_listView_activated(Index);


}

void filetool::setCurrentComboIndex(int listIndex)
{
    QMessageBox *box;
    box->information(this,tr("listIndex"),tr("listIndex = %1").arg(listIndex),tr("ok"));
    ui->comboBox->setCurrentIndex(listIndex);
}

QStringList filetool::selectedFiles()
{
    QStringList filenames;

    QModelIndexList indexes=selModel->selectedIndexes();

    foreach (QModelIndex index,indexes)
    {
        filenames.append(dirModel->filePath(index));
    }

    return filenames;
}



void filetool::on_actionPrevious_triggered()
{
       if(indexHistoryList.isEmpty()) return;


       nextIndex=indexHistoryList.last();
       indexHistoryList.pop_back();

       if((indexForwardList.isEmpty())||(indexForwardList.first()!=nextIndex))
       {
           if(nextIndex.isValid())
           {
                indexForwardList.push_front(nextIndex);
           }


       }

       if(indexHistoryList.isEmpty())
       {

           if(nextIndex.isValid())
           {
               on_listView_clicked(nextIndex);
               on_listView_activated(nextIndex);

           }

           return;
       }
       else
       {
          previousIndex=indexHistoryList.last();

           if(previousIndex.isValid())
           {
              on_listView_clicked(previousIndex);
              on_listView_activated(previousIndex);
           }
       }

/*
       QString str;
       QMessageBox *box;
       int count=0;
       foreach(QModelIndex index, indexHistoryList)
       {

           str.append(dirModel->filePath(index)).append("\n");
           count++;
       }
           box->information(this,tr("count=%1\t row=%2").arg(count),tr("%1").arg(str),tr("ok"));
*/
}

void filetool::on_actionNext_triggered()
{
    if(indexForwardList.isEmpty()) return;


    previousIndex=indexForwardList.first();
    indexForwardList.pop_front();

    if ((indexHistoryList.isEmpty())||(indexHistoryList.last()!=previousIndex))
    {
        if(previousIndex.isValid())
        {
            indexHistoryList.push_back(previousIndex);
        }

    }


    if(indexForwardList.isEmpty())
    {

        if(previousIndex.isValid())
        {
            on_listView_clicked(previousIndex);
            on_listView_activated(previousIndex);

        }

        return;
    }
    else
    {
        nextIndex=indexForwardList.first();


        if(nextIndex.isValid())
        {
            on_listView_clicked(nextIndex);
            on_listView_activated(nextIndex);
        }

    }
/*
    QString str;
    QMessageBox *box;
    int count=0;
    foreach(QModelIndex index, indexForwardList)
    {

        str.append(dirModel->filePath(index)).append("\n");
        count++;
    }
        box->information(this,tr("count=%1\t row=%2").arg(count),tr("%1").arg(str),tr("ok"));
*/
}

QModelIndex filetool::parentDir()
{
    QModelIndex index;

    nextIndex=indexHistoryList.last();

    QString str=dirModel->filePath(nextIndex);

    QString str1=str;
    QRegExp re=QRegExp("[^/]+$");
    str.remove(re);
/*
    QMessageBox *box;
    box->information(this,tr("count=%1\t row=%2").arg(str1),tr("%1").arg(str),tr("ok"));
*/
    parentDirIndex=dirModel->index(str);
    return parentDirIndex;
}

void filetool::on_actionParentDir_triggered()
{
    parentDirIndex=parentDir();

    if(parentDirIndex.isValid())
    {
        on_listView_clicked(parentDirIndex);
        on_listView_activated(parentDirIndex);
    }
}

void filetool::on_action_exit_triggered()
{
    this->close();
}


void filetool::setNOTBusy()
{
    busy=false;
}

void filetool::closeEvent(QCloseEvent *event)
{
    if(!busy)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


void filetool::on_actionDirectory_triggered()
{
    parentDirIndex= indexHistoryList.last();
    QModelIndex index= dirModel->mkdir( parentDirIndex,"New Directory");

    dirModel->setReadOnly(false);

    ui->tableView->edit(index);

}



void filetool::on_action_rename_triggered()
{

    QStringList selected=selectedFiles();
    dirModel->setReadOnly(false);

    if (selected.size()!=1) return;
    QString selectedItem=selected.at(0);

/*    QMessageBox *box;
    box->information(this,tr("count=%1\t row=%2").arg(selectedItem),tr("%1").arg(selectedItem),tr("ok"));
*/
    QModelIndex index=dirModel->index(selectedItem);

    ui->tableView->edit(index);

}



void filetool::on_lineEdit_textChanged(const QString &arg1)
{
    newPath=arg1;
}

void filetool::on_pushButton_clicked()
{
   QModelIndex index= dirModel->index(newPath);

   if(index.isValid())
   {
       on_listView_clicked(index);
       on_listView_activated(index);
   }

}

void filetool::on_actionGo_home_triggered()
{
    QModelIndex index=dirModel->myComputer(Qt::DisplayRole).toModelIndex();
        QString path=dirModel->filePath(index);
        dirModel->setRootPath(path);
        treeViewDirModel->setRootPath(path);
        on_listView_clicked(index);
        on_listView_activated(index);

        ui->lineEdit->setText(tr("MyComputer:"));



}

void filetool::on_tableView_activated(const QModelIndex &index)
{
/*
    QMessageBox *box;
     box->information(this,tr("index"),tr("index.row=%1 \nindex.column=%2").arg(index.row()).arg(index.column()),tr("ok"));
*/
     QModelIndex Index;

     Index=dirModel->index(index.row(),0,indexHistoryList.last());



     on_listView_activated(Index);
}

void filetool::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex Index;

    Index=dirModel->index(index.row(),0,indexHistoryList.last());




     on_listView_clicked(Index);
}




void filetool::on_action_delete_triggered()
{
    QModelIndex Index;


    List=selectedFiles();


    Index=indexHistoryList.last();
    List.append(dirModel->filePath(Index));

    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm Deletion"),tr("Are you sure that you want to Delete %1 items?").arg(List.size()-1),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {

        QString str="delete";
        PasteDialog *DeleteDialog;
        DeleteDialog=new PasteDialog(this);
        DeleteDialog->setAction(str,List);
        busy=true;


        DeleteDialog->show();
        DeleteDialog->raise();
        DeleteDialog->activateWindow();
        connect(DeleteDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
        connect(DeleteDialog, SIGNAL(dialogComplete(bool)), DeleteDialog, SLOT(deleteLater()));

    }


}

void filetool::on_action_cut_triggered()
{



    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm move action"),tr("Are you sure that you want to move %1 items?").arg(selModel->selectedIndexes().size()),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {
        copyIndicator=false;

       List=selectedFiles();
/*        QMessageBox *box;
       for (int i=0;i<List.size();i++)
       {
       box->information(this,tr("Size=  %1").arg(List.size()),tr("ok"));
       box->information(this,tr("List[0]=  %1").arg(List[0]),tr("ok"));
       }
       */

    }





}

void filetool::on_actionC_opy_triggered()
{


    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm copy action"),tr("Are you sure that you want to copy %1 items?").arg(selModel->selectedIndexes().size()),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {
        copyIndicator=true;

        List=selectedFiles();
/*        QMessageBox *box;
        for (int i=0;i<List.size();i++)
        {
        box->information(this,tr("Size=  %1").arg(List.size()),tr("ok"));
        box->information(this,tr("List[0]=  %1").arg(List[0]),tr("ok"));
        }
        */

    }
}

void filetool::on_action_paste_triggered()
{
    QModelIndex Index;


    Index=indexHistoryList.last();
    List.append(dirModel->filePath(Index));
    dirModel->setReadOnly(false);



    if(copyIndicator==true)
    {
        busy=true;
        QString str1="copy";
        PasteDialog *CopyDialog;
        CopyDialog=new PasteDialog(this);
        CopyDialog->setAction(str1,List);

         CopyDialog->show();
         CopyDialog->raise();
        CopyDialog->activateWindow();
        connect(CopyDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
        connect(CopyDialog, SIGNAL(dialogComplete(bool)), CopyDialog, SLOT(deleteLater()));

//        CopyDialog->exec();

    }
    else
    {
        busy=true;
        QString str2="cut";
        PasteDialog *CutDialog;
        CutDialog=new PasteDialog(this);
        CutDialog->setAction(str2,List);


       CutDialog->show();
        CutDialog->raise();
      CutDialog->activateWindow();
       connect(CutDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
        connect(CutDialog, SIGNAL(dialogComplete(bool)), CutDialog, SLOT(deleteLater()));

//    CutDialog->exec();

    }
}


void filetool::on_actionAbout_triggered()
{
    QMessageBox *box;
    box->information(this,tr("Filetool"),tr("This Program is Created by Mihail Savvopoulos using Qt"),QMessageBox::Ok);
}
