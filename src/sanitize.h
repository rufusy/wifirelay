#ifndef _SANITIZE_H
#define _SANITIZE_H

#include "main.h"

extern char serial_in_cfg[20];
extern char sanitize_serial_in_cfg[4][7]; //can store 4 words of 7 characters
extern char sanitize_timer_cfg[2][7]; 

void get_serial_in(char data_in[]);
//void sanitize_serial_in(char delimitor, char src[]);
void sanitize_serial_in(char delimitor, char src[], char dest[][7]);

#endif
