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




#ifndef SOHAGEDITPROWIZARD_H
#define SOHAGEDITPROWIZARD_H

#include <QWizard>
#include <QSqlQuery>
#include <QVariant>



class QLabel;
class QLineEdit;
class QVBoxLayout;
class QGridLayout;
class QStandardItemModel;
class QStandardItem;
class EditTableInfoPage;
class QCheckBox;
class QTableView;

class SohagEditProWizard : public QWizard
{
    Q_OBJECT

public:
    SohagEditProWizard(QString pschemaName, QString ptableName, QSqlQuery &infoQuery, QStandardItemModel *dataBaseModel, QWidget *parent = 0);
    void accept() Q_DECL_OVERRIDE;
    EditTableInfoPage *tableInfoPage;
    QStandardItemModel *conclusion();
    QString proClassName();
    QString proProjectName();
    QString proLocation();
    QString proSchemaName();
    QString proTableName();
    bool setAllLineEdit;
    bool selectAll;

private:
    QStandardItemModel *summaryModel;
    QString className;
    QString projectName;
    QString projectLocation;
    QString schemaName;
    QString tableName;
};


class EditWelcomePage : public QWizardPage
{
    Q_OBJECT

public:
    EditWelcomePage(QWidget *parent = 0);

private:
    QLabel *label;
};






class EditTableInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    EditTableInfoPage(QString pschema,QString ptableName,QSqlQuery &pinfoQuery,QStandardItemModel *model,QWidget *parent = 0);
    bool isComplete() const;
    QStandardItemModel *columnsModel;
    void cleanupPage() Q_DECL_OVERRIDE;
protected:
    void initializePage() Q_DECL_OVERRIDE;
private:
    bool complete;
    QLabel *messageLabel;
    QLabel *label;
    QCheckBox *setAllLineEdit;
    QCheckBox *selectAll;
    QTableView *view;
    QVBoxLayout *tableInfoLayout;
    void tableData(QSqlQuery &infoQuery);
    void columnChecked(bool state);
    QString schema;
    QString tableName;
    QSqlQuery infoQuery;
    QVariant QLineEditValue;
    bool isSchemaInfoPageHidden;
private slots:
    void itemChanged(QStandardItem *item);
    void setAllSelected(int state);
    void setAllQLineEdit(int state);
};


class EditClassInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    EditClassInfoPage(QString ptableName, QWidget *parent = 0);
    void initializePage() Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
private:
    QString tableName;
    QLabel *messageLabel;
    QLabel *label;
    QLabel *classNameL;
    QLineEdit *className;
    QGridLayout *mainLayout;
};


#endif // SOHAGEDITPROWIZARD_H
