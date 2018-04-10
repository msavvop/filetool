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

    void setthread(QString ActionString,   QStringList list);

    bool fileaction(QString ActionString,  QStringList list);

    bool copy(QModelIndex Index, QModelIndexList *copyList);
    bool getSize(QModelIndex Index, QModelIndexList *copyList);



    int showSize();
    protected:
    void run();


public:
    static bool NoToAll;
    static bool YesToAll;

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
    QFileSystemModel *dirModel;
    QFileSystemModel *treeViewDirModel;
    QDirModel *dirModel2;

    QMutex mutex;

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
    bool Implement_Answer();
};

#endif // THREAD_H
