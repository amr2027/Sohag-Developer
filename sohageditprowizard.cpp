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



#include "sohageditprowizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QSqlError>
#include <QLineEdit>
#include <QScrollArea>
#include <QGridLayout>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QDir>


SohagEditProWizard::SohagEditProWizard(QString pschemaName, QString ptableName, QSqlQuery &infoQuery, QStandardItemModel *dataBaseModel, QWidget *parent)
    : QWizard(parent)
{
    schemaName=pschemaName;
    tableName=ptableName;
    addPage(new EditWelcomePage);
//    addPage(new LocationPage(defPath));
//    if(dataBaseModel){
//        addPage(new SchemaInfoPage(dataBaseModel));
//    }
    tableInfoPage=new EditTableInfoPage(schemaName,tableName,infoQuery,dataBaseModel);
    addPage(tableInfoPage);
    addPage(new EditClassInfoPage(tableName));

    setWindowTitle("Sohag Developer Edit Class");
    setWizardStyle(QWizard::ModernStyle);
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::LogoPixmap,QPixmap(":/images/logo.png"));
}




void SohagEditProWizard::accept()
{
    summaryModel=tableInfoPage->columnsModel;
    className = field("className").toString();
    projectName = field("projectName").toString();
    projectLocation = field("projectLocation").toString();
    setAllLineEdit = field("setAllLineEdit").toBool();
    selectAll = field("selectAll").toBool();
    if(tableName.isEmpty()){
        schemaName=field("sIPSchema").toString();
        tableName=field("sIPTable").toString();
    }
    QDialog::accept();
}



QStandardItemModel *SohagEditProWizard::conclusion()
{
    return summaryModel;
}


QString SohagEditProWizard::proClassName()
{
    return className;
}


QString SohagEditProWizard::proProjectName()
{
    return projectName;
}


QString SohagEditProWizard::proLocation()
{
    return projectLocation;
}


QString SohagEditProWizard::proSchemaName()
{
    return schemaName;
}

QString SohagEditProWizard::proTableName()
{
    return tableName;
}


EditWelcomePage::EditWelcomePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Welcome To Sohag Developer"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel(tr("This wizard will guide you to produce Qt/C++ class "
                          "Qt/UI file that contains ui items corresponding to the database table column. "
                          "You need to specify the class name and set a few "
                          "options to produce an ui file , a header file and an "
                          "implementation file for your new C++ class."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}









EditTableInfoPage::EditTableInfoPage(QString pschema, QString ptableName, QSqlQuery &pinfoQuery, QStandardItemModel *model, QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Check columns that will be added to the entity"));
    //    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));
columnsModel=model;
    messageLabel=new QLabel();
    schema=pschema;
    tableName=ptableName;
    infoQuery=pinfoQuery;
    QLineEditValue=1;
    complete=false;
    isSchemaInfoPageHidden=false;
    label = new QLabel(tr("Select the columns you want to use "));
    label->setWordWrap(true);
    view=new QTableView(this);
    view->setAlternatingRowColors(true);
    setAllLineEdit=new QCheckBox(tr("Set all widgets to QLineEdit"));
    selectAll=new QCheckBox(tr("Select all Columns"));

    QHBoxLayout *optionsLayout=new QHBoxLayout;
    optionsLayout->addWidget(selectAll);
    optionsLayout->addWidget(setAllLineEdit);
    tableInfoLayout = new QVBoxLayout;
    tableInfoLayout->addWidget(label);
    tableInfoLayout->addWidget(messageLabel);
    tableInfoLayout->addWidget(view);
    tableInfoLayout->addLayout(optionsLayout);
    setLayout(tableInfoLayout);
    connect(selectAll,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
    connect(setAllLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
    registerField("setAllLineEdit", setAllLineEdit);
    registerField("selectAll", selectAll);
    if(!tableName.isEmpty()){
        tableData(infoQuery);
        isSchemaInfoPageHidden=true;
    }
}


void EditTableInfoPage::initializePage()
{
    if(isSchemaInfoPageHidden){
        return;
    }
    QString newFiledValue=field("sIPTable").toString();
    if(tableName.isEmpty() || tableName!=newFiledValue){
        schema=field("sIPSchema").toString();
        tableName=newFiledValue;
        tableData(infoQuery);
        selectAll->setChecked(false);
        setAllLineEdit->setChecked(false);
    }
}


void EditTableInfoPage::cleanupPage()
{

}

void EditTableInfoPage::tableData(QSqlQuery &infoQuery)
{
//    QString primaryKeyColumn;
//    QSqlQuery query;
//    query.prepare("SELECT constraint_column_usage.column_name FROM information_schema.constraint_column_usage"
//                  " where constraint_name=(SELECT constraint_name FROM information_schema.table_constraints "
//                  " where constraint_type='PRIMARY KEY' and table_constraints.table_schema=? and "
//                  " table_constraints.table_name=?)   and constraint_column_usage.table_schema=?  and"
//                  " constraint_column_usage.table_name=?");
//    query.addBindValue(schema);
//    query.addBindValue(tableName);
//    query.addBindValue(schema);
//    query.addBindValue(tableName);
//    query.exec();
//    query.first();
//    if (query.lastError().isValid()){
//        // Here Add Sql Error to the lable
//        messageLabel->setText("Error:\nSohag Developer Can't Primary Key Column\n You have to add condition to the update and query and delete methods youself \n"+
//                              query.lastError().text());
//        label->setText(query.lastQuery());
//        return;
//    }
//    primaryKeyColumn=query.value(0).toString();


//    query.prepare("SELECT column_name,data_type FROM information_schema.columns WHERE table_schema=? AND table_name=?");
//    query.addBindValue(schema);
//    query.addBindValue(tableName);
//    query.exec();
//    if (query.lastError().isValid()){
//        // Here Add Sql Error to the lable
//        messageLabel->setText("Error:\nSohag Developer Can't retrive table data from the database\n"+
//                              query.lastError().text());
//        return;
//    }

//    columnsModel=new QStandardItemModel(this);
//    int row=0;
//    while (query.next()) {

//        QString columnName=query.value(0).toString();
//        QStandardItem *item=new QStandardItem(columnName);
//        QVariant dataType=query.value(1);
//        item->setData(dataType,Qt::UserRole);
//        if(columnName==primaryKeyColumn){
//            item->setData("pk",Qt::UserRole+1);
//        }
//        item->setCheckable(true);
//        item->setEditable(false);
//        columnsModel->setItem(row,item);

//        QString columnLabel=columnName.replace("_"," ");
//        columnLabel.replace(0,1,columnLabel[0].toUpper());
//        QStandardItem *labelItem=new QStandardItem(columnLabel);
//        columnsModel->setItem(row,1,labelItem);

//        infoQuery.addBindValue(dataType);
//        infoQuery.exec();
//        infoQuery.first();
//        QString widget=infoQuery.value(1).toString();
//        QVariant widgetValue=infoQuery.value(0);
//        if(infoQuery.lastError().isValid()){
//            messageLabel->setText(infoQuery.lastError().text());
//            widget="QLineEdit";
//            widgetValue=QLineEditValue;
//        }
//        if(widget.isEmpty()){
//            widget="QLineEdit";
//            widgetValue=QLineEditValue;
//        }
//        QStandardItem *widgetItem=new QStandardItem(widget);
//        widgetItem->setData(widgetValue,Qt::UserRole);
//        widgetItem->setEditable(false);
//        columnsModel->setItem(row,2,widgetItem);

//        row++;
//    }

//    columnsModel->setHeaderData(0,Qt::Horizontal,"Table Or View Columns");
//    columnsModel->setHeaderData(1,Qt::Horizontal,"Label");
//    columnsModel->setHeaderData(2,Qt::Horizontal,"Qt Widget");
    view->setModel(columnsModel);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    connect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}





void EditTableInfoPage::itemChanged(QStandardItem *item)
{
    if(item->column()==0 && item->checkState()==Qt::Checked){
        complete=true;
        emit completeChanged();
        return;
    }

    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *currentItem=columnsModel->item(row);
        if(currentItem->checkState()==Qt::Checked){
            complete=true;
            emit completeChanged();
            return;
        }
    }

    complete=false;
    emit completeChanged();
}


void EditTableInfoPage::setAllQLineEdit(int state)
{
    disconnect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *widgetItem=columnsModel->item(row,2);
        if(state==Qt::Checked){
            widgetItem->setText("QLineEdit");
            widgetItem->setData(QLineEditValue,Qt::UserRole);
        }else{
            QStandardItem *item=columnsModel->item(row);
            QVariant dataType=item->data(Qt::UserRole);
            infoQuery.addBindValue(dataType);
            infoQuery.exec();
            infoQuery.first();
            QString widget=infoQuery.value(1).toString();
            QVariant widgetValue=infoQuery.value(0);
            if(infoQuery.lastError().isValid()){
                messageLabel->setText(infoQuery.lastError().text());
                widget="QLineEdit";
                widgetValue=QLineEditValue;
            }
            if(widget.isEmpty()){
                widget="QLineEdit";
                widgetValue=QLineEditValue;
            }
            widgetItem->setText(widget);
            widgetItem->setData(widgetValue,Qt::UserRole);
        }
    }
    connect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}


void EditTableInfoPage::setAllSelected(int state)
{
    disconnect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row);
        if(state==Qt::Checked){
            item->setCheckState(Qt::Checked);
            complete=true;
        }else{
            item->setCheckState(Qt::Unchecked);
            complete=false;
        }
    }
    emit completeChanged();
    connect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}



bool EditTableInfoPage::isComplete() const
{
    return complete;
}


EditClassInfoPage::EditClassInfoPage(QString ptableName,QWidget *parent)
    : QWizardPage(parent)
{
    tableName=ptableName;
    setTitle(tr("Creating entity for ").append(tableName));
    messageLabel=new QLabel();
    classNameL=new QLabel(tr("Class Name"));
    className=new QLineEdit();
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel(tr("Select columns you want to use by clicking on the check box"
                          "You can change the label of the generated UI item"));
    label->setWordWrap(true);
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(messageLabel,0,0,1,2);
    mainLayout->addWidget(classNameL,1,0,1,1);
    mainLayout->addWidget(className,1,1,1,1);
    setLayout(mainLayout);

    registerField("className*", className);
}



void EditClassInfoPage::initializePage()
{
    if(className->text().isEmpty()){
        if(tableName.isEmpty()){
            tableName =field("sIPTable").toString();
        }
        QStringList list=tableName.split("_");
        QString newClassName;
        for (int i = 0; i < list.count(); ++i) {
            QString temp=list.at(i);
            temp.replace(0,1,temp[0].toUpper());
            newClassName.append(temp);
        }
        className->setText(newClassName);
    }
}


void EditClassInfoPage::cleanupPage()
{

}
