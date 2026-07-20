#include "natives.h"
#include "common.h"

namespace native
{
//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Create(AMX* amx, cell* params)
	{
		return static_cast<cell>(g_PawnMap->сreate());
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Destroy(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->destroy(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Clear(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->clear(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Clone(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->clone(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Merge(AMX* amx, cell* params)
	{
		int32_t targetId = static_cast<int32_t>(params[1]);
		int32_t sourceId = static_cast<int32_t>(params[2]);

		return static_cast<cell>(g_PawnMap->merge(targetId, sourceId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetVersion(AMX* amx, cell* params)
	{
		cell* addr = nullptr;
		if (amx_GetAddr(amx, params[1], &addr) == AMX_ERR_NONE && addr != nullptr)
		{
			amx_SetString(addr, PLUGIN_VERSION, 0, 0, sizeof(PLUGIN_VERSION));
			return 1;
		}
		return 0;
	}

//----------------------------------------------------------------------------------------------------------------------------
}