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


#ifndef NEWCLASSINFO_H
#define NEWCLASSINFO_H

#include <QWizardPage>

class QLabel;
class QLineEdit;
class QStandardItemModel;
class QComboBox;
class QGridLayout;


class NewClassInfo : public QWizardPage
{
    Q_OBJECT
public:
    NewClassInfo(QStandardItemModel *pClassModel, QWidget *parent = 0);
    void initializePage() Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
private:
    bool isComplete() const;
    QString tableName;
    QLabel *messageLabel;
    QLabel *label;
    QLabel *classNameL;
    QLineEdit *className;
    QGridLayout *mainLayout;
    bool complete;
    QStandardItemModel *classModel;
    QComboBox *baseClass;
    QLabel *baseClassL;
//    void className();
//    QString vClassName;
private slots:
    void validateClassName(QString text);};



#endif // NEWCLASSINFO_H
