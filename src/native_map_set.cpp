#include "natives.h"
#include "common.h"
#include "pawn_map_log.h"

namespace native
{
//----------------------------------------------------------------------------------------------------------------------------

	cell AMX_NATIVE_CALL Map_Set(AMX* amx, cell* params)
	{
		const int32_t containerId = static_cast<int32_t>(params[1]);
		const int32_t keyId = static_cast<int32_t>(params[2]);

		if (!g_PawnMap->isValid(containerId) || keyId < 0)
		{
			return 0;
		}

		cell* srcAddr = nullptr;
		if (amx_GetAddr(amx, params[3], &srcAddr) != AMX_ERR_NONE || srcAddr == nullptr)
		{
			return 0;
		}

		const size_t numCells = static_cast<size_t>(params[4]);
		const size_t sizeInBytes = numCells * sizeof(cell);
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

	cell AMX_NATIVE_CALL Map_SetString(AMX* amx, cell* params)
	{
		cell* dest = nullptr;
		cell* src = nullptr;

		if (amx_GetAddr(amx, params[1], &dest) != AMX_ERR_NONE || dest == nullptr)
		{
			return 0;
		}
		if (amx_GetAddr(amx, params[2], &src) != AMX_ERR_NONE || src == nullptr)
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
}