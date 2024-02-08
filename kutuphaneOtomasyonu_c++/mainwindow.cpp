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

