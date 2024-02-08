#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"

uyeIslemleri::uyeIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uyeIslemleri)
{
    ui->setupUi(this);
    sorguu = new QSqlQuery(db);
    listele();
}

uyeIslemleri::~uyeIslemleri()
{
    delete ui;
}

void uyeIslemleri::listele()
{
    sorguu->prepare("select * from uye");
    if(!sorguu->exec())
    {
        QMessageBox::critical(this,"Hata!","Üye tablosunu aktarırken bir hata oluştu!","OK");
        return;
    }
    modell = new QSqlQueryModel();
    modell->setQuery(*sorguu);
    ui->tableView->setModel(modell);

}

void uyeIslemleri::on_tableView_clicked(const QModelIndex &index)
{
    ui->ln_uyeNo->setText(modell->index(index.row(),0).data().toString());
    ui->ln_uyeAd->setText(modell->index(index.row(),1).data().toString());
    ui->ln_uyeSoyad->setText(modell->index(index.row(),2).data().toString());
}


void uyeIslemleri::on_btn_yeniKayit_clicked()
{

    QString adBosMu = ui->ln_uyeAd->text();
    QString soyadBosMu = ui->ln_uyeSoyad->text();
    if(adBosMu == "" || soyadBosMu == "")
    {
        QMessageBox::critical(this,"Hata!","Gerekli alanları doldurunuz!","OK");
        return;
    }
    else
    {
        sorguu->prepare("insert into uye(uye_ad, uye_soyad) values (?,?)");
        sorguu->addBindValue(ui->ln_uyeAd->text());
        sorguu->addBindValue(ui->ln_uyeSoyad->text());
        if(!sorguu->exec())
        {
            QMessageBox::critical(this,"Hata!","Üye tablosuna ekleme yapılırken bir hata oluştu!","OK");
            return;
        }
        listele();
    }
}


void uyeIslemleri::on_btn_guncelle_clicked()
{
    sorguu->prepare("update uye set uye_ad=?, uye_soyad=? where uye_no=?");
    sorguu->addBindValue(ui->ln_uyeAd->text());
    sorguu->addBindValue(ui->ln_uyeSoyad->text());
    sorguu->addBindValue(ui->ln_uyeNo->text().toInt());
    if(!sorguu->exec())
    {
        QMessageBox::critical(this,"Hata!","Üye tablosunda güncelleme yapılırken bir hata oluştu!","OK");
        return;
    }
    listele();
}


void uyeIslemleri::on_btn_sil_clicked()
{
    sorguu->prepare("select * from odunc_alinan where uye_no=?");
    sorguu->addBindValue(ui->ln_uyeNo->text().toInt());
    if(!sorguu->exec())
    {
        QMessageBox::critical(this, "Hata!", "Üye tablosundan veri çekerken bir hata oluştu!","OK");
        return;
    }
    int sayac=0;
    while(sorguu->next())
        sayac++;
    if(sayac>0)
    {
        QMessageBox::critical(this, "Hata!","Bu üye silinemez! Üyenin henüz teslim etmediği kitaplar var!","OK");
        return;
    }
    else
    {
        sorguu->prepare("delete from uye where uye_no=?");
        sorguu->addBindValue(ui->ln_uyeNo->text().toInt());
        if(!sorguu->exec())
        {
            QMessageBox::critical(this, "Hata!","Üye tablosundan veri silinirken bir hata oluştu!","OK");
            return;
        }
        listele();
    }
}

