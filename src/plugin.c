#include "config.h"

/* plugins are DLLs */
#define WS_BUILD_DLL
#include "ws_symbol_export.h"

#include "epan/proto.h"

#ifndef VERSION_RELEASE
    #define STR(a) # a
    #define VERSION_RELEASE (STR(VERSION_MAJOR) "." STR(VERSION_MINOR))
#endif

WS_DLL_PUBLIC_DEF const gchar plugin_version[] = PLUGIN_VERSION;
WS_DLL_PUBLIC_DEF const gchar plugin_release[] = VERSION_RELEASE;
WS_DLL_PUBLIC_DEF const int plugin_want_major = VERSION_MAJOR;
WS_DLL_PUBLIC_DEF const int plugin_want_minor = VERSION_MINOR;

void proto_register_tll(void);
void proto_reg_handoff_tll(void);

WS_DLL_PUBLIC void plugin_register(void);

void plugin_register(void)
{
	static proto_plugin plug_rts_frame;

    plug_rts_frame.register_protoinfo = proto_register_tll;
    plug_rts_frame.register_handoff = proto_reg_handoff_tll;
	proto_register_plugin(&plug_rts_frame);
}
