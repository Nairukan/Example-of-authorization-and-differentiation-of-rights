#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> //Подключаем библиотеку стандартного объекта Qt, QMainWindow
#include <user_admin.h> //Подключаем нашу библиотеку с классом Пользователя
#include <paintbutton.h> //Подключаем нашу библиотеку кнопки с картинкой
#include <cleveredit.h> //Подключаем нашу библиотеку Умного поля для ввода
#include <QCheckBox>    //Подключаем библиотеку стандартного объекта Qt, такой квадратик в котором можно ставить галочку или снимать, как показатель состояния
#include <QComboBox> //Подключаем библиотеку стандартного объекта Qt, выпадающий список

namespace Ui { //Объявляем пространство имён Ui
class MainWindow; //помещаем в него сигнатуру класса MainWindow
}

class MainWindow : public QMainWindow //Класс главного окна наследуюемый от стандартного объекта Qt QMainWindow
{
    Q_OBJECT //Подключаем определения сигналов и слотах стандартных объектов Qt для корректного взаимодействия между объектами фреймворка

public: //Функции и члены доступные извне данного класса
    MainWindow(User* Activ, QMainWindow* conectedWind=nullptr, std::vector<User*> *Base=nullptr);
    /*Сигнатура конструктора класса, принемающая Указатель на пользователя для которого был произведён вход,
     *указатель на окно решгистрации, указатель на вектор с указателями на экземпляры Пользователей
     */

    ~MainWindow(){ //Деструктор класса(вызывается при удалении экземпляра, нужен для освобождение динамически выделенной памяти)
        Users=nullptr;
        if (PB_ApplyChanges!=nullptr) delete PB_ApplyChanges;
        if (PB_NextCom!=nullptr) delete PB_NextCom;
        if (PB_PrevCom!=nullptr) delete PB_PrevCom;
        if (CE_NewLogin!=nullptr) delete CE_NewLogin;
        if (CE_RepitPassword!=nullptr) delete CE_RepitPassword;
        if (CE_Password!=nullptr) delete CE_Password;
        if (IsBanned!=nullptr) delete IsBanned;
        if (IsRuleFollow!=nullptr) delete IsRuleFollow;
        if (uNames!=nullptr) delete uNames;
        if (connectedWind!=nullptr)
            connectedWind->close();
        delete ui;
    }
private slots: //Слоты это функции, выполнение которые можно связать любым сигналом, например нажатиес кнопки, тут они приватные то есть без возможности переопределения или прямого вызова
    void Apply(); //Слот применяющий изменения произведённый в окне
    void Next(); //Слот перехода на следующую комманду, соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
    void Prev(); //Слот перехода на предыдущую комманду, соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
    void ChooseUser(int index); /*Слот обработки выбора в выпадающем меню другого активного пользователя,
                                 *соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
                                 */
    void on_action_triggered(); //Слот обработки нажатия на вкладку "О программе"

private:
    void paintEvent(QPaintEvent*); //Переопределённая функция объекта QAbstractButton(-||-), вызывающаяся когда поступает запрос на отрисовку объекту(при любом взаиможействии с ним)
    QString PrintingText="Изменить пароль"; //Надпись печатуемамая по центру экрана отображающая суть доступных изменений
    void mousePressEvent(QMouseEvent*); //Переопределённая функция объекта QAbstractButton(-||-), вызывающаяся когда на экземпляр данного класса тыкнули мышкой
    void ChangeCom(); //Функция обработки смены комманды, доступно только при создании окна с активным пользователем со статусом - Администратор
    std::vector<User*> *Users=nullptr; //Указатель на вектор указателей на экземпляры класса пользователей, (по задумке) ссылка на список всех пользователей
    QImage PinkIm=QImage("res\\IMG_5253.jpg"); //Предзагруженное фоновое изображение
    QImage Mozaika=QImage("res\\IMG_5252.jpg"); //Предзагруженная текстура кнопки по умолчанию
    QImage DarkMozaika=QImage("res\\IMG_5251.jpg"); //Предзагруженная текстура кнопки при наведении
    PaintButton *PB_ApplyChanges=nullptr, *PB_NextCom=nullptr, *PB_PrevCom=nullptr; //Объявление указателей на !возможные! экземпляры класса Кнопка с изображением
    CleverEdit *CE_NewLogin=nullptr, *CE_RepitPassword=nullptr, *CE_Password=nullptr; //Объявление указателей  на !возможные! экземпляры класса Умное поле для ввода
    QCheckBox *IsBanned = nullptr, *IsRuleFollow = nullptr;//Объявление указателей на земпляры класса QCheckBox, которые будут созданы только при входе Администратором
    QComboBox *uNames=nullptr; //Объявление указателя на экземпляр класса QComboBox, который будет создан только при входе администратором
    User *ActiveUser=nullptr; //Указатель на пользователя под которым был произведён вход
    int NumOfActiv=-1; //Номер ActiveUser в векторе всех пользовательей, изначально -1, т.е. никакой
    QMainWindow *connectedWind=nullptr; //Указатель на окно через которое был открыт текущий экземпляр
    Ui::MainWindow *ui; //Указатель на элемент области имён Ui, MainWindow - почитать его код(QML) можно в mainwindow.ui
    unsigned int command=0; //Номер текущей комманды в окне, по умолчанию комманда смены пароля
};

#endif // MAINWINDOW_H
