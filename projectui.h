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



#ifndef PROJECTUI_H
#define PROJECTUI_H


#include <QtCore/QVariant>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
//#include <QtWidgets/QPushButton>
//#include <QtWidgets/QSpacerItem>
//#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QSqlDatabase>
#include <QtWidgets/QProgressBar>


class QStandardItemModel;
class QStandardItem;
class Interface;

class ProjectUi : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectUi(QString pProjectName, QString pPath, QTabWidget *parent = 0);
    QString projectName();
    QString projectPath();
    QLineEdit *className;
    //    QLineEdit *projectLocation;
    //    QLineEdit *database;
    QLineEdit *schema;
    QLineEdit *table;


    QVBoxLayout *mainLayout;
    QGroupBox *projectGB;
    QGridLayout *projectGbLayout;
    QHBoxLayout *toolBarLayout;
    //    QSpacerItem *toolbarSpacerL;
    //    QPushButton *editProjectB;
    //    QSpacerItem *toolbarSpacerR;
    QTextBrowser *projectSummary;

    QProgressBar *filesIndecator;
    void newClass(QMap<int, QString> classInfo,QStandardItemModel *conclusion);
    //    void setTableName(QString pdatabase, QString pSchemaName, QString pTableName);
    bool openProject(QString db_file);
    void saveProject();
    void generate(int templateId, int file);
    void modelClone(QStandardItemModel *newModel);
    QStandardItemModel *classesModel;
    QStandardItemModel *classesAttributesModel(QString className);
    void editClassesAttributes(QStandardItemModel *attributesModel);
    void removeClass(QModelIndex index);
    void clean();
    bool isProjectChanged();
    bool createProject();

signals:

public slots:
private:
    QTabWidget *sohagTabWidget;
    QString name;
    QString path;
    QStandardItemModel *columnsModel;
    QSqlDatabase sohagDB;
    QSqlDatabase infoDB;
    //    QString tableName;
    //    QString schemaName;
    void writefile(QString pFileName, QString &pText, bool useUtf=true);
    void generate(QStandardItem *classItem, int templateId, int file);
    void createProFile(QStringList classesNames);
    void updateModel(QStandardItemModel *conculsionModel);
    void addClassInfo(QMap<int,QString> classInfo);
    int status; // General will be marked changes if the project itself or any classes or items data changed
    int projectStatus; //Reflects changes in Project Data only will be marked changes if the project itself but nothing else
    int mode;
    void setProjectStatus(int newStatus);
    bool editProject(QSqlQuery &query);
    bool createClass(QSqlQuery &query, QStandardItem *item);
    bool editClass(QSqlQuery &query, QStandardItem *item);
    bool createAttribute(QSqlQuery &query);
    bool editAttribute(QSqlQuery &query);
    bool deleteClass(QString className);
    bool isWidgetNameDuplicated(QString widgetName);
    void generateSummary();
    void createMainFile(QString defaultClassName);
    Interface *templateInterface;
};

#endif // PROJECTUI_H
