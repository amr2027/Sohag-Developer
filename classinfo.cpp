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

#include "classinfo.h"


#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include "sohaggen.h"
#include <QComboBox>


ClassInfo::ClassInfo(QString tiltle,QString subTitle, QString imagePath, QString labelText, QStandardItemModel *pClassModel, QModelIndex pClassIndex, QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tiltle);
    setSubTitle(subTitle);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(imagePath));
    classModel=pClassModel;
    classIndex=pClassIndex;
    complete=true;

    label = new QLabel(labelText);
    label->setWordWrap(true);

    QLabel *classNameL=new QLabel("Class Name");

    classNameEdit=new QLineEdit(this);
    baseClass=new QComboBox(this);
    baseClassL=new QLabel("Base Class",this);
    baseClass->addItem("QWidget","QWidget");
    baseClass->addItem("QDialog","QDialog");
    baseClass->setCurrentIndex(0);

    registerField("className", classNameEdit);
    registerField("baseClass*", baseClass,"currentData","currentIndexChanged()");
    //    classNameEdit->setText(className);

    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(label);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(classNameL,1,0);
    gridLayout->addWidget(classNameEdit,1,1);
    gridLayout->addWidget(baseClassL,2,0);
    gridLayout->addWidget(baseClass,2,1);

    mainLayout->addLayout(gridLayout);
    //    setLayout(mainLayout);
    if(classNameEdit->text().isEmpty()){
        className();
    }
    QStandardItem *classItem=classModel->itemFromIndex(classIndex);
    QVariant baseClassValue=classItem->data(Qt::UserRole+BaseClassClassInfo);
    int baseClassIndex=baseClass->findData(baseClassValue);
    baseClass->setCurrentIndex(baseClassIndex);
    connect(classNameEdit,SIGNAL(textEdited(QString)),this,SLOT(validateClassName(QString)));

}

void ClassInfo::initializePage()
{

}

void ClassInfo::cleanupPage()
{

}

void ClassInfo::className()
{
    QStandardItem *classItem=classModel->itemFromIndex(classIndex);
    //    classItem->setData("---",Qt::UserRole+ClassNameClassInfo);
    vClassName=classItem->data(Qt::UserRole+ClassNameClassInfo).toString();
    classNameEdit->setText(vClassName);
}



void ClassInfo::validateClassName(QString text)
{
    if(text.isEmpty()){
        complete=false;
        emit completeChanged();
        return;
    }
    if(vClassName!=text){
        for (int row = 0; row < classModel->rowCount(); ++row) {
            QStandardItem *classItem=classModel->item(row);

            if(text==classItem->data(Qt::UserRole+ClassNameClassInfo).toString()){
                complete=false;
                emit completeChanged();
                return;
            }
        }
    }
    complete=true;
    emit completeChanged();
}



bool ClassInfo::isComplete() const
{
    return complete;
}



