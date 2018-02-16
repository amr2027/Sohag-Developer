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




#include "schemainfo.h"
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QCheckBox>
#include "sohaggen.h"
#include <QSqlError>
#include <QHeaderView>



SchemaInfo::SchemaInfo(QStandardItemModel *pDBModel,QWidget *parent)
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

    label = new QLabel(tr("Select Table Or View You Want to Build Entity Upon"));
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

    if(dBModel){
        connect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(schemaItemChange(QStandardItem*)));
        connect(searchText,SIGNAL(textEdited(QString)),this,SLOT(filter(QString)));
    }



}

void SchemaInfo::cleanupPage()
{

}



void SchemaInfo::schemaItemChange(QStandardItem *item)
{
    complete=false;
    emit completeChanged();
    disconnect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(schemaItemChange(QStandardItem*)));
    if(item->checkState()==Qt::Checked){

        QStandardItem *zeroItem=dBModel->item(0,0);
        // Only Allow one Item to be checked.
        // Last checked item data stored in the zeroItem
        // UnCheck the last item checked if found. Then set the current item as the last item checked
        QModelIndex index=zeroItem->data(Qt::UserRole+LastCheckedItemIndex).toModelIndex();
        if(index.isValid()){
            dBModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
        }

        item->setCheckState(Qt::Checked);
        zeroItem->setData(item->index(),Qt::UserRole+LastCheckedItemIndex);
        schema->setText(dBModel->item(item->row(),1)->text());
        table->setText(item->text());
        tableType->setText(dBModel->item(item->row(),2)->text());
        complete=true;
        emit completeChanged();
    }else{
        schema->setText("");
        table->setText("");
        tableType->setText("");
    }
    connect(dBModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(schemaItemChange(QStandardItem*)));
}





void SchemaInfo::filter(QString text)
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


bool SchemaInfo::isComplete() const
{
    return complete;
}

