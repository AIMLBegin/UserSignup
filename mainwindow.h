#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void on_signupButton_clicked();
    void on_showUsersButton_clicked();




private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void initDatabase();
    bool createUser(const QString &username, const QString &password);
    bool validateUser(const QString &username, const QString &password);
};

#endif // MAINWINDOW_H
