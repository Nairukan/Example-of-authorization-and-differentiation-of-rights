#include <registrationwind.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegistrationWind w;
    w.show();
    return a.exec();
}
