#pragma once

#define HAVE_STDINT_H
#include <samp-sdk/amx/amx.h>
#include "pawn_map.h"

//----------------------------------------------------------------------------------------------------------------------------

#define PLUGIN_VERSION "1.0.3"
constexpr uint32_t MAX_LENGHT_BUFFER = 1024;
extern std::unique_ptr<PawnMap> g_PawnMap;

//----------------------------------------------------------------------------------------------------------------------------

namespace native
{
	cell AMX_NATIVE_CALL Map_Create(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Destroy(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Clear(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Clone(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Merge(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetVersion(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_ContainsKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_IsValid(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Get(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetKeyCount(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetFreeKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetKeyByIndex(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_ContainsStringKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetIdByStringKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_RemoveKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Internal_SafeRemoveKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Internal_LoopStep(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_RenameKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Swap(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_SortByKey(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_SortByField(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_FindKeyByField(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_StringKeyToInt(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_GetStringById(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_Set(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL Map_SetString(AMX* amx, cell* params);
}

//----------------------------------------------------------------------------------------------------------------------------