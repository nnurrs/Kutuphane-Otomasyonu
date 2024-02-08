#include "oduncteslimetmeislemleri.h"
#include "ui_oduncteslimetmeislemleri.h"

oduncTeslimEtmeIslemleri::oduncTeslimEtmeIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oduncTeslimEtmeIslemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    sorgu2 = new QSqlQuery(db);
    sorgu3 = new QSqlQuery(db);
    sorgu4 = new QSqlQuery(db);
    listele();
    listele2();
}

oduncTeslimEtmeIslemleri::~oduncTeslimEtmeIslemleri()
{
    delete ui;
}

void oduncTeslimEtmeIslemleri::listele()
{
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç alınan tablosunu aktarırken bir hata oluştu!","OK");
        return;
    }
    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_oduncAlinanKitaplar->setModel(model);


}

void oduncTeslimEtmeIslemleri::listele2()
{
    sorgu->prepare("select * from odunc_teslim_edilen");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç teslim edilen tablosunu aktarırken bir hata oluştu!","OK");
        return;
    }
    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_teslimEdilenKitaplar->setModel(model);
}



void oduncTeslimEtmeIslemleri::on_tableView_oduncAlinanKitaplar_clicked(const QModelIndex &index)
{
    sorgu2->prepare("select * from odunc_alinan ");
    if(!sorgu2->exec())
    {
        QMessageBox::critical(this, "Hata!","Ödünç alınan tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model2 = new QSqlQueryModel();
    model2->setQuery(*sorgu2);

    ui->ln_uyeNo->setText(model2->index(index.row(),0).data().toString());
    uyeNo = model2->index(index.row(),0).data().toString();

    ui->ln_kitapNo->setText(model2->index(index.row(),1).data().toString());
    kitapNo = model2->index(index.row(),1).data().toString();

    kitapNo3 = kitapNo.toInt();
    almaTarih = model2->index(index.row(),2).data().toString();


}

void oduncTeslimEtmeIslemleri::on_btn_oduncuVer_clicked()
{
   //----------ödünç alınan tablosundan siliyoruz---------

    sorgu3->prepare("delete from odunc_alinan where uye_no=? and kitap_no=? ");
    sorgu3->addBindValue(ui->ln_uyeNo->text().toInt());
    sorgu3->addBindValue(ui->ln_kitapNo->text().toInt());

    if(!sorgu3->exec())
    {
        QMessageBox::critical(this, "Hata!","Ödünç Alma Tablosunda Silme İşlemi Yapılırken Hata İle Karşılaşıldı!");
        return;
    }

    model3=new QSqlQueryModel();
    model3->setQuery(*sorgu3);
    ui->tableView_oduncAlinanKitaplar->setModel(model3);
    listele();
    listele2();


    //almaTarihini date türüne çeviriyoruz
    QDate almaTarihBorc = QDate::fromString(almaTarih, "dd/MM/yyyy");


    if(!almaTarihBorc.isValid())
    {
        QMessageBox::critical(this,"Hata!","Alma tarihini Date türüne çevirirken bir hata oluştu!","OK");
        return;
    }

    QDate vermeTarihBorc = ui->dateEdit->date();

    int kacGun = almaTarihBorc.daysTo(vermeTarihBorc);

    //borc hesaplıyoruz
    if(kacGun > 15)
    {
        int borcluGunSayisi =kacGun - 15;
        borc = borcluGunSayisi * 1;
    }
    else
    {
        borc=0;
    }

    sorgu4->prepare("insert into odunc_teslim_edilen(uye_no,kitap_no,alma_tarihi,verme_tarihi,borc) values(?,?,?,?,?)");
    sorgu4->addBindValue(ui->ln_uyeNo->text());
    sorgu4->addBindValue(ui->ln_kitapNo->text());
    sorgu4->addBindValue(almaTarih);
    sorgu4->addBindValue(ui->dateEdit->text());
    sorgu4->addBindValue(borc);

    if(!sorgu4->exec())
    {
        QMessageBox::critical(this, "Hata!","Ödünç teslim edilen tablosuna ekleme yapılırken bir hata oluştu!","OK");
        return;
    }
    model4 = new QSqlQueryModel();
    model4->setQuery(*sorgu4);
    ui->tableView_teslimEdilenKitaplar->setModel(model4);

    listele();
    listele2();


}



