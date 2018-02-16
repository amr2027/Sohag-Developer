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



#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include "sohaggen.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    connect(ui->connectButton,SIGNAL(pressed()),this,SLOT(connectToDB()));
    connect(ui->cancelButton,SIGNAL(pressed()),this,SLOT(reject()));
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}



void ConnectionDialog::connectToDB()
{
    QString host;
    int port;

    if(ui->hostname->text().isEmpty()){
        host=ui->hostname->placeholderText();
    }else{
        host=ui->hostname->text();
    }

    if(ui->port->text().isEmpty()){
        port=ui->port->placeholderText().toInt();
    }else{
        port=ui->port->text().toInt();
    }

    if(ui->database->text().isEmpty()){
        QMessageBox::warning(this,"Sohag Developer","Please insert the database name");
        return;
    }

    if(ui->username->text().isEmpty()){
        QMessageBox::warning(this,"Sohag Developer","Please insert the user name");
        return;
    }

    if(ui->password->text().isEmpty()){
        QMessageBox::warning(this,"Sohag Developer","Please insert the password");
        return;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(ui->database->text());
    db.setPort(port);
    db.setUserName(ui->username->text());
    db.setPassword(ui->password->text());

    if (!db.open()) {
        QMessageBox::critical(this,"Sohag Developer Error","Can't Connect to database\nDatabase Error: "+db.lastError().text());
        return;
    }
    accept();
}



QString ConnectionDialog::databaseName()
{
    return ui->database->text();
}
