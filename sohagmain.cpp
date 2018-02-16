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



#include "sohagmain.h"
#include "ui_sohagmain.h"
#include "connectiondialog.h"
#include "sohaggen.h"
#include <QStandardItemModel>
#include <QSqlError>
#include "sohagnewprowizard.h"
#include "sohageditprowizard.h"
#include <QDir>
#include <QStandardPaths>
#include "projectui.h"
#include <QLineEdit>
#include <QFileDialog>
#include "filedownloader.h"
#include <QTimer>
//#include "sohagnewclasswizard.h"
#include "sohageditclass.h"
#include <QDesktopServices>
#include <QToolButton>
#include <QCloseEvent>
#include "aboutsohagdeveloper.h"
#include "sohagnewclass.h"
#include "databaseinterface.h"
#include <QtWidgets>


SohagMain::SohagMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SohagMain)
{
    ui->setupUi(this);
    ui->sohagSideNavigation->setVisible(false);
    ui->connectionsDock->setVisible(false);
    ui->projectsDock->setVisible(false);
    currentProject=0;
    QTimer::singleShot(400, this, SLOT(delayedActions()));
}



SohagMain::~SohagMain()
{
    delete ui;
}



bool SohagMain::connectToDB()
{
    if(!databaseInterface){
        QMessageBox::warning(this,"Sohag Developer Error","One or more configuration files are deleted or corrupted\n Reinstalling Sohag Developer may solve the problem ");
        return false;
    }
    if(!connectionDialog)
        connectionDialog=new ConnectionDialog(this);
    if(connectionDialog->exec()){
        currentDataBase=connectionDialog->databaseName();
        loadMetaData();
        ui->connectionsDock->setVisible(true);
        return true;
    }
    return false;
}




void SohagMain::loadMetaData()
{
    if(metaDataTreeModel){
        metaDataTreeModel->removeRows(0,metaDataTreeModel->rowCount());
        wizardModel->removeRows(0,wizardModel->rowCount());
    }else{
        metaDataTreeModel=new QStandardItemModel(this);
        wizardModel=new QStandardItemModel(this);
    }
    QSqlQuery metaSchemaQuery;
    metaSchemaQuery.prepare(databaseInterface->schemaQuery());
    metaSchemaQuery.addBindValue(currentDataBase);
    metaSchemaQuery.exec();
    if(metaSchemaQuery.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",tr("Sohag Developer can't load Data from database"));
        return;
    }
    QStandardItem *rootItem=new QStandardItem(currentDataBase);
    metaDataTreeModel->setItem(0,rootItem);
    rootItem->setData(Database);
    QList<QStandardItem *> items;
    while (metaSchemaQuery.next()) {
        QStandardItem *item=new QStandardItem(metaSchemaQuery.value(0).toString());
        item->setData(Schema);
        items.append(item);
        loadTablesMetaData(item);
        //        metaDataTreeModel->setItem(metaDataTreeModel->rowCount(),item);
    }
    rootItem->appendRows(items);

    metaDataTreeModel->setHeaderData(0,Qt::Horizontal,"Database Info");
    ui->metaDataTree->setModel(metaDataTreeModel);

    wizardModel->setHeaderData(0,Qt::Horizontal,"Table/View");
    wizardModel->setHeaderData(1,Qt::Horizontal,"Schema");
    wizardModel->setHeaderData(2,Qt::Horizontal,"Type");

}






void SohagMain::loadTablesMetaData(QStandardItem *parentItem)
{
    QString schemaName=parentItem->text();
    QSqlQuery metaTablesQuery;
    metaTablesQuery.prepare(databaseInterface->tableQuery());
    metaTablesQuery.addBindValue(schemaName); // Schema Name
    metaTablesQuery.exec();
    if(metaTablesQuery.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error",tr("Sohag Developer can't load Data from database"));
    }
    int row=wizardModel->rowCount();
    QList<QStandardItem *> tableItems;
    QList<QStandardItem *> viewItems;
    while (metaTablesQuery.next()) {
        QStandardItem *item=new QStandardItem(metaTablesQuery.value(0).toString());
        item->setData(schemaName,Qt::UserRole);
        item->setData(metaTablesQuery.value(1),Qt::UserRole+2);

        if(metaTablesQuery.value(1).toString()=="BASE TABLE"){
            item->setData(Table,Qt::UserRole+1);
            tableItems.append(item);
        }else if(metaTablesQuery.value(1).toString()=="VIEW"){
            item->setData(View,Qt::UserRole+1);
            viewItems.append(item);
        }
        QStandardItem *WizardItem=item->clone();
        WizardItem->setCheckable(true);
        QStandardItem *wizardSchemaItem=new QStandardItem(schemaName);
        QStandardItem *wizardTableTypeItem=new QStandardItem(metaTablesQuery.value(1).toString());
        wizardModel->setItem(row,0,WizardItem);
        wizardModel->setItem(row,1,wizardSchemaItem);
        wizardModel->setItem(row,2,wizardTableTypeItem);
        ++row;
    }
    QStandardItem *tablesItem=new QStandardItem("Tables");
    tablesItem->appendRows(tableItems);
    QStandardItem *viewsItem=new QStandardItem("Views");
    viewsItem->appendRows(viewItems);

    parentItem->appendRow(tablesItem);
    parentItem->appendRow(viewsItem);
}




void SohagMain::newProject(QModelIndex index)
{
    QStandardItem *currentItem=metaDataTreeModel->itemFromIndex(index);
    int itemType=currentItem->data(Qt::UserRole+1).toInt();
    if(itemType==Table || itemType==View){
        //        currentItem->setData(path,Qt::UserRole+2);
        addNewProject(currentItem);
    }
    //    QString schema=item->data(Qt::UserRole).toString();
    //    QString tabel=item->text();
}



void SohagMain::addNewProject(QStandardItem *item)
{
    //    QString schema=item->data(Qt::UserRole).toString();
    //    QString tabel=item->text();
    // schema,tabel,infoQuery
    SohagNewProWizard *newProWiz=new SohagNewProWizard(defaultPath(),this);
    if(newProWiz->exec()){
        QDir proDir;
        //        QMap<int,QString> classData=newProWiz->classData();
        QString proPath=newProWiz->proLocation()+"/"+newProWiz->proProjectName();
        proDir.setPath(proPath);

        if(!proDir.exists() && !proDir.mkdir(proPath)){
            QMessageBox::critical(this,"Create Project Error","Sohag Developer Can't Create Project Files");
            return;
        }
        //        classData.insert(CatalogNameClassInfo,currentDataBase);
        //        classData.insert(ClassTypeClassInfo,"Master"); // Master / Detail To be changed later
        //        classData.insert(TableTypeClassInfo,item->data(Qt::UserRole+AttribDataTableType).toString());
        //        classData.insert(ModeItemStatus,QString::number(CreateMode));
        //        classData.insert(StatusItemStatus,QString::number(ChangedStatus));


        addProjectTab(newProWiz->proProjectName(),proPath);
    }
}



void SohagMain::addNewProject()
{
    if(!connectionDialog){
        if (!connectToDB()){
            return;
        }
    }else if(currentDataBase.isEmpty()){
        QMessageBox::warning(this,"Connection Error","Sohag Developer can't connect to database.\nPlease connect to database first");
        return;
    }

    SohagNewProWizard *newProWiz=new SohagNewProWizard(defaultPath(),this);
    if(newProWiz->exec()){
        //        QMap<int,QString> classData=newProWiz->classData();
        QString proPath=newProWiz->proLocation()+"/"+newProWiz->proProjectName();
        QDir proDir;
        proDir.setPath(proPath);

        if(!proDir.exists() && !proDir.mkdir(proPath)){
            QMessageBox::critical(this,"Create Project Error","Sohag Developer Can't Create Project Files");
            return;
        }
        //        classData.insert(CatalogNameClassInfo,currentDataBase);
        //        classData.insert(ClassTypeClassInfo,"Master"); // Master / Detail To be changed later
        //        classData.insert(ModeItemStatus,QString::number(CreateMode));
        //        classData.insert(StatusItemStatus,QString::number(ChangedStatus));


        addProjectTab(newProWiz->proProjectName(),proPath);
        //         Uncheck last checked items
        QStandardItem *zeroItem=wizardModel->item(0,0);
        //         Only Allow one Item to be checked.
        //         Last checked item data stored in the zeroItem
        //         UnCheck the last item checked if found. Then set the current item as the last item checked
        QModelIndex index=zeroItem->data(Qt::UserRole+LastCheckedItemIndex).toModelIndex();
        if(index.isValid()){
            wizardModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
        }
    }
}



void SohagMain::editProject()
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Error While trying to save the project");
        return;
    }

    QStandardItemModel *editModel=new QStandardItemModel(this);
    currentPui->modelClone(editModel);
    SohagEditProWizard *sohagEditProWizard=new SohagEditProWizard(currentPui->schema->text(),
                                                                  currentPui->table->text(),infoQuery,editModel,this);
    if(sohagEditProWizard->exec()){
        currentPui->className->setText(sohagEditProWizard->proClassName());

        //        currentPui->updateModel(editModel);
    }
}



void SohagMain::clean()
{
    if(!currentProject){
        return;
    }

    currentProject->clean();
}


QString SohagMain::defaultPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}


void SohagMain::addProjectTab(QString projecName,QString location)
{
    ProjectUi *pUi=new ProjectUi(projecName,location,ui->sohagMainTab);
    if(! pUi->createProject()){
        QMessageBox::warning(this,"Sohag Developer","Sohag Developer can't create project");
        return;
    }
    int tabIndex=ui->sohagMainTab->addTab(pUi,projecName);
    //    pUi->newClass(classInfo,conclusion);
    ui->sohagMainTab->setCurrentIndex(tabIndex);
    newClass();
    populateClassesTree();
    ui->projectsDock->setVisible(true);
}



void SohagMain::newClassFromItem(QModelIndex index)
{
    QStandardItem *currentItem=metaDataTreeModel->itemFromIndex(index);
    int itemType=currentItem->data(Qt::UserRole+1).toInt();
    QStringList tableInfo;
    if(itemType==Table || itemType==View){
        tableInfo.append(currentItem->data(Qt::UserRole).toString());
        tableInfo.append(currentItem->text());
        QString tableType;
        if(itemType==Table){
            tableType="BASE TABLE";
        }else{
            tableType="VIEW";
        }
        tableInfo.append(tableType);
    }else{
        return;// If item type not atable or view then do nothing
    }
    newClass(tableInfo);
}


void SohagMain::newClass()
{
    QStringList dummyList;
    newClass(dummyList);
}

void SohagMain::newClass(QStringList tableInfo)
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::information(this,"Sohag Developer","Please create a project first");
        return;
    }

    if(!connectionDialog || currentDataBase.isEmpty()){
        if (!connectToDB()){
            return;
        }
    }else if(currentDataBase.isEmpty()){
        QMessageBox::warning(this,"Connection Error","Sohag Developer can't connect to database.\nPlease connect to database first");
        return;
    }
    QStandardItemModel *classesModel=currentPui->classesModel;

    //    SohagNewClassWizard  *newclassWiz=new SohagNewClassWizard(infoQuery,wizardModel,classesModel,this);
    SohagNewClass  *newclassWiz=new SohagNewClass(infoQuery,wizardModel,classesModel,tableInfo,this);
    if(newclassWiz->exec()){
        QMap<int,QString> classData=newclassWiz->classData();

        classData.insert(CatalogNameClassInfo,currentDataBase);
        classData.insert(ClassTypeClassInfo,"Master"); // Master / Detail To be changed later
        classData.insert(ModeItemStatus,QString::number(CreateMode));
        classData.insert(StatusItemStatus,QString::number(ChangedStatus));
        currentPui->newClass(classData,newclassWiz->conclusion());

        // Uncheck last checked items
        QStandardItem *zeroItem=wizardModel->item(0,0);
        // Only Allow one Item to be checked.
        // Last checked item data stored in the zeroItem
        // UnCheck the last item checked if found. Then set the current item as the last item checked
        QModelIndex index=zeroItem->data(Qt::UserRole+LastCheckedItemIndex).toModelIndex();
        if(index.isValid()){
            wizardModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
        }
    }
}


void SohagMain::openProject()
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString projectName = QFileDialog::getOpenFileName(this,
                                                       tr("Open Project"),path, tr("Sohag Developer Project (*.shg)"));
    if(projectName.isEmpty()){
        return;
    }

    ProjectUi *pUi=new ProjectUi("","",ui->sohagMainTab);
    if(pUi->openProject(projectName)){
        int tabIndex=ui->sohagMainTab->addTab(pUi,pUi->projectName());
        ui->sohagMainTab->setCurrentIndex(tabIndex);
        populateClassesTree();
        ui->projectsDock->setVisible(true);
    }
}



void SohagMain::saveProject()
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Error While trying to save the project");
        return;
    }
    currentPui->saveProject();
}


void SohagMain::generatAll()
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Error While trying to generate the project files");
        return;
    }

    int templateId=defaultTemplateId;
    QAction *generateAction = qobject_cast<QAction*>(sender());

    if(generateAction){
        templateId=generateAction->data().toInt();
    }

    currentPui->generate(templateId,AllFile);


}



//void SohagMain::generateUi()
//{
//    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
//    if(!currentPui){
//        QMessageBox::warning(this,"Sohag Developer Error","Error While trying to generate the project gui file");
//        return;
//    }
//    currentPui->generate(UiFile);
//}



//void SohagMain::generateClass()
//{
//    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
//    if(!currentPui){
//        QMessageBox::warning(this,"Sohag Developer Error","Error While trying to generate the class code");
//        return;
//    }

//    currentPui->generate(ClassFiles);
//}



void SohagMain::delayedActions()
{
    //---------------------------------------------------------------------------------------------
    metaDataTreeModel=0;
    ui->metaDataTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->mainToolBar->addAction(ui->actionNew_Connection);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionNew_Pro_ject);
    ui->mainToolBar->addAction(ui->actionNew_Class);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_Open);
    ui->mainToolBar->addAction(ui->action_Save);
    ui->mainToolBar->addSeparator();
    //    ui->mainToolBar->addAction(ui->actionGenerate);
    QToolButton *generateButton=new QToolButton(this);
    generateButton->setText("Generate Options");
    ui->mainToolBar->addWidget(generateButton);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionClose_Project);
    ui->mainToolBar->addAction(ui->actionQuit);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionOnline_help);

    connect(ui->actionNew_Connection,SIGNAL(triggered(bool)),this,SLOT(connectToDB()));
    connect(ui->metaDataTree,SIGNAL(activated(QModelIndex)),this,SLOT(newClassFromItem(QModelIndex)));

    infoDB= QSqlDatabase::addDatabase("QSQLITE","info_con");
    //  infoDB.setDatabaseName("C:/Users/Narmar/Desktop/sohag-info.sys"); // For Debug
    infoDB.setDatabaseName(qApp->applicationDirPath()+"/sys/sohag-info.sys"); // For release
    infoDB.open();
    infoQuery=QSqlQuery(infoDB);
    infoQuery.prepare("select pg_widgets.widget_id,pg_widgets.widget from pg_widgets where pg_widgets.data_type=?");

    sohagDB = QSqlDatabase::addDatabase("QSQLITE","sohag_conn");
    //    sohagDB.open();
    connectionDialog=0;

    connect(ui->action_Open,SIGNAL(triggered(bool)),this,SLOT(openProject()));
    connect(ui->action_Save,SIGNAL(triggered(bool)),this,SLOT(saveProject()));
    connect(ui->actionNew_Pro_ject,SIGNAL(triggered(bool)),this,SLOT(addNewProject()));
    connect(ui->actionGenerate,SIGNAL(triggered(bool)),this,SLOT(generatAll()));
    //    connect(ui->actionGenerate_Classes_Only,SIGNAL(triggered(bool)),this,SLOT(generateClass()));
    //    connect(ui->actionGenerate_UI_only,SIGNAL(triggered(bool)),this,SLOT(generateUi()));
    connect(ui->action_EditProject,SIGNAL(triggered(bool)),this,SLOT(editProject()));
    connect(ui->actionNew_Class,SIGNAL(triggered(bool)),this,SLOT(newClass()));
    connect(ui->addClass,SIGNAL(pressed()),this,SLOT(newClass()));
    connect(ui->actionOpenProjectInQt,SIGNAL(triggered(bool)),this,SLOT(openProjectInQt()));
    connect(ui->actionOpen_Project_Location,SIGNAL(triggered(bool)),this,SLOT(openProjectLocation()));
    connect(ui->actionClean_Project,SIGNAL(triggered(bool)),this,SLOT(clean()));
    connect(ui->actionOnline_help,SIGNAL(triggered(bool)),this,SLOT(openSohagDeveloperForum()));
    connect(ui->action_About,SIGNAL(triggered(bool)),this,SLOT(aboutSohagDeveloper()));
    connect(ui->actionClose_Project,SIGNAL(triggered(bool)),this,SLOT(closeProject()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));


    connect(ui->sohagMainTab,SIGNAL(currentChanged(int)),this,SLOT(populateClassesTree()));
    //    connect(ui->classesTree,SIGNAL(clicked(QModelIndex)),this,SLOT(editClass(QModelIndex)));
    connect(ui->editClass,SIGNAL(pressed()),this,SLOT(editClass()));
    connect(ui->removeClass,SIGNAL(pressed()),this,SLOT(removeClass()));
    connect(ui->classesList,SIGNAL(activated(QModelIndex)),this,SLOT(scrollToClass(QModelIndex)));




    //    ui->mainToolBar->addAction(ui->action_About);

    //    ui->mainToolBar->insertAction(ui->actionGenerate_Classes_Only,ui->actionGenerate_UI_only);


    //---------------------------------------------------------------------------------------------
    SohagNews();

    QSqlQuery templatesQuery("",infoDB);
    QString templateQueryString="SELECT templates_id,template,default_template FROM templates ";
    // Only one template will be selected for light ed
    //        templateQueryString.append(" WHERE templates_id=1 LIMIT 0,1");
    templatesQuery.prepare(templateQueryString);
    templatesQuery.exec();
    if(templatesQuery.lastError().isValid()){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't load templates");
    }

    while(templatesQuery.next()){
        QAction *templatAction=new QAction(templatesQuery.value(1).toString(),this);
        templatAction->setData(templatesQuery.value(0));
        connect(templatAction,SIGNAL(triggered(bool)),this,SLOT(generatAll()));
        ui->menu_Generate->addAction(templatAction);
        if(templatesQuery.value(2).toInt()==1){
            defaultTemplateId=templatesQuery.value(0).toInt();
            ui->actionGenerate->setData(defaultTemplateId);
        }

    }

    generateButton->setMenu(ui->menu_Generate);
    generateButton->setPopupMode(QToolButton::MenuButtonPopup);
    generateButton->setDefaultAction(ui->actionGenerate);
    generateButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QString databasePlugIn=sohagDevDBSystem;
    QDir pluginsDir(qApp->applicationDirPath());
    if(QSysInfo::kernelType()=="linux"){
        databasePlugIn=databasePlugIn+".so";
    }else if (QSysInfo::kernelType()=="winnt") {
        databasePlugIn=databasePlugIn+".dll";
    }

    pluginsDir.cd("database");

    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(databasePlugIn));
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        databaseInterface = qobject_cast<DatabaseInterface *>(plugin);
    }else{
        databaseInterface=0;
    }
}


void SohagMain::SohagNews()
{
    int today=QDate::currentDate().dayOfYear();
    int lastDate;
    QSqlQuery newsQuery(infoDB);
    newsQuery.prepare("SELECT news_date,news FROM sohag_news WHERE news_id=1");
    newsQuery.exec();

    newsQuery.first();
    lastDate=newsQuery.value(0).toInt();
    storedNews=newsQuery.value(1).toString();
    if(newsQuery.lastError().isValid()){
        lastDate=35;
        storedNews="";
    }
    if(abs(today-lastDate)<sohagDevUpdatePeriod){
        setNews(newsQuery.value(1).toString());
        return;
    }

    isDownloadFail=false;
    downloader = new FileDownloader(QUrl("http://sohag-developer.com/postgreSQL-Basic-1.txt"),this);
    //    connect(downloader->replay,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(progressBarIndicate(qint64,qint64)));
    connect(downloader,SIGNAL(downloaded()),this,SLOT(addNews()));
    connect(downloader->replay,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(error(QNetworkReply::NetworkError)));
}


//;


void SohagMain::addNews()
{
    if(isDownloadFail){
        return;
    }
    QString downloadedData=downloader->downloadedData();
    QString signature=downloadedData.left(44);

    if(signature==sohagDevEditionSignature){
        downloadedData.replace(signature,"");
        QSqlQuery newsQuery(infoDB);
        newsQuery.prepare("Delete from sohag_news");
        newsQuery.exec();
        newsQuery.prepare("INSERT INTO sohag_news(news_id,news_date,news) VALUES (1,?,?)");
        newsQuery.addBindValue(QDate::currentDate().dayOfYear());
        newsQuery.addBindValue(downloadedData);
        newsQuery.exec();
    }else{
        downloadedData=storedNews;
    }
    //    ui->welcomeBrowser->setText(newHtml);
    setNews(downloadedData);
    isDownloadFail=false;
}



void SohagMain::error(QNetworkReply::NetworkError)
{
    isDownloadFail=true;
    //    QString newHtml=ui->welcomeBrowser->toHtml().replace("LOADING SOHAG DEVELOPER NEWS FROM THE SERVER","SOHAG DEVELOPER CAN'T RETRIVE DATA FROM THE SERVER");
    //    ui->welcomeBrowser->setHtml(newHtml);
    setNews(storedNews);
}



void SohagMain::setNews(QString newsText)
{
    QString newHtml=ui->welcomeBrowser->toHtml().replace("LOADING SOHAG DEVELOPER NEWS FROM THE SERVER",newsText);
    ui->welcomeBrowser->setText(newHtml);

}


void SohagMain::populateClassesTree()
{
    if(ui->sohagMainTab->currentIndex()==0){
        ui->projectsDock->setVisible(false);
        return;
    }
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't retrive the project classes list");
        return;
    }
    currentProject=currentPui;
    QStandardItemModel *classesModel=currentPui->classesModel;
    classesModel->setHeaderData(0,Qt::Horizontal,"Project Classes");
    ui->classesList->setModel(classesModel);
    ui->projectsDock->setVisible(true);
}


void SohagMain::editClass()
{
    QModelIndexList selectedList=ui->classesList->selectionModel()->selectedIndexes();
    QModelIndex selectedIndex;
    if(selectedList.size()>0){
        selectedIndex=selectedList.at(0);
    }else{
        QMessageBox::information(this,"Information","Please select the class you want to edit");
        return;
    }

    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't retrive the project classes list");
        return;
    }
    QStandardItemModel *classesModel=currentPui->classesModel;
    QStandardItem *classItem=classesModel->itemFromIndex(selectedIndex);
    QString className=classItem->data(Qt::UserRole+ClassNameClassInfo).toString();

    //    currentPui->projectSummary->scrollToAnchor(className);
    //    return;

    QStandardItemModel *classesAttributesModel=currentPui->classesAttributesModel(className);

    SohagEditClass *editClass=new SohagEditClass(selectedIndex,classesModel,classesAttributesModel,infoQuery,this);
    if(editClass->exec()){
        classItem->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);
        classItem->setData(EditMode,Qt::UserRole+ModeItemStatus);
        currentPui->editClassesAttributes(editClass->attributesModel());
    }
}


void SohagMain::removeClass()
{
    QModelIndexList selectedList=ui->classesList->selectionModel()->selectedIndexes();
    QModelIndex selectedIndex;
    if(selectedList.size()>0){
        selectedIndex=selectedList.at(0);
    }else{
        QMessageBox::information(this,"Information","Please select the class you want to edit");
        return;
    }



    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't generate project summary");
        return;
    }

    currentPui->removeClass(selectedIndex);
    ui->classesList->hideRow(selectedIndex.row());
    //    QStandardItemModel *classesModel=currentPui->classesModel;
    //    QStandardItem *classItem=classesModel->itemFromIndex(selectedIndex);

    //    classItem->setData(DeleteMode,Qt::UserRole+ModeItemStatus);
    //    classItem->setData(ChangedStatus,Qt::UserRole+StatusItemStatus);


    //    currentPui->generateSummary();
}


void SohagMain::scrollToClass(QModelIndex index)
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't retrive the project classes list");
        return;
    }
    QStandardItemModel *classesModel=currentPui->classesModel;
    QStandardItem *classItem=classesModel->itemFromIndex(index);
    QString className=classItem->data(Qt::UserRole+ClassNameClassInfo).toString();

    currentPui->projectSummary->scrollToAnchor(className);

}




void SohagMain::openSohagDeveloperForum() {
    QDesktopServices::openUrl(QUrl(sohagDeveloperForum));
}



void SohagMain::aboutSohagDeveloper()
{
    //    QMessageBox::information(this,"About Sohag Developer","Sohag Developer is licensed under LGPL v3 \nThanks for Qt & SQLite ");
    AboutSohagDeveloper *developer=new AboutSohagDeveloper(this);
    developer->exec();
}


void SohagMain::openProjectInQt()
{
    ProjectUi *currentPui=qobject_cast<ProjectUi*>(ui->sohagMainTab->currentWidget());
    if(!currentPui){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't open project");
        return;
    }
    QString proFile= currentPui->projectPath()+"/"+currentPui->projectName()+".pro";
    QFile file(proFile);
    if(!file.exists()){
        QMessageBox::warning(this,"Sohag Developer Error","Sohag Developer can't find Qt project file\nPlease make sure that you generate Qt files first");
    }

    QDesktopServices::openUrl("file:///"+proFile);

}


void SohagMain::openProjectLocation()
{
    if(!currentProject){
        return;
    }

    QString path=currentProject->projectPath();
    QDesktopServices::openUrl("file:///"+path);

}




bool SohagMain::closeProject()
{
    if(!currentProject || ui->sohagMainTab->currentIndex()==0){
        return false;
    }

    if (currentProject->isProjectChanged()){
        QMessageBox msgBox;
        msgBox.setText(QString("The project \"%1\" has been modified.").arg(currentProject->projectName()));
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            currentProject->saveProject();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }

    currentProject->close();
    ui->sohagMainTab->removeTab(ui->sohagMainTab->currentIndex());
    return true;
}



void SohagMain::closeEvent(QCloseEvent *event)
{

    if(ui->sohagMainTab->currentIndex()==0 && ui->sohagMainTab->count()>1){
        ui->sohagMainTab->setCurrentIndex(1);
    }

    while (ui->sohagMainTab->count()>1) {
        if(!closeProject()){
            event->ignore();
            return;
        }
    }
    event->accept();
}
