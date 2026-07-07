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

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#define pc_PSCONVER "0.9"
#define pc_PSCONTYPE "Min"
#define pc_PSCONINFO "The PiUE Simple Console for small CPU but not only"

/*! \brief Option definition struct
 *
 *  This structure consist all data to define one option for the command.
 */
struct pc_Option
{
    char Opt;                           //!<Character to identify option (one char only)
    char Type;                          //!<Type of option defined in enum pc_OptionType
};

/*! \brief Defines types of options
 *
 *  This structure consist all data to define one option for the command.
 */
enum pc_OptionType:char
{
    pc_otEnd,                           //!<To identify last empty option in array
    pc_otError,                         //!<To identify error in getting option
    pc_otFlag,                          //!<switch flags, only one letter no value
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
    void (*Fun)(pc_IndexType iCmd);     //!<Command function which be called when command was entered
#if pc_UseOptions==1
    struct pc_Option *Opts;             //!<Pointer to array of options defines
#endif
};

/*! \brief Array of all defined commands
 *
 *  In this array should be all pointers to defined commands
 */
extern struct pc_Cmd const pcCmds[];

/*! \brief Start definition of pc_Cmd commands array.
 */
 #define pc_BeginCmdArr struct pc_Cmd const pcCmds[]=(struct pc_Cmd[]) {

/*! \brief  Finalize definition of pc_Cmd commands array.
 */
#define pc_EndCmdArr }

#if pc_UseOptions==0

/*! \brief Defines one command in no parameters configuration
 *
 * \param 1 Pointer to constant string name of command
 * \param 2 Pointer to void Fun(void) function, which is called when parser identify command
 */
#define pc_DefineCmd(name,fun) {.Cmd=name,.Fun=fun}
#endif

#if pc_UseOptions==1

/*! \brief Defines one command without parameter in configuration
 *
 * \param 1 Pointer to constant string name of command
 * \param 2 Pointer to void Fun(void) function, which is called when parser identify command
 */
#define pc_DefineCmd(name,fun) {.Cmd=name,.Fun=fun, .Opts=nullptr}

/*! \brief Start definition of command with parameters
 *
 * \param 1 Pointer to constant string name of command
 * \param 2 Pointer to void Fun(void) function, which is called when parser identify command
 */
#define pc_BeginCmd(name,fun) (struct pc_Cmd const){.Cmd=name,.Fun=fun,\
    .Opts=(struct pc_Option*) (struct pc_Option const[]) {

/*! \brief Finish definition of command with parameters
 */
#define pc_EndCmd }}

/*! \brief Command to sign end of command list
 */
#define pc_EndCmdList (struct pc_Cmd const){.Cmd=nullptr,.Fun=nullptr,.Opts=nullptr}

/*! \brief Command to print version of console
 */
#define pc_PSVerCmd (struct pc_Cmd const){.Cmd="psver",.Fun=pc_PrintPSConsoleVer,.Opts=nullptr}

/*! \brief Define one parameter
 *
 * \param 1 One char parameters name
 * \param 2
 */
#define pc_DefineParam(opt,type) (struct pc_Option const){.Opt=opt,.Type=type}

/*! \brief
*/
#define pcEndParamList pc_DefineParam('\0',pc_otEnd)

#endif // pc_UseOptions 1

/*! \brief Function find command in command list and realize it
 */
extern void pc_DoCmd(void);

/*! \brief Function gets next parameter id from entered line
 *
 * \param 1 One char parameters name
 */
extern enum pc_OptionType pc_GetNextPrmType(pc_IndexType iCmdCnt);

 #if pc_UseOptions==1

 /*! \brief  Get flag parameter value (ASCII character)
  *
  * \return ASCII character which represent a flag
 */
extern char pc_GetFlag(void);

/*! \brief Get integer parameter value
 *
 * \return Integer value as a result of conversion
 */
extern int32_t pc_GetNextPrmINT32(void);

/*! \brief Get unsigned integer parameter value
 *
 * \return
 */
extern uint32_t pc_GetNextPrmUINT32(void);

/*! \brief Get ne
 *
 * \return Unsigned integer value as a result of conversion
 */
extern char* pc_GetNextPrmSTR(void);

/*! \brief Get last conversion error in getting parameters
 *
 * \return - 1 last conversion was error, 0 OK
 */
extern pc_IndexType pc_GetLastConvError(void);

/*! \brief Function print PiUE Console version and info
 */
extern void pc_PrintPSConsoleVer(pc_IndexType iCmd);

#endif // pc_UseOptions

/*! \brief Run console enter task (thread or concurrence)
 */
extern void pc_Console(void);

#ifdef __cplusplus
}
#endif
#endif // PCON_H_INCLUDED


