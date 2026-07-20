/**
* The MIT License (MIT)
*
* Copyright (c) 2026 Saibot
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


#define HAVE_STDINT_H
#include <malloc.h>
#include "samp-sdk/amx/amx.h"
#include "samp-sdk/plugincommon.h"

#include "natives.h"
#include "common.h"

//------------------------------------------------------------------------------------------------------------

logprintf_t logprintf = nullptr;
std::unique_ptr<PawnMap> g_PawnMap;

//------------------------------------------------------------------------------------------------------------

AMX_NATIVE_INFO Natives[] =
{
	{ "Map_Create",					native::Map_Create },
	{ "Map_Destroy",				native::Map_Destroy },
	{ "Map_Clear",					native::Map_Clear },
	{ "Map_Clone",					native::Map_Clone },
	{ "Map_Merge",					native::Map_Merge },
	{ "Map_GetVersion",				native::Map_GetVersion },
	{ "Map_ContainsKey",			native::Map_ContainsKey },
	{ "Map_IsValid",				native::Map_IsValid },
	{ "Map_Get",					native::Map_Get },
	{ "Map_GetKeyCount",			native::Map_GetKeyCount },
	{ "Map_GetFreeKey",				native::Map_GetFreeKey },
	{ "Map_GetKeyByIndex",			native::Map_GetKeyByIndex },
	{ "Map_ContainsStringKey",		native::Map_ContainsStringKey },
	{ "Map_GetIdByStringKey",		native::Map_GetIdByStringKey },
	{ "Map_RemoveKey",				native::Map_RemoveKey },
	{ "Map_Internal_SafeRemoveKey",	native::Map_Internal_SafeRemoveKey },
	{ "Map_Internal_LoopStep",		native::Map_Internal_LoopStep },
	{ "Map_RenameKey",				native::Map_RenameKey },
	{ "Map_Swap",					native::Map_Swap },
	{ "Map_SortByKey",				native::Map_SortByKey },
	{ "Map_SortByField",			native::Map_SortByField },
	{ "Map_FindKeyByField",			native::Map_FindKeyByField },
	{ "Map_StringKeyToInt",			native::Map_StringKeyToInt },
	{ "Map_GetStringById",			native::Map_GetStringById },
	{ "Map_Set",					native::Map_Set },
	{ "Map_SetString",				native::Map_SetString },
	{ 0, 0 }
};

//------------------------------------------------------------------------------------------------------------


PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
	g_PawnMap = std::make_unique<PawnMap>();
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	logprintf(" ");
	logprintf(" [PawnMap Plugin] ------------------------------------- ");
	logprintf(" [PawnMap Plugin]  Version: %s", PLUGIN_VERSION);
	logprintf(" [PawnMap Plugin]  Author:  Saibot");
	logprintf(" [PawnMap Plugin]  Status:  Loaded successfully");
	logprintf(" [PawnMap Plugin] ------------------------------------- ");
	logprintf(" ");
	return true;
}

//------------------------------------------------------------------------------------------------------------

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("PawnMap Plugin Unloaded");
}

//------------------------------------------------------------------------------------------------------------

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

//------------------------------------------------------------------------------------------------------------

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	return amx_Register(amx, Natives, -1);
}

//------------------------------------------------------------------------------------------------------------

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	return AMX_ERR_NONE;
}

//------------------------------------------------------------------------------------------------------------