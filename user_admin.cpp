#include "user_admin.h" //Указываем с чем связан данный файл реализации

User::User(QString Nikname, QString Password, bool usersRule, bool blocked){

    //!Может быть непонятно
    // this - указатель на текущий экземпляр то есть указывает что допустим в this->Res1, Res1-это именно поле экземпляра класса а не локальная переменная с тем же именем

    this->Nikname=Nikname;
    this->Password=Password;
    this->usesRule=usersRule;
    this->blocked=blocked;

    //Устанавливаем соответствующие значениям заданным в конструкторе
}
