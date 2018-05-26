#include "thread.h"
#include <iostream>
#include <QTextStream>


extern QMutex global_mutex;
extern QWaitCondition global_var_not_set;
extern QWaitCondition global_var_set;

extern QMutex global_mutex2;
extern QWaitCondition global_Return_var_not_set;
extern QWaitCondition global_Return_var_set;


extern int HowManyThreads;


Thread::Thread()
{

//      stopped = false;

      NoToAll=false;
      YesToAll=false;
      HowManyThreads++;
      Answer="";
      NotOverWritten.clear();
      dirModel=new QFileSystemModel;
      dirModel2=new QDirModel;


}



void Thread::setthread(QString ActionString, QString source,QStringList list, QStringList list1)
{
    ActionStr=ActionString;
    List=list;
    Source=source;
    NotOverWrittenList=list1;
//    std::cerr<<"AT SETTHREAD selected item"<<qPrintable(List.at(0))<<endl;
}


void Thread::run()
{

    bool ok;


   

            ok=fileaction(ActionStr,Source,List);

            if(!ok)
            {
                std::cerr <<"fileaction Failed\n"<<std::endl;
//                terminate();


            }
            else
            {
                 std::cerr <<"fileaction ok\n"<<std::endl;


            }
            emit dialogComplete(true);
            emit ReturnTheseValues(ok,NotOverWrittenList);

            deleteLater();



}



/*
void Thread::stop()
{

    stopped = true;

}
*/

int Thread::showSize()
{
    Size=0;

    QString StrIndex;
    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    bool ok;

    StrIndex=List.last();

    Index=dirModel->index(StrIndex);
    if (!Index.isValid()) return 0;

    for(int i=0;i<(List.size())-1;i++)
    {
         QModelIndex indexTemp=dirModel->index(List[i]);
         if (!indexTemp.isValid()) continue;
        std::cerr<<"AT Thread::showSize() selected item "<<qPrintable(List.at(0))<<std::endl;
        copyList->push_back(  indexTemp );

    }

    std::cerr <<"In Showsize CopyList size=\t"<<copyList->size()<<std::endl;
    std::cerr <<"In Showsize CopyList[0]=\t"<<qPrintable( List[0])<<std::endl;

     if (copyList->isEmpty()) return false;

     ok=getSize(Index,copyList);

     std::cerr <<"In ShowSize Size= \t"<<Size<<std::endl;


      if(!ok) std::cerr <<"getSize Failed\n"<<std::endl;

    return Size;
}


bool Thread::fileaction(QString ActionStr, QString Source, QStringList List)
{
    QModelIndex index;
    QString StrIndex;

    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    QModelIndexList *moveList=new QModelIndexList;

     bool ok;

     Watcher=new QFileSystemWatcher;
     QRegExp re=QRegExp("[^/]+$");


     QString string1=Source;
//     string1.remove(re);

     QString string2=List.at(0);
     string2.remove(re);

     QString string3=List.last();
//     string3.remove(re);



     std::cerr <<"fileaction string1=\t"<<qPrintable(string1)<<std::endl;
     std::cerr <<"fileaction string2=\t"<<qPrintable(string2)<<std::endl;
     std::cerr <<"fileaction string3=\t"<<qPrintable(string3)<<std::endl;

     count=0;
      Size=0;


    if(ActionStr=="delete")
    {
        QModelIndex SourceIndex=dirModel->index(Source);

        for(int i=0;i<(List.size())-1;i++)
        {
            std::cerr <<"Copy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



             QModelIndex indexTemp=dirModel->index(List[i]);
              if (!indexTemp.isValid())      continue;// To avoid cp -rf *

            copyList->push_back(  indexTemp );
            std::cerr <<"Copy List size=\t"<<copyList->size()<<std::endl;
            std::cerr <<"Copy List[0]=\t"<<qPrintable( List[0])<<std::endl;


        }

        if (copyList->isEmpty())
        {
            std::cerr<< "invalid copyList in copy"<<std::endl;
            return false;   // To avoid cp -rf *
        }


        ok=del(SourceIndex,copyList);
         if(!ok) std::cerr <<"Remove Failed\n"<<std::endl;


        Watcher->removePath(string2);

        Watcher->deleteLater();
        return true;

    }
    else if(ActionStr=="copy")
    {

        ok=ActionCopy(Source,List);
        Watcher->deleteLater();
        return ok;

    }
    else if (ActionStr=="cut")
    {
        ok=ActionMove(Source,List);
        Watcher->deleteLater();
        return ok;
    }
     else if (ActionStr=="NotOverWrittenUndoMove")
    {
        ok=ActionNotOverWrittenUndoMove(Source,List);
        Watcher->deleteLater();
        return ok;

    }
    else if (ActionStr=="NotOverWrittenUndoCopy")
   {
        ok=ActionNotOverWrittenUndoCopy(Source,List);
        Watcher->deleteLater();
        return ok;



    }
    else if (ActionStr=="RedoMove")
    {
        NoToAll=true;
        ok=ActionMove(Source,List);
        Watcher->deleteLater();
        return ok;
    }
    else if (ActionStr=="RedoCopy")
    {
        NoToAll=true;
        ok=ActionCopy(Source,List);
        Watcher->deleteLater();
        return ok;
    }
}


bool Thread::copy(QModelIndex targetIndex, QModelIndexList *sourceCopyList)
{
    Answer="";
    emit CloseOverWriteDialog(true);
    QModelIndex newTargetIndex;

    dirModel->setReadOnly(false);

    QString string2=dirModel->filePath(targetIndex);
//    Watcher->addPath(string2);

    bool ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::CopyAction);
    if(!ok)
    {


        return 1;
    }




    if(!(sourceCopyList->isEmpty()))
    {


        for (int i=0;i<sourceCopyList->size();i++)
        {


            if (this->isInterruptionRequested())
            {

                std::cerr <<"Copy Failed in subroutine copy"<<std::endl;

                return false;
            }

/*            if (stopped)
            {

                std::cerr <<"Copy Failed in subroutine copy"<<std::endl;

                return false;
            }

*/



            QModelIndex index=sourceCopyList->at(i);


            if (!index.isValid())
            {
                std::cerr <<"Invalid index in copy"<<std::endl;

                return false;
            }
            QString string1=dirModel->filePath(index);
//            Watcher->addPath(string1);



            if (dirModel->fileInfo(index).isDir())
            {


                QString dirName;
                dirName=dirModel->fileName(index);


                QString dirTarget=dirModel->filePath(targetIndex);
                QString dirTargetTemp=dirTarget;
                dirTargetTemp=dirTargetTemp.append("/").append(dirName);
                dirTargetTemp.replace("//","/");
                QModelIndex dirTargetIndex=dirModel->index(dirTargetTemp);


                std::cerr <<"in isDir Copy\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);
                Watcher->addPath(dirTarget);


                if (!(dirModel->fileInfo(dirTargetIndex).exists()))
                {
                    std::cerr <<"dirTargetIndex exists"<<qPrintable(dirTargetTemp)<<std::endl;

                    newTargetIndex= dirModel->mkdir(targetIndex,dirName);

                     count++;



                     emit valueChanged(count);


                 }
                else
                {
                     std::cerr<<"in else dir exists"<<std::endl;


                    newTargetIndex=dirTargetIndex;
                }

                QString newTargetIndexPath=dirModel->filePath(newTargetIndex);

                if (!newTargetIndex.isValid())
                   {

//                      std::cerr <<"Invalid newTargetIndex\t"<<qPrintable(newTargetIndex.row())<<std::endl;
                      std::cerr <<"newTargetIndexPath is not valid\t"<<qPrintable(newTargetIndexPath)<<std::endl;

                   }
                else
                {
                    std::cerr <<"newTargetIndexPath is valid\t"<<qPrintable(newTargetIndexPath)<<std::endl;
                }

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;

//                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

                for(int j=0;j<dirModel2->rowCount(dirModel2->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel2->index(j,0,dirModel2->index(path));
                    const QString path3=dirModel2->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);
                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {
                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");
                }
                    std::cerr <<"list in Copy\n"<<qPrintable(str1)<<std::endl;

                ok=copy(newTargetIndex,newSourceList);
                if (!ok)
                {

                    std::cerr <<"Copy  Failed(returned with failure)\n"<<std::endl;
                    return false;

                }

            }
            else
            {

                    Answer="";

                    QModelIndex indexTemp=sourceCopyList->at(i);
                    QString path2=dirModel->filePath(indexTemp);

                    QString TargetTemp=dirModel->fileName(indexTemp);
                    QString TargetDir=dirModel->filePath(targetIndex);
                    QString TargetDirTemp=TargetDir;
                    TargetTemp=TargetDirTemp.append("/").append(TargetTemp);
                    TargetTemp=TargetTemp.replace("//","/");

                    std::cerr <<"parameter i=\t"<<i<<std::endl;
                    std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
                    std::cerr <<"list Target Temp\t"<<qPrintable(TargetTemp)<<std::endl;
                    std::cerr <<"list path2\t"<<qPrintable(path2)<<std::endl;

                    QModelIndex targetIndexTemp=dirModel->index(TargetTemp);
                    targetIndexTempSet=targetIndexTemp;

                    NotOverWritten=TargetTemp;//takes all files but gets appended to the List only of answer is No or NoToAll



                    if (dirModel->fileInfo(targetIndexTemp).exists())
                    {

//                        std::cerr <<"In block if"<<std::endl;
                        Answer="";



                        if (YesToAll)
                        {
                            dirModel->remove(targetIndexTemp);

                            QString path2=dirModel->filePath(index);
                            QModelIndex indexTemp=dirModel->index(path2);

                            QString TargetDir=dirModel->filePath(targetIndex);
                            QModelIndexList *tempList;
                            tempList=new QModelIndexList;
                             tempList->push_back(indexTemp);

                             std::cerr <<"Copying\t"<<qPrintable( path2)<<std::endl;
                             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;

                             if((indexTemp.isValid())&&(targetIndex.isValid()))
                             {
                               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,targetIndex);
                             }
                             else
                             {
                                 ok=false;
                             }


                               if (!ok)
                               {
                                   std::cerr <<"Copy Failed"<<std::endl;
                               }
                               else
                               {
                                   std::cerr<<"Copy in YesToAll ok"<<std::endl;
                               }

                                count++;



                              emit valueChanged(count);
                                emit CloseOverWriteDialog(true);
                               Answer="";

                        }
                         else if (NoToAll)
                        {
                            NotOverWrittenList.append(NotOverWritten);
                            std::cerr <<"In NoToAll:\tNot Copying file\t "<<qPrintable( path2)<<std::endl;

                           Answer="";
                            emit CloseOverWriteDialog(true);
                            continue;

                        }
                        else
                        {

//                             std::cerr <<"In block if in else"<<std::endl;


                            Answer="";



                            emit fileexists(path2,TargetTemp);





//                             std::cerr <<"Copying path2 \t"<<qPrintable( path2)<<std::endl;
//                             std::cerr <<"to TargetTemp \t"<<qPrintable(TargetTemp)<<std::endl;


                            global_mutex.lock();


                            std::cerr <<"Answer \t"<<qPrintable(Answer)<<std::endl;

                             if(Answer=="")
                             {


                                global_var_not_set.wait(&global_mutex);
                             }

//                             std::cerr <<"Answer \t"<<qPrintable(Answer)<<std::endl;

                             TargetDirSetIndex=targetIndex;
                             pathSetIndex=index;
                             emit CloseOverWriteDialog(false);

                            ok= ImplementAnswerInCopy();
                            if (!ok)
                            {
                                std::cerr <<"Implement_Answer Failed"<<std::endl;
                            }
                            else
                            {
                                std::cerr<<"Implement_Answer ok"<<std::endl;
                            }
                            Answer="";

                             global_var_set.wakeOne();
                             global_mutex.unlock();



                        }

                    }


                    else
                    {
                        QModelIndexList *tempList;
                        tempList=new QModelIndexList;
                         QString TargetDir=dirModel->filePath(targetIndex);
                          tempList->push_back(indexTemp);

                          std::cerr <<"IN ELSE Copying\t"<<qPrintable( path2)<<std::endl;
                          std::cerr <<"IN ELSE list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;

                          if((indexTemp.isValid())&&(targetIndex.isValid()))
                          {
                            ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,targetIndex);
                           }
                          else
                          {
                              ok=false;
                          }

                            if (!ok)
                            {
                                std::cerr <<"Copy Failed"<<std::endl;
                            }
                            else
                            {
                                std::cerr<<"Copy ok in else"<<std::endl;
                            }

                             count++;



                           emit valueChanged(count);

                             emit CloseOverWriteDialog(true);
                             Answer="";
                    }






            }

//            Watcher->removePath(string1);


        }

   }
//    Watcher->removePath(string2);

    return true;
}



bool Thread::move(QModelIndex targetIndex, QModelIndexList *sourceMoveList)
{
    Answer="";
    emit CloseOverWriteDialog(true);
    QModelIndex newTargetIndex;

    dirModel->setReadOnly(false);

    QString string2=dirModel->filePath(targetIndex);
//    Watcher->addPath(string2);


    bool ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::MoveAction);
    if(!ok)
    {


        return 1;
    }




    if(!(sourceMoveList->isEmpty()))
    {


        for (int i=0;i<sourceMoveList->size();i++)
        {


            if (this->isInterruptionRequested())
            {


                 std::cerr <<"Move Failed in subroutine move"<<std::endl;

                return false;
            }

/*
            if (stopped)
            {

                std::cerr <<"Move Failed in subroutine move"<<std::endl;

                return false;
            }

*/
            QModelIndex index=sourceMoveList->at(i);


            if (!index.isValid())
            {
                std::cerr <<"Invalid index in move"<<std::endl;

                 return false;
            }
//            QString string1=dirModel->filePath(index);
//            Watcher->addPath(string1);




            if (dirModel->fileInfo(index).isDir())
            {


                QString dirName;
                dirName=dirModel->fileName(index);


                QString dirTarget=dirModel->filePath(targetIndex);
                QString dirTargetTemp=dirTarget;
                dirTargetTemp=dirTargetTemp.append("/").append(dirName);
                dirTargetTemp.replace("//","/");
                QModelIndex dirTargetIndex=dirModel->index(dirTargetTemp);


                std::cerr <<"in isDir Move\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);

                Watcher->addPath(dirTarget);
                if (!(dirModel->fileInfo(dirTargetIndex).exists()))
                {
                    std::cerr <<"dirTargetIndex exists"<<qPrintable(dirTargetTemp)<<std::endl;

                    newTargetIndex= dirModel->mkdir(targetIndex,dirName);

                     count++;



                     emit valueChanged(count);


                 }
                else
                {
                    std::cerr<<"in else dir exists"<<std::endl;


                    newTargetIndex=dirTargetIndex;
                }

                QString newTargetIndexPath=dirModel->filePath(newTargetIndex);

                if (!newTargetIndex.isValid())
                   {

//                      std::cerr <<"Invalid newTargetIndex\t"<<qPrintable(newTargetIndex.row())<<std::endl;
                      std::cerr <<"newTargetIndexPath is not valid\t"<<qPrintable(newTargetIndexPath)<<std::endl;

                   }
                else
                  {
                    std::cerr <<"newTargetIndexPath is valid\t"<<qPrintable(newTargetIndexPath)<<std::endl;
                  }

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;

//                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

                for(int j=0;j<dirModel2->rowCount(dirModel2->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel2->index(j,0,dirModel2->index(path));
                    const QString path3=dirModel2->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);
                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {
                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");
                }
                    std::cerr <<"list in Move\n"<<qPrintable(str1)<<std::endl;

                ok=move(newTargetIndex,newSourceList);
                if (!ok)
                {

                    std::cerr <<"Move  Failed (returned with failure)\n"<<std::endl;
                    return false;

                }

            }
            else
            {

                    Answer="";

                    QModelIndex indexTemp=sourceMoveList->at(i);
                    QString path2=dirModel->filePath(indexTemp);

                    QString TargetTemp=dirModel->fileName(indexTemp);
                    QString TargetDir=dirModel->filePath(targetIndex);
                    QString TargetDirTemp=TargetDir;
                    TargetTemp=TargetDirTemp.append("/").append(TargetTemp);
                    TargetTemp=TargetTemp.replace("//","/");

                    std::cerr <<"parameter i=\t"<<i<<std::endl;
                    std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
                    std::cerr <<"list Target Temp\t"<<qPrintable(TargetTemp)<<std::endl;
                    std::cerr <<"list path2\t"<<qPrintable(path2)<<std::endl;

                    QModelIndex targetIndexTemp=dirModel->index(TargetTemp);
                    targetIndexTempSet=targetIndexTemp;
                    NotOverWritten=TargetTemp;//takes all files but gets appended to the List only of answer is No or NoToAll




                    if (dirModel->fileInfo(targetIndexTemp).exists())
                    {

//                        std::cerr <<"In block if"<<std::endl;
                        Answer="";



                        if (YesToAll)
                        {
                            dirModel->remove(targetIndexTemp);

                            QString path2=dirModel->filePath(index);
                            QModelIndex indexTemp=dirModel->index(path2);

                            QString TargetDir=dirModel->filePath(targetIndex);
                            QModelIndexList *tempList;
                            tempList=new QModelIndexList;
                             tempList->push_back(indexTemp);

                             std::cerr <<"Moving\t"<<qPrintable( path2)<<std::endl;
                             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;

                             if ((indexTemp.isValid())&&(targetIndex.isValid()))
                             {
                               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::MoveAction,-1,-1,targetIndex);
                              }
                             else
                             {
                                 ok=false;
                             }

                               if (!ok)
                               {
                                   std::cerr <<"Move Failed"<<std::endl;
                               }
                               else
                               {
                                   std::cerr<<"Move in YesToAll ok"<<std::endl;
                               }

                                count++;



                              emit valueChanged(count);
                                emit CloseOverWriteDialog(true);
                               Answer="";

                        }
                         else if (NoToAll)
                        {
                            NotOverWrittenList.append(NotOverWritten);
                            std::cerr <<"In NoToAll:\tNot Moving file\t "<<qPrintable( path2)<<std::endl;

                           Answer="";
                            emit CloseOverWriteDialog(true);
                            continue;

                        }
                        else
                        {

//                             std::cerr <<"In block if in else"<<std::endl;


                            Answer="";



                            emit fileexists(path2,TargetTemp);





//                             std::cerr <<"Moving path2 \t"<<qPrintable( path2)<<std::endl;
//                             std::cerr <<"to TargetTemp \t"<<qPrintable(TargetTemp)<<std::endl;


                            global_mutex.lock();


                            std::cerr <<"Answer \t"<<qPrintable(Answer)<<std::endl;

                             if(Answer=="")
                             {


                                global_var_not_set.wait(&global_mutex);
                             }

//                             std::cerr <<"Answer \t"<<qPrintable(Answer)<<std::endl;

                             TargetDirSetIndex=targetIndex;
                             pathSetIndex=index;
                             emit CloseOverWriteDialog(false);

                            ok= ImplementAnswerInMove();
                            if (!ok)
                            {
                                std::cerr <<"ImplementAnswerInMove Failed"<<std::endl;
                            }
                            else
                            {
                                std::cerr<<"ImplementAnswerInMove ok"<<std::endl;
                            }
                            Answer="";

                             global_var_set.wakeOne();
                             global_mutex.unlock();



                        }

                    }


                    else
                    {
                        QModelIndexList *tempList;
                        tempList=new QModelIndexList;
                         QString TargetDir=dirModel->filePath(targetIndex);
                          tempList->push_back(indexTemp);

                          std::cerr <<"IN ELSE Moving\t"<<qPrintable( path2)<<std::endl;
                          std::cerr <<"IN ELSE list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;

                          if((indexTemp.isValid())&&(targetIndex.isValid()))
                          {
                            ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::MoveAction,-1,-1,targetIndex);
                           }
                          else
                          {
                              ok=false;
                          }

                            if (!ok)
                            {
                                std::cerr <<"Moving Failed"<<std::endl;
                            }
                            else
                            {
                                std::cerr<<"Move ok in else"<<std::endl;
                            }

                             count++;



                           emit valueChanged(count);

                             emit CloseOverWriteDialog(true);
                             Answer="";
                    }






            }
//            Watcher->removePath(string1);



        }

    }

//    Watcher->removePath(string2);

    return true;
}






bool Thread::getSize(QModelIndex targetIndex, QModelIndexList *sourceCopyList)
{
    count=0;


    bool ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::CopyAction);
    if(!ok)
    {

        return 0;
    }


    if(!(sourceCopyList->isEmpty()))
    {
        for (int i=0;i<sourceCopyList->size();i++)
        {
            QModelIndex index=sourceCopyList->at(i);


            if (!index.isValid())
            {
//                std::cerr <<"Invalid index\n"<<qPrintable(index.row())<<std::endl;

                return false;
            }




            if (dirModel->fileInfo(index).isDir())


            {
                Size++;

                QString dirName;
                dirName=dirModel->fileName(index);
                std::cerr <<"in isDir GetSize\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);



                QModelIndex newTargetIndex= dirModel->index(path);


                if (!newTargetIndex.isValid())
                {
//                        std::cerr <<"Invalid newTargetIndex"<<qPrintable(newTargetIndex.row())<<std::endl;

                }

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;

//                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

                for(int j=0;j<dirModel2->rowCount(dirModel2->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel2->index(j,0,dirModel2->index(path));
                    const QString path3=dirModel2->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);

                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {

                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");

                }



                getSize(newTargetIndex,newSourceList);

            }
            else
            {

                    QModelIndexList *tempList;
                    tempList=new QModelIndexList;
                    QString path2=dirModel->filePath(index);
                    QModelIndex indexTemp=dirModel->index(path2);
                    tempList->push_back(indexTemp);
                    std::cerr <<"Counting\t"<<qPrintable( path2)<<"\tin GetSize"<<std::endl;
                    Size++;


            }


        }

   }
    else
   {
            std::cerr<< "invalid copyList in getSize"<<std::endl;
            return false;   // To avoid cp -rf *
    }

    std::cerr <<"In getSize Size= \t"<<Size<<std::endl;
    return true;
}

void Thread::set_Answer(QString answer)
{
//    std::cerr<<"In thread before set_Answer Answer= "<<qPrintable(Answer)<<std::endl;


      Answer=answer;
      std::cerr<<"In thread set_Answer Answer= "<<qPrintable(Answer)<<std::endl;
      if (Answer=="YesToAll") YesToAll=true;
      if (Answer=="NoToAll") NoToAll=true;




}

QString Thread::getAnswer()
{
    return Answer;
}

bool Thread::ImplementAnswerInCopy()
{
    bool ok;
    dirModel->setReadOnly(false);

    std::cerr <<"Implement_Answer\t"<<std::endl;

    ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::CopyAction);
    if(!ok)
    {
            std::cerr <<"Not supportedDropActions\t"<<std::endl;

        return ok;
    }

    QModelIndexList *tempList;

    QString pathSet=dirModel->filePath(pathSetIndex);
    QString TargetDirSet=dirModel->filePath(TargetDirSetIndex);

    if (!pathSetIndex.isValid())
    {
        std::cerr <<"Not Valid PathSet\t"<<qPrintable(pathSet)<<std::endl;
        return false;
    }
    if (!TargetDirSetIndex.isValid())
    {
        std::cerr <<"Not Valid TargetDirSet\t"<<qPrintable(TargetDirSet)<<std::endl;
        return false;
    }

    QString path2=dirModel->filePath(pathSetIndex);


    if(getAnswer()!="")
    {
        std::cerr <<"In loop getanswer check"<<std::endl;



        if (getAnswer()=="YesToAll")
        {
             dirModel->remove(targetIndexTempSet);



            QString TargetDir=dirModel->filePath(TargetDirSetIndex);
            tempList=new QModelIndexList;

             tempList->push_back(pathSetIndex);
             std::cerr <<"Copying\t"<<qPrintable( path2)<<std::endl;
             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,TargetDirSetIndex);


               if (!ok)
               {
                   std::cerr <<"Copy Failed"<<std::endl;
               }
               else
               {
                   std::cerr<<"Copy ok in YesToAll"<<std::endl;
               }

                count++;



              emit valueChanged(count);
                emit CloseOverWriteDialog(true);
               Answer="";
               return ok;


        }
        if (getAnswer()=="NoToAll")
        {
            NotOverWrittenList.append(NotOverWritten);

            std::cerr <<"In NoToAll:\tNot Copying file\t "<<qPrintable( path2)<<std::endl;

            emit CloseOverWriteDialog(true);
           Answer="";
           return true;


        }
        if(getAnswer()=="Yes")
        {
            dirModel->remove(targetIndexTempSet);



            QString TargetDir=dirModel->filePath(TargetDirSetIndex);
            tempList=new QModelIndexList;

             tempList->push_back(pathSetIndex);
             std::cerr <<"Copying\t"<<qPrintable( path2)<<std::endl;
             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,TargetDirSetIndex);


                if (!ok)
                {
                    std::cerr <<"Copy Failed"<<std::endl;
                }
                else
                {
                    std::cerr<<"Copy ok in Yes"<<std::endl;
                }

                count++;



              emit valueChanged(count);


               emit CloseOverWriteDialog(true);
                   Answer="";

                   return ok;
            }

             if(Answer=="No")
               {
                 NotOverWrittenList.append(NotOverWritten);


                 std::cerr <<"In NO:\tNot Copying file\t "<<qPrintable( path2)<<std::endl;

                    emit CloseOverWriteDialog(true);
                  Answer="";
                  return true;



            }


    }
    else
    {
        return false;
    }

}


bool Thread::ImplementAnswerInMove()
{
    bool ok;
    dirModel->setReadOnly(false);

    std::cerr <<"Implement_Answer\t"<<std::endl;

    ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::MoveAction);
    if(!ok)
    {
            std::cerr <<"Not supportedDropActions\t"<<std::endl;

        return ok;
    }

    QModelIndexList *tempList;

    QString pathSet=dirModel->filePath(pathSetIndex);
    QString TargetDirSet=dirModel->filePath(TargetDirSetIndex);

    if (!pathSetIndex.isValid())
    {
        std::cerr <<"Not Valid PathSet\t"<<qPrintable(pathSet)<<std::endl;
        return false;
    }
    if (!TargetDirSetIndex.isValid())
    {
        std::cerr <<"Not Valid TargetDirSet\t"<<qPrintable(TargetDirSet)<<std::endl;
        return false;
    }

    QString path2=dirModel->filePath(pathSetIndex);


    if(getAnswer()!="")
    {
        std::cerr <<"In loop getanswer check"<<std::endl;



        if (getAnswer()=="YesToAll")
        {
             dirModel->remove(targetIndexTempSet);



            QString TargetDir=dirModel->filePath(TargetDirSetIndex);
            tempList=new QModelIndexList;

             tempList->push_back(pathSetIndex);
             std::cerr <<"Moving\t"<<qPrintable( path2)<<std::endl;
             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::MoveAction,-1,-1,TargetDirSetIndex);


               if (!ok)
               {
                   std::cerr <<"Move Failed"<<std::endl;
               }
               else
               {
                   std::cerr<<"Move ok in YesToAll"<<std::endl;
               }

                count++;



              emit valueChanged(count);
                emit CloseOverWriteDialog(true);
               Answer="";
               return ok;


        }
        if (getAnswer()=="NoToAll")
        {
            NotOverWrittenList.append(NotOverWritten);


            std::cerr <<"In NoToAll:\tNot Moving file\t "<<qPrintable( path2)<<std::endl;

            emit CloseOverWriteDialog(true);
           Answer="";
           return true;


        }
        if(getAnswer()=="Yes")
        {
            dirModel->remove(targetIndexTempSet);



            QString TargetDir=dirModel->filePath(TargetDirSetIndex);
            tempList=new QModelIndexList;

             tempList->push_back(pathSetIndex);
             std::cerr <<"Moving\t"<<qPrintable( path2)<<std::endl;
             std::cerr <<"list Target Index Dir\t"<<qPrintable(TargetDir)<<std::endl;
               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::MoveAction,-1,-1,TargetDirSetIndex);


                if (!ok)
                {
                    std::cerr <<"Move Failed"<<std::endl;
                }
                else
                {
                    std::cerr<<"Move ok in Yes"<<std::endl;
                }

                count++;



              emit valueChanged(count);


               emit CloseOverWriteDialog(true);
                   Answer="";

                   return ok;
            }

             if(Answer=="No")
               {
                 NotOverWrittenList.append(NotOverWritten);

                 std::cerr <<"In NO:\tNot Moving file\t "<<qPrintable( path2)<<std::endl;

                    emit CloseOverWriteDialog(true);
                  Answer="";
                  return true;



            }


    }
    else
    {
        return false;
    }

}

bool Thread::DeleteEmptyDirs(QModelIndex& SourceIndex, QModelIndexList *SourceDeleteList)
{
    int count1=0;


     QDirModel *dirModel3;
     dirModel3=new QDirModel;
    count=0;
     const QString path4=dirModel->filePath(SourceIndex);

    bool ok=false;



    if(!(SourceDeleteList->isEmpty()))
    {
        for (int i=0;i<SourceDeleteList->size();i++)
        {
            QModelIndex index=SourceDeleteList->at(i);


            if (!index.isValid())
            {
//                std::cerr <<"Invalid index\n"<<qPrintable(index.row())<<std::endl;

                return false;
            }




            if (dirModel->fileInfo(index).isDir())


            {
                const QString path=dirModel->filePath(index);


                Size++;

                QString dirName;
                dirName=dirModel->fileName(index);



                std::cerr <<"in isDir DeleteEmptyDirs\t Dir\t "<<qPrintable(dirName)<<std::endl;

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;


//                std::cerr <<"rows: \t"<<qPrintable(dirModel3->rowCount(dirModel3->index(path)))<<std::endl;


                for(int j=0;j<dirModel3->rowCount(dirModel3->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel3->index(j,0,dirModel3->index(path));
                    const QString path3=dirModel3->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);

                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {

                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");

                }



                if (newSourceList->isEmpty())
                {
                   if (index.isValid())
                   {
                       dirModel->remove(index);
                        std::cerr <<"Remove Dir \t"<<qPrintable(path)<<std::endl;
                        count1++;
                   }
                     continue;
                }
                else
                {
                    ok=false;

                    ok=DeleteEmptyDirs(index,newSourceList);

                    QString dirName3;
                    dirName3=dirModel->filePath(index);

                    if (ok)
                    {

                        if(index.isValid())
                        {
                            dirModel->remove(index);
                            count1++;

                              std::cerr <<"in ok Remove Dir \t"<<qPrintable(dirName3)<<std::endl;
                        }
                    }
                    else
                    {


                    std::cerr <<"in else in ok Dir \t"<<qPrintable(dirName3)<<"\tNot Empty"<<std::endl;
                    }


                }


            }
            else
            {
                std::cerr<< "There are also simple files. Not only Dirs \t"<<qPrintable(path4)<<std::endl;


            }


        }

   }
    else
   {
            std::cerr<< "invalid copyList in DeleteEmptyDirs"<<std::endl;
            return true;  //empty
    }

    if(count1==SourceDeleteList->size())
    {

        return true; //empty
    }
    else
    {


        return false; //not empty

    }


}

bool Thread::NotOverWrittenDirStructure(QModelIndex targetIndex, QModelIndexList *sourceCopyList)
{

    QModelIndex newTargetIndex;

    dirModel->setReadOnly(false);

    std::cerr <<"in subroutine NotOverWrittenDirStructure"<<std::endl;


    bool ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::CopyAction);
    if(!ok)
    {

        return 1;
    }




    if(!(sourceCopyList->isEmpty()))
    {


        for (int i=0;i<sourceCopyList->size();i++)
        {


            if (this->isInterruptionRequested())
            {

                std::cerr <<"Create Dirs Failed in subroutine NotOverWrittenDirStructure"<<std::endl;

                return false;
            }

/*            if (stopped)
            {

                std::cerr <<"Create Dirs Failed in subroutine NotOverWrittenDirStructure"<<std::endl;

                return false;
            }

*/



            QModelIndex index=sourceCopyList->at(i);


            if (!index.isValid())
            {
//                std::cerr <<"Invalid index in NotOverWrittenDirStructure at sourceCopyList("<<qPrintable(i)<<")"<<std::endl;

                return 0;
            }




            if (dirModel->fileInfo(index).isDir())
            {


                QString dirName;
                dirName=dirModel->fileName(index);


                QString dirTarget=dirModel->filePath(targetIndex);
                QString dirTargetTemp=dirTarget;
                std::cerr <<"dirTarget in  NotOverWrittenDirStructure\t "<<qPrintable(dirTarget)<<std::endl;


                dirTargetTemp=dirTargetTemp.append("/").append(dirName);
                dirTargetTemp.replace("//","/");
                QModelIndex dirTargetIndex=dirModel->index(dirTargetTemp);


                std::cerr <<"in isDir NotOverWrittenDirStructure\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);




                    newTargetIndex=dirModel->mkdir(targetIndex,dirName);
                    if((newTargetIndex.isValid()))
                    {

                        std::cerr <<"dirTargetIndex created in  NotOverWrittenDirStructure\t "<<qPrintable(dirTargetTemp)<<std::endl;
                        count++;
                        emit valueChanged(count);

                    }
                    else
                    {
                        std::cerr <<"dirTargetIndex creation failed in  NotOverWrittenDirStructure\t "<<qPrintable(dirTargetTemp)<<std::endl;

                    }


                    QModelIndexList *newSourceList;

                    newSourceList=new QModelIndexList;

//                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

                for(int j=0;j<dirModel2->rowCount(dirModel2->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel2->index(j,0,dirModel2->index(path));
                    const QString path3=dirModel2->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);
                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {
                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");
                }
                    std::cerr <<"list in NotOverWrittenDirStructure\n"<<qPrintable(str1)<<std::endl;

                ok=NotOverWrittenDirStructure(newTargetIndex,newSourceList);
                if (!ok)
                {

                    std::cerr <<"NotOverWrittenDirStructure Failed(returned with failure)\n"<<std::endl;
                    return false;

                }

            }


        }

   }

    return true;
}


bool Thread::NotOverWrittenMove(QModelIndex targetIndex, QModelIndexList *sourceCopyList)
{

    QString TargetPath1=dirModel->filePath(targetIndex);
    dirModel->setReadOnly(false);
//    Watcher->addPath(TargetPath1);



    bool ok=false;
    ok=dirModel->supportedDropActions().testFlag(Qt::CopyAction);
    if(!ok)
    {

        return 1;
    }




    if(!(sourceCopyList->isEmpty()))
    {


        for (int i=0;i<sourceCopyList->size();i++)
        {


            if (this->isInterruptionRequested())
            {

                std::cerr <<"Move Failed in subroutine NotOverWrittenMove"<<std::endl;

                return false;
            }





            QModelIndex index=sourceCopyList->at(i);


            if (!index.isValid())
            {
//                std::cerr <<"Invalid index"<<qPrintable(index.row())<<std::endl;

                continue;
            }



            QModelIndexList* tempList;
            tempList=new QModelIndexList;
            tempList->append(index);

            QString path1=dirModel->filePath(index);


            QString str=path1;
            QString path2;


            QRegExp re=QRegExp("[^/]+$");
            str.remove(re);
            path2=str;
            QString TargetDir=dirModel->filePath(targetIndex);


            path2=path2.replace(Source,TargetDir+"/");
            path2=path2.replace("//","/");//Needs a / in order to work correctly
            QModelIndex newTargetIndex=dirModel->index(path2);



            if ((index.isValid())&&(targetIndex.isValid()))
            {

//               Watcher->addPath(path1);
//               Watcher->addPath(path2);

               ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::MoveAction,-1,-1,newTargetIndex);

//               Watcher->removePath(path1);
//               Watcher->removePath(path2);
             }
            else
            {
                ok=false;
            }

              if (!ok)
              {
                  std::cerr <<"Move Failed in NotOverWrittenMove"<<std::endl;

              }
              else
              {
                  std::cerr<<"Move in NotOverWrittenMove ok"<<std::endl;

              }

               count++;




        }

//        Watcher->removePath(TargetPath1);
        return true;
    }
    else
    {

//        Watcher->removePath(TargetPath1);
        return false;
    }
}
bool Thread::ActionMove(QString Source,  QStringList list)
{

    QModelIndex index;
    QString StrIndex;

    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    QModelIndexList *moveList=new QModelIndexList;

     bool ok;


     QRegExp re=QRegExp("[^/]+$");


     QString string1=Source;
//     string1.remove(re);

     QString string2=List.at(0);
     string2.remove(re);

     QString string3=List.last();
//     string3.remove(re);



     std::cerr <<"fileaction string1=\t"<<qPrintable(string1)<<std::endl;
     std::cerr <<"fileaction string2=\t"<<qPrintable(string2)<<std::endl;
     std::cerr <<"fileaction string3=\t"<<qPrintable(string3)<<std::endl;
    std::cerr <<"List size as send=\t"<<List.size()<<std::endl;
    std::cerr <<"Move List[0]=\t"<<qPrintable( List[0])<<std::endl;
    std::cerr <<"Move List[1]=\t"<<qPrintable( List[1])<<std::endl;
    dirModel->setReadOnly(false);


    StrIndex=List.last();

    Index=dirModel->index(StrIndex);



           for(int i=0;i<(List.size())-1;i++)
           {
               std::cerr <<"Move\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



                QModelIndex indexTemp=dirModel->index(List[i]);
                 if (!indexTemp.isValid())      continue;// To avoid mv -rf *

               moveList->push_back(  indexTemp );
               std::cerr <<"Move List size=\t"<<moveList->size()<<std::endl;
               std::cerr <<"Move List[0]=\t"<<qPrintable( List[0])<<std::endl;


           }

           if (moveList->isEmpty())
           {
               std::cerr<< "invalid moveList in move"<<std::endl;
               return false;   // To avoid mv -rf *
           }

           Watcher->addPath(string1);
           Watcher->addPath(string3);

           ok=move(Index,moveList);

            if(!ok)
            {
                std::cerr <<"Move Failed\n"<<std::endl;
                return false;
            }

                QModelIndex SourceIndex=dirModel->index(Source);
                ok=DeleteEmptyDirs(SourceIndex,moveList);

                if(!ok)
                {
                    std::cerr <<"Some Source directories are not deleted"<<std::endl;

                }
                Watcher->removePath(string1);

                Watcher->removePath(string3);

            return true;
}


bool Thread::ActionCopy(QString Source,  QStringList list)
{
    QModelIndex index;
    QString StrIndex;

    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    QModelIndexList *moveList=new QModelIndexList;

     bool ok;


     QRegExp re=QRegExp("[^/]+$");


     QString string1=Source;
//     string1.remove(re);

     QString string2=List.at(0);
     string2.remove(re);

     QString string3=List.last();
//     string3.remove(re);



     std::cerr <<"fileaction string1=\t"<<qPrintable(string1)<<std::endl;
     std::cerr <<"fileaction string2=\t"<<qPrintable(string2)<<std::endl;
     std::cerr <<"fileaction string3=\t"<<qPrintable(string3)<<std::endl;
    std::cerr <<"List size as send=\t"<<List.size()<<std::endl;
    std::cerr <<"Move List[0]=\t"<<qPrintable( List[0])<<std::endl;
    std::cerr <<"Move List[1]=\t"<<qPrintable( List[1])<<std::endl;
    dirModel->setReadOnly(false);


    StrIndex=List.last();

    Index=dirModel->index(StrIndex);



           for(int i=0;i<(List.size())-1;i++)
           {
               std::cerr <<"Copy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



                QModelIndex indexTemp=dirModel->index(List[i]);
                 if (!indexTemp.isValid())      continue;// To avoid cp -rf *

               copyList->push_back(  indexTemp );
               std::cerr <<"Copy List size=\t"<<copyList->size()<<std::endl;
               std::cerr <<"Copy List[0]=\t"<<qPrintable( List[0])<<std::endl;


           }

           if (copyList->isEmpty())
           {
               std::cerr<< "invalid copyList in copy"<<std::endl;
               return false;   // To avoid cp -rf *
           }


           Watcher->addPath(string1);
           Watcher->addPath(string3);

           ok=copy(Index,copyList);

            if(!ok)
            {
                std::cerr <<"Copy Failed in fileaction\n"<<std::endl;
                return false;
            }
            Watcher->removePath(string1);

            Watcher->removePath(string3);

            return true;
}



bool Thread::del(QModelIndex& SourceIndex, QModelIndexList *SourceDeleteList)
{
    int count1=0;


     QDirModel *dirModel3;
     dirModel3=new QDirModel;
    count=0;
     const QString path4=dirModel->filePath(SourceIndex);

    bool ok=false;



    if(!(SourceDeleteList->isEmpty()))
    {
        for (int i=0;i<SourceDeleteList->size();i++)
        {


            if (this->isInterruptionRequested())
            {

                 std::cerr <<"Delete Failed"<<std::endl;

                return false;
            }


            QModelIndex index=SourceDeleteList->at(i);


            if (!index.isValid())
            {
//                std::cerr <<"Invalid index\n"<<qPrintable(index.row())<<std::endl;

                return false;
            }




            if (dirModel->fileInfo(index).isDir())


            {
                const QString path=dirModel->filePath(index);


                Size++;

                QString dirName;
                dirName=dirModel->fileName(index);



                std::cerr <<"in isDir Del\t Dir\t "<<qPrintable(dirName)<<std::endl;



                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;


//                std::cerr <<"rows: \t"<<qPrintable(dirModel3->rowCount(dirModel3->index(path)))<<std::endl;


                for(int j=0;j<dirModel3->rowCount(dirModel3->index(path));j++)
                {


                    QModelIndex sourceNewIndex=dirModel3->index(j,0,dirModel3->index(path));
                    const QString path3=dirModel3->filePath(sourceNewIndex);
                    QModelIndex newSourceIndex=dirModel->index(path3);

                    if (sourceNewIndex.isValid()) newSourceList->push_back(newSourceIndex);

                }

                QString str1;
                for (int k=0;k<newSourceList->size();k++)
                {

                    str1.append( dirModel->fileName(newSourceList->at(k)));
                    str1.append("\n");

                }

                if (newSourceList->isEmpty())
                {
                   if (index.isValid())
                   {
                       dirModel->remove(index);
                        std::cerr <<"Remove Dir \t"<<qPrintable(path)<<std::endl;
                        count1++;
                        count++;
                         emit valueChanged(count);
                   }
                     continue;
                }
                else
                {
                    ok=false;

                    ok=del(index,newSourceList);

                    QString dirName3;
                    dirName3=dirModel->filePath(index);

                    if (ok)
                    {

                        if(index.isValid())
                        {
                            dirModel->remove(index);
                            count1++;
                            count++;
                             emit valueChanged(count);

                              std::cerr <<"in ok Remove Dir \t"<<qPrintable(dirName3)<<std::endl;
                        }
                    }
                    else
                    {


                    std::cerr <<"in else in ok Dir \t"<<qPrintable(dirName3)<<"\tNot Empty"<<std::endl;
                    }


                }


            }
            else
            {
                if(index.isValid())
                {


                    QString IndexString=dirModel->filePath(index);
                    dirModel->remove(index);

                        std::cerr <<"removed\t"<<qPrintable(IndexString)<<std::endl;

                        count++;
                        count1++;
                        emit valueChanged(count);



                }




            }


        }

   }
    else
   {
            std::cerr<< "invalid copyList in DeleteEmptyDirs"<<std::endl;
            return true;  //empty
    }

    if(count1==SourceDeleteList->size())
    {

        return true; //empty
    }
    else
    {


        return false; //not empty

    }


}

bool Thread::ActionNotOverWrittenUndoMove(QString Source,  QStringList list)
{
    QModelIndex index;
    QString StrIndex;

    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    QModelIndexList *moveList=new QModelIndexList;

     bool ok;


     QRegExp re=QRegExp("[^/]+$");


     QString string1=Source;
//     string1.remove(re);

     QString string2=List.at(0);
     string2.remove(re);

     QString string3=List.last();
//     string3.remove(re);



     std::cerr <<"fileaction string1=\t"<<qPrintable(string1)<<std::endl;
     std::cerr <<"fileaction string2=\t"<<qPrintable(string2)<<std::endl;
     std::cerr <<"fileaction string3=\t"<<qPrintable(string3)<<std::endl;

    YesToAll=true;


    std::cerr <<"List size as send=\t"<<List.size()<<std::endl;
    std::cerr <<"Copy List[0]=\t"<<qPrintable( List[0])<<std::endl;
    std::cerr <<"Copy List[1]=\t"<<qPrintable( List[1])<<std::endl;
//        Watcher->addPath(string1);
//        Watcher->addPath(string2);
//        Watcher->addPath(string3);


    dirModel->setReadOnly(false);

    StrIndex=List.last();

    Index=dirModel->index(StrIndex);

    QString str=Source;
    str.remove(re);

    QModelIndex ParentIndex=dirModel->index(str);
    QModelIndex SourceIndex=dirModel->index(Source);


    QString newDirName=dirModel->fileName(SourceIndex);
     std::cerr <<"NotOverWrittenUndoMove in fileaction fileName(SourceIndex)=\t"<<qPrintable(newDirName)<<std::endl;


    srand(static_cast<unsigned int>(time(0)));
    int min=0;
    int max=1000000;
    int number=static_cast<int>(static_cast<double>(rand())/RAND_MAX*(max-min)+ min);
    QString numberString;
    numberString=str.setNum(number);
    QString NewStrDir=newDirName+numberString;


    QModelIndex newTargetIndex;
    newTargetIndex= dirModel->mkdir(ParentIndex,NewStrDir);
    QString NewStrDir2=dirModel->filePath(newTargetIndex);
    std::cerr <<"fileaction NewStrDir2=\t"<<qPrintable(NewStrDir2)<<std::endl;
//        Watcher->addPath(NewStrDir2);

    if(!(newTargetIndex.isValid()))
    {
        std::cerr <<"NotOverWrittenUndoMove in fileaction not valid NewStrDir=\t"<<qPrintable(NewStrDir)<<std::endl;
        return false;

    }



           for(int i=0;i<(List.size())-1;i++)
           {
               std::cerr <<"NotOverWrittenUndoMove in fileaction\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



                QModelIndex indexTemp=dirModel->index(List[i]);
                 if (!indexTemp.isValid())      continue;// To avoid cp -rf *

               copyList->push_back(  indexTemp );
               std::cerr <<"NotOverWrittenUndoMove in fileaction copyList size=\t"<<copyList->size()<<std::endl;
               std::cerr <<"NotOverWrittenUndoMove in fileaction copyList[0]=\t"<<qPrintable( List[0])<<std::endl;


           }

           if (copyList->isEmpty())
           {
               std::cerr<< "invalid copyList in NotOverWrittenUndoMove in fileaction "<<std::endl;
               return false;   // To avoid cp -rf *
           }

           Watcher->addPath(string2);
           Watcher->addPath(NewStrDir2);

           ok=NotOverWrittenDirStructure(newTargetIndex,copyList);

            if(!ok)
            {
                std::cerr <<"NotOverWrittenDirStructure Failed in fileaction\n"<<std::endl;
                return false;
            }


            QModelIndexList * copyList2;
            copyList2=new QModelIndexList;
            for(int i=0;i<(NotOverWrittenList.size());i++)
            {
                std::cerr <<"NotOverWrittenUndoMove in fileaction\n"<<"Item is \t"<<qPrintable( NotOverWrittenList.at(i))<<std::endl;


                 QString path=NotOverWrittenList[i];

                 QModelIndex indexTemp=dirModel->index(path);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList2->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoMove in fileaction CopyList2 size=\t"<<copyList2->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoMove in fileaction CopyList2[0]=\t"<<qPrintable( List[0])<<std::endl;


            }

            if (copyList2->isEmpty())
            {
                std::cerr<< "invalid copyList in NotOverWrittenUndoMove in fileaction"<<std::endl;
                return false;   // To avoid cp -rf *
            }


            Watcher->addPath(string2);
            Watcher->addPath(NewStrDir2);

            ok=NotOverWrittenMove(newTargetIndex,copyList2);

            if(!ok)
            {
                std::cerr <<"NotOverWrittenMove Failed in fileaction\n"<<std::endl;
                return false;
            }



            QModelIndexList * copyList1;
            copyList1=new QModelIndexList;

            for(int i=0;i<(List.size())-1;i++)
            {
                std::cerr <<"NotOverWrittenUndoMove in fileaction\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;


                 QString path=List[i];
                 path=path.replace( Source,NewStrDir2+"/");
                 path=path.replace("//","/");//Needs a / in order to work correctly


                 QModelIndex indexTemp=dirModel->index(path);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList1->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoMove in fileaction CopyList1 size=\t"<<copyList1->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoMove in fileaction CopyList1[0]=\t"<<qPrintable( List[0])<<std::endl;


            }

            if (copyList1->isEmpty())
            {
                std::cerr<< "invalid copyList in NotOverWrittenUndoMove in fileaction"<<std::endl;
                return false;   // To avoid cp -rf *
            }

            ok=DeleteEmptyDirs(newTargetIndex,copyList1);
            if(!ok)
            {
                std::cerr <<"Some Source directories are not deleted"<<std::endl;

            }




            Watcher->addPath(string1);
            Watcher->addPath(string3);
            YesToAll=false;
            NoToAll=true;
//                ok = dirModel->dropMimeData(dirModel->mimeData(*copyList),Qt::MoveAction,-1,-1,Index);
            ok=move(Index,copyList);
            if(!ok)
            {
                std::cerr <<"move in UndoNotOverWritten Failed in fileaction\n"<<std::endl;
                return false;
            }







            Watcher->addPath(string2);
            Watcher->addPath(NewStrDir2);



//                  ok = dirModel->dropMimeData(dirModel->mimeData(*copyList1),Qt::MoveAction,-1,-1,SourceIndex);
            YesToAll=true;
            NoToAll=false;
           ok=move(SourceIndex,copyList1);

           if(!ok)
           {
               std::cerr <<"move in NotOverWrittenUndoMove Failed in fileaction\n"<<std::endl;
               return false;
           }

           ok=DeleteEmptyDirs(SourceIndex,copyList);
           if(!ok)
           {
               std::cerr <<"Some Source directories are not deleted"<<std::endl;

           }



            if(newTargetIndex.isValid())
            {
                 std::cerr <<"Deleting\t"<<qPrintable( NewStrDir)<<"\tin FileAction"<<std::endl;
                ok = dirModel->remove(newTargetIndex);
            }


            Watcher->removePath(string1);
            Watcher->removePath(string2);
            Watcher->removePath(string3);
            Watcher->removePath(NewStrDir2);
            Watcher->deleteLater();

            return true;


}
bool Thread::ActionNotOverWrittenUndoCopy(QString Source,  QStringList list)
{

    QModelIndex index;
    QString StrIndex;

    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
    QModelIndexList *moveList=new QModelIndexList;

     bool ok;


     QRegExp re=QRegExp("[^/]+$");


     QString string1=Source;
//     string1.remove(re);

     QString string2=List.at(0);
     string2.remove(re);

     QString string3=List.last();
//     string3.remove(re);



     std::cerr <<"fileaction string1=\t"<<qPrintable(string1)<<std::endl;
     std::cerr <<"fileaction string2=\t"<<qPrintable(string2)<<std::endl;
     std::cerr <<"fileaction string3=\t"<<qPrintable(string3)<<std::endl;

    //       YesToAll=true;

    std::cerr <<"List size as send=\t"<<List.size()<<std::endl;
    std::cerr <<"Copy List[0]=\t"<<qPrintable( List[0])<<std::endl;
    std::cerr <<"Copy List[1]=\t"<<qPrintable( List[1])<<std::endl;
    dirModel->setReadOnly(false);
//       Watcher->addPath(string1);
//       Watcher->addPath(string2);
//       Watcher->addPath(string3);

    StrIndex=List.last();

    Index=dirModel->index(StrIndex);

    QString str=Source;
    str.remove(re);

    QModelIndex ParentIndex=dirModel->index(str);
    QModelIndex SourceIndex=dirModel->index(Source);


    QString newDirName=dirModel->fileName(SourceIndex);
    std::cerr <<"NotOverWrittenUndoCopy in fileaction fileName(SourceIndex)=\t"<<qPrintable(newDirName)<<std::endl;

    srand(static_cast<unsigned int>(time(0)));
    int min=0;
    int max=1000000;
    int number=static_cast<int>(static_cast<double>(rand())/RAND_MAX*(max-min)+ min);
    QString numberString;
    numberString=str.setNum(number);
    QString NewStrDir=newDirName+numberString;



    QModelIndex newTargetIndex;
    newTargetIndex= dirModel->mkdir(ParentIndex,NewStrDir);
    QString NewStrDir2=dirModel->filePath(newTargetIndex);
    std::cerr <<"fileaction NewStrDir2=\t"<<qPrintable(NewStrDir2)<<std::endl;
//       Watcher->addPath(NewStrDir2);

    if(!(newTargetIndex.isValid()))
    {
        std::cerr <<"NotOverWrittenUndoCopy in fileaction not valid NewStrDir=\t"<<qPrintable(NewStrDir)<<std::endl;
         return false;

    }
    else
    {
        std::cerr <<"NotOverWrittenUndoCopy in fileaction valid NewStrDir=\t"<<qPrintable(NewStrDir)<<std::endl;

    }


           for(int i=0;i<(List.size())-1;i++)
           {
               std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



                QModelIndex indexTemp=dirModel->index(List[i]);
                 if (!indexTemp.isValid())      continue;// To avoid cp -rf *

               copyList->push_back(  indexTemp );
               std::cerr <<"NotOverWrittenUndoCopy copyList size=\t"<<copyList->size()<<std::endl;
               std::cerr <<"NotOverWrittenUndoCopy copyList[0]=\t"<<qPrintable( List[0])<<std::endl;


           }

           if (copyList->isEmpty())
           {
               std::cerr<< "invalid copyList in NotOverWrittenUndoCopy"<<std::endl;
               return false;   // To avoid cp -rf *
           }


           Watcher->addPath(string1);
           Watcher->addPath(NewStrDir2);

           ok=NotOverWrittenDirStructure(newTargetIndex,copyList);


            if(!ok)
            {
                std::cerr <<"NotOverWrittenDirStructure Failed in fileaction\n"<<std::endl;
                return false;
            }


            QModelIndexList * copyList2;
            copyList2=new QModelIndexList;

            for(int i=0;i<(NotOverWrittenList.size());i++)
            {
                std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( NotOverWrittenList.at(i))<<std::endl;


                 QString path=NotOverWrittenList[i];



                 QModelIndex indexTemp=dirModel->index(path);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList2->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoCopy CopyList2 size=\t"<<copyList2->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoCopy CopyList2[0]=\t"<<qPrintable( List[0])<<std::endl;


            }

            if (copyList2->isEmpty())
            {
                std::cerr<< "invalid copyList2 in NotOverWrittenUndoCopy"<<std::endl;
                return false;   // To avoid cp -rf *
            }


            Watcher->addPath(string2);
            Watcher->addPath(NewStrDir2);

            ok=NotOverWrittenMove(newTargetIndex,copyList2);


            Watcher->removePath(string2);
            Watcher->removePath(NewStrDir2);
            if(!ok)
            {
                std::cerr <<"NotOverWrittenUndoCopy Failed in fileaction\n"<<std::endl;
                return false;
            }


            QModelIndexList * copyList1;
            copyList1=new QModelIndexList;
            for(int i=0;i<(List.size())-1;i++)
            {
                std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;


                 QString path=List[i];
                 path=path.replace( Source,NewStrDir2+"/");
                 path=path.replace("//","/");//Needs a / in order to work correctly
                 std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( path)<<std::endl;


                 QModelIndex indexTemp=dirModel->index(path);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList1->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoCopy CopyList1 size=\t"<<copyList1->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoCopy CopyList1[0]=\t"<<qPrintable( List[0])<<std::endl;


            }

            if (copyList1->isEmpty())
            {
                std::cerr<< "invalid copyList in NotOverWrittenUndoCopy"<<std::endl;
                return false;   // To avoid cp -rf *
            }

            ok=DeleteEmptyDirs(newTargetIndex,copyList1);
            if(!ok)
            {
                std::cerr <<"Some Source directories are not deleted"<<std::endl;

            }

            Watcher->addPath(string1);
            Watcher->addPath(string2);

            QModelIndexList * copyList4;
            copyList4=new QModelIndexList;
            for(int i=0;i<(List.size())-1;i++)
            {
                std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;



                 QModelIndex indexTemp=dirModel->index(List[i]);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList4->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoCopy copyList size=\t"<<copyList4->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoCopy copyList[0]=\t"<<qPrintable( List[0])<<std::endl;


            }

            if (copyList4->isEmpty())
            {
                std::cerr<< "invalid copyList in NotOverWrittenUndoCopy"<<std::endl;
                return false;   // To avoid cp -rf *
            }

            del(SourceIndex,copyList4);


            QModelIndexList * copyList3;
            copyList3=new QModelIndexList;
            for(int i=0;i<(List.size())-1;i++)
            {
                std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;


                 QString path=List[i];
                 path=path.replace( Source,NewStrDir2+"/");
                 path=path.replace("//","/");//Needs a / in order to work correctly
                 std::cerr <<"NotOverWrittenUndoCopy\n"<<"Item is \t"<<qPrintable( path)<<std::endl;


                 QModelIndex indexTemp=dirModel->index(path);
                  if (!indexTemp.isValid())      continue;// To avoid cp -rf *

                copyList3->push_back(  indexTemp );
                std::cerr <<"NotOverWrittenUndoCopy CopyList1 size=\t"<<copyList3->size()<<std::endl;
                std::cerr <<"NotOverWrittenUndoCopy CopyList1[0]=\t"<<qPrintable( path)<<std::endl;


            }

            if (copyList3->isEmpty())
            {
                std::cerr<< "invalid copyList in NotOverWrittenUndoCopy"<<std::endl;
                return false;   // To avoid cp -rf *
            }

            Watcher->addPath(Source);
            Watcher->addPath(NewStrDir2);

            YesToAll=true;
            ok = dirModel->dropMimeData(dirModel->mimeData(*copyList3),Qt::MoveAction,-1,-1,SourceIndex);


//               ok=move(SourceIndex,copyList3);
            if(!ok)
            {
                std::cerr <<"move in NotOverWrittenUndoCopy Failed in fileaction\n"<<std::endl;
                return false;
            }


            if(newTargetIndex.isValid())
            {
                 std::cerr <<"Deleting\t"<<qPrintable( NewStrDir)<<"\tin FileAction NotOverWrittenUndoCopy"<<std::endl;
                ok = dirModel->remove(newTargetIndex);
            }





            Watcher->removePath(string1);
            Watcher->removePath(string2);
            Watcher->removePath(string3);
            Watcher->removePath(NewStrDir2);
            Watcher->deleteLater();

            return true;

}




