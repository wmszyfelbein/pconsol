/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* PROJECT:                  PiUE Simple Console                                                                        *
* FILENAME:                 pscon_conf.h                                                                               *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* INFO:                     This file consist configuration defines and declarations of user needed functions          *
******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************/

#ifndef PSCON_CONF_H_INCLUDED
#define PSCON_CONF_H_INCLUDED

#include <stdint.h>

//!Defines type used to any index/counter type. Char type generates small (on tested) code in 8b micro-controller.
//!For 16+b should be int..
#define pc_IndexType char

//!Size of input buffer. Type of array is char
#define pc_InBufferSize (10)

//!pc_ConType - Execution type. Loot at pc_ConTypeThread and pc_ConTypeThread
#define pc_ConType (pc_cConTypeConcurrence)
//!pc_ConTypeThread - There is a special thread for execute console. pc_GetChar - macro should block thread
#define pc_ConTypeThread (1)
//!pc_cConTypeConcurrence - There in not a thread for console. Library remember state of between call pc_Console.
#define pc_cConTypeConcurrence (0)

//!pc_UseOptions - Set to 1 to use options
#define pc_UseOptions (0)
#define pc_OptNameType char
#define pc_OptTypeType char

#define pc_OneGetFunUINT (0)
#define pc_MaxIntSize (16)

//!Defines function to put function. Function should have prototype as void pc_TstPutCh(pc_CharType ch).
#define pc_PutChar(x) pc_TstPutCh(x)
//!Defines function to get char from the input. Function should have prototype as pc_CharType pc_TstGetCh(void).
#define pc_GetChar pc_TstGetCh

//!Defines how many commands in command array
#define pc_CmdCnt (3)

//!Info string displayed if command not in command array
#define pc_sInfoNoCmd "Command doasn't exist"

//!Info string displayed if command not in command array
#define pc_sInfoNoOpt "Option for commnad: %s doasn't exist"

//!Prompt string. Can be ""
#define pc_sPromptStr "|>"

//!Function which should be defined to get char (look at macro pc_GetChar). Here is an example.
//!Can be macro to. Remember add your header.
extern char pc_TstGetCh(void);
//!Function which should be defined to put char (look at macro pc_PutChar). Here is an example.
//!Can be macro to. Remember add your header.
extern void pc_TstPutCh(char ch);

#endif // PSCON_CONF_H_INCLUDED

