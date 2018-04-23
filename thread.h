#ifndef THREAD_H
#define THREAD_H


#include <QObject>
#include <QFileSystemModel>
#include <QThread>
#include <QDirModel>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QStyleOption>





class QModelIndex;
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread();

    void setthread(QString ActionString, QString Source,  QStringList list);

    bool fileaction(QString ActionString, QString Source,  QStringList list);

    bool copy(QModelIndex Index, QModelIndexList *copyList);
    bool move(QModelIndex Index, QModelIndexList *copyList);
    bool getSize(QModelIndex Index, QModelIndexList *copyList);
    bool ImplementAnswerInCopy();
    bool ImplementAnswerInMove();
    bool DeleteEmptyDirs(QModelIndex&,QModelIndexList*);


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

    QString Answer;
    int count;
    QString ActionStr;
    volatile bool stopped;
    QStringList List;
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

public slots:
    void stop();

    void set_Answer(QString);
    QString getAnswer();

};

#endif // THREAD_H
