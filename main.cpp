/*=======================================================================================================================
= Sohag Developer Project
= This Project Aims To Create A PostgreSQL Tools
= To Help Users Using That Database
= At That Stage The Sohag Developer Is Generating Qt/C++ Files
= But Sohag Developer's Goal Is To Create A Compelete IDE For PostgreSQL
= Also The Project Will Be Extended To Include Other Databases As Long AS Possible
= For More Details Please Visit Sohag Developer website (http://sohag-developer.com/)
= This Software Is Licensed Under GPLv3 for more details visit (https://www.gnu.org/licenses/gpl-3.0.en.html)
= This Software is Sponsored By AlHuda Software الهدى للبرمجيات(http://alhuda-software.com/)
========================================================================================================================
*/


#include "sohagmain.h"
#include <QApplication>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QString styleFile=qApp->applicationDirPath()+"/styel.css";
    QString styleFile=+":/images/styel.css";
    QFile file(styleFile) ;
    QString styel ;
    if (file.open(QFile::ReadOnly)){
        styel=file.readAll();
        qApp->setStyleSheet(styel);
    }

    SohagMain w;
    w.showMaximized();
//    w.show();

//    if (QApplication::arguments().size() > 1) {
//        const QString fileName = QApplication::arguments().at(1);
//    }

    return a.exec();
}
