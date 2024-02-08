#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncalmaislemleri.h"
#include "oduncteslimetmeislemleri.h"

//database kısmının yolunu kendi bilgisayarınıza göre ayarlayın

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.setDatabaseName("C:/Qt/GP/Lab/kutuphaneOtomasyonu/kutuphaneOtomasyonu.db");
    if(!db.open())
    {
        ui->statusbar->showMessage("Veri tabanına bağlanılamadı!");
    }
    else
    {
        ui->statusbar->showMessage("Veri tabanına bağlanıldı!");
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_uyeIslemleri_clicked()
{
    uyeIslemleri *uyeIslem = new uyeIslemleri(db);
    uyeIslem->show();
}


void MainWindow::on_btn_kitapIslemleri_clicked()
{
    kitapislemleri *kitapIslem = new kitapislemleri(db);
    kitapIslem->show();
}


void MainWindow::on_btn_oduncAlmaIslemleri_clicked()
{
    oduncAlmaIslemleri *oduncAlmaIslem = new oduncAlmaIslemleri(db);
    oduncAlmaIslem->show();
}


void MainWindow::on_btn_oduncTeslimEtmeIslemleri_clicked()
{
    oduncTeslimEtmeIslemleri *oduncTeslimEtmeIslem = new oduncTeslimEtmeIslemleri(db);
    oduncTeslimEtmeIslem->show();
}

//-----------kitapislemleri.cpp--------

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

//-------oduncalmaislemleri.cpp-------------


#include "oduncalmaislemleri.h"
#include "ui_oduncalmaislemleri.h"
#include <QModelIndex>
#include <QVariant>
#include <QList>
# include <QDebug>
oduncAlmaIslemleri::oduncAlmaIslemleri(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oduncAlmaIslemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    sorgu2 = new QSqlQuery(db);
    sorgu3 = new QSqlQuery(db);
    sorgu4 = new QSqlQuery(db);
    sorgu5 = new QSqlQuery(db);
    sorgu6 = new QSqlQuery(db);


    listele();
    listele2();
    listele3();
}

oduncAlmaIslemleri::~oduncAlmaIslemleri()
{
    delete ui;
}

void oduncAlmaIslemleri::listele()
{
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç alınan tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model = new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView_OduncAlinanKitaplar->setModel(model);

}

void oduncAlmaIslemleri::listele2()
{

    sorgu2->prepare("select * from uye");
    if(!sorgu2->exec())
    {
        QMessageBox::critical(this,"Hata!","Üye tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model2 = new QSqlQueryModel();
    model2->setQuery(*sorgu2);
    ui->tableView_tumUyeler->setModel(model2);
}

void oduncAlmaIslemleri::listele3()
{

    sorgu3->prepare("select * from kitap");
    if(!sorgu3->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap tablosunu çekerken bir hata oluştu!","OK");
        return;
    }
    model3 = new QSqlQueryModel();
    model3->setQuery(*sorgu3);
    ui->tableView_tumKitaplar->setModel(model3);
}

void oduncAlmaIslemleri::on_tableView_tumUyeler_clicked(const QModelIndex &index)
{
    sorgu->prepare("select * from uye"); //db deki personel tablosunu çekme sorgusu
    // sorguyu çalıştırıyoruuz
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"Hata!","Üye tablosunu çekerken bir hata ile karşılaşıldı!","ok");
        return;
    }
    model= new QSqlQueryModel(); //çektiğimiz sorguyu modele atıyoruz
    model->setQuery(*sorgu);

    ui->ln_uyeNo->setText(model->index(index.row(),0).data().toString());
    uyeNo2= model->index(index.row(),0).data().toString();
}


void oduncAlmaIslemleri::on_tableView_tumKitaplar_clicked(const QModelIndex &index)
{
    sorgu2->prepare("select * from kitap");
    if(!sorgu2->exec())
    {
        QMessageBox::critical(this,"Hata!","Kitap tablosunu çekerken bir hata ile karşılaşıldı!","ok");
        return;
    }
    model2= new QSqlQueryModel(); //çektiğimiz sorguyu modele atıyoruz
    model2->setQuery(*sorgu2);
    ui->ln_kitapNo->setText(model2->index(index.row(),0).data().toString());
    kitapNo2 = model2->index(index.row(),0).data().toString();
    kitapNo3 = kitapNo2.toInt();

}


void oduncAlmaIslemleri::on_btn_oduncAl_clicked()
{

    sorgu4->exec("select * from odunc_alinan");
    while(sorgu4->next())
    {
        uyeNo2 = sorgu4->value(0).toString();
        kitapNo2 = sorgu4->value(1).toString();

        if(uyeNo2 == ui->ln_uyeNo->text() && kitapNo2 == ui->ln_kitapNo->text())
        {
            QMessageBox::critical(this, "Hata!","Bu üye bu kitabın bir tanesini şu an ödünç almış\nTekrar ödünç verilemez");
            return;
        }

    }
    sorgu6->prepare("select * from kitap where kitap_no=?");
    sorgu6->addBindValue(kitapNo3);
    sorgu6->exec();


    sorgu5->exec("select * from kitap");
    while(sorgu5->next())
    {
        int no = sorgu5->value(0).toInt();

        if(no == kitapNo3)
        {
            stok = sorgu5->value(3).toInt();
            break;
        }
    }

    if(stok-1 < 0)
    {
        QMessageBox::critical(this, "Hata!","Bu kitap şu anda stokta bulunmamaktadır!");
        return;
    }
    sorgu6->prepare("update kitap set sonStok=? where kitap_no=?");
    sorgu6->addBindValue(stok-1);
    sorgu6->addBindValue(kitapNo3);
    sorgu6->exec();

    sorgu4->prepare("insert into odunc_alinan(uye_no, kitap_no, odunc_alma_tarihi) values(?,?,?)");
    sorgu4->addBindValue(ui->ln_uyeNo->text());
    sorgu4->addBindValue(ui->ln_kitapNo->text());
    sorgu4->addBindValue(ui->dateEdit->text());


    if(!sorgu4->exec())
    {
        QMessageBox::critical(this,"Hata!","Ödünç alınan tablosuna ekleme yaparken bir hata oluştu!","OK");
        return;
    }
    listele();


}


//--------------oduncteslimetmeislemleri------------

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



//----------uyeislemleri.cpp---------

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

