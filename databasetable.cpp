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



#include "databasetable.h"
#include "sohaggen.h"
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>



DatabaseTable::DatabaseTable(QStringList tableInfo,QStandardItemModel *pDBModel, QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Please Select Schema and Table or View "));
    setSubTitle("Select a table or view");

    complete=false;
    messageLabel=new QLabel();
    schemaTableL=new QLabel("Selected Table");
    schema=new QLineEdit();
    schema->setReadOnly(true);
    schema->setFrame(false);
    table=new QLineEdit();
    table->setReadOnly(true);
    table->setFrame(false);
    tableType=new QLineEdit();
    tableType->setReadOnly(true);
    tableType->setFrame(false);
    registerField("sIPSchema",schema);
    registerField("sIPTable", table);
    registerField("sIPTableType", tableType);
    searchTextL=new QLabel(tr("Find Table"));
    searchText=new QLineEdit();
    //    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/logo.png"));

    label = new QLabel(tr("Select Table Or View You Want to Build Class Upon"));
    label->setWordWrap(true);
    dBModel=pDBModel;
    schemaView=new QTableView(this);
    schemaView->setAlternatingRowColors(true);
    schemaView->setModel(dBModel);
    schemaView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    schemaView->resizeColumnsToContents();
    schemaView->horizontalHeader()->setStretchLastSection(true);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(messageLabel,0,0,1,4);
    mainLayout->addWidget(searchTextL,1,0,1,1);
    mainLayout->addWidget(searchText,1,1,1,3);
    mainLayout->addWidget(schemaTableL,2,0,1,1);
    mainLayout->addWidget(schema,2,1,1,1);
    mainLayout->addWidget(table,2,2,1,1);
    mainLayout->addWidget(tableType,2,3,1,1);
    mainLayout->addWidget(schemaView,3,0,1,4);
    setLayout(mainLayout);
    setDefaultTbleInfo(tableInfo);
    if(dBModel){
        connect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(newClassItemChange(QStandardItem*)));
        connect(searchText,SIGNAL(textEdited(QString)),this,SLOT(filter(QString)));
    }
}


void DatabaseTable::setDefaultTbleInfo(QStringList tableInfo)
{
    if(tableInfo.count()>0){
        //        schema->setText(tableInfo.at(0));
        //        table->setText(tableInfo.at(1));
        //        tableType->setText(tableInfo.at(2));

        QString schemaName=tableInfo.at(0);
        QString tableName=tableInfo.at(1);
        QString tableType=tableInfo.at(2);

        for (int row = 0; row < dBModel->rowCount(); ++row) {
            QString itemText=dBModel->item(row)->text();

            if(itemText==tableName){
                if(dBModel->item(row,1)->text()==schemaName && tableType==dBModel->item(row,2)->text()){
                    QStandardItem *tableItem=dBModel->item(row);
                tableItem->setCheckState(Qt::Checked);
                newClassItemChange(tableItem);
                return;
            }
            }
        }
    }
}

void DatabaseTable::initializePage()
{
}

void DatabaseTable::cleanupPage()
{
}



void DatabaseTable::newClassItemChange(QStandardItem *item)
{
    complete=false;

    if(item->checkState()==Qt::Checked){

        QStandardItem *zeroItem=dBModel->item(0,0);
        // Only Allow one Item to be checked.
        // Last checked item data stored in the zeroItem
        // UnCheck the last item checked if found. Then set the current item as the last item checked
        QModelIndex index=zeroItem->data(Qt::UserRole+LastCheckedItemIndex).toModelIndex();
        if(index.isValid()){
            disconnect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(newClassItemChange(QStandardItem*)));
            dBModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
            item->setCheckState(Qt::Checked);
            connect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(newClassItemChange(QStandardItem*)));
        }

        zeroItem->setData(item->index(),Qt::UserRole+LastCheckedItemIndex);
        schema->setText(dBModel->item(item->row(),1)->text());
        table->setText(item->text());
        tableType->setText(dBModel->item(item->row(),2)->text());
        complete=true;
    }else{
        schema->setText("");
        table->setText("");
        tableType->setText("");
    }
    emit completeChanged();

}





void DatabaseTable::filter(QString text)
{

    if(text.isEmpty()){
        for (int row = 0; row < dBModel->rowCount(); ++row) {
            schemaView->setRowHidden(row,false);
        }
    }else{
        for (int row = 0; row < dBModel->rowCount(); ++row) {
            QString itemText=dBModel->item(row)->text();
            if(itemText.contains(text,Qt::CaseInsensitive)){
                schemaView->setRowHidden(row,false);
            }else{
                schemaView->setRowHidden(row,true);
            }
        }
    }
}


bool DatabaseTable::isComplete() const
{
    return complete;
}

