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
    bool dbState = db.Connect("../db.s3db");
    ui->winStack->setCurrentIndex(0);

    ui->passwordBox->setEchoMode(QLineEdit::Password);
    ui->passwordBox->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
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

        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM sys_users WHERE username = (:un) AND passwd = (:pw)");
        fetcher.bindValue(":un", this->username);
        fetcher.bindValue(":pw", this->password);
        fetcher.exec();

        int idFname = fetcher.record().indexOf("fname");
        int idMname = fetcher.record().indexOf("mname");
        int idLname = fetcher.record().indexOf("lname");
        int idRank = fetcher.record().indexOf("rank");
        QString fullname, rank;

        while (fetcher.next())
        {
            fullname = fetcher.value(idFname).toString();
            fullname += " " + fetcher.value(idMname).toString();
            fullname += " " + fetcher.value(idLname).toString();
            rank = fetcher.value(idRank).toString();
        }
        ui->nameLabel->setText(fullname);
        ui->rankLabel->setText(rank);
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

