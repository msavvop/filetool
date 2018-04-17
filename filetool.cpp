#include "filetool.h"

#include "ui_filetool.h"


#include <QMessageBox>
#include <string>

QMutex global_mutex;
QWaitCondition global_var_not_set;
QWaitCondition global_var_set;

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
    pDialog=new OverwriteDialog();

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
    createContextMenu();
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
    QString ParentDir_name=dirModel->filePath(parentDirIndex);
    QModelIndex index= dirModel->mkdir( parentDirIndex,"New Directory");

    dirModel->setReadOnly(false);

    ui->tableView->edit(index);

    QString katalog_name=dirModel->filePath(index);
    std::cerr<<"in Create Dir new Dir name"<<qPrintable (katalog_name)<<std::endl;

     List.clear();

     List.push_back(katalog_name);

     command.set_UndoRedoCommand("create_dir");
     command.set_UndoRedoList(List);
     command.set_UndoRedoSourceDir(ParentDir_name);
     command.set_UndoRedoTargetDir(katalog_name);

//    QString New_katalog_name=dirModel->filePath(index);

//    std::cout<<qPrintable(Old_katalog_name)<<endl;

    undovector.push_back(command);
    List.clear();

}



void filetool::on_action_rename_triggered()
{

    parentDirIndex= indexHistoryList.last();
    QString ParentDir_name=dirModel->filePath(parentDirIndex);
    std::cerr<<"AT RENAME ParentDir_name=   "<<qPrintable(ParentDir_name)<<std::endl;

    if (selModel->selectedIndexes().size()!=1) return;



    QStringList selected=selectedFiles();

    if (selected.size()!=1) return;
    QString selectedItem=selected.at(0);


    std::cerr<<"AT RENAME selected item=    "<<qPrintable(selectedItem)<<std::endl;

    QModelIndex index=dirModel->index(selectedItem);
     reference_wrapper <QModelIndex> ref(index);
//    int row=dirModel->index(selectedItem).row();





    dirModel->setReadOnly(false);

    if(ui->tableView->isActiveWindow())
    {
    ui->tableView->edit(ref);
    }
    else if(ui->listView->isActiveWindow())
    {
        ui->listView->edit(ref);
    }
    else if (ui->iconView->isActiveWindow())
    {
        ui->iconView->edit(ref);
    }
    else if (ui->treeView->isActiveWindow())
    {
        ui->treeView->edit(ref);
    }


//    QModelIndex index1=dirModel->index(row,0,parentDirIndex);

    if(!index.isValid()) return;
    QString newname;

    newname= dirModel->filePath(ref);
//  newname= dirModel->filePath(index1);

    std::cerr<<"AT RENAME New_katalog_name newname =    "<<qPrintable(newname)<<std::endl;

     List.clear();

     List.push_back(selectedItem);

     command.set_UndoRedoCommand("rename");
     command.set_UndoRedoList(List);
     command.set_UndoRedoSourceDir(ParentDir_name);
     command.set_UndoRedoTargetDir(newname);

//    QString newname=dirModel->filePath(index);

//    std::cout<<"AT RENAME new katalog name="<<qPrintable(newname)<<std::endl;

      undovector.push_back(command);
      List.clear();
}



void filetool::on_lineEdit_textChanged(const QString &arg1)
{
    newPath=arg1;
//    std::cout<<qPrintable(newPath)<<endl;
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

    List.clear();

    List=selectedFiles();


    Index=indexHistoryList.last();
    List.append(dirModel->filePath(Index));//  carefull because I add the path of the current directory as a last element
                                            //in this list and you do not want to delete that.

    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm Deletion"),tr("Are you sure that you want to Delete %1 items? "
                                                        "Removing them is permanent and cannot be undone").arg(List.size()-1),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {

        Delete();

    }
    List.clear();


}

void filetool::on_action_cut_triggered()
{

    QModelIndex Index;

    Index=indexHistoryList.last();
    QString ParentDir_name=dirModel->filePath(Index);

    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm move action"),tr("Are you sure that you want to move %1 items?").arg(selModel->selectedIndexes().size()),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {
        copyIndicator=false;

        List.clear();

        List=selectedFiles();

         command.set_UndoRedoSourceDir(ParentDir_name);
         Source=ParentDir_name;




//       std::cerr<<"AT CUT selected item"<<qPrintable(List.at(0))<<endl;
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
    QModelIndex Index;

    Index=indexHistoryList.last();
    QString ParentDir_name=dirModel->filePath(Index);

    QMessageBox *box1;
    int r=box1->question(this,tr("Confirm copy action"),tr("Are you sure that you want to copy %1 items?").arg(selModel->selectedIndexes().size()),
                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default|QMessageBox::Escape);

    if(r==QMessageBox::Yes)
    {
        copyIndicator=true;
        command.set_UndoRedoSourceDir(ParentDir_name);
        Source=ParentDir_name;


        List.clear();
        List=selectedFiles();

//       std::cerr<<"AT COPY selected item"<<qPrintable(List.at(0))<<endl;

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

    if(List.isEmpty())  return;
    Index=indexHistoryList.last();
    QString newname=dirModel->filePath(Index);
    List.append(newname);

    std::cerr <<" AT PASTE CopyList[0]=\t"<<qPrintable( List[0])<<std::endl;
    dirModel->setReadOnly(false);

    if(copyIndicator==true)
    {
        Copy();

        command.set_UndoRedoCommand("copy");
        command.set_UndoRedoList(List);
        command.set_UndoRedoTargetDir(newname);
    }
    else
    {
        Cut();

        command.set_UndoRedoCommand("cut");
        command.set_UndoRedoList(List);
        command.set_UndoRedoTargetDir(newname);
    }

    undovector.push_back(command);
    List.clear();
}


void filetool::on_actionAbout_triggered()
{
    QMessageBox *box;
    box->information(this,tr("Filetool"),tr("This Program is Created by Mihail Savvopoulos using Qt. "
                                            "The icons were  taken from Linux. (Default KDE icons from Mint.)"),QMessageBox::Ok);
}


void filetool::on_actionRe_do_triggered()
{
    QStringList List1;
    QString SourceDir;
    QString TargetDir;

    if(redovector.isEmpty()) return;

    PreviousObject=redovector.last();
    redovector.pop_back();


    if ((undovector.isEmpty())||(undovector.last()!=PreviousObject))
    {

            undovector.push_back(PreviousObject);


    }


    if(redovector.isEmpty())
    {
        if (PreviousObject.get_UndoRedoCommand()=="create_dir")
        {
            std::cerr<<"Redo create_dir under construction"<<std::endl;
    /*
            List.clear();
            List1= PreviousObject.get_UndoRedoList();
            SourceDir=PreviousObject.get_UndoRedoSourceDir();
            TargetDir=PreviousObject.get_UndoRedoTargetDir();


            QModelIndex parentDirIndex=dirModel->index(SourceDir);
            QString name=TargetDir.replace(SourceDir,"");
            name=name.replace("/","");
            QModelIndex index= dirModel->mkdir( parentDirIndex,name);
            List.clear();

   */
        }
        else if (PreviousObject.get_UndoRedoCommand()=="rename")
        {
            std::cerr<<"Redo rename under construction"<<std::endl;
    /*

            List.clear();
            List1= PreviousObject.get_UndoRedoList();
            SourceDir=PreviousObject.get_UndoRedoSourceDir();
            TargetDir=PreviousObject.get_UndoRedoTargetDir();


            std::cerr<<"SourceDir="<<qPrintable(SourceDir)<<std::endl;
            std::cerr<<"TargetDir="<<qPrintable(TargetDir)<<std::endl;
            std::cerr<<"old name="<<qPrintable(List1[0])<<std::endl;

            QModelIndex Index=dirModel->index(SourceDir);

            QString name=TargetDir;
            name=name.replace(SourceDir,"");
            name=name.replace("/","");
            std::cerr<<"name of Dir redone to the new one again="<<qPrintable(name)<<std::endl;


            dirModel->setReadOnly(false);
            QModelIndex index1= dirModel->mkdir(Index,name); // create again new dir to redo command
            if (!index1.isValid())
            {
                std::cerr<<"could not create Dir in undo"<<std::endl;
                return;
            }

            QString newdir=List1[0];
            List1.clear();

            for(int j=0;j<dirModel->rowCount(dirModel->index(newdir));j++)
            {
                QModelIndex sourceNewIndex=dirModel->index(j,0,dirModel->index(newdir));
                const QString path3=dirModel->filePath(sourceNewIndex);
                List1.append(path3);
            }



            List1.push_back(SourceDir);
            List=List1;
            Cut();

            QStringList List2;
            List2.append(TargetDir);
            List2.append(SourceDir);
            List=List2;
            Delete();
            List.clear();

    */
        }
        else if (PreviousObject.get_UndoRedoCommand()=="cut")
        {
            List.clear();
            List1= PreviousObject.get_UndoRedoList();
            SourceDir=PreviousObject.get_UndoRedoSourceDir();
            TargetDir=PreviousObject.get_UndoRedoTargetDir();
 //            List1.pop_back(); //TargetDir already in List1
 //            List1.append(TargetDir);//TargetDir already in List1
            List=List1;
            Cut();
            List.clear();

        }
        else if (PreviousObject.get_UndoRedoCommand()=="copy")
        {
            List.clear();
            List1= PreviousObject.get_UndoRedoList();
            SourceDir=PreviousObject.get_UndoRedoSourceDir();
            TargetDir=PreviousObject.get_UndoRedoTargetDir();
//            List1.pop_back(); //TargetDir already in List1
//            List1.append(TargetDir);//TargetDir already in List1
            List=List1;
            Copy();
            List.clear();


        }


        return;
    }
    else
    {
        NextObject=undovector.last();


        if (NextObject.get_UndoRedoCommand()=="create_dir")
        {
            std::cerr<<"Redo create_dir under construction"<<std::endl;
            /*
            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();


            QModelIndex parentDirIndex=dirModel->index(SourceDir);
            QString name=TargetDir.replace(SourceDir,"");
            name=name.replace("/","");
            QModelIndex index= dirModel->mkdir( parentDirIndex,name);
            List.clear();

            */
        }
        else if (NextObject.get_UndoRedoCommand()=="rename")
        {
            std::cerr<<"Redo rename under construction"<<std::endl;
    /*

            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();


            std::cerr<<"SourceDir="<<qPrintable(SourceDir)<<std::endl;
            std::cerr<<"TargetDir="<<qPrintable(TargetDir)<<std::endl;
            std::cerr<<"old name="<<qPrintable(List1[0])<<std::endl;

            QModelIndex Index=dirModel->index(SourceDir);

            QString name=TargetDir;
            name=name.replace(SourceDir,"");
            name=name.replace("/","");
            std::cerr<<"name of Dir redone to the new one again="<<qPrintable(name)<<std::endl;


            dirModel->setReadOnly(false);
            QModelIndex index1= dirModel->mkdir(Index,name); // create again new dir to redo command
            if (!index1.isValid())
            {
                std::cerr<<"could not create Dir in undo"<<std::endl;
                return;
            }

            QString newdir=List1[0];
            List1.clear();

            for(int j=0;j<dirModel->rowCount(dirModel->index(newdir));j++)
            {
                QModelIndex sourceNewIndex=dirModel->index(j,0,dirModel->index(newdir));
                const QString path3=dirModel->filePath(sourceNewIndex);
                List1.append(path3);
            }



            List1.push_back(SourceDir);
            List=List1;
            Cut();

            QStringList List2;
            List2.append(TargetDir);
            List2.append(SourceDir);
            List=List2;
            Delete();
            List.clear();

    */
        }
        else if (NextObject.get_UndoRedoCommand()=="cut")
        {
            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();
//            List1.pop_back(); //TargetDir already in List1
//            List1.append(TargetDir);//TargetDir already in List1
            List=List1;
            Cut();
            List.clear();

        }
        else if (NextObject.get_UndoRedoCommand()=="copy")
        {
            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();
//            List1.pop_back(); //TargetDir already in List1
//            List1.append(TargetDir);//TargetDir already in List1
           List=List1;
            Copy();
            List.clear();


        }


    }
    List.clear();

}


void filetool::on_action_undo_triggered()
{
     QStringList List1;
     QString SourceDir;
     QString TargetDir;


    if(undovector.isEmpty()) return;


    NextObject=undovector.last();
    undovector.pop_back();

    if((redovector.isEmpty())||(redovector.last()!=NextObject))
    {
             redovector.push_back(NextObject);
    }


    if(undovector.isEmpty())
    {

        if (NextObject.get_UndoRedoCommand()=="create_dir")
        {
            std::cerr<<"Undo Create_dir under construction"<<std::endl;

     /*
            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();
            List1.push_back(SourceDir);
            List=List1;
            Delete();
            List.clear();

    */
        }
        else if (NextObject.get_UndoRedoCommand()=="rename")
        {
            std::cerr<<"Undo rename under construction"<<std::endl;

    /*
            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();


            std::cerr<<"SourceDir="<<qPrintable(SourceDir)<<std::endl;
            std::cerr<<"TargetDir="<<qPrintable(TargetDir)<<std::endl;
            std::cerr<<"old name="<<qPrintable(List1[0])<<std::endl;

            QModelIndex Index=dirModel->index(SourceDir);

            QString name=List1[0];
            name=name.replace(SourceDir,"");
            name.replace("/","");
            std::cerr<<"name of Dir undone to the old one="<<qPrintable(name)<<std::endl;


            dirModel->setReadOnly(false);
            QModelIndex index1= dirModel->mkdir(Index,name); // create old dir to resore

            if (!index1.isValid())
            {
                std::cerr<<"could not create Dir in undo"<<std::endl;
                return;
            }

            List1.clear();

            for(int j=0;j<dirModel->rowCount(dirModel->index(TargetDir));j++)
            {
                QModelIndex sourceNewIndex=dirModel->index(j,0,dirModel->index(TargetDir));
                const QString path3=dirModel->filePath(sourceNewIndex);
                List1.append(path3);
            }



            List1.push_back(SourceDir);
            List=List1;
            Cut();

            QStringList List2;
            List2.append(TargetDir);
            List2.append(SourceDir);
            List=List2;
            Delete();
            List.clear();

    */
        }
        else if (NextObject.get_UndoRedoCommand()=="cut")
        {

            List.clear();
            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();
            List1.pop_back();

            for(int i=0;i<List1.size();i++)
            {
                QString path=List1[i];
                path=path.replace(SourceDir,TargetDir+"/");
                path=path.replace("//","/");//Needs a / in order to work correctly
                List1[i]=path;

            }
            List1.push_back(SourceDir);
            List=List1;
            Cut();
            List.clear();


        }
        else if (NextObject.get_UndoRedoCommand()=="copy")
        {
            List.clear();

            List1= NextObject.get_UndoRedoList();
            SourceDir=NextObject.get_UndoRedoSourceDir();
            TargetDir=NextObject.get_UndoRedoTargetDir();
            List1.pop_back();
            for(int i=0;i<List1.size();i++)
            {
                QString path=List1[i];
                path=path.replace(SourceDir,TargetDir+"/");
                path=path.replace("//","/");       ;//Needs a / in order to work correctly
                List1[i]=path;
            std::cerr<<"name of files in undo ="<<qPrintable(path)<<std::endl;
            }
            List1.push_back(TargetDir);
            std::cerr<<"name of TargetDir in undo ="<<qPrintable(TargetDir)<<std::endl;
            List=List1;
            Delete();
            List.clear();

        }

        return;
    }
    else
    {
       PreviousObject=redovector.last();

       if (PreviousObject.get_UndoRedoCommand()=="create_dir")
       {
           std::cerr<<"Undo Create_dir under construction"<<std::endl;

     /*
           List.clear();
           List1= PreviousObject.get_UndoRedoList();
           SourceDir=PreviousObject.get_UndoRedoSourceDir();
           TargetDir=PreviousObject.get_UndoRedoTargetDir();
           List1.push_back(SourceDir);
           List=List1;
           Delete();
           List.clear();



   */
       }
       else if (PreviousObject.get_UndoRedoCommand()=="rename")
       {
           std::cerr<<"Undo rename under construction"<<std::endl;

   /*        List.clear();
           List1= PreviousObject.get_UndoRedoList();
           SourceDir=PreviousObject.get_UndoRedoSourceDir();
           TargetDir=PreviousObject.get_UndoRedoTargetDir();


           std::cerr<<"SourceDir="<<qPrintable(SourceDir)<<std::endl;
           std::cerr<<"TargetDir="<<qPrintable(TargetDir)<<std::endl;
           std::cerr<<"old name="<<qPrintable(List1[0])<<std::endl;

           QModelIndex Index=dirModel->index(SourceDir);

           QString name=List1[0];
           name=name.replace(SourceDir,"");
           name.replace("/","");
           std::cerr<<"name of Dir undone to the old one="<<qPrintable(name)<<std::endl;


           dirModel->setReadOnly(false);
           QModelIndex index1= dirModel->mkdir(Index,name); // create old dir to resore

           if (!index1.isValid())
           {
               std::cerr<<"could not create Dir in undo"<<std::endl;
               return;
           }

           List1.clear();

           for(int j=0;j<dirModel->rowCount(dirModel->index(TargetDir));j++)
           {
               QModelIndex sourceNewIndex=dirModel->index(j,0,dirModel->index(TargetDir));
               const QString path3=dirModel->filePath(sourceNewIndex);
               List1.append(path3);
           }



           List1.push_back(SourceDir);
           List=List1;
           Cut();

           QStringList List2;
           List2.append(TargetDir);
           List2.append(SourceDir);
           List=List2;
           Delete();
           List.clear();

   */
       }
       else if (PreviousObject.get_UndoRedoCommand()=="cut")
       {
           List.clear();
           List1= PreviousObject.get_UndoRedoList();
           SourceDir=PreviousObject.get_UndoRedoSourceDir();
           TargetDir=PreviousObject.get_UndoRedoTargetDir();
           List1.pop_back();

           for(int i=0;i<List1.size();i++)
           {
               QString path=List1[i];
               path=path.replace(SourceDir,TargetDir+"/");
               path=path.replace("//","/");//Needs a / in order to work correctly
               std::cerr<<"name of files in undo ="<<qPrintable(path)<<std::endl;
               List1[i]=path;

           }

           List1.push_back(SourceDir);
           std::cerr<<"name of SourceDir in undo ="<<qPrintable(SourceDir)<<std::endl;
           List=List1;
           Cut();
           List.clear();


       }
       else if (PreviousObject.get_UndoRedoCommand()=="copy")
       {
           List.clear();

           List1= PreviousObject.get_UndoRedoList();
           SourceDir=PreviousObject.get_UndoRedoSourceDir();
           TargetDir=PreviousObject.get_UndoRedoTargetDir();
           List1.pop_back();

           for(int i=0;i<List1.size();i++)
           {
               QString path=List1[i];
               path=path.replace(SourceDir,TargetDir+"/");
               path=path.replace("//","/");//Needs a / in order to work correctly
               std::cerr<<"name of files in undo ="<<qPrintable(path)<<std::endl;
               List1[i]=path;

           }

           List1.push_back(TargetDir);
           std::cerr<<"name of files in undo ="<<qPrintable(TargetDir)<<std::endl;
           List=List1;
           Delete();
           List.clear();

       }
    }


    List.clear();
}


void filetool::Delete()
{
QString str="delete";
PasteDialog *DeleteDialog;
DeleteDialog=new PasteDialog(this);
DeleteDialog->setAction(str,Source,List);
busy=true;


DeleteDialog->show();
DeleteDialog->raise();
DeleteDialog->activateWindow();
connect(DeleteDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
connect(DeleteDialog, SIGNAL(dialogComplete(bool)), DeleteDialog, SLOT(deleteLater()));

}

void filetool::Copy()
{
busy=true;
QString str1="copy";
PasteDialog *CopyDialog;
CopyDialog=new PasteDialog(this);
CopyDialog->setAction(str1,Source,List);

 CopyDialog->show();
 CopyDialog->raise();
CopyDialog->activateWindow();
connect(CopyDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
connect(CopyDialog, SIGNAL(dialogComplete(bool)), CopyDialog, SLOT(deleteLater()));
pasteDialog=CopyDialog;
connect(CopyDialog,SIGNAL(file_Exists(QString,QString)),this,SLOT(fileDialog(QString,QString)));


}

void filetool::Cut()
{
busy=true;
QString str2="cut";
PasteDialog *CutDialog;
CutDialog=new PasteDialog(this);
CutDialog->setAction(str2,Source,List);


CutDialog->show();
CutDialog->raise();
CutDialog->activateWindow();
connect(CutDialog, SIGNAL(dialogComplete(bool)), this, SLOT(setNOTBusy()));
connect(CutDialog, SIGNAL(dialogComplete(bool)), CutDialog, SLOT(deleteLater()));

pasteDialog=CutDialog;
connect(CutDialog,SIGNAL(file_Exists(QString, QString)),this,SLOT(fileDialog(QString,QString)));


//    CutDialog->exec();
}
void filetool::createContextMenu()
{
ui->listView->addAction(ui->actionC_opy);
ui->listView->addAction(ui->action_cut);
ui->listView->addAction(ui->action_paste);
ui->listView->addAction(ui->action_rename);
ui->listView->addAction(ui->actionDirectory);
ui->listView->addAction(ui->actionrecycle);
ui->listView->addAction(ui->action_delete);
ui->listView->addAction(ui->action_undo);
ui->listView->addAction(ui->actionRe_do);

ui->treeView->addAction(ui->actionC_opy);
ui->treeView->addAction(ui->action_cut);
ui->treeView->addAction(ui->action_paste);
ui->treeView->addAction(ui->action_rename);
ui->treeView->addAction(ui->actionDirectory);
ui->treeView->addAction(ui->actionrecycle);
ui->treeView->addAction(ui->action_delete);
ui->treeView->addAction(ui->action_undo);
ui->treeView->addAction(ui->actionRe_do);

ui->tableView->addAction(ui->actionC_opy);
ui->tableView->addAction(ui->action_cut);
ui->tableView->addAction(ui->action_paste);
ui->tableView->addAction(ui->action_rename);
ui->tableView->addAction(ui->actionDirectory);
ui->tableView->addAction(ui->actionrecycle);
ui->tableView->addAction(ui->action_delete);
ui->tableView->addAction(ui->action_undo);
ui->tableView->addAction(ui->actionRe_do);

ui->iconView->addAction(ui->actionC_opy);
ui->iconView->addAction(ui->action_cut);
ui->iconView->addAction(ui->action_paste);
ui->iconView->addAction(ui->action_rename);
ui->iconView->addAction(ui->actionDirectory);
ui->iconView->addAction(ui->actionrecycle);
ui->iconView->addAction(ui->action_delete);
ui->iconView->addAction(ui->action_undo);
ui->iconView->addAction(ui->actionRe_do);

ui->listView->setContextMenuPolicy(Qt::ActionsContextMenu);
ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
ui->iconView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void filetool::on_actionrecycle_triggered()
{
    std::cerr<<"not implemented yet"<<std::endl;
}
void filetool::fileDialog(QString Source,QString Target)
{

    std::cerr <<"In Filetool FileDialog Source\t"<<qPrintable( Source)<<std::endl;
    std::cerr <<"In filetool FileDialog Target\t"<<qPrintable(Target)<<std::endl;

    pDialog->show();
    pDialog->raise();
    pDialog->activateWindow();
    pDialog->setFiles(Source,Target);

    std::cerr <<"Copying Source \t"<<qPrintable(Source)<<std::endl;
    std::cerr <<"to Target \t"<<qPrintable(Target)<<std::endl;

//    pDialog->exec();
    connect(pDialog,SIGNAL(sendAnswer(QString)),this,SLOT(setAnswer(QString)));
    connect(pasteDialog, SIGNAL(CloseOverWriteDialog(bool)), pDialog, SLOT(closedialog(bool)));






}
void filetool::setAnswer(QString answer)
{
    global_mutex.lock();
    if((getAnswer()!=""))
    {

       global_var_set.wait(&global_mutex);

    }
    Answer=answer;

//    std::cerr<<"in filetool setAnswer Answer= "<<qPrintable(getAnswer())<<std::endl;



    pasteDialog->setAnswer(getAnswer());
    global_var_not_set.wakeOne();
          global_mutex.unlock();

    Answer="";





}

QString filetool::getAnswer()
{
    return Answer;
}
