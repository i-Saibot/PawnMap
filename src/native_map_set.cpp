#define SAMP_SDK_WANT_AMX_EVENTS

#include "samp-sdk/samp_sdk.hpp"
#include "natives.h"
#include "pawn_map_log.h"

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Set, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);

	const int32_t containerId = p.Get<int32_t>(0);
	const int32_t keyId = p.Get<int32_t>(1);
	
	if (!g_PawnMap->isValid(containerId) || keyId < 0)
	{
		return 0;
	}
	const cell* srcAddr = Samp_SDK::amx::Get_Addr_Safe(amx, 2);
	
	if (!srcAddr)
	{
		return 0;
	}
	const size_t numCells = static_cast<size_t>(p.Get<int32_t>(3));
	const size_t sizeInBytes = static_cast<size_t>(numCells) * sizeof(cell);
	const size_t dataSizeInBytes = g_PawnMap->getDataSizeInBytes(containerId);
	
	if (dataSizeInBytes == 0)
	{
		g_PawnMap->setDataSizeInBytes(containerId, sizeInBytes);
	}
	else if (dataSizeInBytes != sizeInBytes)
	{
		pawn_map_log::logPawnMap(
			"[Warning] Map_Set: %s (provided %zu, expected %zu). Container: %d",
			(sizeInBytes < dataSizeInBytes ? "less data" : "larger array"),
			sizeInBytes,
			dataSizeInBytes,
			containerId
		);
		return 0;
	}
	return static_cast<cell>(g_PawnMap->setData(
		containerId,
		keyId,
		reinterpret_cast<const uint8_t*>(srcAddr),
		sizeInBytes
	));
}

//------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_SetString, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);

	cell* dest = Samp_SDK::amx::Get_Addr_Safe(amx, 0);
	cell* src  = Samp_SDK::amx::Get_Addr_Safe(amx, 1);
	
	if (!dest || !src)
	{
		return 0;
	}
	int len = 0;
	
	while (src[len] != 0)
	{
		len++;
	}
	len++;
	
	memcpy(dest, src, len * sizeof(cell));
	return 1;
}

//------------------------------------------------------------------------------------------------------------