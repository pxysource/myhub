#include "iso14443a.h"

void request_card(int fd)
{
    init_REQUEST();
    char recvinfo[128];

    char a[] = "|/-\\";
    for(int i=0; ; i++)
    {
        // 向串口发送指令
        tcflush (fd, TCIFLUSH);
        write(fd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

        usleep(10*1000);

        bzero(recvinfo, 128);
        read(fd, recvinfo, 128);

        //应答帧状态部分为0 则请求成功
        if(recvinfo[2] == 0x00)
        {
            break;
        }
        else
        {
            fprintf(stderr, "等待卡片%c\r", a[i%4]);
            usleep(200*1000);
        }
    }
}


int get_id(int fd)
{
    // 刷新串口缓冲区
    tcflush (fd, TCIFLUSH);
    tcflush (fd, TCOFLUSH);

    // 初始化获取ID指令并发送给读卡器
    init_ANTICOLL();
    write(fd, PiccAnticoll1, PiccAnticoll1[0]);

    usleep(10*1000);

    // 获取读卡器的返回值
    char info[256];
    bzero(info, 256);
    read(fd, info, 128);

    // 应答帧状态部分为0 则成功
    uint32_t id = 0;
    if(info[2] == 0x00)
    {
        memcpy(&id, &info[4], info[3]);

        if(id == 0)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return id;
}

void beep(int times, float sec)
{
    int buz = open("/dev/beep", O_RDWR);
    if(buz <= 0)
    {
        perror("打开蜂鸣器失败");
        return;
    }

    for(int i=0; i<times; i++)
    {
        // 响
        ioctl(buz, 0, 1);
        usleep(sec*1000*1000);

        // 静
        ioctl(buz, 1, 1);
        usleep(sec*1000*1000);
    }

    close(buz);
}

bool flagxx = true;

void refresh(int sig)
{
    // 卡片离开1秒后
    flagxx = true;
}
