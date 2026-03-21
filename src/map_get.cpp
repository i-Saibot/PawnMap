#include "pawn_map.h"

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::containsKey(int32_t containerId, const int32_t keyId)
{
	if (!isValid(containerId))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	return container->dataMap.contains(static_cast<uint32_t>(keyId));
}
//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::isValid(const int32_t containerId) const
{
	if (containerId < 0 || containerId >= static_cast<int32_t>(mapContainers.size()))
	{
		return false;
	}
	return mapContainers[containerId] != nullptr;
}
//----------------------------------------------------------------------------------------------------------------------------

std::vector<uint8_t>* PawnMap::getData(const int32_t containerId, const int32_t keyId)
{
	auto* container = mapContainers[containerId].get();
	const auto it = container->dataMap.find(static_cast<uint32_t>(keyId));

	if (it != container->dataMap.end())
	{
		return &it->second;
	}
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------------

int PawnMap::getKeyCount(const int32_t containerId)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	return static_cast<int32_t>(mapContainers[containerId]->dataMap.size());
}
//----------------------------------------------------------------------------------------------------------------------------

int32_t PawnMap::getFreeKeyId(const int32_t containerId)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	auto* container = mapContainers[containerId].get();
	uint32_t keyId = 0;

	while (container->dataMap.contains(keyId))
	{
		keyId++;

		if (keyId == 0x7FFFFFFF)
		{
			break;
		}
	}
	return static_cast<int32_t>(keyId);
}

//----------------------------------------------------------------------------------------------------------------------------

int32_t PawnMap::getKeyByIndex(const int32_t containerId, const int32_t index)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	auto* container = mapContainers[containerId].get();

	if (index < 0 || index >= static_cast<int32_t>(container->keyCache.size()))
	{
		return INVALID_MAP_VALUE; 
	}
	return static_cast<int32_t>(container->keyCache[index]);
}

//----------------------------------------------------------------------------------------------------------------------------

size_t PawnMap::getDataSizeInBytes(const int containerId)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	auto* container = mapContainers[containerId].get();
	return container->dataSizeInBytes;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::containsStringKey(const int32_t containerId, const std::string_view key)
{
	if (!isValid(containerId))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	return container->keyStringMap.find(std::string(key)) != container->keyStringMap.end();
}

//----------------------------------------------------------------------------------------------------------------------------

int32_t PawnMap::getStringKey(const int32_t containerId, const std::string_view key)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	auto* container = mapContainers[containerId].get();
	
	auto it = container->keyStringMap.find(std::string(key));
    
	if (it != container->keyStringMap.end())
	{
		return it->second;
	}
	return INVALID_MAP_VALUE;
}

//----------------------------------------------------------------------------------------------------------------------------
