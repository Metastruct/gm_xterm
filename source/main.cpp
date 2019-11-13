#include "main.hpp"

class CSpewListener : public ILoggingListener
{
public:
	void Log(const LoggingContext_t *pContext, const char *msg)
	{
		int RED    = pContext->m_Color.r;
		int GREEN  = pContext->m_Color.g;
		int BLUE   = pContext->m_Color.b;
		//int ALPHA  = pContext->m_Color.a;
		
		const LoggingSeverity_t Type = pContext->m_Severity;
		
		bool printcol = RED>0 || GREEN>0 || BLUE>0;
		bool white = RED==255 && GREEN == 255 && BLUE == 255;
		if (Type==LS_WARNING && white) {
			RED=255;
			GREEN=100;
			BLUE=50;
		} else if (Type==LS_MESSAGE && white) {
			RED=200;
			GREEN=235;
			BLUE=255;
		} else if ( (Type==LS_ERROR || Type==LS_ASSERT) && white) {
			RED=255;
			GREEN=0;
			BLUE=0;
		} else if (RED==156 && GREEN==241 && BLUE==255) { // Msg
			RED=255;
			//GREEN=241;
			BLUE=156;
		} else if (RED==136 && GREEN==221 && BLUE==221) { // MsgN
			RED=221;
			//GREEN=221;
			BLUE=136;
		} else if (RED==136 && GREEN==221 && BLUE==255) { // error
			RED=255;
			GREEN=221 - 120;
			BLUE=136 - 80;
		}
		
		if (!printcol) {
			if (Type != LS_MESSAGE) {
				RED = 255;
				BLUE = 255;
			} else {
				printf( "%s", msg );
				return;
			}
		}
		
		int val = rgb2xterm(RED,GREEN,BLUE);
		//const char * grp = GetSpewOutputGroup();
		
		//printf("<%d,%d,%d,%d|%d,%d,%s>\x1b[38;5;%dm",RED,GREEN,BLUE,ALPHA,  Type,GetSpewOutputLevel(),grp?grp:"", val); // set color
		
		char buf[20] = "";
		snprintf(buf,sizeof(buf),"\x1b[38;5;%dm",val); // set color
		
		
		printf( "%s", buf );
		printf( "%s", msg );
		printf( "\x1b[0m" );
		
	}
} g_SpewListener;


GMOD_MODULE_OPEN( )
{
	
	void *tier0lib;
	
	tier0lib = dlopen("bin/linux64/libtier0.so",RTLD_LAZY);
	
	*(void**)&LoggingSystem_PushLoggingState = dlsym(tier0lib, "LoggingSystem_PushLoggingState");
	*(void**)&LoggingSystem_RegisterLoggingListener = dlsym(tier0lib, "LoggingSystem_RegisterLoggingListener");
	*(void**)&LoggingSystem_ResetCurrentLoggingState = dlsym(tier0lib, "LoggingSystem_ResetCurrentLoggingState");
	*(void**)&LoggingSystem_PopLoggingState = dlsym(tier0lib, "LoggingSystem_PopLoggingState");
	
	LoggingSystem_PushLoggingState(false, true);
	LoggingSystem_RegisterLoggingListener(&g_SpewListener);
	return 0;
}

GMOD_MODULE_CLOSE( )
{
	LoggingSystem_PopLoggingState(false);
	return 0;
}
