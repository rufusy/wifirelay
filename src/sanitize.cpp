#include "sanitize.h"

char serial_in_cfg[100];
char sanitize_serial_in_cfg[10][10]; //can store 10 words of 10 characters
size_t i,j,cnt;

void get_serial_in(void)
{
    strcpy(serial_in_cfg,"red.green.blue");
}


void sanitize_serial_in(void)
{
    j=0; cnt=0;
    for(i=0;i<=(strlen(serial_in_cfg));i++)
    {
        // if space or NULL found, assign NULL into splitStrings[cnt]
        if(serial_in_cfg[i]=='.'||serial_in_cfg[i]=='\0')
        {
            sanitize_serial_in_cfg[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            sanitize_serial_in_cfg[cnt][j]=serial_in_cfg[i];
            j++;
        }
    }
}
