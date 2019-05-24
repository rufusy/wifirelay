#include "sanitize.h"

char    serial_in_cfg[20] = {'\0'},
        sanitize_serial_in_cfg[4][7] = {'\0'},   //can store 10 words of 10 characters
        sanitize_timer_cfg[2][7] = {'\0'}; 
   
void
get_serial_in(char data_in[])
{   
    memset(serial_in_cfg, '\0', sizeof(serial_in_cfg));
    strcpy(serial_in_cfg,data_in);
}


/*void
sanitize_serial_in(char delimitor, char src[])
{
    char i,j,cnt;
    j=0; cnt=0;
    memset(sanitize_serial_in_cfg, '\0', sizeof(sanitize_serial_in_cfg));
    for(i=0;i<=(strlen(src));i++)
    {
        // if delimitor or NULL found, assign NULL into splitStrings[cnt]
        if(src[i]== delimitor ||src[i]=='\0')
        {
            sanitize_serial_in_cfg[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            sanitize_serial_in_cfg[cnt][j]=src[i];
            j++;
        }
    }
}
*/

void
sanitize_serial_in(char delimitor, char src[], char dest[][7])
{
    char i,j,cnt;
    j=0; cnt=0;
    memset(dest, '\0', sizeof(dest));
    for(i=0;i<=(strlen(src));i++)
    {
        // if delimitor or NULL found, assign NULL into splitStrings[cnt]
        if(src[i]== delimitor ||src[i]=='\0')
        {
            dest[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            dest[cnt][j]=src[i];
            j++;
        }
    }
    memset(src, '\0', sizeof(src));
}

