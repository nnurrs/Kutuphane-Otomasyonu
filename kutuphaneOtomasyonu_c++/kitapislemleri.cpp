#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"

kitapislemleri::kitapislemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kitapislemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    sorgu2 = new QSqlQuery(db);
    sorgu3 = new QSqlQuery(db);
    listele();
}

kitapislemleri::~kitapislemleri()
{
    delete ui;
}

void kitapislemleri::listele()
{
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this, "Hata!","Kitap tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model= new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_tumKitaplar->setModel(model);
}



void kitapislemleri::on_tableView_tumKitaplar_clicked(const QModelIndex &index)
{
    ui->ln_kitapNo->setText(model->index(index.row(),0).data().toString());
    ui->ln_kitapAd->setText(model->index(index.row(),1).data().toString());
    ui->ln_stok->setText(model->index(index.row(),2).data().toString());

    sorgu2->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu2->addBindValue(ui->ln_kitapNo->text().toInt());
    if(!sorgu2->exec())
    {
        QMessageBox::critical(this, "Hata!","Ödünç alınan tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model2 = new QSqlQueryModel();
    model2->setQuery(*sorgu2);
    ui->tableView_oduncDurum->setModel(model2);


    sorgu3->prepare("select * from odunc_teslim_edilen where kitap_no=?");
    sorgu3->addBindValue(ui->ln_kitapNo->text().toInt());
    if(!sorgu3->exec())
    {
        QMessageBox::critical(this, "Hata!","Ödünç teslim edilen tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model3 = new QSqlQueryModel();
    model3->setQuery(*sorgu3);
    ui->tableView_oncedenOduncDurum->setModel(model3);
}


void kitapislemleri::on_btn_yeniKayit_clicked()
{
    QString adBosMu = ui->ln_kitapAd->text();
    QString soyadBosMu = ui->ln_stok->text();
    if(adBosMu == "" || soyadBosMu == "")
    {
        QMessageBox::critical(this,"Hata!","Gerekli alanları doldurunuz!","OK");
        return;
    }
    else
    {
        sorgu->prepare("insert into kitap(kitap_ad, kitap_sayisi, sonStok) values (?,?,?)");
        sorgu->addBindValue(ui->ln_kitapAd->text());
        sorgu->addBindValue(ui->ln_stok->text());
        sorgu->addBindValue(ui->ln_stok->text());
;        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"Hata!","Kitap tablosuna ekleme yapılırken bir hata oluştu!","OK");
            return;
        }
        listele();
    }
}


void kitapislemleri::on_btn_guncelle_clicked()
{
    sorgu->prepare("update kitap set kitap_ad=?, kitap_sayisi=? where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitapAd->text());
    sorgu->addBindValue(ui->ln_stok->text());
    sorgu->addBindValue(ui->ln_kitapNo->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap tablosunda güncelleme yapılırken bir hata oluştu!","OK");
        return;
    }
    listele();
}


void kitapislemleri::on_btn_sil_clicked()
{
    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitapNo->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this, "Hata!", "Kitap tablosundan veri çekerken bir hata oluştu!","OK");
        return;
    }
    int sayac=0;
    while(sorgu->next())
        sayac++;
    if(sayac>0)
    {
        QMessageBox::critical(this, "Hata!","Bu kitap silinemez! Bir üyeye ödünç verilmiştir!","OK");
        return;
    }
    else
    {
        sorgu->prepare("delete from kitap where kitap_no=?");
        sorgu->addBindValue(ui->ln_kitapNo->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this, "Hata!","Kitap tablosundan veri silinirken bir hata oluştu!","OK");
            return;
        }
        listele();
    }
}

