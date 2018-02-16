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

#ifndef CLASSATTRIBUTES_H
#define CLASSATTRIBUTES_H


#include <QWizardPage>
#include <QSqlQuery>


class QLabel;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class QCheckBox;
class QComboBox;




class ClassAttributes : public QWizardPage
{
    Q_OBJECT
public:
    ClassAttributes(QString tiltle, QString subTitle, QString imagePath, QString labelText, QString database, QString schema, QString table, QStandardItemModel *pAttributesModel, QSqlQuery &pInfoQuery, QWidget *parent = 0);
    QStandardItemModel *attributesModel;
private:
    QLabel *messageLabel;
    bool isComplete() const;
    bool complete;
    QLabel *label;
    QTableView *tableView;
    QCheckBox *selectAllItems;
    QCheckBox *setAllWidgitsToLineEdit;
    QSqlQuery infoQuery;
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


#endif // CLASSATTRIBUTES_H
