#ifndef REGISTRATIONWIND_H
#define REGISTRATIONWIND_H

#include <QMainWindow>  //Подключаем библиотеку стандартного объекта Qt, QMainWindow
#include <cleveredit.h> //Подключаем нашу библиотеку Умного поля для ввода
#include <paintbutton.h>//Подключаем нашу библиотеку кнопки с картинкой
#include <user_admin.h> //Подключаем нашу библиотеку с классом Пользователя
#include <vector>       //подключаем беблеотеку стандартного динамического контейнера STL, std::vector
#include <mainwindow.h> //подключаем нашу библиотеку с главным окном
#include <MACRO_FOR_RUN.h>  //Подключаем библиотеку содержащую макросы, управляющие выполнением программы
#include <QMessageBox> //Подключение библиотеки стандартного окна уведомлений Qt

#ifdef DEBAG //Если в MACRO_FOR_RUN объявлен режим отладки, то
   #include <iostream> //Подключение библиотеки работы с консолью
#endif //Конец условия

namespace Ui { //Объявляем пространство имён Ui
class RegistrationWind; //помещаем в него сигнатуру класса RegistrationWind
}

class RegistrationWind : public QMainWindow //Класс окна для входа наследуюемый от стандартного объекта Qt QMainWindow
{
    Q_OBJECT //Подключаем определения сигналов и слотах стандартных объектов Qt для корректного взаимодействия между объектами фреймворка

public: //Функции и члены доступные извне данного класса
    explicit RegistrationWind(QWidget *parent = nullptr); //Сигнатура конструктораы
    ~RegistrationWind(); //Сигнатура деструктора
private slots: //Слоты это функции, выполнение которые можно связать любым сигналом, например нажатиес кнопки, тут они приватные то есть без возможности переопределения или прямого вызова
    void TryToEnter(); //Слот который обрабатывает попытку авторизации
    void on_action_triggered();  //Слот обработки нажатия на вкладку "О программе"

private:
    void paintEvent(QPaintEvent*); //Переопределённая функция объекта QAbstractButton(-||-), вызывающаяся когда поступает запрос на отрисовку объекту(при любом взаиможействии с ним)
    void mousePressEvent(QMouseEvent*); //Переопределённая функция объекта QAbstractButton(-||-), вызывающаяся когда на экземпляр данного класса тыкнули мышкой
    std::vector<User*> Users; //Вектор-аналог динамического массива, объявляем вектор хранящий указатели на экземпляр класса Пользователь
    std::vector<int> TryEnterLeft; //Объявляем вектор, хранящий числа обозначающее кол-во доступных попыток для соответсвующего пользователя
    void FillUsersFromFile(); //Функция загрузки базы существующих пользователей
    MainWindow* w=nullptr; //Указатель на возможный экземпляр главного окна
    QImage PinkIm=QImage("res\\IMG_5253.jpg"); //Предзагруженная текстура фона
    QImage Mozaika=QImage("res\\IMG_5252.jpg"); //Предзагруженная текстура кнопки по умолчанию
    QImage DarkMozaika=QImage("res\\IMG_5251.jpg"); //Предзагруженная текстура кнопки при наведении
    PaintButton* PB_Enter=nullptr; //Объявление указателя на экземпляр класса Кнопка с изображением
    CleverEdit *CE_Login=nullptr, *CE_Password=nullptr; //Объявление указателей  на экземпляры класса Умное поле для ввода
    Ui::RegistrationWind *ui; //Указатель на элемент области имён Ui, RegistrationWind - почитать его код(QML) можно в regestrationwind.ui
};

#endif // REGISTRATIONWIND_H
