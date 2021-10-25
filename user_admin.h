#ifndef USER_ADMIN_H
#define USER_ADMIN_H

#include <QString>
#include <TOOLS.h> //Подключаем библиотеку полезных инструментов
#include <MACRO_FOR_RUN.h> //Подключаем библиотеку содержащую макросы, управляющие выполнением программы

class User //Объявляем класс пользователя
{
public:  //Функции и члены доступные извне данного класса
    User(QString Nikname="", QString Password="/b.*/b", bool usesRule=false, bool blocked=false);
    /* Сигнатура конструктора(совсем забыл упомянуть раньше что конструктор это функция выполняющая при каждом создании экземпляра класса) класса,
     * содержащая: Имя пользователя, изначально пустое, Пароль пользователя, изначально соответсвующее "типорегульрному выражению обозначающее что угодно-любой длинны",
     * Показатель ограничен ли пароль данного пользователя установленным правилом, и Показатель заблокирован ли этот пользователь
     */

    static User* TryReadFromOpenBinFile(std::fstream* OpenIn){ /*Статическая функция которая при успешной попытке считывания необходимой информации
                                                                *из бинарного файла, возвращает указатель на пользователя созданного по считанным данным
                                                                *необходима чтобы исключить возможность пользователя подменить данные при считывании
                                                                */
        std::string NameUser;   //Строка в которую запишется возможное Имя пользователя считанной из файла
        std::string PassUser;   //Строка в которую запишется возможный Пароль пользователя считанной из файла
        std::string FollowRule;   //Строка в которую запишется возможный показатель ограничения пользователя считанный из файла
        std::string IsBlocked;   //Строка в которую запишется возможный показатель блокировки пользователя считанный из файла

        if (!OpenIn->eof() && OpenIn->is_open()){ //Если предоставленный поток бинарного файла, не закончился (eof-end of file) и яляется открытым

            int NameLen=TOOLS::BinaryRead(OpenIn, NameLen); //Считываем кол-во следующих символов отображающих Имя пользователя
            for (int i=0; i<NameLen; i++){ //Запускаем цикл для считывания всех этих символов
                if (OpenIn->eof()) return nullptr; //перед считыванием очередного символа проверяем не достигнут ли конец файла и если достигнут, то возвращаем nullptr(нулевой указатель)
                int elem=TOOLS::BinaryRead(OpenIn, elem); //Считываем код очередного символа из файла
                NameUser+=char(elem); //Дописываем символ с соответсвующим кодом к строке Имени пользователя
            }

            if (OpenIn->eof()) return nullptr; //если после считывания Имени был достигнут end of file, то возвращаем nullptr
            int PassLen=TOOLS::BinaryRead(OpenIn, PassLen); //Считываем кол-во следующих символов отображающих Пароль пользователя
            for (int i=0; i<PassLen; i++){ //Запускаем цикл для считывания всех этих символов
                if (OpenIn->eof()) return nullptr; //перед считыванием очередного символа проверяем не достигнут ли конец файла и если достигнут, то возвращаем nullptr(нулевой указатель)
                int elem=TOOLS::BinaryRead(OpenIn, elem); //Считываем код очередного символа из файла
                PassUser+=char(elem); //Дописываем символ с соответсвующим кодом к строке Пароля пользователя
            }

            if (OpenIn->eof()) return nullptr; //если после считывания Пароля был достигнут end of file, то возвращаем nullptr
            int RuleLen=TOOLS::BinaryRead(OpenIn, RuleLen); //Считываем кол-во следующих символов отображающих Показатель ограничения пользователя
            for (int i=0; i<RuleLen; i++){ //Запускаем цикл для считывания всех этих символов
                if (OpenIn->eof()) return nullptr; //перед считыванием очередного символа проверяем не достигнут ли конец файла и если достигнут, то возвращаем nullptr(нулевой указатель)
                int elem=TOOLS::BinaryRead(OpenIn, elem); //Считываем код очередного символа из файла
                FollowRule+=char(elem); //Дописываем символ с соответсвующим кодом к строке Показателя ограничения пользователя
            }
            bool IsRule; //Объявляем показатель ограничения считываемого пользователя
            if (decrypt_func(FollowRule)=="True") IsRule=true; //Если считанная строка соответсвует "True", то устанавливаем соответсвующее значение показателю
            else IsRule=false; //Иначе устанавливаем соответсвующее значение показателю

            if (OpenIn->eof()) return nullptr; //если после считывания Показателя ограничения был достигнут end of file, то возвращаем nullptr
            int BlocedLen=TOOLS::BinaryRead(OpenIn, RuleLen); //Считываем кол-во следующих символов отображающих Показатель блокирования пользователя
            for (int i=0; i<BlocedLen; i++){ //Запускаем цикл для считывания всех этих символов
                if (OpenIn->eof()) return nullptr; //перед считыванием очередного символа проверяем не достигнут ли конец файла и если достигнут, то возвращаем nullptr(нулевой указатель)
                int elem=TOOLS::BinaryRead(OpenIn, elem); //Считываем код очередного символа из файла
                IsBlocked+=char(elem); //Дописываем символ с соответсвующим кодом к строке Показателя блокировки пользователя
            }
            bool blocked; //Объявляем показатель блокировки считываемого пользователя
            if (decrypt_func(IsBlocked)=="True") blocked=true; //Если считанная строка соответсвует "True", то устанавливаем соответсвующее значение показателю
            else blocked=false; //Иначе устанавливаем соответсвующее значение показателю

            return new User(QString::fromStdString(decrypt_func(NameUser)), QString::fromStdString(decrypt_func(PassUser)), IsRule, blocked); //Возвращаем указатель на новый экземпляр, собранный по считанным данным
        }
        return nullptr;//Иначе если файл закрыт или уже закончился то возвращаем nullptr
    }

    bool CheckPassword(QString MaybePassword){ //Функция проверки ввода пароля, необходимо чтобы приватный член Password нельзя было получить или изменить извне лишь сравнить с некоторой строкой
        return this->Password==MaybePassword; //возвращает показатель сравнения на равенство
    }

    bool PassAbleInValidFunc(bool (*ValidFunc)(QString)){ //Функция возвращающая Показатель валидности пароля при поверки его через переданную функцию валидации в качестве параметра
        return (*ValidFunc)(this->Password); //Возвращаем результат валидации
    }

    bool GetFollowRule(){ //Функция Геттер, возвращающая значение показателя ограничения экземпляра
        return this->usesRule;
    }

    bool GetBanned(){ //Функция Геттер, возвращающая значение показателя блокировки экземпляра
        return blocked;
    }

    void setFollowRule(bool newFollowRule, User* MainUser=nullptr){ /*Функция установки значения показателя ограничения экземпляра
                                                                     *Принемает новое значение показателя и Указатель на Доступного пользователя с наивысшими правами, изначально nullptr
                                                                     */
        if (MainUser!=nullptr && MainUser->GetName()=="ADMIN") //Если указатель на Доступного экземпляра с наивысшими правами указывает хоть на что-то и если имя этого пользователя ADMIN
            this->usesRule=newFollowRule; //То устанавливаем текущему пользователю заданный ему Показатель ограничения
    }

    void setBlocked(bool newBlockedState, User* MainUser=nullptr){ /*Функция установки значения показателя ограничения экземпляра
                                                                    *Принемает новое значение показателя и Указатель на Доступного пользователя с наивысшими правами, изначально nullptr
                                                                    */
        if (MainUser!=nullptr && MainUser->GetName()=="ADMIN") //Если указатель на Доступного экземпляра с наивысшими правами указывает хоть на что-то и если имя этого пользователя ADMIN
            this->blocked=newBlockedState; //То устанавливаем текущему пользователю заданный ему Показатель ограничения
    }

    QString GetName(){return this->Nikname;} //Функция Геттер, возвращающая значение Имени пользователя

    bool WriteToOpenBinWrite(std::fstream* OpenIn){ /*Нестатическая функция-член которая пробует записать данные текущего экземпляра,
                                                     *в открытый бинарный файл, возвращаяя показатель успешности опперации
                                                     */

        std::string NameUser=encrypt_func((this->Nikname).toStdString()); //Получаем строку с Именем пользователя путём конвертации QString -> std::string
        std::string PassUser=encrypt_func((this->Password).toStdString()); //Получаем строку с Пароля пользователя путём конвертации QString -> std::string
        bool FollowRule=this->usesRule; //Устанавливаем локальный Показатель ограничения текущего экземпляра

        std::string IsRule;//Объявялем строку соответсвующую состоянию ограничения экземпляра
        if (FollowRule) IsRule="True"; //Если экземпляр ограничен то ставим строке соответсвующее значение
        else IsRule="False"; //Если экземпляр не ограничен то ставим строке соответсвующее значение
        IsRule=encrypt_func(IsRule);
        std::string IsBlocked;//Объявялем строку соответсвующую состоянию блокировки экземпляра
        if (blocked) IsBlocked="True"; //Если экземпляр заблокирован то ставим строке соответсвующее значение
        else IsBlocked="False"; //Если экземпляр не заблокирован то ставим строке соответсвующее значение
        IsBlocked=encrypt_func(IsBlocked);
        if (OpenIn->is_open()){//Если файовый поток открыт, то
            TOOLS::BinaryWrite(OpenIn, (int)NameUser.length()); //Записываем длинну Имени экземпляра
            for (int i=0; i<NameUser.length(); i++) //Далее в цикле проходим по каждому символу Имени
                TOOLS::BinaryWrite(OpenIn, (int)NameUser[i]); //Записывая их код в файл
            TOOLS::BinaryWrite(OpenIn, (int)PassUser.length()); //Записываем длинну Пароля экземпляра
            for (int i=0; i<PassUser.length(); i++) //Далее в цикле проходим по каждому символу Пароля
                TOOLS::BinaryWrite(OpenIn, (int)PassUser[i]); //Записывая их код в файл
            TOOLS::BinaryWrite(OpenIn, (int)IsRule.length()); //Записываем длинну строки, отображающей показатель ограничения экземпляра
            for (int i=0; i<IsRule.length(); i++) //Далее в цикле проходим по каждому символу этой строки
                TOOLS::BinaryWrite(OpenIn, (int)IsRule[i]); //Записывая их код в файл
            TOOLS::BinaryWrite(OpenIn, (int)IsBlocked.length()); //Записываем длинну строки, отображающей показатель ограничения экземпляра
            for (int i=0; i<IsBlocked.length(); i++) //Далее в цикле проходим по каждому символу этой строки
                TOOLS::BinaryWrite(OpenIn, (int)IsBlocked[i]); //Записывая их код в файл
            return true; //Возвращаем что запись прошла успешно
        }
        return false; //Возвращаем что запись прошла не успешно
    }

#ifdef DEBAG //Если в MACRO_FOR_RUN объявлен режим отладки, то
    QString GetInfo(){return "["+Nikname+"] ["+Password+"] ["+QString::number(usesRule);} //Доступная извне функция возвращающая информацию о экземпляре
#endif //Конец условия
private: //Функции и члены закрытые от внешнего воздействия

    static std::string decrypt_func(std::string input) //CHANGE TO 3-th lab
    {
        for (int i=0; i<input.length(); i++)
            input[i]=input[i]^KEY_TO_ENCRYPT_DECRYPT[i%input.length()];
        return input;
    }
    static std::string encrypt_func(std::string input){ //CHANGE TO 3-th lab
        return decrypt_func(input);
    }
    inline static const std::string KEY_TO_ENCRYPT_DECRYPT="12d248bd28f33ec9021b3066cf6e2f5e";


    QString Nikname=""; //Имя экземпляра класса
    QString Password=""; //Пароль экземпляра класса
    bool usesRule=false, blocked=false;  //Показатели ограничения и блокировки экземпляра класса
};

#endif // USER_ADMIN_H
