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




#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QWizardPage>

class QLabel;
class QLineEdit;
class QTableView;
class QGridLayout;
class QStandardItemModel;
class QStandardItem;


class DatabaseTable : public QWizardPage
{
    Q_OBJECT

public:
    DatabaseTable(QStringList tableInfo, QStandardItemModel *pDBModel=0,  QWidget *parent = 0);
    bool isComplete() const;
    void initializePage() Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
private:
    bool complete;
    QLabel *messageLabel;
    QLabel *schemaTableL;
    QLineEdit *schema;
    QLineEdit *table;
    QLineEdit *tableType;
    QLabel *label;
    QLabel *searchTextL;
    QLineEdit *searchText;
    QTableView *schemaView;
    QGridLayout *mainLayout;
    QStandardItemModel *dBModel;
    void setDefaultTbleInfo(QStringList tableInfo);
private slots:
    void newClassItemChange(QStandardItem *item);
    void filter(QString text);
};


#endif // DATABASETABLE_H
