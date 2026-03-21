#define SAMP_SDK_WANT_AMX_EVENTS

#include "../samp-sdk/samp_sdk.hpp"
#include "natives.h"

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Create, AMX* amx, cell* params)
{
	return static_cast<cell>(g_PawnMap->сreate());
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Destroy, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->destroy(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Clear, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->clear(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Clone, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->clone(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Merge, AMX* amx, cell* params)
{
	int32_t targetId;
	int32_t sourceId;

	Register_Parameters(targetId, sourceId);
	return static_cast<cell>(g_PawnMap->merge(targetId, sourceId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_GetVersion, AMX* amx, cell* params)
{
	if (cell* addr = Samp_SDK::amx::Get_Addr_Safe(amx, 0))
	{
		Samp_SDK::amx::Set_String(addr, PLUGIN_VERSION, sizeof(PLUGIN_VERSION));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------