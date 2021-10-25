#include "mainwindow.h" //Указываем с чем связан данный файл реализации
#include "ui_mainwindow.h" //Подключаем ui файл

MainWindow::MainWindow(User* Activ, QMainWindow* conectedWind, std::vector<User*> *Base) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow)
  //Реализация конструктора главного окна, сначала выполняется унаследованный конструктор родителя - QMainWindow(с нулевым родителем) и конструктор Ui класса формы

{

    ui->setupUi(this); //Устанавливаем что подключенная форма, подключена для текущего экземпляра
    this->ActiveUser=Activ; //Записываем указатель на активного пользователя
    for (int i=0; Base!=nullptr && i<Base->size(); i++) //Если база задана то проходим по всем её эллментам
        if ((*Base)[i]==Activ) this->NumOfActiv=i; //Если очередной элемент из базы является активным пользователем, то запоминаем его номер
    if (connectedWind!=nullptr) //Если связанное окно задано, то
        this->connectedWind=connectedWind; //Запоминаем указатель на него

    this->CE_Password=new CleverEdit(this, "Введите новый пароль", CleverEdit::Password, &CleverEdit::NoEmptyValidFunc);
    /*Создаём экземпляр Умного поля для ввода, привязанное к этому окну с заданным текстом подсказки, закрытым для отображения содержимого и копирования
     *и функции валидации соответсвующие правилу "Не пустой текст", и параметром разрешения пробелов по умолчанию(Запрещены)
     */

    CE_Password->move(200, 200); //Передвигаем Умное поле для ввода пароля на некоторые координаты
    CE_Password->resize(400, 100); //Изменяем размер


    this->CE_RepitPassword=new CleverEdit(this, "Повторите новый пароль", CleverEdit::Password, &CleverEdit::NoEmptyValidFunc);
    /*Создаём экземпляр Умного поля для ввода, привязанное к этому окну с заданным текстом подсказки, закрытым для отображения содержимого и копирования
     *и функции валидации соответсвующие правилу "Не пустой текст", и параметром разрешения пробелов по умолчанию(Запрещены)
     */
    CE_RepitPassword->move(200, 320); //передвигаем
    CE_RepitPassword->resize(400, 100); //Изменяем размер

    PB_ApplyChanges=new PaintButton(this, this->Mozaika, this->DarkMozaika);
    //Создаём экземпляр кнопки с изображением, привзязанное к этому окну с заданными изображениями и прямоугольниками по умолчанию

    PB_ApplyChanges->setGeometry(200, 450, 400, 100); //устанавливаем положение и размер
    connect(PB_ApplyChanges, SIGNAL(clicked()), this, SLOT(Apply()));  //связываем нажатие на кнопку с функцией
    QFont UniversalFont=QFont(PB_ApplyChanges->font().family(), PB_ApplyChanges->font().pointSize()+6,  PB_ApplyChanges->font().weight()+6); //Запоминаем настройки шрифта
    PB_ApplyChanges->setFont(UniversalFont); //Устанавливаем шрифт
    PB_ApplyChanges->setText("Установить новый пароль"); //Устанавливаем текст

    if (Activ->GetName()=="ADMIN" && NumOfActiv!=-1){ //Если активный пользователь имеет статус Администратора и если он был найден в переданной базе
        this->Users=Base; //То запоминаем переданную базу

        PB_NextCom=new PaintButton(this, Mozaika, DarkMozaika);
        //Создаём экземпляр кнопки с изображением, привзязанное к этому окну с заданными изображениями и прямоугольниками по умолчанию
        PB_NextCom->setFont(UniversalFont); //Устанавливаем шрифт
        PB_NextCom->setText(">"); //Устанавливаем текст
        PB_NextCom->resize(50, 300); //Изменяем размер
        PB_NextCom->move(this->width()-30-PB_NextCom->width(), (this->height()-PB_NextCom->height())/2+30); //Передвигаем
        connect(PB_NextCom, SIGNAL(clicked()), this, SLOT(Next()));  //связываем нажатие на кнопку с функцией

        PB_PrevCom=new PaintButton(this, Mozaika, DarkMozaika);
        //Создаём экземпляр кнопки с изображением, привзязанное к этому окну с заданными изображениями и прямоугольниками по умолчанию
        PB_PrevCom->setFont(UniversalFont); //Устанавливаем шрифт
        PB_PrevCom->setText("<"); //Устанавливаем текст
        PB_PrevCom->resize(50, 300); //Изменяем размер
        PB_PrevCom->move(30, (this->height()-PB_NextCom->height())/2+30); //Передвигаем
        connect(PB_PrevCom, SIGNAL(clicked()), this, SLOT(Prev()));  //связываем нажатие на кнопку с функцией

        this->uNames=new QComboBox(this); //Создаём экземпляр QComboBox и привязываем его к этому окну
        uNames->setFont(UniversalFont); //Устанавливаем шрифт
        connect(uNames, SIGNAL(activated(int)), this, SLOT(ChooseUser(int)));  //связываем смену выбранного элемента с функцией
        uNames->setGeometry(300, 200, 200, 50); //устанавливаем положение и размер

        this->IsBanned=new QCheckBox("Заблокирован", this); //Создаём экземпляр QCheckBox, привязываем его к этому окну и задаём текст
        IsBanned->setFont(UniversalFont); //Устанавливаем шрифт
        QFontMetrics fm(UniversalFont); //Создаём инструмент для работы со шрифтами по текущему шрифту элемента
        int IsBanned_Width=IsBanned->iconSize().width()*2+fm.width(IsBanned->text()); //Узнаём действительную ширину QCheckBox
        int IsBanned_Height=std::max(IsBanned->iconSize().height(),fm.height()); //Узнаём действительную высоту QCheckBox
        IsBanned->setGeometry((width()-IsBanned_Width)/2, 300, IsBanned_Width, IsBanned_Height);

        this->IsRuleFollow=new QCheckBox("Ограничение пароля", this); //Создаём экземпляр QCheckBox, привязываем его к этому окну и задаём текст
        IsRuleFollow->setFont(UniversalFont); //Устанавливаем шрифт
        int IsRuleFollow_Width=IsRuleFollow->iconSize().width()*2+fm.width(IsRuleFollow->text()); //Узнаём действительную ширину QCheckBox
        int IsRuleFollow_Height=std::max(IsRuleFollow->iconSize().height(),fm.height()); //Узнаём действительную высоту QCheckBox
        IsRuleFollow->setGeometry((width()-IsRuleFollow_Width)/2, 370, IsRuleFollow_Width, IsRuleFollow_Height); //устанавливаем положение и размер

        this->CE_NewLogin=new CleverEdit(this, "Имя нового пользователя", CleverEdit::PasswordEchoOnEdit, &CleverEdit::NoEmptyValidFunc);
        /*Создаём экземпляр Умного поля для ввода, привязанное к этому окну с заданным текстом подсказки, закрытым копирования
         *и функции валидации соответсвующие правилу "Не пустой текст", и параметром разрешения пробелов по умолчанию(Запрещены)
         */
        CE_NewLogin->move(200, 300); //Передвигаем
        CE_NewLogin->resize(400, 100); //Изменяем размер

        ChangeCom(); //Вызываем обработку текущей активной комманды
        repaint(); //Вызываем перерисовку элемента
    }
    this->setFocus(); //Устанавливаем фокус на фон
}

void MainWindow::paintEvent(QPaintEvent*){  //Реализация функции отрисовки объекта, принадлежащей пространству имён класса MainWindow
    QPainter p(this); //Создаём устройство для рисование нацеленное на конкретный экземпляр данного класса
    p.drawImage(QRect(0,0,this->width(),this->height()), this->PinkIm); //Рисуем фон
    QFont UniversalFont=QFont(p.font().family(), p.font().pointSize()+10, p.font().weight()+10); //Запоминаем настройки шрифта
    p.setFont(UniversalFont); //Устанавливаем шрифт
    QFontMetrics fm(UniversalFont); //Создаём инструмент для работы со шрифтами по текущему шрифту элемента
    int TextWidth=fm.width(PrintingText); //Запоминаем ширину текста(Описания текущей комманды)
    int TextHeight=fm.height(); //Запоминаем высоту текста(Описания текущей комманды)
    p.drawText(QRect((width()-TextWidth)/2, 100, TextWidth, TextHeight), PrintingText); //Рисуем текст, установив выравнивание по центру

    //При перерисовке окна, перерисовываются так же все объекты прикреплённый к нему
}

void MainWindow::Apply(){ //Функция применяющая изменения произведённый в окне
    if (this->command==1) //Если комманда добавления нового пользователя

        if (CE_NewLogin->ValidFuncTrue(CE_NewLogin->text()) && Users!=nullptr){ //Если Содержимое поля для ввода соответсвует заданному правилу и если база пользователей передавалась, то
            for (int i=0; i<Users->size(); i++) //Проходим по всем пользователям из базы
                if ((*Users)[i]->GetName()==CE_NewLogin->text()){ //Если имя очередного пользователя совпадает с именем для создания нового, то
                    QMessageBox::critical(this, "Ошибка", "Пользователь с данным логином уже существует!"); //Выводим сообщение об ошибке
                    goto NO1; //Перемещаемся на отметку NO1
                }
            this->Users->push_back(new User(CE_NewLogin->text())); //Если же имя не сопало то создаём нового пользоватя с пустым паролем
            QMessageBox::information(this, "Успех", "Опперация выполнена успешно!"); //Выводим сообщение о успешном завершении
            NO1:{} //Метка для прыжка
        }else //Иначе
            QMessageBox::critical(this, "Ошибка", "Не задана база пользователей или\nневерный формат Имени нового пользователя");  //Выводим сообщение об ошибке
    else if (command==2) //Если комманда изменения параметров пользователей
        if (Users!=nullptr){ //Если база пользователей задана, то
            User* ChangeU=(*Users)[uNames->currentIndex()]; //Получаем указатель на выбранного для изменения пользователя
            ChangeU->setFollowRule(this->IsRuleFollow->checkState()==Qt::CheckState::Checked, ActiveUser); //Запрашиваем изменение показателя ограничения выбранного пользователя
            ChangeU->setBlocked(this->IsBanned->checkState()==Qt::CheckState::Checked, ActiveUser); //Запрашиваем изменение показателя блокировки выбранного пользователя
            QMessageBox::information(this, "Успех", "Опперация выполнена успешно!"); //Выводим сообщение о успешном завершении
        }
        else //Иначе
            QMessageBox::critical(this, "Ошибка", "Не загруженна база данных пользователей"); //Выводим сообщение об ошибке
    else if (command==0) //Если команда смены пароля
        if ((CE_Password->ValidFuncTrue(CE_Password->text()) & CE_RepitPassword->ValidFuncTrue(CE_RepitPassword->text()))==1 && CE_Password->text()==CE_RepitPassword->text()){
            //Если Поля для ввода пароля соответсвуют правилам содержимого этих полей и введены одинаковые новые пароли
            User tempActiv(ActiveUser->GetName(), CE_Password->text(), ActiveUser->GetFollowRule());
            /* то Создаём дубликат выбранного пользователя но с другим паролем т.е. по факту нового пользователя с тем же именем и показателем ограничения, но новым паролем и
             * разблокированным показателем блокировки, т.к. заблокированный пользователь не смог бы попасть в данное меню
             */

            *ActiveUser=tempActiv; //Заменяем экземпляр, находящийся по указателю активного пользователя, созданным дубликатом
            QMessageBox::information(this, "Успех", "Опперация выполнена успешно!"); //Выводим сообщение о успешном завершении
        }
        else QMessageBox::critical(this, "Ошибка", "Пароли не совпадают,\nлибо не соответсвуют установленным правилам!"); //Иначе выводим сообщение об ошибке
}

void MainWindow::mousePressEvent(QMouseEvent*){
    this->setFocus(); //Устанавливаем фокус на фон
}

void MainWindow::ChangeCom(){ //Функция обработки смены комманды, вызовется только при входе Администратором
    this->CE_NewLogin->hide(); //Прячем поле для ввода имени нового пользователя
    this->CE_Password->hide(); //Прячем поле для ввода нового пароля
    this->CE_RepitPassword->hide(); //Прячем поле для повторного ввода нового пароля
    this->IsBanned->hide(); //Прячем Показатель блокировки выбранного пользователя
    this->IsRuleFollow->hide(); //Прячем Показатель ограничения выбранного пользователя
    this->uNames->hide(); //Прячем выпадающий список для выбора пользователя

    if (command==0){ //Если команда смены пароля
        CE_Password->show();
        PrintingText="Изменить пароль"; //Устанавливаем текст надписи печатуемамоц по центру экрана отображающий суть доступных изменений
        CE_Password->setText(""); //Устанавливаем текст
        CE_RepitPassword->show();
        CE_RepitPassword->setText(""); //Устанавливаем текст
        PB_ApplyChanges->setText("Установить новый пароль"); //Устанавливаем текст кнопки
        repaint(); //Перерисовываем окно
    }else if (command==1){ //Если комманда добавления нового пользователя
        PrintingText="Добавить нового пользователя"; //Устанавливаем текст надписи печатуемамоц по центру экрана отображающий суть доступных изменений
        CE_NewLogin->show();  //Покзываем поле для ввода имени нового пользователя
        CE_NewLogin->setText(""); //Устанавливаем текст
        PB_ApplyChanges->setText("Добавить нового пользователя"); //Устанавливаем текст кнопки
        repaint(); //Перерисовываем окно
    }else if (command==2){ //Если комманда изменения параметров пользователей
        PrintingText="Изменение параметров для пользователей"; //Устанавливаем текст надписи печатуемамоц по центру экрана отображающий суть доступных изменений
        uNames->show(); //Показываем выпадающий список для выбора пользователя
        uNames->clear(); //Очищаем выпадающий список для выбора пользователя
        for (int i=0; Users!=nullptr && i<Users->size(); i++) //Если база задана, то проходим по всем её элементам
            uNames->addItem((*Users)[i]->GetName()); //Добавляем в выпадающее меню имя очередного пользователя
        IsRuleFollow->show(); //Показываем Показатель ограничения выбранного пользователя
        IsBanned->show(); //Показываем Показатель блокировки выбранного пользователя
        PB_ApplyChanges->setText("Сохранить изменения"); //Устанавливаем текст кнопки
        repaint(); //Перерисовываем окно
    }
}

void MainWindow::Next(){  //Слот перехода на следующую комманду, соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
    this->command=(3+command+1)%3; //Кольцевое установление следующей комманды
    ChangeCom(); //Запрашиваем обработку изменение комманды
}

void MainWindow::Prev(){ //Слот перехода на предыдущую комманду, соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
    this->command=(3+command-1)%3; //Кольцевое установление предыдущей комманды
    ChangeCom(); //Запрашиваем обработку изменение комманды
}

void MainWindow::ChooseUser(int index){ /*Слот обработки выбора в выпадающем меню другого активного пользователя,
                                         *соединяется с сигналом только при создании окна с активным пользователем со статусом - Администратор
                                         */

    int n=uNames->currentIndex(); //Узнаём номер выбранного пользователя для изменения, или -1 если не выбран никакой пользователь
    if (n!=-1 && Users!=nullptr){  //Если выбран какой либо пользователь из базы и если база была задана
        IsRuleFollow->setChecked((*Users)[n]->GetFollowRule()); //Устанавливаем полю соответвующее значение текущего показателя ограничения пользователя
        IsBanned->setChecked((*Users)[n]->GetBanned()); //Устанавливаем полю соответвующее значение текущего показателя блокировки пользователя
    }
}

void MainWindow::on_action_triggered() //Если нажата вкладка "О программе"
{
    QMessageBox::information(this, "О программе", "GitHub: https://github.com/Nairukan");
}
