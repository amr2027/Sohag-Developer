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



#include "sohagnewclass.h"
#include <QStandardItem>
#include "sohaggen.h"
#include "welcomepage.h"
#include "classattributes.h"
#include "newclassinfo.h"
#include "databasetable.h"
#include "tableattributes.h"


SohagNewClass::SohagNewClass(QSqlQuery &infoQuery, QStandardItemModel *pDBModel,QStandardItemModel *pClassModel,QStringList tableInfo, QWidget *parent)
    : QWizard(parent)
{
    setWindowTitle("Sohag Developer Create A New Class");
    setWizardStyle(QWizard::ModernStyle);
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::LogoPixmap,QPixmap(":/images/logo.png"));
    QString labelText=tr("This wizard will guide you to create a new Qt/C++ class ");
    addPage(new WelcomePage("Welecome to Sohag Developer create new class",":/images/wizard.png",labelText));
    addPage(new DatabaseTable(tableInfo,pDBModel,parent));
    tableAttributes=new TableAttributes(infoQuery,parent);
    addPage(tableAttributes);
    addPage(new NewClassInfo(pClassModel));
}




void SohagNewClass::accept()
{
    summaryModel=tableAttributes->columnsModel;
    classInfo[ClassNameClassInfo]=field("className").toString();
    classInfo[BaseClassClassInfo]=field("baseClass").toString();

//    setAllLineEdit = field("setAllLineEdit").toBool();
//    selectAll = field("selectAll").toBool();

    classInfo[SchemaNameClassInfo]=field("sIPSchema").toString();
    classInfo[TableNameClassInfo]=field("sIPTable").toString();
    classInfo[TableTypeClassInfo]=field("sIPTableType").toString();
    QDialog::accept();
}



QStandardItemModel *SohagNewClass::conclusion()
{
    return summaryModel;
}


QMap<int, QString> SohagNewClass::classData()
{
    return classInfo;
}
