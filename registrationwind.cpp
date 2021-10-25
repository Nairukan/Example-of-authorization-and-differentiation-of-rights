#include "registrationwind.h" //Указываем с чем связан данный файл реализации
#include "ui_registrationwind.h" //Подключаем ui файл
#include <cleveredit.h> //Подключаем нашу библиотеку Умного поля для ввода
#include <fstream> //Подключаем библиотеку работы с файлами
#include <ios> //Подключаем библиотеку настроек для работы с потоками

bool IndividualTaskValidFunc(QString a){ //Функция валидации заданная в индивидуальном задании
    for (int i=0; i<a.length(); i++){ //Проходимся по всем символам строки
        if (a.lastIndexOf(a[i])!=i) return false;
        /*Если индекс полученный первым с конца символом равным очередному символу не совпадает с индексом этого очредного символа,
        *то это значит что данный символ встречаетя в строке дальше -> возвращаем что дання строка не подходит
        */
    }
    return true; //Иначе если ранее не было доказано обратного возвращаем что текущая строка подходит
}

RegistrationWind::RegistrationWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegistrationWind)
    //Реализация конструктора главного окна, сначала выполняется унаследованный конструктор родителя - QMainWindow(с нулевым родителем) и конструктор Ui класса формы
{
    ui->setupUi(this); //Устанавливаем что подключенная форма, подключена для текущего экземпляра

    this->CE_Login=new CleverEdit(this, "Введите Логин", CleverEdit::PasswordEchoOnEdit, &CleverEdit::NoEmptyValidFunc);
    /*Создаём экземпляр Умного поля для ввода, привязанное к этому окну с заданным текстом подсказки, закрытым для копирования
     *и функции валидации соответсвующие правилу "Не пустой текст", и параметром разрешения пробелов по умолчанию(Запрещены)
     */
    CE_Login->resize(400, 100); //Меняем размер
    CE_Login->move((this->width()-CE_Login->width())/2, this->height()/2-CE_Login->height()-10); //Передвигаем

    this->CE_Password=new CleverEdit(this, "Введите Пароль", CleverEdit::Password, &CleverEdit::ValidFuncTrue);
    /*Создаём экземпляр Умного поля для ввода, привязанное к этому окну с заданным текстом подсказки, закрытым для отображения и копирования
     *и функции валидации соответсвующие правилу "Что угодно", и параметром разрешения пробелов по умолчанию(Запрещены)
     */
    CE_Password->resize(CE_Login->width(), CE_Login->height()); //Меняем размер
    CE_Password->move((this->width()-CE_Password->width())/2, this->height()/2+10); //Передвигаем

    this->setFocus();  //Устанавливаем фокус на фон

    this->PB_Enter=new PaintButton(this, Mozaika, DarkMozaika);
    //Создаём экземпляр кнопки с изображением, привзязанное к этому окну с заданными изображениями и прямоугольниками по умолчанию

    PB_Enter->move(CE_Login->x(), CE_Password->y()+CE_Password->height()+30); //Передвигаем
    PB_Enter->resize(CE_Login->width(), CE_Password->height()); //Меняем размер
    PB_Enter->setText("Войти"); //Устанавливаем текст кнопки
    PB_Enter->setFont(CE_Password->font()); //Устанавливаем шрифт соответсвующий шрифту поля для ввода
    connect(PB_Enter, SIGNAL(clicked()), this, SLOT(TryToEnter()));  //связываем нажатие на кнопку с функцией
}

void RegistrationWind::FillUsersFromFile(){  //Функция загрузки базы существующих пользователей
    std::fstream fBase; //Создаём файловый поток
    try{ //Пробуем совершить следующий код в котором можем отлавливать ошибки, допустим ошибку что пытаемся считать файл который пустой или которого не существует
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary); //Открываем файл для считывания в бинарном режиме
        if (User::TryReadFromOpenBinFile(&fBase)==nullptr) throw -1; //Если при попытке считывание не получилось считать инфрммацию хоть об одном пользователе вызываем ошибку
        fBase.close(); //Если Успешно считался хоть один пользователь, то закрываем файловый поток
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary); //Открываем файл для считывания в бинарном режиме
    }
    catch(...){ //Если в блоке try была вызвана ошибка то выполняется следующий код
        fBase.open("res\\Base.bs", std::ios_base::out|std::ios_base::binary); //Открываем файл для ввода в бинарном режиме
        User ADM("ADMIN", "/b.*/b", false); //Создаём экземпляр Администратора с пустым паролем
        ADM.WriteToOpenBinWrite(&fBase);
        //Вызываем через этого пользователя функцию-член которая пробует записать данные пользователя экземпляра, в открытый бинарный файл, возвращаяя показатель успешности опперации
        fBase.close(); //Закрываем файловый поток
        fBase.open("res\\Base.bs", std::ios_base::in|std::ios_base::binary); //Открываем файл для считывания в бинарном режиме
    }
    User* newUs = User::TryReadFromOpenBinFile(&fBase); //Пробуем считать пользователя из файла
    Users.clear(); //Очищаем базу пользователей
    while (newUs!=nullptr){ //Пока считывание проходит успешно, то повторям следующее:
#ifdef DEBAG //Если в MACRO_FOR_RUN определён режим отладки
        QMessageBox::critical(this, "",newUs->GetInfo()); //Выводим информацию о очередном считанном пользователе
#endif //Конец условия
        this->Users.push_back(newUs); //Добавляем в базу указатель на считанного пользователя
        TryEnterLeft.push_back(3); //Добавляем в базу кол-во попыток входа для считанного пользователя
        newUs = User::TryReadFromOpenBinFile(&fBase); //Пробуем считать следующего пользователя
    }
}

RegistrationWind::~RegistrationWind() //Реализация деструктора
{
    if (Users.size()!=0){ //Если база не пустая, то
        std::fstream fBase; //Объявляем файловый поток
        fBase.open("res\\Base.bs", std::ios_base::out|std::ios_base::binary); //Открываем файл для ввода в бинарном режиме
        for (User* U : Users){ //Проходим по всем элементам базы
            U->WriteToOpenBinWrite(&fBase); //Записывая информацию очередного пользователя
            delete U; //И освобождаем память которую занимает данныйпользователь
        }
        fBase.close(); //Закрываем фаловый поток
    }
    if (PB_Enter!=nullptr) delete PB_Enter;
    if (CE_Login!=nullptr) delete CE_Login;
    if (CE_Password!=nullptr) delete CE_Password;
    delete ui; //Удаляем экземпляр Ui класса
}

void RegistrationWind::paintEvent(QPaintEvent *){ //Реализация функции отрисовки объекта, принадлежащей пространству имён класса MainWindow
    QPainter p(this); //Создаём устройство для рисование нацеленное на конкретный экземпляр данного класса
    p.drawImage(QRect(0,0,this->width(),this->height()), this->PinkIm);
}

void RegistrationWind::mousePressEvent(QMouseEvent *){
    this->setFocus(); //Устанавливаем фокус на фон
}

void RegistrationWind::TryToEnter(){ //Слот который обрабатывает попытку авторизации
    if (this->CE_Login->GetState()==QValidator::Acceptable){  //Если Содержимое поля для ввода имени соответсвует заданному правилу
        this->FillUsersFromFile(); //Загружаем базу пользователей
        for (int i=0; i<Users.size(); i++){ //Проходим по всем пользователям базы
            if (Users[i]->GetName()==CE_Login->text()){ //Если введённое имя совпадает с именем очередного пользователя из базы, то
                if (Users[i]->GetFollowRule() && !IndividualTaskValidFunc(CE_Password->text())){  //Если данный пользователь ограничен и введённый пароль не соответсвует ограничению
                    QMessageBox::critical(this, "Ошибка", "Пароль для данного пользователя ограничен правилом, \nно введённый пароль не соответствует правилу"); //Выводим сообщение о ошибке
                    goto BAD_END; //Переходим на отметку BAD_END
                }
                if (Users[i]->GetFollowRule() && !Users[i]->PassAbleInValidFunc(&IndividualTaskValidFunc)){ //Если пользователь ограничен но текущий пароль пользователя не соответсвует правилу, то
                    QMessageBox::warning(this, "Внимание", "Установленный пароль для пользователя ограничен правилом,\nоднако не соответсвует ему. Действующий пароль сброшен,\nустановите новый пароль!");
                    //Вывод сообщение о данной ситуации

                    User* CopyUser=Users[i]; //запоминаем указатель активного пользователя
                    Users[i]=new User(CopyUser->GetName(), "/b.*/b", CopyUser->GetFollowRule()); //Подменяем Пользователя дубликатом но с "пустым паролем"
                    delete CopyUser; //Удаляем экземпляр Пользователя находящийся по указателю первоначального активного
                }
                if (TryEnterLeft[i]<=0){ //Если не осталось попыток для входа для данного пользователя
                    QMessageBox::critical(this, "Ошибка", "Попытки входа для данного пользователя кончились.\nПопробуйте снова позже"); //Выводим сообщение о ошибке
                    goto BAD_END; //Переходим на отметку BAD_END
                }
                if (Users[i]->CheckPassword("/b.*/b") || Users[i]->CheckPassword(CE_Password->text())){ //Если Пароль для данного пользователя пустой или совпадает с введённым, то
                    //OK_STATE
                    if (Users[i]->GetBanned()){ //Проверям заблокирован ли этот пользователь
                        QMessageBox::critical(this, "Ошибка", "Данный пользователь заблокирован,\nобратитесь к администратору"); //Выводим сообщение о ошибке
                        goto BAD_END; //Переходим на отметку BAD_END
                    }
                    //Если всё хорошо
                    QMainWindow * tp=this; //Создаём указатель на текущее окно в оболочке класса от которого проводилось наследование
                    w=new MainWindow(Users[i], tp, &(this->Users)); /*Создаём экземпляр класса Главное Окно, передавая в него указатель пользователя по которому соввершён вход,
                                                                     *указатель на родительское окно и ссылку на базу пользователей
                                                                     */

                    this->hide();//Прячем текущее окно
                    w->show(); //Показываем экземпляр Главного окна
                    goto GOOD_END; //Переходим на отметку GOOD_END
                }else{
                    QMessageBox::critical(this, "Ошибка", "Неверный пароль,\nосталось попыток "+QString::number(--TryEnterLeft[i])); //Выводим сообщение о ошибке, уменьшая кол-во попыток входа
                    goto BAD_END; //Переходим на отметку BAD_END
                }
            }
        }

        QMessageBox::critical(this, "Ошибка", "Не найден пользователь с данным именем"); //Выводим сообщение о ошибке
BAD_END:{}
        Users.clear(); //Очищаем базу, для того чтобы она была загружена в памяти лишь в момент нажатия на кнопку
GOOD_END:{}
    }
}

void RegistrationWind::on_action_triggered() //Если нажата вкладка "О программе"
{
    QMessageBox::information(this, "О программе", "GitHub: https://github.com/Nairukan");
}
