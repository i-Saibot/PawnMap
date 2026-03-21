#include "pawn_map.h"

//----------------------------------------------------------------------------------------------------------------------------

bool PawnMap::setData(const int32_t containerId, const int32_t keyId, const uint8_t* sourceData, const size_t sizeInBytes)
{
	auto& container = mapContainers[containerId];
	const uint32_t uKey = static_cast<uint32_t>(keyId);
	
	auto& vec = container->dataMap[uKey];
	const bool isNew = vec.empty(); 
	
	vec.assign(sourceData, sourceData + sizeInBytes);

	if (isNew)
	{
		container->keyCache.push_back(uKey);
		container->sortedByKey = false;
	}
	container->sortedByField = false;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------