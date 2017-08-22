#include <iostream>
#include <QApplication>
#include <mainwindow.h>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>

using namespace std;

std::ostream& operator<< (std::ostream& os, const QString& qs)
{
    os << qs.toStdString();
    return os;
}

std::ostream& operator<< (std::ostream& os, const QStringList& qsl)
{
    os << "[" << std::endl;
    for (const auto& qs: qsl)
    {
        os << '\t' << qs << std::endl;
    }
    os << "]";
    return os;
}



int main (int argc, char** argv)
{
    QApplication app (argc, argv);

    MainWindow* mainw = new MainWindow;
    mainw->show();

    return app.exec();
}
