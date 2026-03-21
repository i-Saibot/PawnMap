#include "pawn_map.h"
#include <algorithm>
#include <chrono>

#include "samp_sdk.hpp"

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::removeKey(const int32_t containerId, const int32_t keyId)
{
	if (!containsKey(containerId, keyId))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	const uint32_t uKey = static_cast<uint32_t>(keyId);
	auto& cache = container->keyCache;

	auto it = std::remove(cache.begin(), cache.end(), uKey);

	if (it != cache.end())
	{
		cache.erase(it, cache.end());
	}
	container->dataMap.erase(uKey);
	container->sortedByKey = false;
	container->sortedByField = false;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::safeRemoveKey(const int32_t containerId, const int32_t index)
{
	if (!isValid(containerId))
	{
		return false;
	}
	auto* conteinerId = mapContainers[containerId].get();

	conteinerId->pendingDeleteIdx = index;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

int PawnMap::loopStep(const int32_t containerId, int32_t* ptr)
{
	if (!isValid(containerId))
	{
		return 0;
	}
	auto* container = mapContainers[containerId].get();

	if (container->pendingDeleteIdx != -1)
	{
		int32_t targetIdx = container->pendingDeleteIdx;

		if (targetIdx >= 0 && targetIdx < static_cast<int32_t>(container->keyCache.size()))
		{
			uint32_t keyToDelete = container->keyCache[targetIdx];
			container->dataMap.erase(keyToDelete);

			container->keyCache.erase(container->keyCache.begin() + targetIdx);

			if (ptr)
			{
				*ptr -= 1;
			}
		}
		container->pendingDeleteIdx = -1;
	}
	return static_cast<int>(container->keyCache.size());
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::setDataSizeInBytes(const int32_t containerId, const size_t value)
{
	if (!isValid(containerId))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	
	container->dataSizeInBytes = value;
	return true;
}
//----------------------------------------------------------------------------------------------------------------------------

int PawnMap::stringKeyToInt(const int32_t containerId, const std::string_view key)
{
	if (!isValid(containerId))
	{
		return INVALID_MAP_VALUE;
	}
	auto& container = mapContainers[containerId];
	
	auto [it, inserted] = container->keyStringMap.try_emplace(std::string(key), container->keyStringNextId);

	if (inserted)
	{
		return container->keyStringNextId++;
	}
	return it->second;
}

//----------------------------------------------------------------------------------------------------------------------------

std::string_view PawnMap::findStringKeyByInt(const int32_t containerId, const int32_t keyId)
{
	if (!isValid(containerId))
	{
		return {};
	}
	auto* container = mapContainers[containerId].get();
	
	for (const auto& [str, id] : container->keyStringMap)
	{
		if (id == keyId)
		{
			return str;
		}
	}
	return {}; 
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::renameKey(const int32_t containerId, const int32_t oldKeyId, const int32_t newKeyId)
{
	if (newKeyId < 0 || oldKeyId == newKeyId
		|| !containsKey(containerId, oldKeyId)
		|| containsKey(containerId, newKeyId))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	auto& cache = container->keyCache;
	const uint32_t uOldKey = static_cast<uint32_t>(oldKeyId);
	const uint32_t uNewKey = static_cast<uint32_t>(newKeyId);

	auto it = std::find(cache.begin(), cache.end(), uOldKey);

	if (it != cache.end())
	{
		*it = uNewKey;
	}
	auto itMap = container->dataMap.find(uOldKey);

	if (itMap != container->dataMap.end())
	{
		container->dataMap.emplace(uNewKey, std::move(itMap->second));
		container->dataMap.erase(itMap);
	}
	container->sortedByKey = false;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::swap(const int32_t containerId, const int32_t keyId1, const int32_t keyId2)
{
	if (!containsKey(containerId, keyId1)
		|| !containsKey(containerId, keyId2))
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();

	auto it1 = container->dataMap.find(keyId1);
	auto it2 = container->dataMap.find(keyId2);

	if (it1 != container->dataMap.end() && it2 != container->dataMap.end())
	{
		std::swap(it1->second, it2->second);

		container->sortedByKey = false;
		container->sortedByField = false;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::sortByKey(const int32_t containerId, const PawnMapSortOrder order)
{
	if (!isValid(containerId)
		|| order <= PawnMapSortOrder::SORT_NONE
		|| order > PawnMapSortOrder::SORT_DESCENDING)
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();

	if (container->sortedByKey && container->currentSortKeyOrder == order)
	{
		return false;
	}
	if (order == PawnMapSortOrder::SORT_DESCENDING)
	{
		std::sort(container->keyCache.begin(), container->keyCache.end(), std::greater<uint32_t>());
	}
	else
	{
		std::sort(container->keyCache.begin(), container->keyCache.end());
	}
	container->currentSortKeyOrder = order;
	container->sortedByKey = true;
	container->sortedByField = false;
	container->currentSortFieldOffset = -1;
	container->currentSortValueOrder = PawnMapSortOrder::SORT_NONE;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::sortByField(const int32_t containerId, const int32_t offset, const PawnMapType fieldType, const PawnMapSortOrder order)
{
	if (!isValid(containerId)
		|| order <= PawnMapSortOrder::SORT_NONE
		|| order > PawnMapSortOrder::SORT_DESCENDING)
	{
		return false;
	}
	auto* container = mapContainers[containerId].get();
	
	if (container->keyCache.empty())
	{
		return false;
	}
	if (container->sortedByField 
		&& container->currentSortFieldOffset == offset
		&& container->currentSortValueOrder == order)
	{
		return true;
	}
	const bool descending = (order == PawnMapSortOrder::SORT_DESCENDING);
	const size_t byteOffset = static_cast<size_t>(offset) * sizeof(cell);
	
	std::sort(container->keyCache.begin(), container->keyCache.end(), [&](uint32_t a, uint32_t b)
	{
		const auto& bufA = container->dataMap.at(a);
		const auto& bufB = container->dataMap.at(b);
		
		if (byteOffset + 4 > bufA.size() || byteOffset + 4 > bufB.size())
		{
			return a < b;
		}
		const uint8_t* ptrA = &bufA[byteOffset];
		const uint8_t* ptrB = &bufB[byteOffset];

		switch (fieldType)
		{
			case PawnMapType::TYPE_INT:
			{
				const auto valA = *reinterpret_cast<const int32_t*>(ptrA);
				const auto valB = *reinterpret_cast<const int32_t*>(ptrB);
				
				if (valA != valB)
				{
					return descending ? (valA > valB) : (valA < valB);
				}
				break;
			}
			case PawnMapType::TYPE_FLOAT:
			{
				const auto valA = *reinterpret_cast<const float*>(ptrA);
				const auto valB = *reinterpret_cast<const float*>(ptrB);
				
				const bool nanA = std::isnan(valA);
				const bool nanB = std::isnan(valB);

				if (nanA || nanB)
				{
					if (nanA != nanB)
					{
						return descending ? nanB : nanA;
					}
					break;
				}
				if (std::fabs(valA - valB) > 0.00001f)
				{
					return descending ? (valA > valB) : (valA < valB);
				}
				break;
			}
			case PawnMapType::TYPE_STRING:
			{
				const size_t maxA = (bufA.size() - byteOffset) / sizeof(int32_t);
				const size_t maxB = (bufB.size() - byteOffset) / sizeof(int32_t);
				auto* strA = reinterpret_cast<const int32_t*>(ptrA);
				auto* strB = reinterpret_cast<const int32_t*>(ptrB);

				size_t i = 0;
				while (i < maxA && i < maxB && strA[i] == strB[i] && strA[i] != 0)
				{
					i++;
				}
				const int32_t cA = (i < maxA) ? strA[i] : 0;
				const int32_t cB = (i < maxB) ? strB[i] : 0;
				const int32_t diff = cA - cB;

				if (diff != 0)
				{
					return descending ? (diff > 0) : (diff < 0);
				}
				break;
			}
			default: break;
		}
		return a < b;
	});
	container->sortedByField = true;
	container->currentSortFieldOffset = offset;
	container->currentSortValueOrder = order;
	container->sortedByKey = false;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

int PawnMap::findKeyByField(const int32_t containerId, const int32_t byteOffset, const void* searchValue, PawnMapType fieldType)
{
	if (!isValid(containerId) || !searchValue)
	{
		return INVALID_MAP_ID;
	}
	auto* container = mapContainers[containerId].get();

	for (const auto& [keyId, buffer] : container->dataMap)
	{
		const size_t available = buffer.size();
		if (static_cast<size_t>(byteOffset) + sizeof(int32_t) > available)
			continue;

		const uint8_t* targetPtr = &buffer[byteOffset];

		switch (fieldType)
		{
			case PawnMapType::TYPE_INT:
			{
				const int32_t target = *reinterpret_cast<const int32_t*>(targetPtr);
				const int32_t search = *static_cast<const int32_t*>(searchValue);
				
				if (target == search)
				{
					return static_cast<int>(keyId);
				}
				break;
			}
			case PawnMapType::TYPE_FLOAT:
			{
				const float target = *reinterpret_cast<const float*>(targetPtr);
				const float search = *static_cast<const float*>(searchValue);
				
				if (target == search)
				{
					return static_cast<int>(keyId);
				}
				break;
			}
			case PawnMapType::TYPE_STRING:
			{
				const int32_t* targetStr = reinterpret_cast<const int32_t*>(targetPtr);
				const int32_t* searchStr = static_cast<const int32_t*>(searchValue);
				const size_t maxCells = (available - byteOffset) / sizeof(int32_t);

				size_t i = 0;
				while (i < maxCells && targetStr[i] == searchStr[i])
				{
					if (targetStr[i] == 0)
					{
						return static_cast<int>(keyId);
					}
					i++;
				}
				break;
			}
			default: break;
		}
	}
	return INVALID_MAP_VALUE;
}

//----------------------------------------------------------------------------------------------------------------------------