/******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************
* PROJECT:                  PiUE Simple Console                                                                        *
* FILENAME:                 pscon.h                                                                                    *
* AUTHOR:                   Wojciech M. Szyfelbein                                                                     *
* INFO:                     This file consist declaration of all console function and needed types and macros          *
******************************************* PiUE - Wojciech M. Szyfelbein  ********************************************/

#ifndef PCON_H_INCLUDED
#define PCON_H_INCLUDED

#include <stdint.h>

#include "pscon_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Option definition struct
 *
 *  This structure consist all data to define one option for the command.
 */
struct pc_Option
{
    pc_OptNameType Opt;                 //!<Character to identify option (one char only)
    pc_OptTypeType Type;                //!<Type of option defined in enum pc_OptionType
};

/*! \brief Defines types of options
 *
 *  This structure consist all data to define one option for the command.
 */
enum pc_OptionType
{
    pc_otEnd,                           //!<To identify last empty option in array
    pc_otFlag,                          //!<switch flags, only one letter
    pc_otChar,                          //!<integer value, can use 0xHex 0bBinary 0Octal or 'c'
    pc_otInt8=pc_otChar,                //!<integer value, can use 0xHex 0bBinary 0Octal
    pc_otUInt8,                         //!<integer value, can use 0xHex 0bBinary 0Octal
    pc_otInt,                           //!<integer value, can use 0xHex 0bBinary 0Octal
    pc_otUInt,                          //!<integer value, can use 0xHex 0bBinary 0Octal
    pc_otStr,                           //!<string value
};

/*! \brief Command definition struct
 *
 *  This structure consist all data to define one command
 */
struct pc_Cmd
{
    char *Cmd;                          //!<Command string
    void (*Fun)(void);                  //!<Command function which be called when command was entered
#if pc_UseOptions!=0
    struct pc_Option *Opts;             //!<Pointer to array of options defines
#endif
};

/*! \brief Array of all defined commands
 *
 *  In this array should be all pointers to defined commands
 */
extern struct pc_Cmd pcCmds[];

/*! \brief
 *
 *  Function realize command is entered
 */
extern void pc_DoCmd(void);

/*! \brief
 *
 *  Function
 */
extern char pc_GetNextPrmType(void);

/*! \brief
 *
 *  Function
 */
extern char pc_GetNextPrmFlag(void);
extern uint32_t pc_GetNextPrmUINT32(void);
extern int32_t pc_GetNextPrmINT32(void);
extern char pc_GetNextPrmSTR(void);

/*! \brief Run console enter task (thread or concurrence)
 *
 *  Detailed description starts here.
 */
extern void pc_Console(void);

#ifdef __cplusplus
}
#endif

#endif // PCON_H_INCLUDED


