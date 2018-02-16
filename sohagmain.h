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




#ifndef SOHAGMAIN_H
#define SOHAGMAIN_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSqlQuery>
#include <QNetworkReply>

class QStandardItem;
class QStandardItemModel;
class ConnectionDialog;
class FileDownloader;
class ProjectUi;
class DatabaseInterface;

namespace Ui {
class SohagMain;
}

class SohagMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit SohagMain(QWidget *parent = 0);
    ~SohagMain();

private:
    Ui::SohagMain *ui;
    QString currentDataBase;
    QString lastDirLocation;
    QStandardItemModel *metaDataTreeModel;
    QStandardItemModel *wizardModel;

    void addNewProject(QStandardItem *item);
    void addProjectTab(QString projecName, QString location);
    QSqlDatabase infoDB;
    QSqlDatabase sohagDB;
    QSqlQuery infoQuery;
    ConnectionDialog *connectionDialog;
    QString defaultPath();
    void SohagNews();
    FileDownloader *downloader ;
    bool isDownloadFail;
    int defaultTemplateId;
    ProjectUi *currentProject;
    void newClass(QStringList tableInfo);
    void setNews(QString newsText);
    QString storedNews;
    DatabaseInterface *databaseInterface;
private slots:
    void delayedActions();
    bool connectToDB();
    void loadMetaData();
    void loadTablesMetaData(QStandardItem *parentItem);
    void newProject(QModelIndex index);
    void newClassFromItem(QModelIndex index);
    void addNewProject();
    void openProject();
    void saveProject();
    void generatAll();
    //    void generateUi();
    //    void generateClass();
    void editProject();
    void addNews();
    void error(QNetworkReply::NetworkError);
    void newClass();
    void populateClassesTree();
    void editClass();
    void scrollToClass(QModelIndex index);
    void openSohagDeveloperForum();
    void aboutSohagDeveloper();
    void openProjectInQt();
    void removeClass();
    void clean();
    void openProjectLocation();
    bool closeProject();
    void closeEvent(QCloseEvent *event);
};

#endif // SOHAGMAIN_H
