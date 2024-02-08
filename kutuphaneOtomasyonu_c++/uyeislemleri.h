#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>


namespace Ui {
class uyeIslemleri;
}

class uyeIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit uyeIslemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~uyeIslemleri();
    void listele();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_btn_yeniKayit_clicked();

    void on_btn_guncelle_clicked();

    void on_btn_sil_clicked();

private:
    Ui::uyeIslemleri *ui;
    QSqlQuery *sorguu; //kullannacağımız sorguyu oluşturduk
    QSqlQueryModel *modell; //veritabaından veri tablosu çekip buunu tableview e aktaracğımız için kullandık

};

#endif // UYEISLEMLERI_H
