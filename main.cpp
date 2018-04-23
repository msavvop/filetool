#include "filetool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("MSavvopOpenSource");
    QCoreApplication::setOrganizationDomain("m_savvop.otenet.gr");
    QCoreApplication::setApplicationName("filetool");



    QTranslator filetoolTranslator;
    QString filename = QString("filetool_%1").arg(QLocale::system().name());

//    QString filename = QString("filetool_el.qm");
    filename = filename.toLower();
    filetoolTranslator.load(filename);
    a.installTranslator(&filetoolTranslator);




    filetool w;
    w.show();

    return a.exec();
}
