#ifndef CRFID_H
#define CRFID_H

#ifdef __cplusplus
extern "C" {
#endif


void request_card(int fd);

int get_id(int fd);

void beep(int times, float sec);

void refresh(int sig);

#ifdef __cplusplus
}
#endif

#endif // RFID_H
