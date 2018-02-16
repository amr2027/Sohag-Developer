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




#include "newclassinfo.h"


#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include "sohaggen.h"
#include <QComboBox>



NewClassInfo::NewClassInfo(QStandardItemModel *pClassModel, QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Creating entity for ").append(tableName));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));
    complete=false;
    classModel=pClassModel;
    messageLabel=new QLabel();
    messageLabel->setStyleSheet("color:red;");
    classNameL=new QLabel(tr("Class Name"));
    className=new QLineEdit();
    baseClass=new QComboBox(this);
    baseClassL=new QLabel("Base Class",this);
    baseClass->addItem("QWidget","QWidget");
    baseClass->addItem("QDialog","QDialog");
    baseClass->setCurrentIndex(0);

    label = new QLabel(tr("Select columns you want to use by clicking on the check box"
                          "You can change the label of the generated UI item"));
    label->setWordWrap(true);
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(messageLabel,0,0,1,2);
    mainLayout->addWidget(classNameL,1,0,1,1);
    mainLayout->addWidget(className,1,1,1,1);
    mainLayout->addWidget(baseClassL,2,0);
    mainLayout->addWidget(baseClass,2,1);
    setLayout(mainLayout);

    registerField("className*", className);
    registerField("baseClass*", baseClass,"currentData","currentIndexChanged()");
    connect(className,SIGNAL(textEdited(QString)),this,SLOT(validateClassName(QString)));
}



void NewClassInfo::initializePage()
{
    if(className->text().isEmpty()){
        tableName =field("sIPTable").toString();
        QStringList list=tableName.split("_");
        QString newClassName;
        for (int i = 0; i < list.count(); ++i) {
            QString temp=list.at(i);
            temp.replace(0,1,temp[0].toUpper());
            newClassName.append(temp);
        }
        className->setText(newClassName);
        validateClassName(newClassName);
    }
}


void NewClassInfo::cleanupPage()
{

}



void NewClassInfo::validateClassName(QString text)
{
    if(text.isEmpty()){
        complete=false;
        emit completeChanged();
        return;
    }

    for (int row = 0; row < classModel->rowCount(); ++row) {
        QStandardItem *classItem=classModel->item(row);

        if(text==classItem->data(Qt::UserRole+ClassNameClassInfo).toString()){
            complete=false;
            emit completeChanged();
            messageLabel->setText("There is another class with this name.Class name can't be duplicated.\nPlease chang class name.");
            messageLabel->setStyleSheet("background-color:red;");
            return;
        }else{
            messageLabel->setText("");
            messageLabel->setStyleSheet("");
        }
    }

    complete=true;
    emit completeChanged();
}



bool NewClassInfo::isComplete() const
{
    return complete;
}

