#ifndef FCTRL_H
#define FCTRL_H

#define O_RDONLY     0x0
#define O_WRONLY     0x2
#define O_RDWR       0x3

#define O_CREAT      0x4
#define O_NONBLOCK   0x8

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

enum {
	F_GETFL = 0,
	F_SETFL
};

#ifdef __cplusplus
extern "C" {
#endif


int  open(const char* name, int oflag);
void close(int fd);
int  fcntl(int fd, int cmd, int data);

#ifdef __cplusplus
}
#endif

#endif
