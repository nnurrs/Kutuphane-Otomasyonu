#ifndef ODUNCTESLIMETMEISLEMLERI_H
#define ODUNCTESLIMETMEISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include  <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QString>

namespace Ui {
class oduncTeslimEtmeIslemleri;
}

class oduncTeslimEtmeIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit oduncTeslimEtmeIslemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~oduncTeslimEtmeIslemleri();
    void listele();
    void listele2();

private slots:
    void on_tableView_oduncAlinanKitaplar_clicked(const QModelIndex &index);

    void on_btn_oduncuVer_clicked();


private:
    Ui::oduncTeslimEtmeIslemleri *ui;
    QString almaTarih,vermeTarih,almaTarih2,uyeNo,kitapNo;
    int borc;
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
    int stok,kitapNo3;

};

#endif // ODUNCTESLIMETMEISLEMLERI_H
