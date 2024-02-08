#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase> //veritabanı için
#include <QSqlTableModel>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_uyeIslemleri_clicked();

    void on_btn_kitapIslemleri_clicked();

    void on_btn_oduncAlmaIslemleri_clicked();

    void on_btn_oduncTeslimEtmeIslemleri_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE"); //veritabanını tanımladık

};
#endif // MAINWINDOW_H
