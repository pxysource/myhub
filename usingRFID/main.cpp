#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (3 != argc)
    {
        printf("Usage: ./usingRFID <IpAddress> <Port>");
        exit(-1);
    }
    //为闹钟信号指定注册函数
    signal(SIGALRM, refresh);
    Widget w;
    w.setHostIp(QString(argv[1]));
    w.setPort(QString(argv[2]));
    w.show();

    return a.exec();
}
