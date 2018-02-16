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



#ifndef CLASSINFO_H
#define CLASSINFO_H

#include <QWizardPage>
#include <QModelIndex>

class QLabel;
class QLineEdit;
class QStandardItemModel;
class QComboBox;

class ClassInfo : public QWizardPage
{
    Q_OBJECT

public:
    ClassInfo(QString tiltle, QString subTitle, QString imagePath, QString labelText, QStandardItemModel *pClassModel, QModelIndex pClassIndex, QWidget *parent=0);
private:
    bool isComplete() const;
    void initializePage() Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
    bool complete;
    QModelIndex classIndex;
    QLabel *label;
    QLineEdit *classNameEdit;
    QStandardItemModel *classModel;
    void className();
    QString vClassName;
    QComboBox *baseClass;
    QLabel *baseClassL;
private slots:
    void validateClassName(QString text);
};


#endif // CLASSINFO_H
