/*
* Pawn Unordered Map Plugin v1.0.0
* Author: Saibot
* Created: 2026
* * The MIT License (MIT)
* Copyright (c) 2026 Saibot
*/

#if defined _pawn_map_included
	#endinput
#endif
#define _pawn_map_included

// -----------------------------------------------------------------------------
// Versioning

#define MAP_PLUGIN_VERSION          "1.0.1"

// -----------------------------------------------------------------------------
// Macros for Any tags

#if !defined PP_ALL_TAGS
	#define PP_ALL_TAGS _,bool,Float

	#if defined PP_ADDITIONAL_TAGS
		#define AnyTag {PP_ALL_TAGS,PP_ADDITIONAL_TAGS}
	#else
		#define AnyTag {PP_ALL_TAGS}
	#endif
#else
	#define AnyTag {PP_ALL_TAGS}
#endif

// -----------------------------------------------------------------------------
// Constants and Types

const PawnMap:INVALID_MAP_ID	= PawnMap:-1;
const INVALID_MAP_KEY_ID        = -1;

enum PawnMapSortOrder
{
	MAP_SORT_ASC = 1,
	MAP_SORT_DESC
};

enum PawnMapType
{
    MAP_TYPE_INT = 1,
    MAP_TYPE_FLOAT,
    MAP_TYPE_STRING
};

// -----------------------------------------------------------------------------
// Helper Iterators

#define mapfor(%0,%1) \
	for(new %1_idx = 0, %1; \
		%1_idx < Map_Internal_LoopStep(%0, %1_idx) && ((%1 = Map_GetKeyByIndex(%0, %1_idx)) != -1); \
		%1_idx++)

// -----------------------------------------------------------------------------
// Core API

native PawnMap:Map_Create();
native bool:Map_Destroy(PawnMap:mapid);
native bool:Map_Clear(PawnMap:mapid);
native bool:Map_IsValid(PawnMap:mapid);
native bool:Map_Merge(PawnMap:dest, PawnMap:source);
native PawnMap:Map_Clone(PawnMap:mapid);

// -----------------------------------------------------------------------------
// Data Manipulation API

native bool:Map_Set(PawnMap:mapid, key, const AnyTag:data_array[] = "", const size = sizeof(data_array));
native bool:Map_Get(PawnMap:mapid, key, AnyTag:data_array[], const size = sizeof(data_array));

native bool:Map_RemoveKey(PawnMap:mapid, key);

/*
	Map_SafeRemoveKey(mapid);
*/
#define Map_SafeRemoveKey(%0)		if(Map_Internal_SafeRemoveKey(%0, _mpidx)) continue

native bool:Map_RenameKey(PawnMap:mapid, old_key, new_key);
native bool:Map_Swap(PawnMap:mapid, key1, key2);

// -----------------------------------------------------------------------------
// Query and Info API

native bool:Map_ContainsKey(PawnMap:mapid, key);
native Map_GetKeyByIndex(PawnMap:mapid, index);
native Map_GetKeyCount(PawnMap:mapid);
native Map_GetFreeKey(PawnMap:mapid);
native Map_FindKeyByField(PawnMap:mapid, AnyTag:offset, PawnMapType:type, {Float, _}:...);

// -----------------------------------------------------------------------------
// Sorting

native bool:Map_SortByKey(PawnMap:mapid, PawnMapSortOrder:order);
native bool:Map_SortByField(PawnMap:mapid, AnyTag:offset, PawnMapSortOrder:order, PawnMapType:typeid = MAP_TYPE_INT);

// -----------------------------------------------------------------------------
// String Utilities

native bool:Map_SetString(dest[], const source[], const max_lenght_source = sizeof(source));

// -----------------------------------------------------------------------------
// Key String To Int

native Map_StringKeyToInt(PawnMap:mapid, const key[]);
native Map_GetIdByStringKey(PawnMap:mapid, const key[]);
native bool:Map_ContainsStringKey(PawnMap:mapid, const key[]);
native bool:Map_GetStringById(PawnMap:mapid, keyid, const key_str[], const size = sizeof(key_str));

// -----------------------------------------------------------------------------
// Internal Natives (Do not call directly)

native Map_Internal_LoopStep(PawnMap:mapid, &index);
native bool:Map_Internal_SafeRemoveKey(PawnMap:mapid, index);

// -----------------------------------------------------------------------------
// Version
native Map_GetVersion(value[], const len = sizeof(value));

// -----------------------------------------------------------------------------
// Initialization Hook

public OnGameModeInit()
{
	new version[16];
	Map_GetVersion(version, sizeof(version));

	if (strcmp(version, MAP_PLUGIN_VERSION) != 0)
	{
		printf(" ");
		printf("[Map Plugin] Error: Version mismatch!");
		printf("[Map Plugin] Plugin binary v.%s | Include v.%s", version, MAP_PLUGIN_VERSION);
		printf("[Map Plugin] Please update your .inc file or the plugin binary.");
		printf(" ");
	}
	#if defined pawnmap_OnGameModeInit
		return pawnmap_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit pawnmap_OnGameModeInit
#if defined pawnmap_OnGameModeInit
	forward pawnmap_OnGameModeInit();
#endif