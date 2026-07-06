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
#if pc_UseOptions==0
void Fun1(pc_IndexType iCmd)
{
    printf("\n\rFun1 z 2\n\r");
}

void Fun2(pc_IndexType iCmd)
{
    printf("\n\rFun2 z 2\n\r");
}
#endif
#if pc_UseOptions==1
static char *apcOptTypes[]= {"End","Error","Flag","Int8","UInt8","Int","UInt","Str"};

void Fun1p(pc_IndexType iCmd)
{
    enum pc_OptionType Ot;
    printf("\n\rFun1p z 2\n\r");
    do {
        Ot=pc_GetNextPrmType(iCmd);
        printf("Option type: %s\n\r", apcOptTypes[Ot]);
        switch (Ot) {
            case pc_otFlag:
                printf("Flag value: %c\n\r\n\r",pc_GetFlag());
                break;
            case pc_otUInt:
                uint32_t uRes=pc_GetNextPrmUINT32();
                printf("UINT value: %d\n\r\n\r",uRes);
                break;
            case pc_otStr:
                printf("Flag value: %s\n\r\n\r",pc_GetNextPrmSTR());
                break;
            default:
                break;
        }
    } while (Ot!=pc_otEnd && Ot!=pc_otError);
}

void Fun2p(pc_IndexType iCmd)
{
    printf("\n\rFun2p z 2\n\r");
}
#endif //pc_UseOptions==0

void Exit(pc_IndexType iCmd)
{
    printf("Exit\n\r");
    exit(0);
}

#if pc_UseOptions==0
pc_BeginCmdArr
    pc_DefineCmd("cmd1", Fun1),
    pc_DefineCmd("cmd2", Fun2),
    pc_DefineCmd("exit", Exit),
    pc_EndCmdList
pc_EndCmdArr;
#endif

#if pc_UseOptions==1
pc_BeginCmdArr
    pc_BeginCmd("cmd1p",Fun1p)
        pc_DefineParam('f',pc_otFlag),
        pc_DefineParam('F',pc_otFlag),
        pc_DefineParam('u',pc_otUInt),
        pc_DefineParam('s',pc_otStr),
        pcEndParamList,
    pc_EndCmd,
    pc_BeginCmd("cmd2p",Fun2p)
        pc_DefineParam('1',pc_otFlag),
        pc_DefineParam('3',pc_otFlag),
        pcEndParamList,
    pc_EndCmd,
    pc_BeginCmd("exit",Exit)
        pc_DefineParam('3',pc_otEnd),
    pc_EndCmd,
    pc_PSVerCmd,
    pc_EndCmdList
pc_EndCmdArr;
#endif



int main()
{
    pc_InitTrmCon();
    while (pc_ConType == pc_cConTypeConcurrence) pc_Console();
    pc_DeInitTrmCon();
    return 0;
}
