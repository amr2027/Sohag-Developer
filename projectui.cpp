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



#include <QtWidgets>
#include "projectui.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include "sohaggen.h"
#include <QDir>
#include <QTextStream>
#include <QGridLayout>
#include <QLineEdit>
//#include <QTabWidget>
#include "interface.h"


ProjectUi::ProjectUi(QString pProjectName,QString pPath,QTabWidget *parent) : QWidget(parent)
{
    sohagTabWidget=parent;
    name=pProjectName;
    path=pPath;
    //    projectLocation=new QLineEdit(this);
    //    projectLocation->setReadOnly(true);
    //    projectLocation->setFrame(false);
    //    QLabel *projectLocationL=new QLabel("Project Location",this);
    //    database=new QLineEdit(this);
    //    database->setReadOnly(true);
    //    database->setFrame(false);
    //    QLabel *databaseL=new QLabel("Database",this);
    //    schema=new QLineEdit(this);
    //    schema->setReadOnly(true);
    //    schema->setFrame(false);
    //    QLabel *schemaL=new QLabel("Schema",this);
    //    table=new QLineEdit(this);
    //    table->setReadOnly(true);
    //    table->setFrame(false);
    //    QLabel *tableL=new QLabel("Table",this);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(11, 11, 11, 11);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    projectGB = new QGroupBox("Summary",this);
    projectGB->setObjectName(QStringLiteral("projectGB"));
    projectGbLayout = new QGridLayout(projectGB);


    projectGbLayout->setObjectName(QStringLiteral("projectGbLayout"));


    //    projectGbLayout->addWidget(databaseL,0,0);
    //    projectGbLayout->addWidget(database,0,1);
    //    projectGbLayout->addWidget(schemaL,1,0);
    //    projectGbLayout->addWidget(schema,1,1);
    //    projectGbLayout->addWidget(tableL,2,0);
    //    projectGbLayout->addWidget(table,2,1);




    projectSummary = new QTextBrowser(projectGB);
    projectSummary->setObjectName(QStringLiteral("projectSummary"));

    projectGbLayout->addWidget(projectSummary,3,0,1,4);

    filesIndecator=new QProgressBar(this);

    mainLayout->addWidget(projectGB);
    mainLayout->addWidget(filesIndecator);

    classesModel=new QStandardItemModel(this);

    columnsModel=new QStandardItemModel(0,4,this);
    columnsModel->setHeaderData(AttribColumnItem,Qt::Horizontal,"Column Name");
    columnsModel->setHeaderData(AttribColumnLabelItem,Qt::Horizontal,"Label");
    columnsModel->setHeaderData(AttribColumnWidgetItem,Qt::Horizontal,"Widget");
    columnsModel->setHeaderData(AttribColumnClassItem,Qt::Horizontal,"Class Name");

    sohagDB=QSqlDatabase::database("sohag_conn",false);
    infoDB=QSqlDatabase::database("info_con");
    status=ChangedStatus;
    projectStatus=ChangedStatus;
    mode=CreateMode;


    sohagDB.setDatabaseName(path+"/"+name+".shg");
    sohagDB.open();
    sohagDB.transaction();

    //    QTableView *view=new QTableView(this);
    //    mainLayout->addWidget(view);
    //    view->setModel(columnsModel);
}


QString ProjectUi::projectName()
{
    return name;
}


QString ProjectUi::projectPath()
{
    return path;
}


void ProjectUi::newClass(QMap<int, QString> classInfo,QStandardItemModel *conclusion)
{
    addClassInfo(classInfo);
    QString className=classInfo[ClassNameClassInfo];
    //    int classNameColumn=conclusion->columnCount();
    //    conclusion->appendColumn();
    for (int row = 0; row < conclusion->rowCount(); ++row) {
        QStandardItem *item=conclusion->item(row,AttribColumnItem);
        item->setData(className,Qt::UserRole+ClassNameClassInfo);

        QString wedgitName=item->text();
        if(isWidgetNameDuplicated(wedgitName)){
            int widgetNamePostFix=2;
            wedgitName=item->text()+"_"+QString::number(widgetNamePostFix);
            while (isWidgetNameDuplicated(wedgitName)) {
                ++widgetNamePostFix;
                wedgitName=item->text()+"_"+QString::number(widgetNamePostFix);
            }
        }
        item->setData(wedgitName,Qt::UserRole+AttribDataWidgetName);

        QStandardItem *classItem=new QStandardItem(className);
        classItem->setEditable(false);
        conclusion->setItem(row,AttribColumnClassItem,classItem);
    }
    conclusion->setHeaderData(AttribColumnClassItem,Qt::Horizontal,"Class Name");
    updateModel(conclusion);
    setProjectStatus(ChangedStatus);
}


bool ProjectUi::isWidgetNameDuplicated(QString widgetName)
{
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row,AttribColumnItem);
        if(item->data(Qt::UserRole+AttribDataWidgetName).toString()==widgetName){
            return true;
        }
    }
    return false;
}



void ProjectUi::updateModel(QStandardItemModel *conculsionModel)
{
    //    columnsModel->clear();
    int columnModelLastrow =columnsModel->rowCount();
    //    int columnCount=columnsModel->columnCount();
    for (int row = 0; row < conculsionModel->rowCount(); ++row) {
        for (int column = 0; column < conculsionModel->columnCount(); ++column) {
            QStandardItem *item=conculsionModel->item(row,column)->clone();
            columnsModel->setItem(columnModelLastrow+row,column,item);
        }
        //        QStandardItem *item=conculsionModel->item(row,AttribColumnItem);

        //        QStandardItem *testItem=new QStandardItem(item->data(Qt::UserRole+AttribDataWidgetName).toString()); //conculsionModel->item(row,column)->clone();
        //        columnsModel->setItem(row,columnCount,testItem);
        //        columnsModel->setHeaderData(column,Qt::Horizontal,conculsionModel->headerData(column,Qt::Horizontal));
    }
    generateSummary();
}




void ProjectUi::addClassInfo(QMap<int, QString> classInfo)
{
    QStandardItem *item=new QStandardItem(classInfo.value(ClassNameClassInfo));

    item->setData(classInfo.value(ClassNameClassInfo),Qt::UserRole+ClassNameClassInfo);
    item->setData(classInfo.value(ClassTypeClassInfo),Qt::UserRole+ClassTypeClassInfo);
    item->setData(classInfo.value(ProjectNameClassInfo),Qt::UserRole+ProjectNameClassInfo);
    item->setData(classInfo.value(ProjectLocationClassInfo),Qt::UserRole+ProjectLocationClassInfo);
    item->setData(classInfo.value(CatalogNameClassInfo),Qt::UserRole+CatalogNameClassInfo);
    item->setData(classInfo.value(SchemaNameClassInfo),Qt::UserRole+SchemaNameClassInfo);
    item->setData(classInfo.value(TableNameClassInfo),Qt::UserRole+TableNameClassInfo);
    item->setData(classInfo.value(BaseClassClassInfo),Qt::UserRole+BaseClassClassInfo);
    item->setData(classInfo.value(TableTypeClassInfo),Qt::UserRole+TableTypeClassInfo);
    item->setData(classInfo.value(ModeItemStatus),Qt::UserRole+ModeItemStatus);
    item->setData(classInfo.value(StatusItemStatus),Qt::UserRole+StatusItemStatus);

    classesModel->appendRow(item);
    /////////////////////////////////////////////////////////////
    projectSummary->clear();
    generateSummary();
}



void ProjectUi::modelClone(QStandardItemModel *newModel)
{
    newModel->clear();

    for (int column = 0; column < columnsModel->columnCount(); ++column) {
        for (int row = 0; row < columnsModel->rowCount(); ++row) {
            QStandardItem *item=columnsModel->item(row,column)->clone();
            newModel->setItem(row,column,item);
        }
        newModel->setHeaderData(column,Qt::Horizontal,columnsModel->headerData(column,Qt::Horizontal));
    }
}



void ProjectUi::generateSummary()
{
    QString summary;
    summary.append("<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' 'http://www.w3.org/TR/REC-html40/strict.dtd'>\n<html><head><meta name='qrichtext' content='1' />"
                   "<style type='text/css'> table th {background-color:#00557F;color: white;} "
                   "table {border-width: 1px;border-style: solid;border-color:#0070A8;margin-top: 0px;margin-bottom: 0px;color: black;}"
                   "th,td {padding: 7px;} .odd{background-color:#FFFFFF;} .even{background-color:#E1EEf4;}"
                   "</style>"
                   "</head><body>");

    for (int row = 0; row < classesModel->rowCount(); ++row) {
        QStandardItem *classItem=classesModel->item(row);
        if(classItem->data(Qt::UserRole+ModeItemStatus).toInt()!=DeleteMode){
            QString className=classItem->data(Qt::UserRole+ClassNameClassInfo).toString();
            summary.append(QString("<a name='%1'></a>").arg(className));
            summary.append("<table style='width:100%;font-size:12pt;padding:80px;margin:20px;'>\n");
            QString classHeader=QString("<tr>\n<th colspan='6'>%5</th>\n</tr>\n\n<tr class='even'>\n<td>DataBase</td>\n<td>Schema</td>\n<td>Table/View Name</td>\n<td>Table Type</td>\n<td>Base Class</td>\n<td>Class Type</td>\n</tr>\n\n"
                                        "<tr class='odd'>\n<td>%1</td>\n<td>%2</td>\n<td>%3</td>\n<td style='text-transform:lowercase;'>%4</td>\n\n<td>%6</td>\n<td>%7</td>\n</tr>\n\n").arg(classItem->data(Qt::UserRole+CatalogNameClassInfo).toString(),classItem->data(Qt::UserRole+SchemaNameClassInfo).toString(),classItem->data(Qt::UserRole+TableNameClassInfo).toString(),classItem->data(Qt::UserRole+TableTypeClassInfo).toString(),className,classItem->data(Qt::UserRole+BaseClassClassInfo).toString(),classItem->data(Qt::UserRole+ClassTypeClassInfo).toString());
            summary.append(classHeader);

            //        const int classNameColumn=3;

            summary.append("<tr><td colspan='6' align='center' >Attributes (Columns)</td></tr><tr>\n<th>Column Name</th>\n  <th>Widget</th>\n  <th>Widget Name</th>\n  <th>Label</th>\n    <th>Used</th>\n    <th>Constraints</th>\n  </tr>\n");
            int selectedRows=0;
            for (int row = 0; row < columnsModel->rowCount(); ++row) {
                QStandardItem *item=columnsModel->item(row,AttribColumnItem);
                if(item->checkState()==Qt::Checked || item->data(Qt::UserRole+AttribDataConstraint)=="pk"){

                    QString itemClassName=columnsModel->item(row,AttribColumnClassItem)->text();
                    if(itemClassName==className){

                        QString used="No";
                        QString usedStyle="";
                        if(item->checkState()==Qt::Checked){
                            used="Yes";
                            usedStyle="style='color:#00557F;'";
                        }
                        QString rowClass="even";
                        if(selectedRows%2!=0){
                            rowClass="odd";
                        }
                        QString constraint=item->data(Qt::UserRole+AttribDataConstraint).toString();
                        QString widgetName=item->data(Qt::UserRole+AttribDataWidgetName).toString();

                        QStandardItem *labelItem=columnsModel->item(row,AttribColumnLabelItem);
                        QStandardItem *widgetItem=columnsModel->item(row,AttribColumnWidgetItem);
                        summary.append(QString("<tr class='%7' %5>\n    <td >%1</td>\n    <td>%2</td>\n  <td>%8</td>\n  <td>%3</td>\n    <td>%4</td>\n    <td>%6</td>\n  </tr>\n").arg(
                                           item->text(),widgetItem->text(),labelItem->text(),used,usedStyle,constraint,rowClass,widgetName));
                    }
                    ++selectedRows;
                }
            }
            summary.append("\n</table><p></p>");
        }
    }
    summary.append("\n</body>\n</html> ");
    projectSummary->setText(summary);
}




void ProjectUi::saveProject()
{
    if(status==UnChangedStatus){
        return;
    }


    QSqlQuery query("",sohagDB);

    filesIndecator->setMaximum(columnsModel->rowCount());
    filesIndecator->setFormat("Saving %p%");

    if(projectStatus==ChangedStatus){

        //        if(mode==CreateMode){
        //            if(!createProject(query)){
        //                return;
        //            }
        //        }else{
        if(!editProject(query)){
            return;
        }
        //        }
    }

    query.prepare("INSERT INTO `classes`(`class_id`,`class_name`,`base_class`,`class_type`,`catalog_name`,`schema_name`,`table_name`,`table_type`) VALUES (?,?,?,?,?,?,?,?)");
    QSqlQuery editClassQuery("",sohagDB);
    editClassQuery.prepare("UPDATE `classes` SET `class_name`=?,`base_class`=?,`class_type`=?,`catalog_name`=?,`schema_name`=?,`table_name`=?,`table_type`=? WHERE class_id=?");

    QSqlQuery attributeQuery("",sohagDB);
    attributeQuery.prepare("INSERT INTO columns(id,column_name,data_type,widget_id,widget,widget_name,label,used,pk,class_name) values (?,?,?,?,?,?,?,?,?,?)");
    QSqlQuery editAttributeQuery("",sohagDB);
    editAttributeQuery.prepare("UPDATE columns SET column_name=?,data_type=?,widget_id=?,widget=?,widget_name=?,label=?,used=?,pk=?,class_name=? WHERE id=?");

    QList <int> deletedRows;
    for (int row = 0; row < classesModel->rowCount(); ++row) {
        QStandardItem *item=classesModel->item(row);
        if(item->data(Qt::UserRole+StatusItemStatus).toInt()==ChangedStatus){
            if(item->data(Qt::UserRole+ModeItemStatus).toInt()==CreateMode){
                if (!createClass(query,item)){
                    return;
                }
                if (!createAttribute(attributeQuery)){
                    return;
                }
                item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
            }else if(item->data(Qt::UserRole+ModeItemStatus).toInt()==EditMode){
                if(!editClass(editClassQuery,item)){
                    return;
                }
                if(! editAttribute(editAttributeQuery)){
                    return;
                }
                item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
            }else if(item->data(Qt::UserRole+ModeItemStatus).toInt()==DeleteMode){
                if(!deleteClass(item->text())){
                    return;
                }
                deletedRows.append(row);
                //                classesModel->removeRow(row);
            }
        }
        sohagDB.commit();

        setProjectStatus(UnChangedStatus);
        mode=EditMode;
    }
    while (!deletedRows.isEmpty()){
        int row= deletedRows.takeLast();
        classesModel->removeRow(row);
    }
}


bool ProjectUi::createProject()
{
    QSqlQuery query("",sohagDB);
    query.exec("DROP TABLE IF EXISTS `columns`");
    query.exec("DROP TABLE IF EXISTS `classes`");
    query.exec("DROP TABLE IF EXISTS `projects`");

    query.exec("CREATE TABLE `columns` ( `id` INTEGER NOT NULL PRIMARY KEY, `column_name` TEXT NOT NULL, `data_type` TEXT NOT NULL, `widget_id` INTEGER NOT NULL, `widget` TEXT NOT NULL,`widget_name` TEXT NOT NULL,`label` TEXT NOT NULL, `used` TEXT NOT NULL,`pk` TEXT NOT NULL, `class_name` TEXT NOT NULL)");
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error ",query.lastError().text());
    }
    // backword_compatiblity is used to detrmine the oldest version of the program that caan open the file with out any problems
    // backword_compatiblity_read_only is used to detrmine the oldest version of the program that can open the file in read only mode -- This will be implemented later at the program but will be added to the file
    query.exec("CREATE TABLE `projects` ( `pro_id` INTEGER PRIMARY KEY AUTOINCREMENT, `pro_name` TEXT NOT NULL, `db_system` TEXT NOT NULL, `sohag_dev_version` double NOT NULL, `file_version` double NOT NULL, `backwards_compatibility` double NOT NULL, `backwards_compatibility_read_only` double NOT NULL)");
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
    }
    //    query.exec("CREATE TABLE `class_types` ( `class_type_id` INTEGER PRIMARY KEY AUTOINCREMENT, `class_type` TEXT NOT NULL)");
    query.exec("CREATE TABLE `classes` ( `class_id` INTEGER PRIMARY KEY, `class_name` TEXT NOT NULL UNIQUE,`base_class` TEXT NOT NULL,`class_type` TEXT NOT NULL,`catalog_name` TEXT NOT NULL,`schema_name` TEXT NOT NULL,`table_name` TEXT NOT NULL,`table_type` TEXT NOT NULL)");
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
    }

    query.prepare("	INSERT INTO `projects`(`pro_name`,`db_system`,`sohag_dev_version`,`file_version`,`backwards_compatibility`,`backwards_compatibility_read_only`) VALUES (?,?,?,?,?,?)");
    query.addBindValue(name);
    query.addBindValue(sohagDevDBSystem);
    query.addBindValue(sohagDevVersion);
    query.addBindValue(sohagDevFileVersion);
    query.addBindValue(sohagDevBackwardsCompatibility);
    query.addBindValue(sohagDevBackwardsCompatibilityReadOnly);
    //    query.addBindValue(projectLocation->text());
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
        sohagDB.rollback();
        return false;
    }
    projectStatus=UnChangedStatus;
    return true;
}


bool ProjectUi::editProject(QSqlQuery &query)
{
    query.prepare("UPDATE `projects` SET `pro_name`=?");
    query.addBindValue(name);
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
        sohagDB.rollback();
        return false;
    }
    projectStatus=UnChangedStatus;
    return true;
}



void ProjectUi::clean()
{
    //    QString mainFile
    QStringList errorStack;
    QFile file(path+"/main.cpp");
    if (!file.remove()){
        errorStack.append("Can't remove file \n"+file.fileName()+"\n"+file.errorString());
    }
    file.setFileName(path+"/"+name+".pro");
    if (!file.remove()){
        errorStack.append("Can't remove file \n"+file.fileName()+"\n"+file.errorString());
    }

    for (int row = 0; row < classesModel->rowCount(); ++row) {
        QStandardItem *classItem=classesModel->item(row);
        QString className=classItem->text().toLower();

        file.setFileName(path+"/"+className+".h");
        if (!file.remove()){
            errorStack.append("Can't remove file \n"+file.fileName()+"\n"+file.errorString());
        }
        file.setFileName(path+"/"+className+".cpp");
        if (!file.remove()){
            errorStack.append("Can't remove file \n"+file.fileName()+"\n"+file.errorString());
        }
        file.setFileName(path+"/"+className+".ui");
        if (!file.remove()){
            errorStack.append("Can't remove file \n"+file.fileName()+"\n"+file.errorString());
        }
    }

    if(!errorStack.isEmpty()){
        QMessageBox::warning(this,"Error While Removing File(s)",errorStack.join("\n"));
    }
}




bool ProjectUi::createClass(QSqlQuery &query,QStandardItem *item)
{
    int rowId=item->row()+1;
    query.addBindValue(rowId);
    query.addBindValue(item->data(Qt::UserRole+ClassNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+BaseClassClassInfo));
    query.addBindValue(item->data(Qt::UserRole+ClassTypeClassInfo));
    query.addBindValue(item->data(Qt::UserRole+CatalogNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+SchemaNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+TableNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+TableTypeClassInfo));
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
        sohagDB.rollback();
        return false;
    }
    item->setData(rowId,Qt::UserRole+RowID);
    item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
    return true;
}



bool ProjectUi::editClass(QSqlQuery &query,QStandardItem *item)
{
    query.addBindValue(item->data(Qt::UserRole+ClassNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+BaseClassClassInfo));
    query.addBindValue(item->data(Qt::UserRole+ClassTypeClassInfo));
    query.addBindValue(item->data(Qt::UserRole+CatalogNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+SchemaNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+TableNameClassInfo));
    query.addBindValue(item->data(Qt::UserRole+TableTypeClassInfo));
    query.addBindValue(item->data(Qt::UserRole+RowID));
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
        sohagDB.rollback();
        return false;
    }
    item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
    return true;
}


bool ProjectUi::createAttribute(QSqlQuery &query)
{
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row,AttribColumnItem);

        if(item->data(Qt::UserRole+StatusItemStatus)==ChangedStatus){
            int rowId=item->row()+1;
            query.addBindValue(rowId);
            query.addBindValue(item->text());
            query.addBindValue(item->data(Qt::UserRole+AttribDataDataType));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnWidgetItem)->data(Qt::UserRole+AttribDataWidgetValue));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnWidgetItem)->text());
            query.addBindValue(item->data(Qt::UserRole+AttribDataWidgetName));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnLabelItem)->text());
            QString used="false";
            if(item->checkState()==Qt::Checked){
                used="true";
            }
            query.addBindValue(used);
            QString isPk="false";
            if(item->data(Qt::UserRole+AttribDataConstraint)=="pk"){
                isPk="true";
            }
            query.addBindValue(isPk);
            query.addBindValue(item->data(Qt::UserRole+ClassNameClassInfo));

            query.exec();
            if(query.lastError().isValid()){
                QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
                sohagDB.rollback();
                return false;
            }
            item->setData(rowId,Qt::UserRole+RowID);
            item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);

        }
        filesIndecator->setValue(row+1);
    }
    return true;
}



bool ProjectUi::editAttribute(QSqlQuery &query)
{
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *item=columnsModel->item(row,AttribColumnItem);

        //"  UPDATE columns SET column_name=?,data_type=?,widget_id=?,widget=?,label=?,used=?,pk=?,class_name=? WHERE id=?
        if(item->data(Qt::UserRole+StatusItemStatus)==ChangedStatus){
            query.addBindValue(item->text());
            query.addBindValue(item->data(Qt::UserRole+AttribDataDataType));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnWidgetItem)->data(Qt::UserRole+AttribDataWidgetValue));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnWidgetItem)->text());
            query.addBindValue(item->data(Qt::UserRole+AttribDataWidgetName));
            query.addBindValue(columnsModel->item(item->row(),AttribColumnLabelItem)->text());
            QString used="false";
            if(item->checkState()==Qt::Checked){
                used="true";
            }
            query.addBindValue(used);
            QString isPk="false";
            if(item->data(Qt::UserRole+AttribDataConstraint)=="pk"){
                isPk="true";
            }
            query.addBindValue(isPk);
            query.addBindValue(item->data(Qt::UserRole+ClassNameClassInfo));
            query.addBindValue(item->data(Qt::UserRole+RowID));

            query.exec();
            if(query.lastError().isValid()){
                QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
                sohagDB.rollback();
                return false;
            }
            item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);

        }
        filesIndecator->setValue(row+1);
    }
    return true;

}


bool ProjectUi::deleteClass(QString className)
{
    QSqlQuery query("",sohagDB);

    query.prepare("DELETE FROM `columns` WHERE `class_name`=?");
    query.addBindValue(className);
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error","Error while saving project ");
        sohagDB.rollback();
        return false;
    }

    query.prepare("DELETE FROM `classes` WHERE `class_name`=?");
    query.addBindValue(className);
    query.exec();
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error","Error while saving project ");
        sohagDB.rollback();
        return false;
    }
    return true;
    //    classesModel->removeRow(classItem->row());
}


void ProjectUi::removeClass(QModelIndex index)
{
    QStandardItem *classItem=classesModel->itemFromIndex(index);
    classItem->setData(DeleteMode,Qt::UserRole+ModeItemStatus);
    classItem->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);

    //    classesModel->s(classItem->row());
    generateSummary();
    setProjectStatus(ChangedStatus);
}



bool ProjectUi::openProject(QString db_file)
{
    path=QFileInfo(db_file).absolutePath();
    sohagDB.setDatabaseName(db_file);

    if (!sohagDB.open()){
        QMessageBox::critical(this,"Error","This file is not recognized by Sohag Developer.\nOr It may be corrupted file");
        return false;
    }
    name=db_file;
    QSqlQuery query("",sohagDB);

    query.exec("select `pro_id`,`pro_name`,`db_system`,`sohag_dev_version`,`file_version`,`backwards_compatibility`,`backwards_compatibility_read_only` from projects");
    query.first();
    if(query.lastError().isValid()){
        QMessageBox::critical(this,"Error","This file is not recognized by Sohag Developer.\nOr It may be corrupted file");
        return false;
    }
    enum {a_pro_id,a_pro_name,a_db_system,a_sohag_dev_version,a_file_version,a_backwards_compatibility,a_backwards_compatibility_read_only};
    if(query.value(a_db_system).toString()!=sohagDevDBSystem){
        QMessageBox::critical(this,"Error","This file recognized by Sohag Developer but it is not for PostrgreSQL.\n"
                                           "Please use the Sohag Developer edition used to create that file");
        return false;
    }

    name=(query.value(a_pro_name).toString());
    //    projectLocation->setText(query.value(2).toString());

    query.exec("select class_id,class_name,base_class,class_type,catalog_name,schema_name,table_name,table_type from classes");
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
    }

    while (query.next()) {
        QStandardItem *item=new QStandardItem(query.value(1).toString());

        item->setData(query.value(0),Qt::UserRole+RowID);
        item->setData(query.value(1),Qt::UserRole+ClassNameClassInfo);
        item->setData(query.value(2),Qt::UserRole+BaseClassClassInfo);
        item->setData(query.value(3),Qt::UserRole+ClassTypeClassInfo);
        item->setData(query.value(4),Qt::UserRole+CatalogNameClassInfo);
        item->setData(query.value(5),Qt::UserRole+SchemaNameClassInfo);
        item->setData(query.value(6),Qt::UserRole+TableNameClassInfo);
        item->setData(query.value(7),Qt::UserRole+TableTypeClassInfo);
        item->setData(OpenMode,Qt::UserRole+ModeItemStatus);
        item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
        classesModel->appendRow(item);
    }


    enum{a_id,a_column_name,a_data_type,a_widget_id,a_widget,a_widget_name,a_label,a_used,a_pk,a_class_name};
    query.exec("select id,column_name,data_type,widget_id,widget,widget_name,label,used,pk,class_name from columns");
    if(query.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",query.lastError().text());
    }
    int row=0;
    while (query.next()) {
        QString columnName=query.value(a_column_name).toString();
        QStandardItem *item=new QStandardItem(columnName);
        item->setData(query.value(a_id),Qt::UserRole+RowID);
        QVariant dataType=query.value(a_data_type);
        item->setData(dataType,Qt::UserRole+AttribDataDataType);
        item->setData(OpenMode,Qt::UserRole+ModeItemStatus);
        item->setData(UnChangedStatus,Qt::UserRole+StatusItemStatus);
        if(query.value(a_pk).toBool()==true){
            item->setData("pk",Qt::UserRole+AttribDataConstraint);
        }
        item->setData(query.value(a_class_name),Qt::UserRole+ClassNameClassInfo);
        item->setCheckable(true);
        item->setEditable(false);
        if(query.value(a_used).toBool()){
            item->setCheckState(Qt::Checked);
        }
        item->setData(query.value(a_widget_name),Qt::UserRole+AttribDataWidgetName);
        columnsModel->setItem(row,AttribColumnItem,item);

        QString columnLabel=query.value(a_label).toString();
        QStandardItem *labelItem=new QStandardItem(columnLabel);
        columnsModel->setItem(row,AttribColumnLabelItem,labelItem);

        QString widget=query.value(a_widget).toString();
        QVariant widgetValue=query.value(a_widget_id);

        QStandardItem *widgetItem=new QStandardItem(widget);
        widgetItem->setData(widgetValue,Qt::UserRole+AttribDataWidgetValue);
        widgetItem->setEditable(false);
        columnsModel->setItem(row,AttribColumnWidgetItem,widgetItem);

        //        QString itemClassName=columnsModel->item(row,classNameColumn)->text();
        QStandardItem *classItem=new QStandardItem(query.value(a_class_name).toString());
        classItem->setEditable(false);
        columnsModel->setItem(row,AttribColumnClassItem,classItem);

        row++;
    }

    //        QTableView *view=new QTableView(this);
    //        mainLayout->addWidget(view);
    //        view->setModel(columnsModel);

    generateSummary();
    setProjectStatus(UnChangedStatus);
    mode=OpenMode;
    return true;
}


void ProjectUi::generate(int templateId,int file)
{
    QStringList classes;

    for (int row = 0; row < classesModel->rowCount(); ++row) {
        QStandardItem *item=classesModel->item(row);
        QString className=item->data(Qt::UserRole+ClassNameClassInfo).toString();
        generate(item,templateId,file);
        classes.append(className.toLower());
    }

    QStandardItem *item=classesModel->item(0);
    QString defaultClass=item->data(Qt::UserRole+ClassNameClassInfo).toString();

    createProFile(classes);
    createMainFile(defaultClass);
}



void ProjectUi::generate(QStandardItem *classItem,int templateId,int file)
{
    filesIndecator->reset();
    filesIndecator->setFormat("Generating Files %p%");
    filesIndecator->setMaximum(AllFile);
    QSqlQuery infoQuery("",infoDB);

    infoQuery.prepare("SELECT template_code FROM templates WHERE templates_id=?");
    infoQuery.addBindValue(templateId);
    infoQuery.exec();
    infoQuery.first();
    QString templateFile=infoQuery.value(0).toString();
    if(infoQuery.lastError().isValid()){
        templateFile="basic";
    }



    QString header;
    QString implentation;
    QString ui_text;
    QString classFileName;

    QDir pluginsDir(qApp->applicationDirPath());


    if(QSysInfo::kernelType()=="linux"){
        templateFile=templateFile+".so";
    }else if (QSysInfo::kernelType()=="winnt") {
        templateFile=templateFile+".dll";
    }

    pluginsDir.cd("data");


    //    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(templateFile));
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        templateInterface = qobject_cast<Interface *>(plugin);
        if (templateInterface)
            templateInterface->generateTemplate(infoQuery,classItem,columnsModel,templateId,header,implentation
                                                ,ui_text,classFileName,filesIndecator);
    }else{
        QMessageBox::warning(this,"Sohag Developer","Sohag Developer can't find the required template");
        return;
    }
    //    }



    pluginLoader.unload();

    if(file==UiFile || file==ClassFiles || file==AllFile ){
        writefile(classFileName+".ui",ui_text);
    }

    //    header.replace("*CLASSNAME*",className.toUpper());
    //    header.replace("*ClassName*",className);
    //    header.replace("*BaseClass*",baseClass);
    if(file==HeaderFile || file==ClassFiles || file==AllFile ){
        writefile(classFileName+".h",header);
    }

    if(file==ImplementationFile || file==ClassFiles || file==AllFile ){
        writefile(classFileName+".cpp",implentation);
    }

    filesIndecator->setValue(3);

}







void ProjectUi::createProFile(QStringList classesNames)
{
    QString pro;
    QSqlQuery query("",infoDB);
    query.prepare("SELECT code,file FROM `files` WHERE code='pro'");
    query.exec();
    query.first();
    pro=query.value(1).toString();

    QString headers=classesNames.join(".h\\\n\t\t");
    headers.append(".h");
    QString cpps=classesNames.join(".cpp\\\n\t\t");
    cpps.append(".cpp");
    QString uis=classesNames.join(".ui\\\n\t\t");
    uis.append(".ui");

    pro.replace("*projectname*",name.toLower());
    pro.replace("*classname.cpp*",cpps);
    pro.replace("*classname.h*",headers);
    pro.replace("*classname.ui*",uis);
    //    if(file==AllFile){
    writefile(name.toLower()+".pro",pro,false);
    //    }
}



void ProjectUi::createMainFile(QString defaultClassName)
{
    QString mainFunction;
    QSqlQuery query("",infoDB);
    query.prepare("SELECT code,file FROM `files` WHERE code='main'");
    query.exec();
    query.first();
    mainFunction=query.value(1).toString();

    mainFunction.replace("*classname*",defaultClassName.toLower());
    mainFunction.replace("*ClassName*",defaultClassName);
    mainFunction.replace("*dbConnectionFile*","");
    mainFunction.replace("*createConnection*","");

    writefile("main.cpp",mainFunction);
    filesIndecator->setValue(AllFile);
}


void ProjectUi::writefile(QString pFileName, QString &pText,bool useUtf)
{
    QString fileName =path+"/"+pFileName;
    QFile outFile(fileName);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&outFile);
        if(useUtf){
            out.setCodec("UTF-8");
            out.setGenerateByteOrderMark(true);
        }
        out <<  pText ;
        outFile.close();
    }else{
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't write file \n"+outFile.errorString());
    }
}



void ProjectUi::setProjectStatus(int newStatus)
{
    status=newStatus;
    int index=sohagTabWidget->indexOf(this);
    if(status==ChangedStatus && index>-1){
        sohagTabWidget->setTabText(index,"  * "+ name);
    }else{
        sohagTabWidget->setTabText(index,name);
    }
}


bool ProjectUi::isProjectChanged()
{
    if(status==ChangedStatus){
        return true;
    }
    return false;
}

QStandardItemModel *ProjectUi::classesAttributesModel(QString className)
{
    QStandardItemModel *attributesModel=new QStandardItemModel(this);
    int columnCount=columnsModel->columnCount();
    int attributesModelRow=0;
    for (int row = 0; row < columnsModel->rowCount(); ++row) {
        QStandardItem *baseItem=columnsModel->item(row,AttribColumnItem);
        if(className==baseItem->data(Qt::UserRole+ClassNameClassInfo).toString()){
            for (int column = 0; column <columnCount; ++column) {
                QStandardItem *item=columnsModel->item(row,column)->clone();
                item->setData(columnsModel->item(row,column)->index(),Qt::UserRole+AttribDataIndex);
                attributesModel->setItem(attributesModelRow,column,item);
            }
            ++attributesModelRow;
        }
    }

    for (int column = 0; column <columnCount; ++column) {
        attributesModel->setHeaderData(column,Qt::Horizontal,columnsModel->headerData(column,Qt::Horizontal));
    }

    return attributesModel;
}



void ProjectUi::editClassesAttributes(QStandardItemModel *attributesModel)
{
    setProjectStatus(ChangedStatus);
    for (int row = 0; row < attributesModel->rowCount(); ++row) {
        QStandardItem *attribItem=attributesModel->item(row,AttribColumnItem);
        if(attribItem->data(Qt::UserRole+StatusItemStatus)==ChangedStatus){
            QModelIndex index=attribItem->data(Qt::UserRole+AttribDataIndex).toModelIndex();
            QStandardItem *item=columnsModel->itemFromIndex(index);
            item->setCheckState(attribItem->checkState());
            item->setData(attribItem->data(Qt::UserRole+ClassNameClassInfo),Qt::UserRole+ClassNameClassInfo);
            item->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
            item->setData(EditMode,Qt::UserRole+ModeItemStatus);

            columnsModel->item(index.row(),AttribColumnWidgetItem)->setText(attributesModel->item(row,AttribColumnWidgetItem)->text());
            columnsModel->item(index.row(),AttribColumnLabelItem)->setText(attributesModel->item(row,AttribColumnLabelItem)->text());
            columnsModel->item(index.row(),AttribColumnClassItem)->setText(attributesModel->item(row,AttribColumnClassItem)->text());
        }
    }
    generateSummary();
}
