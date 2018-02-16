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



#include "classattributes.h"

#include <QLabel>
#include <QTableView>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QCheckBox>
#include "sohaggen.h"
#include <QSqlError>
#include <QComboBox>
#include <QPushButton>



ClassAttributes::ClassAttributes(QString tiltle,QString subTitle,QString imagePath,QString labelText,QString database,QString schema,QString table,QStandardItemModel *pAttributesModel,QSqlQuery &pInfoQuery,QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tiltle);
    setSubTitle(subTitle);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(imagePath));

    infoQuery=pInfoQuery;
    label = new QLabel(labelText);
    label->setWordWrap(true);

    attributesModel=pAttributesModel;
    complete=true;

    messageLabel=new QLabel();
    QLabel *catalogLabel = new QLabel("DtataBase: "+database);
    QLabel *schemaLabel = new QLabel("Schema: "+schema);
    QLabel *tableLabel = new QLabel("Table/View: "+table);
    widgetL=new QLabel("Set Widget To",this);
    widget=new QComboBox(this);
    QPushButton *setWidget=new QPushButton("Set Widget",this);
    widgetL->setMaximumWidth(90);
    setWidget->setMaximumWidth(80);
    QHBoxLayout *widgetLayout=new QHBoxLayout();
    widgetLayout->addWidget(widgetL);
    widgetLayout->addWidget(widget);
    widgetLayout->addWidget(setWidget);

    QHBoxLayout *tableLayout = new QHBoxLayout;
    tableLayout->addWidget(catalogLabel);
    tableLayout->addWidget(schemaLabel);
    tableLayout->addWidget(tableLabel);

    tableView=new QTableView(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(messageLabel);
    layout->addWidget(label);
    layout->addLayout(widgetLayout);
    layout->addLayout(tableLayout);
    layout->addWidget(tableView);
    tableView->setModel(attributesModel);

    selectAllItems=new QCheckBox("Select All",this);
    setAllWidgitsToLineEdit=new QCheckBox("Set to QlineEdit",this);

    QHBoxLayout *footerLayout = new QHBoxLayout;
    footerLayout->addWidget(selectAllItems);
    footerLayout->addWidget(setAllWidgitsToLineEdit);

    layout->addLayout(footerLayout);
    setLayout(layout);
    testIfAllSelected();
    testIfAllQLineEdits();
    connect(setWidget,SIGNAL(pressed()),this,SLOT(setWidgets()));
    connect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    connect(selectAllItems,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
    connect(setAllWidgitsToLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
    fillWidgetsCombo();
}



void ClassAttributes::fillWidgetsCombo()
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


void ClassAttributes::setWidgets()
{
    QModelIndexList selectedList=tableView->selectionModel()->selectedIndexes();
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
        attributesModel->item(selectedIndex.row(),AttribColumnWidgetItem)->setText(theWidget);
        attributesModel->item(selectedIndex.row(),AttribColumnWidgetItem)->setData(theWidgetId,Qt::UserRole+AttribDataWidgetValue);
    }

}



void ClassAttributes::itemChanged(QStandardItem *item)
{
    disconnect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));

    QStandardItem *mainItem=attributesModel->item(item->row(),AttribColumnItem);
    mainItem->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
    mainItem->setData(EditMode,Qt::UserRole+ModeItemStatus);
    testIfAllSelected();
    testIfAllQLineEdits();

    connect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));

    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *item=attributesModel->item(row);
        if (item->checkState()==Qt::Checked){
            complete=true;
            emit completeChanged();
            return;
        }
    }

    complete=false;
    emit completeChanged();
}



void ClassAttributes::setAllSelected(int state)
{
    disconnect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *item=attributesModel->item(row);
        item->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
        item->setData(EditMode,Qt::UserRole+ModeItemStatus);

        if(state==Qt::Checked){
            item->setCheckState(Qt::Checked);
            complete=true;
        }else{
            item->setCheckState(Qt::Unchecked);
            complete=false;
        }
    }
    emit completeChanged();
    connect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}




void ClassAttributes::setAllQLineEdit(int state)
{
    disconnect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
    QVariant QLineEditValue=1;

    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *widgetItem=attributesModel->item(row,AttribColumnWidgetItem);
        if(state==Qt::Checked){
            widgetItem->setText("QLineEdit");
            widgetItem->setData(QLineEditValue,Qt::UserRole+AttribDataWidgetValue);
        }else{
            QStandardItem *item=attributesModel->item(row);
            QVariant dataType=item->data(Qt::UserRole+AttribDataDataType);
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
            widgetItem->setData(widgetValue,Qt::UserRole+AttribDataWidgetValue);
        }
    }
    connect(attributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}





bool ClassAttributes::isComplete() const
{
    return complete;
}




void ClassAttributes::testIfAllSelected()
{
    disconnect(selectAllItems,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *item=attributesModel->item(row);
        if (item->checkState()==Qt::Unchecked){
            selectAllItems->setChecked(false);
            return;
        }
    }
    selectAllItems->setChecked(true);
    connect(selectAllItems,SIGNAL(stateChanged(int)),this,SLOT(setAllSelected(int)));
}


void ClassAttributes::testIfAllQLineEdits()
{
    disconnect(setAllWidgitsToLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *item=attributesModel->item(row,AttribColumnWidgetItem);
        if (item->text()!="QLineEdit"){
            setAllWidgitsToLineEdit->setChecked(false);
            return;
        }
    }
    setAllWidgitsToLineEdit->setChecked(true);
    connect(setAllWidgitsToLineEdit,SIGNAL(stateChanged(int)),this,SLOT(setAllQLineEdit(int)));
}
