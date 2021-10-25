#include "regwind.h"
#include "ui_mainwindow.h"
#include <cleveredit.h>
#include <QFile>
#include <QFileDevice>
#include <fstream>
#include <ios>

bool ValidFuncTrue(QString){ return true;}

RegWindow::RegWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->CE_Login=new CleverEdit(this, "Введите Логин", CleverEdit::PasswordEchoOnEdit, CleverEdit::NoEmptyValidFunc);
    CE_Login->resize(400, 100);
    CE_Login->move((this->width()-CE_Login->width())/2, this->height()/2-CE_Login->height()-10);
    this->CE_Password=new CleverEdit(this, "Введите Пароль", CleverEdit::Password, &ValidFuncTrue);
    CE_Password->resize(CE_Login->width(), CE_Login->height());
    CE_Password->move((this->width()-CE_Password->width())/2, this->height()/2+10);
    QTimer *T=new QTimer(this);
    connect(T, SIGNAL(timeout()), this, SLOT(Tick()));
    this->setFocus();
    this->PB_Enter=new PaintButton(this, Mozaika, DarkMozaika);
    PB_Enter->move(CE_Login->x(), CE_Password->y()+CE_Password->height()+30);
    PB_Enter->resize(CE_Login->width(), CE_Password->height());
    PB_Enter->setText("Войти");
    PB_Enter->setFont(CE_Password->font());
    connect(PB_Enter, SIGNAL(clicked()), this, SLOT(TryToEnter()));
}

void RegWindow::FillUsersFromFile(){
    std::fstream fBase;
    try{
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary);
        if (User::TryReadFromOpenBinFile(&fBase)==nullptr) throw -1;
        fBase.close();
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary);
    }
    catch(...){
        fBase.open("res\\Base.bs", std::ios_base::out|std::ios_base::binary);
        User ADM("ADMIN", "/b.*/b", false);
        ADM.WriteToOpenBinWrite(&fBase);
        fBase.close();
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary);
    }
    User* newUs = User::TryReadFromOpenBinFile(&fBase);
    Users.clear();
    while (newUs!=nullptr){
#ifdef DEBAG
        QMessageBox::critical(this, "",newUs->GetInfo());
#endif
        this->Users.push_back(newUs);
        newUs = User::TryReadFromOpenBinFile(&fBase);
    }
}

RegWindow::~RegWindow()
{
    if (Users.size()!=0){
        std::fstream fBase;
        fBase.open("res\\Base.bs", std::ios_base::out|std::ios_base::binary);
        for (User* U : Users){
#ifdef DEBAG
            QMessageBox::critical(this, "",U->GetInfo());
#endif
            U->WriteToOpenBinWrite(&fBase);
        }
        fBase.close();
    }
    delete ui;
}

void RegWindow::Tick(){
}

void RegWindow::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawImage(QRect(0,0,this->width(),this->height()), this->PinkIm);
}

void RegWindow::mousePressEvent(QMouseEvent *){
    this->setFocus();
}

void RegWindow::resizeEvent(QResizeEvent*){

}

void MainWindow::TryToEnter(){
    f (this->CE_Login->GetState()==QValidator::Acceptable){
        this->FillUsersFromFile();

    }
#ifdef DEBAG
    QString Info;
    for (User* U : this->Users){
        Info+="\n\n"+U->GetInfo();
    }
    QMessageBox::information(this, "", Info);
#endif
}
