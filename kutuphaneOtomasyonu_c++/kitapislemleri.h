#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include  <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class kitapislemleri;
}

class kitapislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit kitapislemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~kitapislemleri();
    void listele();

private slots:
    void on_tableView_tumKitaplar_clicked(const QModelIndex &index);

    void on_btn_yeniKayit_clicked();

    void on_btn_guncelle_clicked();

    void on_btn_sil_clicked();

private:
    Ui::kitapislemleri *ui;
    QSqlQuery *sorgu; //kullannacağımız sorguyu oluşturduk
    QSqlQueryModel *model;
    QSqlQuery *sorgu2;
    QSqlQueryModel *model2;
    QSqlQuery *sorgu3;
    QSqlQueryModel *model3;
    QString kitapSayisi;
    int sonStok;
};

#endif // KITAPISLEMLERI_H
