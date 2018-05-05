#ifndef THREAD_H
#define THREAD_H


#include <QObject>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QThread>
#include <QDirModel>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QStyleOption>
#include <cstdlib>
#include <ctime>

#define _CRT_RAND_S



class QModelIndex;
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread();

    void setthread(QString ActionString, QString Source,  QStringList list, QStringList list1);

    bool fileaction(QString ActionString, QString Source,  QStringList list);
    bool ActionMove(QString Source,  QStringList list);
    bool ActionCopy(QString Source,  QStringList list);
    bool ActionNotOverWrittenUndoMove(QString Source,  QStringList list);
    bool ActionNotOverWrittenUndoCopy(QString Source,  QStringList list);
    bool copy(QModelIndex Index, QModelIndexList *copyList);
    bool move(QModelIndex Index, QModelIndexList *copyList);
    bool del(QModelIndex& , QModelIndexList *);
    bool getSize(QModelIndex Index, QModelIndexList *copyList);
    bool ImplementAnswerInCopy();
    bool ImplementAnswerInMove();
    bool DeleteEmptyDirs(QModelIndex&,QModelIndexList*);
    bool NotOverWrittenDirStructure(QModelIndex targetIndex, QModelIndexList *sourceCopyList);
    bool NotOverWrittenMove(QModelIndex Index, QModelIndexList *copyList);
    int showSize();
    protected:
    void run();


public:

    bool NoToAll;
    bool YesToAll;

private:
    QMutex mutex1;
    QMutex mutex2;
    QWaitCondition var_not_set;
    QWaitCondition var_set;
    QFileSystemWatcher *Watcher;
    QString Answer;

    int count;
    QString ActionStr;
//    volatile bool stopped;
    QStringList List;
    QStringList NotOverWrittenList;
    QString NotOverWritten;
    QString Source;
    QFileSystemModel *dirModel;
    QFileSystemModel *treeViewDirModel;
    QDirModel *dirModel2;




    int Size;
    QModelIndex pathSetIndex;
    QModelIndex TargetDirSetIndex;
    QModelIndex targetIndexTempSet;


    signals:
   void valueChanged(int);
   void dialogComplete(bool);
   void fileexists(QString, QString);
    void send_Answer(QString);
    void CloseOverWriteDialog(bool);
    void ReturnTheseValues(bool,QStringList);

public slots:
//    void stop();

    void set_Answer(QString);
    QString getAnswer();

};

#endif // THREAD_H
