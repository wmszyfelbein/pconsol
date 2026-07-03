/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* FILENAME:                 pscon.c                                                                                    *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* DESCRIPTION:                                                                                                         *
*		                                                                                                               *
******************************************** PiUE - Wojciech M. Szyfelbein  *******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __linux__
#include <sys/ioctl.h>
#include <termios.h>
#endif // __linux__

#if defined(WIN32) || defined(WIN64)
#include <conio.h>
#endif //defined(WIN32) || defined(WIN64)

#include "pscon.h"

#ifdef __linux__

static struct termios oldattr, newattr;
void pc_InitTrmCon(void)
{
   tcgetattr( STDIN_FILENO, &oldattr );
   newattr=oldattr;
   newattr.c_lflag &= ~(ICANON | ECHO);
   newattr.c_oflag &= ~(OCRNL);
   newattr.c_iflag &= ~(IGNCR | ICRNL);
   tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
   tcsetattr( STDOUT_FILENO, TCSANOW, &newattr );
   //return pc_Terminal;
}

void pc_DeInitTrmCon(void)
{
   tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
   tcsetattr( STDOUT_FILENO, TCSANOW, &newattr );
}

#define _getch getchar

#endif // __linux__

#if defined(WIN32) || defined(WIN64)

#define pc_InitTrmCon()
#define pc_DeInitTrmCon()

#endif // defined(WIN32) || defined(WIN64)

char pc_TstGetCh(void)
{
   return (char)_getch();
}

void pc_TstPutCh(char ch)
{
   (void)putchar((int)ch);
}


//----------------------------------------------------------------------------------------------------------------------
void Fun1(void)
{
    printf("\n\rFun1 z 2\n\r");
}

void Fun2(void)
{
    printf("\n\rFun2 z 2\n\r");
}

void Exit(void)
{
    printf("Exit\n\r");
    exit(0);
}

struct pc_Cmd pcCmds[]=
{
    {"cmd1", Fun1},
    {"cmd2", Fun2},
    {"exit", Exit}
};


int main()
{
    printf("START\n");
    pc_InitTrmCon();
    pc_Console();
    pc_DeInitTrmCon();
    printf("\nEND\n");
    return 0;
}
