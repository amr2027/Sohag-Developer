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



#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QString>


class DatabaseInterface
{
public:
    virtual ~DatabaseInterface() {}
    virtual QString schemaQuery() = 0;
    virtual QString tableQuery() = 0;

};


QT_BEGIN_NAMESPACE

#define DatabaseInterface_iid "org.qt-project.Qt.Examples.DatabaseInterface"

Q_DECLARE_INTERFACE(DatabaseInterface, DatabaseInterface_iid)
QT_END_NAMESPACE

#endif
