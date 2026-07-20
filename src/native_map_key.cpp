#include "natives.h"
#include "common.h"

#include <cstring>

namespace native
{
//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_RemoveKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t keyId = static_cast<int32_t>(params[2]);

		return static_cast<cell>(g_PawnMap->removeKey(containerId, keyId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Internal_SafeRemoveKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t index = static_cast<int32_t>(params[2]);

		return static_cast<cell>(g_PawnMap->safeRemoveKey(containerId, index));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Internal_LoopStep(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		cell* loopIdxPtr = nullptr;
		if (amx_GetAddr(amx, params[2], &loopIdxPtr) != AMX_ERR_NONE || loopIdxPtr == nullptr)
		{
			return 0;
		}

		return static_cast<cell>(g_PawnMap->loopStep(containerId, loopIdxPtr));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_RenameKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t oldKeyId = static_cast<int32_t>(params[2]);
		int32_t newKeyId = static_cast<int32_t>(params[3]);

		return static_cast<cell>(g_PawnMap->renameKey(containerId, oldKeyId, newKeyId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Swap(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t keyId1 = static_cast<int32_t>(params[2]);
		int32_t keyId2 = static_cast<int32_t>(params[3]);

		return static_cast<cell>(g_PawnMap->swap(containerId, keyId1, keyId2));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_SortByKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		PawnMapSortOrder order = static_cast<PawnMapSortOrder>(static_cast<uint8_t>(params[2]));

		order = static_cast<PawnMapSortOrder>(static_cast<uint8_t>(order) + 1);
		return static_cast<cell>(g_PawnMap->sortByKey(containerId, order));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_SortByField(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t offset = static_cast<int32_t>(params[2]);
		PawnMapSortOrder order = static_cast<PawnMapSortOrder>(static_cast<int32_t>(params[3]));
		PawnMapType fieldType = static_cast<PawnMapType>(static_cast<int32_t>(params[4]));

		return static_cast<cell>(g_PawnMap->sortByField(containerId, offset, fieldType, order));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_FindKeyByField(AMX* amx, cell* params)
	{
		const int32_t containerId = static_cast<int32_t>(params[1]);
		const int32_t byteOffset = static_cast<int32_t>(params[2]) * sizeof(cell);
		const PawnMapType fieldType = static_cast<PawnMapType>(static_cast<int32_t>(params[3]));

		if (!g_PawnMap->isValid(containerId))
			return static_cast<cell>(INVALID_MAP_ID);

		cell* srcAddr = nullptr;
		if (amx_GetAddr(amx, params[4], &srcAddr) != AMX_ERR_NONE || srcAddr == nullptr)
			return static_cast<cell>(INVALID_MAP_ID);

		return static_cast<cell>(g_PawnMap->findKeyByField(
			containerId,
			byteOffset,
			reinterpret_cast<const void*>(srcAddr),
			fieldType
		));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_StringKeyToInt(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		char* key = nullptr;
		amx_StrParam(amx, params[2], key);

		if (key == nullptr || std::strlen(key) == 0)
		{
			return static_cast<cell>(INVALID_MAP_VALUE);
		}

		return static_cast<cell>(g_PawnMap->stringKeyToInt(containerId, std::string(key)));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetStringById(AMX* amx, cell* params)
	{
		const int32_t containerId = static_cast<int32_t>(params[1]);
		const int32_t keyId = static_cast<int32_t>(params[2]);

		if (keyId < MIN_ID_FOR_KEY_STRING)
		{
			return 0;
		}
		std::string_view key = g_PawnMap->findStringKeyByInt(containerId, keyId);

		if (!key.empty())
		{
			cell* addr = nullptr;
			if (amx_GetAddr(amx, params[3], &addr) == AMX_ERR_NONE && addr != nullptr)
			{
				const int32_t maxLen = static_cast<int32_t>(params[4]);
				amx_SetString(addr, std::string(key).c_str(), 0, 0, maxLen);
				return 1;
			}
		}
		return 0;
	}

//----------------------------------------------------------------------------------------------------------------------------
}