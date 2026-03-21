# *PawnMap*

> Fast hash map plugin for SA-MP/open.mp. Store arrays by unique IDs with near-instant access. A lightweight alternative to massive Pawn arrays.

---

## 📖 Documentation
Detailed information about functions and usage can be found in the **[PawnMap Wiki](https://github.com/i-Saibot/PawnMap/wiki)**.

<br></br>
<details>
<summary>Click to expand Benchmark</summary>
  
<br></br>
  
| OPERATION | PawMap | Raw Array | DIFF (NT/PM) |
| :--- | :--- | :--- | :--- |
| CREATE & DELL|     2 ms    |     0 ms    |  x0.0 |
| SET|     2 ms    |     1 ms    |  x0.5 |
| GET|     1 ms    |     2 ms    |  x2.0 |
| ADD|     1 ms    |     0 ms    |  x0.0 |
| FIND|     8 ms    |   417 ms    |  x52.1 |
| CONTAINS|     0 ms    |  1052 ms    |  x1052.0 |
| REMOVE|    44 ms    |  1901 ms    |  x43.2 |
| SWAP|     0 ms    |     4 ms    |  x4.0 |
| SORT|     4 ms    |    18 ms    |  x4.5 |
| SET STR|     1 ms    |     1 ms    |  x1.0 |
| CONS STR|     0 ms    |  1394 ms    |  x1394.0 |
|  `TOTAL TIME` |     `63 ms`    |  `4790 ms`   |  `x76.0` |

<br></br>

```pawn
new res_set_pm, res_set_nt;
new res_get_pm, res_get_nt;
new res_find_item_pm, res_find_item_nt;
new res_remove_item_pm, res_remove_item_nt;
new res_cons_key_item_pm, res_cons_key_item_nt;
new res_swap_pm, res_swap_nt;
new res_sort_pm, res_sort_nt;

new res_create_pm;
new res_create_nt;
new res_add_pm, res_add_nt;

new res_set_string_pm, res_cons_string_pm;
new res_set_string_nt, res_cons_string_nt;


const MAX_TEST_ITEMS = 7_000; 


//------Pawn Map

enum playerData
{
    Item,
    Amount,
    Name[24]
}
new PawnMap:PawnMap;

enum stringData
{
    Item
}

//-------Array

enum e_raw_array
{
    Raw_Item,
    Raw_Amount,
    Raw_Name[24]
}
new gRawArray[MAX_TEST_ITEMS][e_raw_array];
new gRawArrayStr[MAX_TEST_ITEMS][16];

//----------

new str1[500];
new str2[500];


main()
{
    printf(" ");
    printf("--- [ START BENCHMARK: PAWN MAP - RAW ARRAY ] ---");
    printf("Items: %d", MAX_TEST_ITEMS);
    printf("--------------------------------------------------");

    CheckPawnMap();
    CheckRawArray();

    PrintResults();
}

stock PrintResults()
{
    new 
        total_pm_int = res_set_pm + res_get_pm + res_find_item_pm + 
                    res_cons_key_item_pm + res_remove_item_pm + 
                    res_swap_pm + res_sort_pm + res_create_pm + res_add_pm +
                    res_set_string_pm + res_cons_string_pm,
                    
        total_nt_int = res_set_nt + res_get_nt + res_find_item_nt + 
                    res_cons_key_item_nt + res_remove_item_nt + 
                    res_swap_nt + res_sort_nt + res_create_nt + res_add_nt + 
                    res_set_string_nt + res_cons_string_nt;

    printf(" ");
    printf("======================================================================");
    printf("  OPERATION     |    PawMap   |  Raw Array  |  DIFF (NT/PM) ");
    printf("======================================================================");

    #define PR_L(%0,%1,%2) printf(" %-15s|  %4d ms    |  %4d ms    |  x%.1f ", %0, %1, %2, (%1 > 0) ? (float(%2)/float(%1)) : float(%2))

    PR_L("CREATE & DELL",   res_create_pm,               res_create_nt);
    PR_L("SET",             res_set_pm,                  res_set_nt);
    PR_L("GET",             res_get_pm,                  res_get_nt);
    PR_L("ADD",             res_add_pm,                  res_add_nt);
    PR_L("FIND",            res_find_item_pm,           res_find_item_nt);
    PR_L("CONTAINS",        res_cons_key_item_pm,       res_cons_key_item_nt);
    PR_L("REMOVE",          res_remove_item_pm,         res_remove_item_nt);
    PR_L("SWAP",            res_swap_pm,                res_swap_nt);
    PR_L("SORT",            res_sort_pm,                res_sort_nt);
    PR_L("SET STR",         res_set_string_pm,          res_set_string_nt);
    PR_L("CONS STR",        res_cons_string_pm,         res_cons_string_nt);

    #undef PR_L

    printf("======================================================================");
    printf(" TOTAL TIME     |  %4d ms    |  %4d ms   |  x%.1f ", total_pm_int, total_nt_int, (total_pm_int > 0) ? (float(total_nt_int)/float(total_pm_int)) : 0.0);
    printf("======================================================================");
    printf(" ");
}

stock CheckPawnMap()
{
//===============Create
    new start_t = GetTickCount();

    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        PawnMap = Map_Create();
        Map_Destroy(PawnMap);
    }
    res_create_pm = GetTickCount() - start_t;
//===============SET
    PawnMap = Map_Create();

    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        new player[playerData];

        player[Item] = i + 1;

        switch(i % 3)
        {
            case 0: player[Amount] = 1000;
            case 1: player[Amount] = 5000;
            case 2: player[Amount] = 3000;
        }
        
        Map_SetString(player[Name], "Qwerasdfxcfasfdwqesdas");

        Map_Set(PawnMap, i, player);
    }
    res_set_pm = GetTickCount() - start_t;

//===============GET
    start_t = GetTickCount();
    mapfor(PawnMap, key)
    {
        static player[playerData];

        Map_Get(PawnMap, key, player);

        format(str1, sizeof(str1),
            "%d %d %d %s",
            key,
            player[Item],
            player[Amount],
            player[Name]
        );
    }
    res_get_pm = GetTickCount() - start_t;

//===============ADD
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        static player[playerData];

        Map_Get(PawnMap, 3500, player);

        player[Amount] += 100;

        Map_Get(PawnMap, 3500, player);
    }
    res_add_pm = GetTickCount() - start_t;

//===============FIND
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        if (Map_FindKeyByField(PawnMap, playerData:Item, MAP_TYPE_INT, 1500))
        {

        }
    }
    res_find_item_pm = GetTickCount() - start_t;

//===============CONTAINS
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        if (Map_ContainsKey(PawnMap, 3877))
        {
            
        }
    }
    res_cons_key_item_pm = GetTickCount() - start_t;


//===============Remove ITEM
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        new slotid = Map_FindKeyByField(PawnMap, playerData:Item, MAP_TYPE_INT, 2500);

        if (slotid != INVALID_MAP_KEY_ID)
        {
            Map_RemoveKey(PawnMap, slotid);
        }
    }
    res_remove_item_pm = GetTickCount() - start_t;

//===============SWAP Slot
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        Map_Swap(PawnMap, 100, 1500);
    }
    res_swap_pm = GetTickCount() - start_t;

//===============SORT
    start_t = GetTickCount();
    Map_SortByField(PawnMap, playerData:Amount, MAP_SORT_DESC);
    res_sort_pm = GetTickCount() - start_t;

//===============Set String
    new PawnMap:map_str = Map_Create();
    new key_buffer_1[16];

    start_t = GetTickCount();
    
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        valstr(key_buffer_1, i);
        new strid = Map_StringKeyToInt(map_str, key_buffer_1);

        static data[stringData];
        data[Item] = 1;
        Map_Set(map_str, strid, data);
    }
    res_set_string_pm = GetTickCount() - start_t;

//===============Contains String
    new key_buffer_2[16];

    start_t = GetTickCount();
    for(new i = MAX_TEST_ITEMS / 2; i >= 0; i--)
    {
        valstr(key_buffer_2, i);
        
        if (Map_ContainsStringKey(map_str, key_buffer_2))
        {

        }
    }
    res_cons_string_pm = GetTickCount() - start_t;

    Map_Destroy(PawnMap);
    Map_Destroy(map_str);
}

stock CheckRawArray()
{
    res_create_nt = 0;
//===============SET
    new start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        gRawArray[i][Raw_Item] = i + 1;
        switch(i % 3)
        {
            case 0: gRawArray[i][Raw_Amount] = 1000;
            case 1: gRawArray[i][Raw_Amount] = 5000;
            case 2: gRawArray[i][Raw_Amount] = 3000;
        }
        format(gRawArray[i][Raw_Name], 24, "%s", "Qwerasdfxcfasfdwqesdas");
    }
    res_set_nt = GetTickCount() - start_t;
    
//===============GET
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        format(str2, sizeof(str2),
            "%d %d %d %s",
            i,
            gRawArray[i][Raw_Item],
            gRawArray[i][Raw_Amount],
            gRawArray[i][Raw_Name]
        );
    }
    res_get_nt = GetTickCount() - start_t;

//===============ADD
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        gRawArray[3500][Raw_Amount] += 100;
    }
    res_add_nt = GetTickCount() - start_t;

//===============FIND
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        for(new x = 0; x < sizeof(gRawArray); x++)
        {
            if (gRawArray[x][Raw_Item] == 1500)
            {
                break;
            }
        }
    }
    res_find_item_nt = GetTickCount() - start_t;

//===============CONTAINS
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        for(new x = 0; x < sizeof(gRawArray); x++)
        {
            if (gRawArray[x][Raw_Item] == 3877)
            {
                break;
            }
        }
    }
    res_cons_key_item_nt = GetTickCount() - start_t;

//===============Remove ITEM
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        for(new x = 0; x < sizeof(gRawArray); x++)
        {
            if (gRawArray[x][Raw_Item] == 2500)
            {
                gRawArray[x][Raw_Item] = 0;
                gRawArray[x][Raw_Amount] = 0;
                gRawArray[x][Raw_Name][0] = EOS;
                break;
            }
        }
    }
    res_remove_item_nt = GetTickCount() - start_t;
    
//===============SWAP
    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        new idx1 = -1, idx2 = -1;
        
        if(gRawArray[200][Raw_Item] != 0) idx1 = 200;
        if(gRawArray[1800][Raw_Item] != 0) idx2 = 1800;
        
        if(idx1 != -1 && idx2 != -1)
        {
            new temp_item = gRawArray[idx1][Raw_Item]; 
            new temp_amount = gRawArray[idx1][Raw_Amount]; 
            new temp_name[24];
            format(temp_name, 24, gRawArray[idx1][Raw_Name]);

            gRawArray[idx1][Raw_Item] = gRawArray[idx2][Raw_Item];
            gRawArray[idx1][Raw_Amount] = gRawArray[idx2][Raw_Amount];
            format(gRawArray[idx1][Raw_Name], 24, gRawArray[idx2][Raw_Name]);

            gRawArray[idx2][Raw_Item] = temp_item;
            gRawArray[idx2][Raw_Amount] = temp_amount;

            format(gRawArray[idx2][Raw_Name], 24, temp_name);

        }
    }
    res_swap_nt = GetTickCount() - start_t;

//===============SORT
    start_t = GetTickCount();
    RAW_SortByAmount(0, MAX_TEST_ITEMS - 1);
    res_sort_nt = GetTickCount() - start_t;

//===============Set String
    new key_buffer_1[16];

    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        valstr(key_buffer_1, i); 
        format(gRawArrayStr[i], 16, key_buffer_1);
    }
    res_set_string_nt = GetTickCount() - start_t;

//===============Contains String
    new key_buffer_2[16];

    start_t = GetTickCount();
    for(new i = 0; i < MAX_TEST_ITEMS; i++)
    {
        valstr(key_buffer_2, i);
        
        for(new j = MAX_TEST_ITEMS / 2; j >= 0; j--)
        {
            if(!strcmp(gRawArrayStr[j], key_buffer_2))
            {
                break; 
            }
        }
    }
    res_cons_string_nt = GetTickCount() - start_t;
}


stock RAW_SortByAmount(left, right)
{
    new i = left, j = right;
    new pivot = gRawArray[(left + right) / 2][Raw_Amount];
    new temp_id, temp_amount, temp_name[24];

    while (i <= j) 
    {
        while (gRawArray[i][Raw_Amount] > pivot) i++;
        while (gRawArray[j][Raw_Amount] < pivot) j--;

        if (i <= j) 
        {
            temp_id = gRawArray[i][Raw_Item];
            gRawArray[i][Raw_Item] = gRawArray[j][Raw_Item];
            gRawArray[j][Raw_Item] = temp_id;

            temp_amount = gRawArray[i][Raw_Amount];
            gRawArray[i][Raw_Amount] = gRawArray[j][Raw_Amount];
            gRawArray[j][Raw_Amount] = temp_amount;

            memcpy(temp_name, gRawArray[i][Raw_Name], 0, 24 * 4);
            memcpy(gRawArray[i][Raw_Name], gRawArray[j][Raw_Name], 0, 24 * 4);
            memcpy(gRawArray[j][Raw_Name], temp_name, 0, 24 * 4);

            i++;
            j--;
        }
    }
    if (left < j) RAW_SortByAmount(left, j);
    if (i < right) RAW_SortByAmount(i, right);
}
```
</details>
