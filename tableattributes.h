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



#ifndef TABLEATTRIBUTES_H
#define TABLEATTRIBUTES_H

#include <QWizardPage>
#include <QSqlQuery>
#include <QVariant>

class QLabel;
class QLineEdit;
class QStandardItemModel;
class QComboBox;
class QCheckBox;
class QTableView;
class QVBoxLayout;
class QStandardItem;

class TableAttributes : public QWizardPage
{
    Q_OBJECT

public:
    TableAttributes(QSqlQuery &pinfoQuery, QWidget *parent = 0);
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
    void testIfAllSelected();
    void testIfAllQLineEdits();
    QLabel *widgetL;
    QComboBox *widget;
    void fillWidgetsCombo();
private slots:
    void itemChanged(QStandardItem *item);
    void setAllSelected(int state);
    void setAllQLineEdit(int state);
    void setWidgets();
};


#endif // TABLEATTRIBUTES_H
