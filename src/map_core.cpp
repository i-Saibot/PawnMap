#include "pawn_map.h"

//----------------------------------------------------------------------------------------------------------------------------

int32_t PawnMap::сreate()
{
	int32_t containerId = INVALID_MAP_ID;

	if (!freeContainerIds.empty())
	{
		containerId = static_cast<int32_t>(freeContainerIds.back());
		freeContainerIds.pop_back();
		mapContainers[containerId] = std::make_unique<PawnMapContainer>();
	}
	else
	{
		mapContainers.push_back(std::make_unique<PawnMapContainer>());
		containerId = static_cast<int32_t>(mapContainers.size() - 1);
	}
	return containerId;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::destroy(const int32_t containerId)
{
	if (!isValid(containerId))
	{
		return false;
	}
	mapContainers[containerId].reset();
	freeContainerIds.push_back(containerId);
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::clear(const int32_t containerId)
{
	if (!isValid(containerId))
	{
		return false;
	}
	mapContainers[containerId]->dataClear();
	return false;
}

//----------------------------------------------------------------------------------------------------------------------------

int PawnMap::clone(const int32_t containerId)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_ID;
	}
	const auto* oldContainer = mapContainers[containerId].get();

	int32_t newId = INVALID_MAP_ID;

	if (!freeContainerIds.empty())
	{
		newId = static_cast<int32_t>(freeContainerIds.back());
		freeContainerIds.pop_back();
		mapContainers[newId] = std::make_unique<PawnMapContainer>();
	}
	else
	{
		mapContainers.push_back(std::make_unique<PawnMapContainer>());
		newId = static_cast<int>(mapContainers.size() - 1);
	}
	auto* newContainer = mapContainers[newId].get();
	
	newContainer->dataSizeInBytes = oldContainer->dataSizeInBytes;

	newContainer->dataMap.reserve(oldContainer->dataMap.size());
	newContainer->keyCache.reserve(oldContainer->keyCache.size());

	newContainer->dataMap = oldContainer->dataMap;
	newContainer->keyCache = oldContainer->keyCache;

	newContainer->sortedByKey = oldContainer->sortedByKey;
	newContainer->currentSortKeyOrder = oldContainer->currentSortKeyOrder;
	newContainer->sortedByField = oldContainer->sortedByField;
	newContainer->currentSortFieldOffset = oldContainer->currentSortFieldOffset;
	newContainer->currentSortValueOrder = oldContainer->currentSortValueOrder;
	return newId;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::merge(const int32_t destId, const int32_t sourceId)
{
	if (!isValid(destId) || !isValid(sourceId) || destId == sourceId)
	{
		return false;
	}
	auto* destConteiner = mapContainers[destId].get();
	auto* sourceConteiner = mapContainers[sourceId].get();

	if (destConteiner->dataSizeInBytes != sourceConteiner->dataSizeInBytes)
	{
		return false;
	}
	destConteiner->dataMap.reserve(destConteiner->dataMap.size() + sourceConteiner->dataMap.size());

	for (const auto& [key, buffer] : sourceConteiner->dataMap)
	{
		destConteiner->dataMap.insert_or_assign(key, buffer);
	}
	destConteiner->keyCache.clear();
	destConteiner->keyCache.reserve(destConteiner->dataMap.size());

	for (auto const& [key, _] : destConteiner->dataMap)
	{
		destConteiner->keyCache.push_back(key);
	}
	std::sort(destConteiner->keyCache.begin(), destConteiner->keyCache.end());

	destConteiner->sortedByKey = true;
	destConteiner->currentSortKeyOrder = PawnMapSortOrder::SORT_ASCENDING;
	destConteiner->sortedByField = false;
	destConteiner->currentSortFieldOffset = -1;
	destConteiner->currentSortValueOrder = PawnMapSortOrder::SORT_NONE;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------