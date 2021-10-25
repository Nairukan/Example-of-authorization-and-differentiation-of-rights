#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QMainWindow>
#include <cleveredit.h>
#include <paintbutton.h>
#include <user_admin.h>
#include <vector>
#include <MACRO_FOR_RUN.h>

#ifdef DEBAG
    #include <QMessageBox>
    #include <iostream>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class RegWindow; }
QT_END_NAMESPACE

class RegWindow : public QMainWindow
{
    Q_OBJECT

public:
    RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

public slots:
    void resizeEvent(QResizeEvent*);
    void Tick();
    void TryToEnter();
private:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    std::vector<User*> Users;
    void FillUsersFromFile();
    QImage PinkIm=QImage("res\\IMG_5253.jpg");
    QImage Mozaika=QImage("res\\IMG_5252.jpg");
    QImage DarkMozaika=QImage("res\\IMG_5251.jpg");
    PaintButton* PB_Enter=nullptr;
    CleverEdit *CE_Login=nullptr, *CE_Password=nullptr;
    Ui::RegWindow *ui;
};
#endif // REGWINDOW_H
