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



#include "tableattributes.h"
#include <QLabel>
#include <QLineEdit>
#include <QStandardItemModel>
#include "sohaggen.h"
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QCheckBox>
#include <QSqlError>
#include <QHeaderView>



TableAttributes::TableAttributes(QSqlQuery &pinfoQuery, QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Select Columns");
    setSubTitle(tr("Check columns that will be added to the entity"));
    //    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    messageLabel=new QLabel();
    infoQuery=pinfoQuery;
    QLineEditValue=1;
    complete=false;
    label = new QLabel(tr("Select the columns you want to use "));
    label->setWordWrap(true);
    widgetL=new QLabel("Set Widget To",this);
    widget=new QComboBox(this);
    QPushButton *setWidget=new QPushButton("Set Widget",this);
    widgetL->setMaximumWidth(90);
    setWidget->setMaximumWidth(80);
    QHBoxLayout *widgetLayout=new QHBoxLayout();
    widgetLayout->addWidget(widgetL);
    widgetLayout->addWidget(widget);
    widgetLayout->addWidget(setWidget);

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
    tableInfoLayout->addLayout(widgetLayout);
    tableInfoLayout->addWidget(view);
    tableInfoLayout->addLayout(optionsLayout);
    setLayout(tableInfoLayout);
    connect(setWidget,SIGNAL(pressed()),this,SLOT(setWidgets()));
    connect(selectAll,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
    connect(setAllLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
    registerField("setAllLineEdit", setAllLineEdit);
    registerField("selectAll", selectAll);
    fillWidgetsCombo();

    //    tableName=field("sIPTable").toString();
    //    schema=field("sIPSchema").toString();

    //    tableData(infoQuery);
}


void TableAttributes::fillWidgetsCombo()
{
    QSqlQuery query(infoQuery);
    query.prepare("SELECT widget_id,widget FROM widgets where widget is not null");
    query.exec();
    if(query.lastError().isValid()){
        messageLabel->setText("Warning:\nSohag Developer Can't find widgets");
        messageLabel->setStyleSheet("background-color:yellow;color:red;");
    }

    while (query.next()) {
        widget->addItem(query.value(1).toString(),query.value(0));
    }
}


void TableAttributes::setWidgets()
{
    QModelIndexList selectedList=view->selectionModel()->selectedIndexes();
    QModelIndex selectedIndex;
    int count = selectedList.count();
    if(count==0){
        messageLabel->setText("Please select widgets you want to chmge first");
        messageLabel->setStyleSheet("background-color:yellow;color:red;");
        return;
    }
    messageLabel->setText("");
    messageLabel->setStyleSheet("");


    QString theWidget=widget->currentText();
    int theWidgetId=widget->currentData().toInt();
    for (int i=0; i < count; ++i) {
        selectedIndex=selectedList.at(i);
        columnsModel->item(selectedIndex.row(),AttribColumnWidgetItem)->setText(theWidget);
        columnsModel->item(selectedIndex.row(),AttribColumnWidgetItem)->setData(theWidgetId,Qt::UserRole+AttribDataWidgetValue);
    }
}


void TableAttributes::initializePage()
{

    tableData(infoQuery);
    selectAll->setChecked(false);
    //    setAllLineEdit->setChecked(false);

}


void TableAttributes::cleanupPage()
{

}

void TableAttributes::tableData(QSqlQuery &infoQuery)
{
    tableName=field("sIPTable").toString();
    schema=field("sIPSchema").toString();
    QString primaryKeyColumn;
    QSqlQuery query;
    query.prepare("SELECT constraint_column_usage.column_name FROM information_schema.constraint_column_usage"
                  " where constraint_name=(SELECT constraint_name FROM information_schema.table_constraints "
                  " where constraint_type='PRIMARY KEY' and table_constraints.table_schema=? and "
                  " table_constraints.table_name=?)   and constraint_column_usage.table_schema=?  and"
                  " constraint_column_usage.table_name=?");
    query.addBindValue(schema);
    query.addBindValue(tableName);
    query.addBindValue(schema);
    query.addBindValue(tableName);
    query.exec();
    query.first();
    if (query.lastError().isValid()){
        // Here Add Sql Error to the lable
        messageLabel->setText("Error:\nSohag Developer Can't Primary Key Column\n You have to edit conditions of update, query and delete methods manually \n"+
                              query.lastError().text());
        messageLabel->setStyleSheet("background-color:red;");
        label->setText(query.lastQuery());
        return;
    }else if(query.value(0).toString().isEmpty()){
        messageLabel->setText("Warning:\nSohag Developer Can't Find Primary Key Column\n You have to edit conditions of update, query and delete methods manually \n");
        messageLabel->setStyleSheet("background-color:yellow;color:red;");
    }else{
        messageLabel->setText("");
        messageLabel->setStyleSheet("");
    }
    primaryKeyColumn=query.value(0).toString();

    query.prepare("SELECT column_name,data_type FROM information_schema.columns WHERE table_schema=? AND table_name=?");
    query.addBindValue(schema);
    query.addBindValue(tableName);
    query.exec();
    if (query.lastError().isValid()){
        // Here Add Sql Error to the lable
        messageLabel->setText("Error:\nSohag Developer Can't retrive table data from the database\n"+
                              query.lastError().text());
        messageLabel->setStyleSheet("background-color:red;");
        return;
    }


    columnsModel=new QStandardItemModel(this);
    int row=0;
    while (query.next()) {

        QString columnName=query.value(0).toString();
        QStandardItem *item=new QStandardItem(columnName);
        QVariant dataType=query.value(1);
        item->setData(dataType,Qt::UserRole+AttribDataDataType);
        item->setData(CreateMode,Qt::UserRole+ModeItemStatus);
        item->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
        if(columnName==primaryKeyColumn){
            item->setData("pk",Qt::UserRole+AttribDataConstraint);
        }
        item->setCheckable(true);
        item->setEditable(false);
        columnsModel->setItem(row,AttribColumnItem,item);

        QString columnLabel=columnName.replace("_"," ");
        columnLabel.replace(0,1,columnLabel[0].toUpper());
        QStandardItem *labelItem=new QStandardItem(columnLabel);
        columnsModel->setItem(row,AttribColumnLabelItem,labelItem);
        QString widget;
        QVariant widgetValue;
        //        infoQuery.addBindValue(dataType);
        //        infoQuery.exec();
        //        infoQuery.first();
        //        widget=infoQuery.value(1).toString();
        //        widgetValue=infoQuery.value(0);
        //        if(infoQuery.lastError().isValid()){
        //            messageLabel->setText(infoQuery.lastError().text());
        //            messageLabel->setStyleSheet("background-color:yellow;color:red;");
        //            widget="QLineEdit";
        //            widgetValue=QLineEditValue;
        //        }
        //        if(widget.isEmpty()){
        widget="QLineEdit";
        widgetValue=QLineEditValue;
        //        }
        QStandardItem *widgetItem=new QStandardItem(widget);
        widgetItem->setData(widgetValue,Qt::UserRole+AttribDataWidgetValue);
        widgetItem->setEditable(false);
        columnsModel->setItem(row,AttribColumnWidgetItem,widgetItem);

        row++;
    }

    columnsModel->setHeaderData(AttribColumnItem,Qt::Horizontal,"Table Or View Columns");
    columnsModel->setHeaderData(AttribColumnLabelItem,Qt::Horizontal,"Label");
    columnsModel->setHeaderData(AttribColumnWidgetItem,Qt::Horizontal,"Qt Widget");
    view->setModel(columnsModel);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    connect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    setAllLineEdit->setChecked(true);
}





void TableAttributes::itemChanged(QStandardItem *item)
{
    testIfAllSelected();
    testIfAllQLineEdits();

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


void TableAttributes::setAllQLineEdit(int state)
{
    disconnect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *widgetItem=columnsModel->item(row,AttribColumnWidgetItem);
        if(state==Qt::Checked){
            widgetItem->setText("QLineEdit");
            widgetItem->setData(QLineEditValue,Qt::UserRole+AttribDataWidgetValue);
        }else{
            QStandardItem *item=columnsModel->item(row);
            QVariant dataType=item->data(Qt::UserRole+AttribDataDataType);
            infoQuery.addBindValue(dataType);
            infoQuery.exec();
            infoQuery.first();
            QString widget=infoQuery.value(1).toString();
            QVariant widgetValue=infoQuery.value(0);
            if(infoQuery.lastError().isValid()){
                messageLabel->setText(infoQuery.lastError().text());
                messageLabel->setStyleSheet("background-color:yellow;color:red;");
                widget="QLineEdit";
                widgetValue=QLineEditValue;
            }
            if(widget.isEmpty()){
                widget="QLineEdit";
                widgetValue=QLineEditValue;
            }
            widgetItem->setText(widget);
            widgetItem->setData(widgetValue,Qt::UserRole+AttribDataWidgetValue);
        }
    }
    connect(columnsModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}


void TableAttributes::setAllSelected(int state)
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



bool TableAttributes::isComplete() const
{
    return complete;
}



void TableAttributes::testIfAllSelected()
{
    disconnect(selectAll,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
    bool check=true;
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row);
        if (item->checkState()==Qt::Unchecked){
            check=false;
            break;
        }
    }
    selectAll->setChecked(check);
    connect(selectAll,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
}


void TableAttributes::testIfAllQLineEdits()
{
    disconnect(setAllLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
    bool check=true;
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row,AttribColumnWidgetItem);
        if (item->text()!="QLineEdit"){
            check=false;
            break;
        }
    }
    setAllLineEdit->setChecked(check);
    connect(setAllLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
}
