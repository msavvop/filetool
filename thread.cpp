#include "thread.h"
#include <iostream>
#include <QTextStream>

Thread::Thread()
{
      stopped = false;

      dirModel=new QFileSystemModel;
      dirModel2=new QDirModel;
}


void Thread::setthread(QString ActionString, QStringList list)
{
    ActionStr=ActionString;
    List=list;
//    std::cerr<<"AT SETTHREAD selected item"<<qPrintable(List.at(0))<<endl;
}


void Thread::run()
{
    bool ok;

        ok=fileaction(ActionStr,List);
        if(!ok)
        {
            std::cerr <<"fileaction Failed\n"<<std::endl;

        }

        emit(dialogComplete(true));


}



void Thread::stop()
{
    mutex.lock();
    stopped = true;
    mutex.unlock();

}

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


bool Thread::fileaction(QString ActionStr, QStringList List)
{
    QModelIndex index;
    QString StrIndex;
    QModelIndex Index;
    QModelIndexList *copyList=new QModelIndexList;
     bool ok;

     count=0;
      Size=0;


    if(ActionStr=="delete")
    {

        index=dirModel->index(List[0]);

        if (!index.isValid())      return false; // to avoid something like rm -rf *



            for (int i=0;i<(List.size())-1;i++)

            {
                index=dirModel->index(List[i]);





                if (!index.isValid())      continue;

                std::cerr <<"Deleting\t"<<qPrintable( List[i])<<"\tin FileAction"<<std::endl;


                if (dirModel->fileInfo(index).isDir())
                {

                    ok = dirModel->remove(index);
                }
                else
                {
                    ok = dirModel->remove(index);
                }



                count++;

                mutex.lock();

                if (stopped)
                {
                    stopped = false;
                    mutex.unlock();

                    std::cerr <<"Delete Failed\n"<<"Item is \t"<<qPrintable( dirModel->filePath(index))<<std::endl;

                    return false;
                }

                mutex.unlock();


                emit valueChanged(count);


                 if(!ok) std::cerr <<"Remove Failed\n"<<std::endl;
            }





        return true;

    }
    else if(ActionStr=="copy")
    {

        std::cerr <<"List size as send=\t"<<List.size()<<std::endl;
        std::cerr <<"Copy List[0]=\t"<<qPrintable( List[0])<<std::endl;
        std::cerr <<"Copy List[1]=\t"<<qPrintable( List[1])<<std::endl;
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


               ok=copy(Index,copyList);

                if(!ok)
                {
                    std::cerr <<"Copy Failed\n"<<std::endl;
                    return false;
                }

                return true;
    }
    else if (ActionStr=="cut")
    {


        dirModel->setReadOnly(false);


        StrIndex=List.last();

        Index=dirModel->index(StrIndex);

        ok=dirModel->supportedDropActions().testFlag(Qt::MoveAction);
        if(!ok)
        {

             std::cerr <<"Move2 Failed\n"<<std::endl;

            return true;
        }



        for(int i=0;i<(List.size())-1;i++)
        {


 //            std::cerr <<"list\n"<<qPrintable(dirModel->filePath(List[i]))<<std::endl;
            QModelIndex indexTemp=dirModel->index(List[i]);

            if (!indexTemp.isValid())      continue;// to avoid mv -rf *

           copyList->push_back(  indexTemp );


           std::cerr <<"Cut List size=\t"<<copyList->size()<<std::endl;
           std::cerr <<"Cut List[0]=\t"<<qPrintable( List[0])<<std::endl;

        }
        if (copyList->isEmpty())
        {
            std::cerr<< "invalid copyList in move"<<std::endl;
            return  false;         // to avoid mv -rf *
        }



        ok = dirModel->dropMimeData(dirModel->mimeData(*copyList),Qt::MoveAction,-1,-1,Index);


        if(!ok)
        {
 //           std::cerr <<"Move Failed\n"<<std::endl;

              ok=fileaction("copy",List);
              if (!ok)
              {
                  std::cerr <<"Copy in Move Failed\n"<<std::endl;
                  return false;
              }

              ok=fileaction("delete",List);
              if (!ok)
              {
                  std::cerr <<"delete in Move Failed\n"<<std::endl;
                  return false;
              }
        } else
        {
            emit valueChanged(Size);
        }


        return true;
    }

    std::cerr <<"if Failed"<<std::endl;

    return false;
}

bool Thread::copy(QModelIndex targetIndex, QModelIndexList *sourceCopyList)
{


    dirModel->setReadOnly(false);


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

            mutex.lock();

            if (stopped)
            {
                stopped = false;
                mutex.unlock();

                std::cerr <<"Copy Failed\n"<<"Item is \t"<<qPrintable( List.at(i))<<std::endl;

                return false;
            }

            mutex.unlock();

            QModelIndex index=sourceCopyList->at(i);


            if (!index.isValid())
            {
                std::cerr <<"Invalid index"<<qPrintable(index.row())<<std::endl;

                return 0;
            }


            bool ok;

            if (dirModel->fileInfo(index).isDir())
            {


                QString dirName;
                dirName=dirModel->fileName(index);
                std::cerr <<"in isDir Copy\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);



                QModelIndex newTargetIndex= dirModel->mkdir(targetIndex,dirName);

                count++;



                emit valueChanged(count);

                if (!newTargetIndex.isValid())
                {
                        std::cerr <<"Invalid newTargetIndex"<<qPrintable(newTargetIndex.row())<<std::endl;

                }

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;

                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

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
//                    std::cerr <<"list\n"<<qPrintable(str1)<<std::endl;


                ok=copy(newTargetIndex,newSourceList);
                if (!ok)
                {

                    std::cerr <<"Copy in Move Failed\n"<<std::endl;
                    return false;

                }

            }
            else
            {
                    QModelIndexList *tempList;
                    tempList=new QModelIndexList;
                    QString path2=dirModel->filePath(index);
                    QModelIndex indexTemp=dirModel->index(path2);
                    tempList->push_back(indexTemp);
                    std::cerr <<"Copying\t"<<qPrintable( path2)<<std::endl;
                    ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,targetIndex);


                    if (!ok) std::cerr <<"Copy Failed"<<std::endl;

                    count++;



                    emit valueChanged(count);
            }


        }

   }
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
                std::cerr <<"Invalid index\n"<<qPrintable(index.row())<<std::endl;

                return false;
            }




            if (dirModel->fileInfo(index).isDir())


            {
                Size++;

                QString dirName;
                dirName=dirModel->fileName(index);
                std::cerr <<"in isDir GetSize\t Dir\t "<<qPrintable(dirName)<<std::endl;
                const QString path=dirModel->filePath(index);

//                QModelIndex newTargetIndex= dirModel->mkdir(targetIndex,dirName);

                QModelIndex newTargetIndex= dirModel->index(path);


                if (!newTargetIndex.isValid())
                {
                        std::cerr <<"Invalid newTargetIndex"<<qPrintable(newTargetIndex.row())<<std::endl;

                }

                QModelIndexList *newSourceList;
                newSourceList=new QModelIndexList;

                std::cerr <<"rows: \t"<<qPrintable(dirModel2->rowCount(dirModel2->index(path)))<<std::endl;

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
//                    std::cerr <<"list\n"<<qPrintable(str1)<<std::endl;


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

//                    ok = dirModel->dropMimeData(dirModel->mimeData(*tempList),Qt::CopyAction,-1,-1,targetIndex);




//                    if (!ok) std::cerr <<"GetSize Failed"<<std::endl;
            }


        }

   }
    std::cerr <<"In getSize Size= \t"<<Size<<std::endl;
    return true;
}



