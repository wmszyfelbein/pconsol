/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* PROJECT:                  PiUE Simple Console                                                                        *
* FILENAME:                 pscon.c                                                                                    *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* INFO:                     Example application for console                                                            *
******************************************** PiUE - Wojciech M. Szyfelbein  *******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#if defined(__linux__)
#include <sys/ioctl.h>
#include <termios.h>
#endif // __linux__

#if defined(WIN32)
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
    pc_InitTrmCon();
    while (pc_ConType == pc_cConTypeConcurrence) pc_Console();
    pc_DeInitTrmCon();
    return 0;
}
