/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* PROJECT:                  PiUE Simple Console                                                                        *
* FILENAME:                 pscon.c                                                                                    *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* INFO:                                                                                                                *
******************************************** PiUE - Wojciech M. Szyfelbein  *******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pscon.h"

typedef unsigned pc_IndexType pc_uidx;

enum
{
    pc_CmdReady=-1,
    pc_CmdPrepLine=-2,
    pc_CmdParamProbe=-3,
};

static char PCInBuffer[pc_InBufferSize+1];//1 to end 0 char
static pc_IndexType pc_iCurPos=pc_CmdPrepLine;
static pc_IndexType pc_iError;

pc_IndexType pc_GetLastConvError()
{
    return pc_iError<0?pc_iError=0,-1:0;
};

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
#if pc_UseOptions==1
        for(pc_IndexType iPos=0;iPos<(pc_InBufferSize+1);iPos++) PCInBuffer[(pc_uidx)iPos]='\0';
#endif // pc_UseOptions
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
    for(iCmdCnt=0;pcCmds[(pc_uidx)iCmdCnt].Cmd!=nullptr;iCmdCnt++) {
        pc_IndexType iLnChrCnt;
        pc_IndexType iCmdChrCnt;
        for(iLnChrCnt=iStCmd,iCmdChrCnt=0;iLnChrCnt<=iSpCmd;iLnChrCnt++,iCmdChrCnt++) {
            if ((pcCmds[(pc_uidx)iCmdCnt].Cmd[(pc_uidx)iCmdChrCnt]=='\0') &&
                ((PCInBuffer[(pc_uidx)iLnChrCnt]=='\0') || PCInBuffer[(pc_uidx)iLnChrCnt]==' ')) {
                //command found, do it
                pc_iCurPos=iSpCmd;
                pcCmds[(pc_uidx)iCmdCnt].Fun(iCmdCnt);
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

void pc_PrintPSConsoleVer(pc_IndexType iCmd)
{
    pc_Print("\n\r"pc_PSCONINFO"\n\r");
    pc_Print("Ver: "pc_PSCONVER" "pc_PSCONTYPE"\n\r");
}

 #if pc_UseOptions==1
char pc_GetFlag(void)
{
    return PCInBuffer[(pc_uidx)pc_iCurPos++];
}

enum pc_AsciiCode
{
    pc_acBin =  2,
    pc_acOct =  8,
    pc_acDec = 10,
    pc_acHex = 16
};

static char pc_FromAsciiToBin(char cChr,enum pc_AsciiCode acCode)
{
    pc_iError=0;
    cChr-='0';
    do {
        if (cChr<0) {
            pc_iError=-1;
            break;
        }
        if ((cChr<acCode && acCode<pc_acHex) || ((cChr<pc_acDec && acCode==pc_acHex))) break;
        if (cChr>=acCode && acCode<pc_acHex) {
            pc_iError=-1;
            break;
        }
        cChr=cChr-('A'-'0')+10;
        if (cChr>=0xa && cChr<=0xf) break;
        cChr=cChr-('a'-'A');
        if (cChr>=0xa && cChr<=0xf) break;
        pc_iError=-1;
    } while (0);
    return cChr;
}

int32_t pc_GetNextPrmINT32(void)
{
    uint32_t uRes=0;
    void pc_GetNextPrmUINT32AndTest(void)
    {
        uRes=pc_GetNextPrmUINT32();
        if (uRes>0x80000000ul) {
            uRes=0xFFFFFFFFul;
            pc_iError=-1;
        }
    }
    if (PCInBuffer[(pc_uidx)pc_iCurPos]=='-') {
        pc_iCurPos++;
        pc_GetNextPrmUINT32AndTest();
        uRes=0-uRes;
    } else pc_GetNextPrmUINT32AndTest();
    return *((int32_t*)(&uRes));
}

uint32_t pc_GetNextPrmUINT32(void)
{
    uint32_t uRes=0;
    enum pc_AsciiCode acCode;
    do {
        if (PCInBuffer[(pc_uidx)pc_iCurPos]!='0') {
            acCode=pc_acDec;
            break;
        }
        pc_iCurPos++;
        switch (PCInBuffer[(pc_uidx)pc_iCurPos]) {
            case 'x': case 'X':
                acCode=pc_acHex; pc_iCurPos++;
                break;
            case 'b':
                acCode=pc_acBin ;pc_iCurPos++;
                break;
            default:
                acCode=pc_acOct;
                break;
        }
    } while(0);
    do {
        if (PCInBuffer[(pc_uidx)pc_iCurPos]=='\0' || PCInBuffer[(pc_uidx)pc_iCurPos]=='0') {
                pc_iError=-1;
                break;
        }
        do {
            uRes*=acCode;
            uRes+=pc_FromAsciiToBin(PCInBuffer[(pc_uidx)pc_iCurPos++],acCode);
            if(pc_iError!=0) break;
            if (PCInBuffer[(pc_uidx)pc_iCurPos]=='\0' || PCInBuffer[(pc_uidx)pc_iCurPos]==' ') {
                pc_iError=0;
                break;
            }
        } while(1);
    } while (0);
    return uRes;
}

char* pc_GetNextPrmSTR(void)
{
    char *pcStr = &PCInBuffer[(pc_uidx)pc_iCurPos];
    //looking for end string, now only string up to 0 or space tab or end buffer
    for (;pc_iCurPos<pc_InBufferSize || PCInBuffer[(pc_uidx)pc_iCurPos]!='\0' || PCInBuffer[(pc_uidx)pc_iCurPos]!='\0';
            pc_iCurPos++);
    PCInBuffer[(pc_uidx)pc_iCurPos]='\0';
    return pcStr;
}

enum pc_OptionType pc_GetNextPrmType(pc_IndexType iCmdCnt)
{
    enum pc_OptionType oType=pc_otEnd;
    for (;pc_iCurPos<pc_InBufferSize && PCInBuffer[(pc_uidx)pc_iCurPos]!='\0'; pc_iCurPos++) {
        //looking for parameter - one char
        for (pc_IndexType iOptId=0; pcCmds[(pc_uidx)iCmdCnt].Opts[(pc_uidx)iOptId].Type!=pc_otEnd; iOptId++) {
            if (PCInBuffer[(pc_uidx)pc_iCurPos]==pcCmds[(pc_uidx)iCmdCnt].Opts[(pc_uidx)iOptId].Opt) {
                oType=pcCmds[(pc_uidx)iCmdCnt].Opts[(pc_uidx)iOptId].Type;
                char cChr=PCInBuffer[(pc_uidx)pc_iCurPos+1];
                do {
                    if (cChr=='\0') {
                        if (oType==pc_otFlag ) break;
                        oType=pc_otError;
                        break;
                    }
                    if (cChr==' ' || cChr=='\t' ) {
                        if (oType==pc_otFlag ) break;
                        pc_iCurPos+=2;
                    } else {
                        oType=pc_otError;
                        break;
                    }
                    do {
                        cChr=cChr=PCInBuffer[(pc_uidx)pc_iCurPos];
                        if (cChr==' ' || cChr=='\t' ) {
                            pc_iCurPos++;
                            continue;
                        } else {
                            if (cChr=='\0')
                                oType=pc_otError;
                        }
                        break;
                    } while (1);
                } while (0);
                return oType;
            }
        }
    }
    return oType;
};
#endif // pc_UseOptions


