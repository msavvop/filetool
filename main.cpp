#include "filetool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
