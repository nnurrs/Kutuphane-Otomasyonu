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


