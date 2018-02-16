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



#include "sohagnewprowizard.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include <QDir>
#include "sohaggen.h"
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include "schemainfo.h"



SohagNewProWizard::SohagNewProWizard(QString defPath, QWidget *parent)
    : QWizard(parent)
{
    setWizardStyle(QWizard::ModernStyle);
    addPage(new WelcomePageA);
    addPage(new LocationPage(defPath));
    setWindowTitle("Sohag Developer Create New Project");
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::LogoPixmap,QPixmap(":/images/logo.png"));
}




void SohagNewProWizard::accept()
{
    projectName = field("projectName").toString();
    projectLocation = field("projectLocation").toString();
    QDialog::accept();
}





QString SohagNewProWizard::proProjectName()
{
    return projectName;
}


QString SohagNewProWizard::proLocation()
{
    return projectLocation;
}




WelcomePageA::WelcomePageA(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Welcome To Sohag Developer"));
    //    setSubTitle("Create New Project");

    label = new QLabel(tr("This wizard will guide you to create a new project then"
                          " create a Qt/C++ class and "
                          "Qt/UI file that contains ui items corresponding to the database table columns. "
                          "You need to specify the project and class name and set a few "
                          "options to produce an ui file , a header file and an "
                          "implementation file for the new C++ class."));
    label->setWordWrap(true);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}


LocationPage::LocationPage(QString defaultPath,QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Project Name & Location"));
    complete=false;
    messageLabel=new QLabel();
    projectNameL=new QLabel(tr("Project Name"));
    projectName=new QLineEdit();
    projectLocationL=new QLabel(tr("Project Name"));
    projectLocation=new QLineEdit();
    QPushButton *fileDialogButton=new QPushButton("Change Location",this);
    connect(fileDialogButton,SIGNAL(pressed()),this,SLOT(selectedFolderName()));

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel(tr("Please add a valid project name and location"));
    label->setWordWrap(true);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(label);

    QHBoxLayout *nameLayout=new QHBoxLayout();
    nameLayout->addWidget(projectNameL);
    nameLayout->addWidget(projectName);
    mainLayout->addLayout(nameLayout);

    QHBoxLayout *locationLayout=new QHBoxLayout();
    locationLayout->addWidget(projectLocationL);
    locationLayout->addWidget(projectLocation);
    locationLayout->addWidget(fileDialogButton);
    mainLayout->addLayout(locationLayout);

    setLayout(mainLayout);
    //    projectName->setText(tableName);
    projectLocation->setText(defaultPath);
    registerField("projectName*", projectName);
    registerField("projectLocation", projectLocation);
    connect(projectName,SIGNAL(textChanged(QString)),this,SLOT(testPathName()));
    connect(projectLocation,SIGNAL(textChanged(QString)),this,SLOT(testPathName()));
}

void LocationPage::cleanupPage()
{

}
void LocationPage::testPathName()
{
    QDir proDir(projectLocation->text());
    if(!proDir.exists() || projectLocation->text().isEmpty() || projectName->text().isEmpty()){
        messageLabel->setText("Please add a vaild project name and project location");
        messageLabel->setStyleSheet("background-color:red;");
        complete=false;
        emit completeChanged();
        return;
    }
    proDir.setPath(projectLocation->text()+"/"+projectName->text());
    if(proDir.exists()){
        messageLabel->setText("A folder with same name already exists.\nSome files may be overwritten");
        messageLabel->setStyleSheet("background-color:yellow;color:red;");
        complete=true;
        emit completeChanged();
        return;
    }else{
        messageLabel->setText("");
        messageLabel->setStyleSheet("");
        complete=true;
        emit completeChanged();
        return;
    }
}


void LocationPage::createProject()
{
    //    QDir proDir;
    //    if(!proDir.mkdir(){
    ////        messageLabel->setText("Sohag Developer can't create project directory at the selected path");

    //        return;
    //    }

}



bool LocationPage::isComplete() const
{
    return complete;
}



void LocationPage::selectedFolderName()
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //    QString projectPath = QFileDialog::getOpenFileName(this,tr("Choose Directory"),path);
    //    QFileDialog *fileDialog=new QFileDialog(this,tr("Choose Directory"),path);
    //    fileDialog->setFileMode(QFileDialog::Directory);
    //    fileDialog->setOption(QFileDialog::ShowDirsOnly);
    //    QString projectPath=fileDialog->getExistingDirectory();
    QString projectPath=QFileDialog::getExistingDirectory(this,"Choose Directory",path,QFileDialog::ShowDirsOnly);
    projectLocation->setText(projectPath);
}
