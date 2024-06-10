#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initDatabase();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)")) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

bool MainWindow::createUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Error", "Username and password cannot be empty.");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::warning(this, "Signup Error", query.lastError().text());
        return false;
    }

    return true;
}

bool MainWindow::validateUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toString() == password;
}

void MainWindow::on_loginButton_clicked() {
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (validateUser(username, password)) {
        QMessageBox::information(this, "Login", "Login successful!");
    } else {
        QMessageBox::warning(this, "Login", "Invalid username or password.");
    }
}

void MainWindow::on_signupButton_clicked() {
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (createUser(username, password)) {
        QMessageBox::information(this, "Signup", "User created successfully!");
    } else {
        QMessageBox::warning(this, "Signup", "Failed to create user.");
    }
}

void MainWindow::on_showUsersButton_clicked()
{
    QSqlQuery query("SELECT username FROM users");
        QStringList users;
        while (query.next()) {
            users << query.value(0).toString();
        }
        QMessageBox::information(this, "User Details", "Users:\n" + users.join("\n"));
}
