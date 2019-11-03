#include "widget.h"
#include <QApplication>
#include <signal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //为闹钟信号注册函数
    signal(SIGALRM, refresh);
    Widget w;
    w.show();

    return a.exec();
}
