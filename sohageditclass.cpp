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



#include "sohageditclass.h"
//#include <QLabel>
//#include <QVBoxLayout>
//#include <QSqlError>
//#include <QLineEdit>
//#include <QScrollArea>
//#include <QGridLayout>
//#include <QCheckBox>
//#include <QStandardItemModel>
//#include <QTableView>
//#include <QHeaderView>
//#include <QMessageBox>
//#include <QDir>
#include <QStandardItem>
#include "sohaggen.h"
#include "welcomepage.h"
#include "classattributes.h"
#include "classinfo.h"



SohagEditClass::SohagEditClass(QModelIndex classIndex,QStandardItemModel *classesModel,QStandardItemModel *attributesModel,QSqlQuery &infoQuery,QWidget *parent)
    : QWizard(parent)
{
    //    addPage(new WelcomePageNewClass);
    //    addPage(new SchemaInfoPageNewClass(dataBaseModel));
    //    tableInfoPage=new TableInfoPageNewClass(infoQuery);
    //    addPage(tableInfoPage);
    //    addPage(new ClassInfoPageNewClass());

    setWindowTitle("Sohag Developer Build New Class");
    setWizardStyle(QWizard::ModernStyle);
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::LogoPixmap,QPixmap(":/images/logo.png"));
    QString labelText=tr("This wizard will guide you to edit Qt/C++ class ");
    addPage(new WelcomePage("Welecome to Sohag Developer edit class",":/images/wizard.png",labelText));

    classItem=classesModel->itemFromIndex(classIndex);
    QString database=classItem->data(Qt::UserRole+CatalogNameClassInfo).toString();
    QString schema=classItem->data(Qt::UserRole+SchemaNameClassInfo).toString();
    QString table=classItem->data(Qt::UserRole+TableNameClassInfo).toString();

    classAttributes=new ClassAttributes("Class Atrributes","Columns Information","","Please select or deselect columns you want to\n add or remove from the class attributes.\nYou can also chang the label or the widget related to that database column."
                                        ,database,schema,table,attributesModel,infoQuery,this);
    addPage(classAttributes);

    addPage(new ClassInfo("Class Atrributes","Class Information","","Please select or deselect columns you want to\n add or remove from the class attributes.\nYou can also chang the label or the widget related to that database column."
                          ,classesModel,classIndex,this));

}




void SohagEditClass::accept()
{
    classItem->setData(field("baseClass").toString(),Qt::UserRole+BaseClassClassInfo);
    QString newClassName=field("className").toString();
    if(classItem->text()!=newClassName){
        classItem->setText(newClassName);
        classItem->setData(newClassName,Qt::UserRole+ClassNameClassInfo);
        QStandardItemModel *attribModel=attributesModel();
        for (int row = 0; row < attribModel->rowCount(); ++row) {
            QStandardItem *item=attribModel->item(row,AttribColumnItem);
            item->setData(newClassName,Qt::UserRole+ClassNameClassInfo);
            item->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
            item->setData(EditMode,Qt::UserRole+ModeItemStatus);
            QStandardItem *classItem=attribModel->item(row,AttribColumnClassItem);
            classItem->setText(newClassName);
        }
    }
    QDialog::accept();
}




QStandardItemModel *SohagEditClass::attributesModel()
{
    return classAttributes->attributesModel;
}
