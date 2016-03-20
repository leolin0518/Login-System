#include "loginsystem.h"
#include "ui_loginsystem.h"
#include "qdb.h"
#include <QSqlRecord>

QDBLite::DB db;

LoginSystem::LoginSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginSystem)
{
    ui->setupUi(this);
    db.dbstate = db.Connect("../LogSys/db.s3db");
    ui->winStack->setCurrentIndex(0);

    ui->passwordBox->setEchoMode(QLineEdit::Password);
    ui->passwordBox->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui->pBox->setEchoMode(QLineEdit::Password);
    ui->pBox->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui->pBox_2->setEchoMode(QLineEdit::Password);
    ui->pBox_2->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
}

LoginSystem::~LoginSystem()
{
    delete ui;
}

void LoginSystem::on_loginButton_clicked()
{
    this->loggedIn = Login(ui->usernameBox->text(), ui->passwordBox->text());

    if(this->loggedIn)
    {
        this->username = ui->usernameBox->text();
        this->password = ui->passwordBox->text();

        ui->loginLabel->setText("");
        ui->winStack->setCurrentIndex(2);
    }
    else
    {
        ui->loginLabel->setText("Login failed: Invalid credentials!");
    }
}

bool LoginSystem::Login(QString u, QString p)
{
    bool exists = false;

    QSqlQuery checkQuery(db.db);
    checkQuery.prepare("SELECT username FROM sys_users WHERE username = (:un) AND passwd = (:pw)");
    checkQuery.bindValue(":un", u);
    checkQuery.bindValue(":pw", p);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}


void LoginSystem::on_regButton_clicked()
{
    ui->uBox->setText(ui->usernameBox->text());
    ui->pBox->setText(ui->passwordBox->text());
    ui->winStack->setCurrentIndex(1);
}

void LoginSystem::on_logoutButton_clicked()
{
    this->loggedIn = false;
    ui->passwordBox->setText("");
    ui->loginLabel->setText("You signed out!");
    ui->winStack->setCurrentIndex(0);
}

void LoginSystem::on_completeRegButton_clicked()
{
    bool halt = false;

    if(ui->uBox->text() == "")
    {
        ui->uBox->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox->text() == "")
    {
        ui->pBox->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox->text() == "")
    {
        ui->eBox->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox->text() == "")
    {
        ui->fBox->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox->text() == "")
    {
        ui->mBox->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox->text() == "")
    {
        ui->lBox->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery(db.db);
    cQuery.prepare("SELECT username FROM sys_users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());

    if(cQuery.exec())
    {
        if(cQuery.next())
        {
            ui->uBox->setText("");
            ui->uBox->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }

    QSqlQuery cQuery2(db.db);
    cQuery2.prepare("SELECT email FROM sys_users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next())
        {
            ui->eBox->setText("");
            ui->eBox->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {
        ui->regLabel->setText("Please correct your mistakes.");
    }
    else
    {
        ui->regLabel->setText("");
        QSqlQuery iQuery(db.db);
        iQuery.prepare("INSERT INTO sys_users(username, passwd, fname, mname, lname, email)"\
                       "VALUES(:un, :pw, :fn, :mn, :ln, :em)");
        iQuery.bindValue(":un", ui->uBox->text());
        iQuery.bindValue(":pw", ui->pBox->text());
        iQuery.bindValue(":fn", ui->fBox->text());
        iQuery.bindValue(":mn", ui->mBox->text());
        iQuery.bindValue(":ln", ui->lBox->text());
        iQuery.bindValue(":em", ui->eBox->text());

        if(iQuery.exec())
        {
            ui->loginLabel->setText("Registration Successful! You can now login.");
            ui->winStack->setCurrentIndex(0);
        }

    }
}

void LoginSystem::on_backButton_clicked()
{
    ui->loginLabel->setText("");
    ui->winStack->setCurrentIndex(0);
}


void LoginSystem::on_backButton_2_clicked()
{
    ui->winStack->setCurrentIndex(2);
}

void LoginSystem::on_editButton_clicked()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM sys_users WHERE username = (:un) AND passwd = (:pw)");
    fetcher.bindValue(":un", this->username);
    fetcher.bindValue(":pw", this->password);
    fetcher.exec();

    int idUsername = fetcher.record().indexOf("username");
    int idPasswd = fetcher.record().indexOf("passwd");
    int idEmail = fetcher.record().indexOf("email");
    int idFname = fetcher.record().indexOf("fname");
    int idMname = fetcher.record().indexOf("mname");
    int idLname = fetcher.record().indexOf("lname");

    while (fetcher.next())
    {
        ui->uBox_2->setText(fetcher.value(idUsername).toString());
        ui->pBox_2->setText(fetcher.value(idPasswd).toString());
        ui->eBox_2->setText(fetcher.value(idEmail).toString());
        ui->fBox_2->setText(fetcher.value(idFname).toString());
        ui->mBox_2->setText(fetcher.value(idMname).toString());
        ui->lBox_2->setText(fetcher.value(idLname).toString());
    }

    ui->winStack->setCurrentIndex(3);
}

void LoginSystem::on_delButton_clicked()
{
    QSqlQuery dQuery(db.db);
    dQuery.prepare("DELETE FROM sys_users WHERE username = (:un)");
    dQuery.bindValue(":un", this->username);

    if(dQuery.exec())
    {
        ui->loginLabel->setText("Account deleted!");
        ui->winStack->setCurrentIndex(0);
    }
}

void LoginSystem::on_editedButton_clicked()
{


    bool halt = false;

    if(ui->uBox_2->text() == "")
    {
        ui->uBox_2->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox_2->text() == "")
    {
        ui->pBox_2->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox_2->text() == "")
    {
        ui->eBox_2->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox_2->text() == "")
    {
        ui->fBox_2->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox_2->text() == "")
    {
        ui->mBox_2->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox_2->text() == "")
    {
        ui->lBox_2->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery(db.db);
    cQuery.prepare("SELECT username FROM sys_users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());

    if(cQuery.exec())
    {
        if(cQuery.next() && ui->uBox_2->text() != cQuery.value(0).toString())
        {
            ui->uBox_2->setText("");
            ui->uBox_2->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }

    QSqlQuery cQuery2(db.db);
    cQuery2.prepare("SELECT email FROM sys_users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox_2->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next() && ui->eBox_2->text() != cQuery2.value(0).toString())
        {
            ui->eBox_2->setText("");
            ui->eBox_2->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {
        ui->regLabel_2->setText("Please correct your mistakes.");
    }
    else
    {
        ui->regLabel_2->setText("");
        QSqlQuery iQuery(db.db);
        iQuery.prepare("UPDATE sys_users SET username=(:un), passwd=(:pw), fname=(:fn), mname=(:mn), lname=(:ln), email=(:em) WHERE username=(:uno)");
        iQuery.bindValue(":un", ui->uBox_2->text());
        iQuery.bindValue(":pw", ui->pBox_2->text());
        iQuery.bindValue(":fn", ui->fBox_2->text());
        iQuery.bindValue(":mn", ui->mBox_2->text());
        iQuery.bindValue(":ln", ui->lBox_2->text());
        iQuery.bindValue(":em", ui->eBox_2->text());
        iQuery.bindValue(":uno", ui->uBox_2->text());

        if(iQuery.exec())
        {
            ui->winStack->setCurrentIndex(2);
        }

    }
}

void LoginSystem::on_winStack_currentChanged(int arg1)
{
    if(arg1 == 2 && this->loggedIn)
    {
        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM sys_users WHERE username = (:un)");
        fetcher.bindValue(":un", this->username);
        fetcher.exec();

        int idFname = fetcher.record().indexOf("fname");
        int idMname = fetcher.record().indexOf("mname");
        int idLname = fetcher.record().indexOf("lname");
        int idRank = fetcher.record().indexOf("rank");
        int idEmail = fetcher.record().indexOf("email");

        QString fullname, rank, email;

        while (fetcher.next())
        {
            fullname = fetcher.value(idFname).toString();
            fullname += " " + fetcher.value(idMname).toString();
            fullname += " " + fetcher.value(idLname).toString();
            rank = fetcher.value(idRank).toString();
            email = fetcher.value(idEmail).toString();
        }
        ui->nameLabel->setText(fullname);
        ui->rankLabel->setText(rank);
        ui->emailLabel->setText(email);
    }
}
