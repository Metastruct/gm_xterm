#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/LuaInterface.h>
#include <lua.hpp>
#include <stdint.h>
#include <stdio.h>
#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/SourceCompat.h>
#include <scanning/symbolfinder.hpp>
#include <detouring/hook.hpp>
#include <dlfcn.h>

extern "C" {
	#include "rgb2xterm.h"
}

// TODO
//#define WINDOWS_SIGNATURE "\x55\x8B\xEC\x83\xEC\x14\x8B\x45\x08\x53\x8B\xD9\x89\x5D\xFC\x85"
//#define WINDOWS_SIGNATURE "\x83\xEC\x14\x8B\x44\x24\x18\x8B\x54\x24\x1C\x53\x89\x44\x24\x08\x56\x8D\x04\x40"

#define LINUX_SIGNATURE "LoggingSystem_Log"
#define MAC_SIGNATURE "LoggingSystem_Log"


void (*LoggingSystem_PushLoggingState)(bool bThreadLocal, bool bClearState);
void (*LoggingSystem_PopLoggingState)(bool bThreadLocal);
void (*LoggingSystem_RegisterLoggingListener)(void* listener);
void (*LoggingSystem_ResetCurrentLoggingState)(void);

enum LoggingResponse_t
{
	LR_CONTINUE,
	LR_DEBUGGER,
	LR_ABORT,
};

typedef int LoggingChannelID_t;
const LoggingChannelID_t INVALID_LOGGING_CHANNEL_ID = -1;
enum LoggingSeverity_t
{
	LS_MESSAGE = 0,
	LS_WARNING = 1,
	LS_ASSERT = 2,
	LS_ERROR = 3,
	LS_HIGHEST_SEVERITY = 4,
};

enum LoggingChannelFlags_t
{
	LCF_CONSOLE_ONLY = 0x00000001,
	LCF_DO_NOT_ECHO = 0x00000002,
};

struct LoggingContext_t
{
	LoggingChannelID_t m_ChannelID;
	LoggingChannelFlags_t m_Flags;
	LoggingSeverity_t m_Severity;
	Color m_Color;
};

class ILoggingListener
{
public:
	virtual void Log(const LoggingContext_t *pContext, const char *pMessage) = 0;
};