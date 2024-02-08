#ifndef ODUNCALMAISLEMLERI_H
#define ODUNCALMAISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include  <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QStringList>
namespace Ui {
class oduncAlmaIslemleri;
}

class oduncAlmaIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit oduncAlmaIslemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~oduncAlmaIslemleri();
    void listele();
    void listele2();
    void listele3();


private slots:
    void on_tableView_tumUyeler_clicked(const QModelIndex &index);


    void on_btn_oduncAl_clicked();

    void on_tableView_tumKitaplar_clicked(const QModelIndex &index);

private:
    Ui::oduncAlmaIslemleri *ui;
    QSqlQuery *sorgu; //kullannacağımız sorguyu oluşturduk
    QSqlQueryModel *model;
    QSqlQuery *sorgu2;
    QSqlQueryModel *model2;
    QSqlQuery *sorgu3;
    QSqlQueryModel *model3;
    QSqlQuery *sorgu4;
    QSqlQueryModel *model4;
    QSqlQuery *sorgu5;
    QSqlQueryModel *model5;
    QSqlQuery *sorgu6;
    QSqlQueryModel *model6;
    QSqlQuery *sorgu7;
    QSqlQuery *sorgu8;

    QString uyeNo2,kitapNo2;
    int kitapNo3;
    int syc=0;
    int kitapSayisiInt;
    int sonStok;
     int stok;

};

#endif // ODUNCALMAISLEMLERI_H
