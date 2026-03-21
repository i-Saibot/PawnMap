#pragma once

#include <vector>
#include <memory>
#include "../ankerl/unordered_dense.h"

//----------------------------------------------------------------------------------------------------------------------------

constexpr int32_t INVALID_MAP_ID			= -1;
constexpr int32_t INVALID_MAP_VALUE 		= -1;
constexpr int32_t MIN_ID_FOR_KEY_STRING		= 200'000;

//----------------------------------------------------------------------------------------------------------------------------

enum class PawnMapSortOrder
{
	SORT_NONE,
	SORT_ASCENDING,
	SORT_DESCENDING
};

//----------------------------------------------------------------------------------------------------------------------------

enum class PawnMapType
{
	TYPE_NONE,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING
};

//----------------------------------------------------------------------------------------------------------------------------

struct PawnMapContainer
{
	ankerl::unordered_dense::map<uint32_t, std::vector<uint8_t>> dataMap;
	ankerl::unordered_dense::map<std::string, int> keyStringMap;
	std::vector<uint32_t> keyCache;
	
	bool sortedByKey = false;
	bool sortedByField = false;
	
	size_t dataSizeInBytes = 0;
	int32_t pendingDeleteIdx = INVALID_MAP_VALUE;
	int32_t currentSortFieldOffset = INVALID_MAP_VALUE;
	int32_t keyStringNextId = MIN_ID_FOR_KEY_STRING;
	
	PawnMapSortOrder currentSortKeyOrder = PawnMapSortOrder::SORT_NONE;
	PawnMapSortOrder currentSortValueOrder = PawnMapSortOrder::SORT_NONE;

	void dataClear()
	{
		dataMap.clear();
		keyStringMap.clear();
		keyCache.clear();

		sortedByKey = false;
		sortedByField = false;
		
		dataSizeInBytes = 0;
		pendingDeleteIdx = INVALID_MAP_VALUE;
		currentSortFieldOffset = INVALID_MAP_VALUE;
		keyStringNextId = MIN_ID_FOR_KEY_STRING;
		
		currentSortKeyOrder = PawnMapSortOrder::SORT_NONE;
		currentSortValueOrder = PawnMapSortOrder::SORT_NONE;
	}
};

//----------------------------------------------------------------------------------------------------------------------------

class PawnMap
{
public:
	// Core operations
	int сreate();
	bool destroy(const int32_t containerId);
	bool clear(const int32_t containerId);
	int clone(const int32_t containerId);
	bool merge(const int32_t destId, const int32_t sourceId);

	// Key operations
	bool removeKey(const int32_t containerId, const int32_t keyId);
	bool safeRemoveKey(const int32_t containerId, const int32_t index);
	bool renameKey(const int32_t containerId, const int32_t oldKeyId, const int32_t newKeyId);
	bool swap(const int32_t containerId, const int32_t keyId1, const int32_t keyId2);
	bool sortByKey(const int32_t containerId, const PawnMapSortOrder order);
	bool sortByField(const int32_t containerId, const int32_t offset, const PawnMapType fieldType, const PawnMapSortOrder order);
	int findKeyByField(const int32_t containerId, const int32_t byteOffset, const void* searchValue, PawnMapType fieldType);
	int loopStep(const int32_t containerId, int32_t* ptr);
	bool setDataSizeInBytes(const int32_t containerId, const size_t value);
	int stringKeyToInt(const int32_t containerId, const std::string_view key);
	std::string_view findStringKeyByInt(const int32_t containerId, const int32_t keyId);
		
	// Set operations
	bool setData(const int32_t containerId, const int32_t keyId, const uint8_t* sourceData, const size_t size);

	// Get operations
	bool containsKey(const int32_t containerId, const int32_t keyId);
	bool isValid(const int32_t containerId) const;
	std::vector<uint8_t>* getData(const int32_t containerId, const int32_t keyId);
	int getKeyCount(const int32_t containerId);
	int getFreeKeyId(const int32_t containerId);
	int getKeyByIndex(const int32_t containerId, const int32_t index);
	size_t getDataSizeInBytes(const int32_t containerId);
	bool containsStringKey(const int32_t containerId, const std::string_view key);
	int getStringKey(const int32_t containerId, const std::string_view key);

private:
	// Data storage
	std::vector<std::unique_ptr<PawnMapContainer>> mapContainers;
	std::vector<uint32_t> freeContainerIds;
};

//----------------------------------------------------------------------------------------------------------------------------