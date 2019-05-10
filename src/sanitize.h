#ifndef _SANITIZE_H
#define _SANITIZE_H

#include "main.h"

extern char serial_in_cfg[100];
extern char sanitize_serial_in_cfg[10][10]; //can store 10 words of 10 characters
extern size_t i,j,cnt;

void get_serial_in(void);
void sanitize_serial_in(void);

#endif
