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



#ifndef SOHAGEDITCLASS_H
#define SOHAGEDITCLASS_H

#include <QWizard>
#include <QSqlQuery>



class QStandardItemModel;
class QStandardItem;
class ClassAttributes;
class ClassInfo;

class SohagEditClass: public QWizard
{
    Q_OBJECT
public:
    SohagEditClass(QModelIndex classIndex,QStandardItemModel *classesModel,QStandardItemModel *attributesModel,QSqlQuery &infoQuery,QWidget *parent = 0);
    QStandardItemModel *attributesModel();
    void accept() Q_DECL_OVERRIDE;
private:
    ClassAttributes *classAttributes;
    //    ClassInfo *classInfo;
    QStandardItem *classItem;
};

#endif // SOHAGEDITCLASS_H
