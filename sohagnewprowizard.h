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




#ifndef SOHAGNEWPROWIZARD_H
#define SOHAGNEWPROWIZARD_H

#include <QWizard>
#include <QSqlQuery>
#include <QVariant>



class QLabel;
class QLineEdit;
class QVBoxLayout;
class QGridLayout;
class QStandardItem;
class TableInfoPage;
class QCheckBox;
class QTableView;
class QComboBox;
class SchemaInfo;

class SohagNewProWizard : public QWizard
{
    Q_OBJECT
public:
    SohagNewProWizard(QString defPath, QWidget *parent = 0);
    void accept() Q_DECL_OVERRIDE;
    QString proProjectName();
    QString proLocation();
private:
    QString projectName;
    QString projectLocation;
};


class WelcomePageA : public QWizardPage
{
    Q_OBJECT

public:
    WelcomePageA(QWidget *parent = 0);

private:
    QLabel *label;
};


class LocationPage : public QWizardPage
{
    Q_OBJECT

public:
    LocationPage(QString defaultPath,QWidget *parent = 0);
    bool isComplete() const;
    void cleanupPage() Q_DECL_OVERRIDE;
private:
    QLabel *messageLabel;
    QLabel *label;
    QLabel *projectNameL;
    QLineEdit *projectName;
    QLabel *projectLocationL;
    QLineEdit *projectLocation;
    QVBoxLayout *mainLayout;
    bool complete;
    void createProject();
private slots:
    void testPathName();
    void selectedFolderName();
};




#endif // SOHAGNEWPROWIZARD_H
