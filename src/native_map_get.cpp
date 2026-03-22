#define SAMP_SDK_WANT_AMX_EVENTS

#include "../samp-sdk/samp_sdk.hpp"
#include "natives.h"
#include "pawn_map_log.h"

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_ContainsKey, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t keyId;

	Register_Parameters(containerId, keyId);
	return static_cast<cell>(g_PawnMap->containsKey(containerId, keyId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_IsValid, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->isValid(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Get, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);

	const int32_t containerId = p.Get<int32_t>(0);
	const int32_t keyId = p.Get<int32_t>(1);
	
	cell* destAddr = Samp_SDK::amx::Get_Addr_Safe(amx, 2);

	if (!destAddr)
	{
		return 0;
	}
	const size_t numCells = static_cast<size_t>(p.Get<int32_t>(3));
	const size_t sizeInBytes = static_cast<size_t>(numCells) * sizeof(cell);
	
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

Plugin_Native(Map_GetKeyCount, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->getKeyCount(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_GetFreeKey, AMX* amx, cell* params)
{
	int32_t containerId;

	Register_Parameters(containerId);
	return static_cast<cell>(g_PawnMap->getFreeKeyId(containerId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_GetKeyByIndex, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t index;

	Register_Parameters(containerId, index);
	return static_cast<cell>(g_PawnMap->getKeyByIndex(containerId, index));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_ContainsStringKey, AMX* amx, cell* params)
{
	int32_t containerId;
	std::string key;

	Register_Parameters(containerId, key);
	return static_cast<cell>(g_PawnMap->containsStringKey(containerId, key));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_GetIdByStringKey, AMX* amx, cell* params)
{
	int32_t containerId;
	std::string key;

	Register_Parameters(containerId, key);
	
	if (!key.empty())
	{
		return static_cast<cell>(INVALID_MAP_VALUE);
	}
	return static_cast<cell>(g_PawnMap->getStringKey(containerId, key));
}

//----------------------------------------------------------------------------------------------------------------------------