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


#include "welcomepage.h"
#include <QLabel>
#include <QHBoxLayout>


WelcomePage::WelcomePage(QString tiltle,QString imagePath,QString labelText,QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tiltle);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(imagePath));

    label = new QLabel(labelText);
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
