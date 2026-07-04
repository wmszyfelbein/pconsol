/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* PROJECT:                  PiUE Simple Console                                                                        *
* FILENAME:                 pscon.c                                                                                    *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* INFO:                                                                                                                *
******************************************** PiUE - Wojciech M. Szyfelbein  *******************************************/

#include <stdio.h>
#include <stdlib.h>

#include "pscon.h"

typedef unsigned pc_IndexType pc_uidx;

enum
{
    pc_CmdReady=-1,
    pc_CmdPrepLine=-2,
};

static char PCInBuffer[pc_InBufferSize];
static pc_IndexType pc_iCurPos=pc_CmdPrepLine;

static void pc_Print(char* sStr)
{
    while(*sStr) pc_PutChar(*sStr++);
}

static pc_IndexType pc_FindStCmd(void)
{
    pc_IndexType iPos;
    for (iPos=0; PCInBuffer[(pc_uidx)iPos]==' '
        && iPos<pc_InBufferSize; iPos++);
    return iPos;
}

static pc_IndexType pc_FindSpCmd(pc_IndexType iPos)
{
    for(;PCInBuffer[(pc_uidx)iPos]!='\0' && PCInBuffer[(pc_uidx)iPos]!=' ' && iPos<pc_InBufferSize; iPos++);
    return iPos;
}

static void pc_PrepNewLine(void)
{
    if (pc_iCurPos==pc_CmdPrepLine) {
        pc_Print("\r\n");
        pc_iCurPos=0;
        if (sizeof(pc_sPromptStr)>1) pc_Print(pc_sPromptStr); //should be optimized in final
    }
}

void pc_GetCmdLine(void)
{
    pc_PrepNewLine();
    do {
        char cChar=pc_GetChar();
        switch (cChar) {
            case '\b': case '\x7f':
                if (pc_iCurPos==0) continue;
                pc_Print("\b \b");
                pc_iCurPos--;
                continue;
            case '\r':case '\n':
                PCInBuffer[(pc_uidx)pc_iCurPos++]='\0';
                pc_iCurPos=pc_CmdReady;
                return;
            default:
                if (!(cChar>=' ' && cChar<='~')) continue;
                if (pc_iCurPos>=(pc_InBufferSize-1)) continue;
                PCInBuffer[(pc_uidx)pc_iCurPos++]=cChar;
        }
        pc_PutChar(cChar);
    } while (pc_ConType == pc_ConTypeThread);
}

void pc_DoCmd(void)
{
    if (pc_iCurPos!=pc_CmdReady) return;
    pc_IndexType iStCmd = pc_FindStCmd();
    pc_IndexType iSpCmd = pc_FindSpCmd(iStCmd);
    pc_IndexType iCmdCnt;
    for(iCmdCnt=0;iCmdCnt<pc_CmdCnt;iCmdCnt++) {
        pc_IndexType iLnChrCnt;
        pc_IndexType iCmdChrCnt;
        for(iLnChrCnt=iStCmd,iCmdChrCnt=0;iLnChrCnt<=iSpCmd;iLnChrCnt++,iCmdChrCnt++) {
            if ((pcCmds[(pc_uidx)iCmdCnt].Cmd[(pc_uidx)iCmdChrCnt]=='\0') &&
                ((PCInBuffer[(pc_uidx)iLnChrCnt]=='\0') || PCInBuffer[(pc_uidx)iLnChrCnt]==' ')) {
                //command found, do it
                pcCmds[(pc_uidx)iCmdCnt].Fun();
                pc_iCurPos=pc_CmdPrepLine;
                return;
            }
            if ((pcCmds[(pc_uidx)iCmdCnt].Cmd[(pc_uidx)iCmdChrCnt]==
                                  PCInBuffer[(pc_uidx)iLnChrCnt]) && (pcCmds[(pc_uidx)iCmdCnt].Cmd[(pc_uidx)iCmdChrCnt]!=0))
                continue;
            //command did't find or test next command
            break;
        }
    }
    pc_Print("\r\n"pc_sInfoNoCmd);
    pc_iCurPos=pc_CmdPrepLine;
}

void pc_Console(void)
{
    do {
        pc_GetCmdLine();
        pc_DoCmd();
    } while (pc_ConType == pc_ConTypeThread);
}
