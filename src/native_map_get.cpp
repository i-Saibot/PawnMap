#include "natives.h"
#include "pawn_map_log.h"
#include "common.h"

#include <cstring>

namespace native
{
//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_ContainsKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t keyId = static_cast<int32_t>(params[2]);

		return static_cast<cell>(g_PawnMap->containsKey(containerId, keyId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_IsValid(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->isValid(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Get(AMX* amx, cell* params)
	{
		const int32_t containerId = static_cast<int32_t>(params[1]);
		const int32_t keyId = static_cast<int32_t>(params[2]);

		cell* destAddr = nullptr;
		if (amx_GetAddr(amx, params[3], &destAddr) != AMX_ERR_NONE || destAddr == nullptr)
		{
			return 0;
		}
		const size_t numCells = static_cast<size_t>(params[4]);
		const size_t sizeInBytes = numCells * sizeof(cell);

		if (!g_PawnMap->isValid(containerId) || keyId < 0)
		{
			memset(destAddr, 0, sizeInBytes);
			return 0;
		}
		const auto* rowData = g_PawnMap->getData(containerId, keyId);

		if (!rowData)
		{
			memset(destAddr, 0, sizeInBytes);
			return 0;
		}
		const size_t dataSizeInBytes = g_PawnMap->getDataSizeInBytes(containerId);

		if (dataSizeInBytes != sizeInBytes)
		{
			pawn_map_log::logPawnMap(
				"[Warning] Map_Get: %s (provided %zu, expected %zu). Container: %d",
				(sizeInBytes < dataSizeInBytes ? "less data" : "larger array"),
				sizeInBytes,
				dataSizeInBytes,
				containerId
			);
			memset(destAddr, 0, sizeInBytes);
			return 0;
		}
		std::memcpy(destAddr, rowData->data(), sizeInBytes);
		return 1;
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetKeyCount(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->getKeyCount(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetFreeKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		return static_cast<cell>(g_PawnMap->getFreeKeyId(containerId));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetKeyByIndex(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);
		int32_t index = static_cast<int32_t>(params[2]);

		return static_cast<cell>(g_PawnMap->getKeyByIndex(containerId, index));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_ContainsStringKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		char* key = nullptr;
		amx_StrParam(amx, params[2], key);

		if (key == nullptr)
		{
			return 0;
		}

		return static_cast<cell>(g_PawnMap->containsStringKey(containerId, std::string(key)));
	}

//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_GetIdByStringKey(AMX* amx, cell* params)
	{
		int32_t containerId = static_cast<int32_t>(params[1]);

		char* key = nullptr;
		amx_StrParam(amx, params[2], key);

		if (key == nullptr || std::strlen(key) == 0)
		{
			return static_cast<cell>(INVALID_MAP_VALUE);
		}

		return static_cast<cell>(g_PawnMap->getStringKey(containerId, std::string(key)));
	}

//----------------------------------------------------------------------------------------------------------------------------
}