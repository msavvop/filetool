#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QFileSystemModel>
#include <QThread>
#include <QDirModel>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

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

    private:
    int count;
    QString ActionStr;
    volatile bool stopped;
    QStringList List;
    QFileSystemModel *dirModel;
    QFileSystemModel *treeViewDirModel;
    QDirModel *dirModel2;
    QMutex mutex;
    int Size;


    signals:
   void valueChanged(int);
   void dialogComplete(bool);

public slots:
    void stop();
};

#endif // THREAD_H
