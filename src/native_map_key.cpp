#define SAMP_SDK_WANT_AMX_EVENTS

#include "../samp-sdk/samp_sdk.hpp"
#include "natives.h"

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_RemoveKey, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t keyId;

	Register_Parameters(containerId, keyId);
	return static_cast<cell>(g_PawnMap->removeKey(containerId, keyId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Internal_SafeRemoveKey, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t index;

	Register_Parameters(containerId, index);
	return static_cast<cell>(g_PawnMap->safeRemoveKey(containerId, index));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Internal_LoopStep, AMX* amx, cell* params)
{
	int32_t containerId;
	
	Register_Parameters(containerId);
	
	cell* loopIdxPtr = Samp_SDK::amx::Get_Addr_Safe(amx, 1);

	if (!loopIdxPtr)
	{
		return 0;
	}
	return static_cast<cell>(g_PawnMap->loopStep(containerId, loopIdxPtr));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_RenameKey, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t oldKeyId;
	int32_t newKeyId;

	Register_Parameters(containerId, oldKeyId, newKeyId);
	return static_cast<cell>(g_PawnMap->renameKey(containerId, oldKeyId, newKeyId));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_Swap, AMX* amx, cell* params)
{
	int32_t containerId;
	int32_t keyId1;
	int32_t keyId2;

	Register_Parameters(containerId, keyId1, keyId2);
	return static_cast<cell>(g_PawnMap->swap(containerId, keyId1, keyId2));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_SortByKey, AMX* amx, cell* params)
{
	int32_t containerId;
	PawnMapSortOrder order;

	Register_Parameters(containerId, order);

	order = static_cast<PawnMapSortOrder>(static_cast<uint8_t>(order) + 1);
	return static_cast<cell>(g_PawnMap->sortByKey(containerId, order));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_SortByField, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);
	
	int32_t containerId = p.Get<int32_t>(0);
	int32_t offset = p.Get<int32_t>(1);
	PawnMapSortOrder order = static_cast<PawnMapSortOrder>(p.Get<int32_t>(2));
	PawnMapType fieldType = static_cast<PawnMapType>(p.Get<int32_t>(3));
	
	return static_cast<cell>(g_PawnMap->sortByField(containerId, offset, fieldType, order));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_FindKeyByField, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);
    
	const int32_t containerId = p.Get<int32_t>(0);
	const int32_t byteOffset  = p.Get<int32_t>(1) * sizeof(cell);
	const PawnMapType fieldType = static_cast<PawnMapType>(p.Get<int32_t>(2));

	if (!g_PawnMap->isValid(containerId))
		return static_cast<cell>(INVALID_MAP_ID);

	const cell* srcAddr = Samp_SDK::amx::Get_Addr_Safe(amx, 3);
	if (!srcAddr)
		return static_cast<cell>(INVALID_MAP_ID);

	return static_cast<cell>(g_PawnMap->findKeyByField(
		containerId,
		byteOffset,
		reinterpret_cast<const void*>(srcAddr),
		fieldType
	));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_StringKeyToInt, AMX* amx, cell* params)
{
	int32_t containerId;
	std::string key;

	Register_Parameters(containerId, key);
	
	if (!key.empty())
	{
		return static_cast<cell>(INVALID_MAP_VALUE);
	}
	return static_cast<cell>(g_PawnMap->stringKeyToInt(containerId, key));
}

//----------------------------------------------------------------------------------------------------------------------------

Plugin_Native(Map_GetStringById, AMX* amx, cell* params)
{
	Samp_SDK::Native_Params p(amx, params);
	const int32_t containerId = p.Get<int32_t>(0);
	const int32_t keyId = p.Get<int32_t>(1);
	
	if (keyId < MIN_ID_FOR_KEY_STRING)
	{
		return 0;
	}
	std::string_view key = g_PawnMap->findStringKeyByInt(containerId, keyId);
	
	if (!key.empty())
	{
		if (cell* addr = Samp_SDK::amx::Get_Addr_Safe(amx, 2))
		{
			const int32_t maxLen = p.Get<int32_t>(3);
			Samp_SDK::amx::Set_String(addr, std::string(key).c_str(), maxLen);
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------