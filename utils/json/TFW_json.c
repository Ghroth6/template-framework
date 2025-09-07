/*
 * TFW_json.c
 *
 * TFW JSON操作工具实现文件
 *
 * 该文件实现了JSON操作相关的函数接口
 */

#include "TFW_json.h"
#include "TFW_utils_log.h"
#include <stdlib.h>

int32_t GetStringItemByJsonObject(const cJSON *json, const char * const string, char *target, uint32_t targetLen)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target) || (0 == targetLen)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return -1;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return -1;
    }

    if (!cJSON_IsString(item)) {
        TFW_LOGE_UTILS("Item is not a string: %s", string);
        return -1;
    }

    if (strlen(item->valuestring) >= targetLen) {
        TFW_LOGE_UTILS("Target buffer too small for string: %s", string);
        return -1;
    }

    strcpy(target, item->valuestring);
    return 0;
}

bool GetJsonObjectStringItem(const cJSON *json, const char * const string, char *target, uint32_t targetLen)
{
    return (GetStringItemByJsonObject(json, string, target, targetLen) == 0) ? true : false;
}

bool GetJsonObjectNumber16Item(const cJSON *json, const char * const string, uint16_t *target)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsNumber(item)) {
        TFW_LOGE_UTILS("Item is not a number: %s", string);
        return false;
    }

    if ((item->valuedouble < 0) || (item->valuedouble > UINT16_MAX)) {
        TFW_LOGE_UTILS("Number out of range for uint16_t: %s", string);
        return false;
    }

    *target = (uint16_t)item->valueint;
    return true;
}

bool GetJsonObjectNumberItem(const cJSON *json, const char * const string, int32_t *target)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsNumber(item)) {
        TFW_LOGE_UTILS("Item is not a number: %s", string);
        return false;
    }

    *target = item->valueint;
    return true;
}

bool GetJsonObjectSignedNumberItem(const cJSON *json, const char * const string, int32_t *target)
{
    // 实现与GetJsonObjectNumberItem相同
    return GetJsonObjectNumberItem(json, string, target);
}

bool GetJsonObjectNumber64Item(const cJSON *json, const char * const string, int64_t *target)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsNumber(item)) {
        TFW_LOGE_UTILS("Item is not a number: %s", string);
        return false;
    }

    *target = (int64_t)item->valuedouble;
    return true;
}

bool GetJsonObjectSignedNumber64Item(const cJSON *json, const char * const string, int64_t *target)
{
    // 实现与GetJsonObjectNumber64Item相同
    return GetJsonObjectNumber64Item(json, string, target);
}

bool GetJsonObjectDoubleItem(const cJSON *json, const char * const string, double *target)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsNumber(item)) {
        TFW_LOGE_UTILS("Item is not a number: %s", string);
        return false;
    }

    *target = item->valuedouble;
    return true;
}

bool GetJsonObjectBoolItem(const cJSON *json, const char * const string, bool *target)
{
    const cJSON *item = NULL;

    if ((NULL == json) || (NULL == string) || (NULL == target)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsBool(item)) {
        TFW_LOGE_UTILS("Item is not a boolean: %s", string);
        return false;
    }

    *target = (item->type == cJSON_True) ? true : false;
    return true;
}

bool AddStringToJsonObject(cJSON *json, const char * const string, const char *value)
{
    cJSON *item = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_CreateString(value);
    if (NULL == item) {
        TFW_LOGE_UTILS("Failed to create string item: %s", string);
        return false;
    }

    if (cJSON_AddItemToObject(json, string, item) == 0) {
        TFW_LOGE_UTILS("Failed to add string item to object: %s", string);
        cJSON_Delete(item);
        return false;
    }

    return true;
}

bool AddStringArrayToJsonObject(cJSON *json, const char * const string, const char * const *strings, int32_t count)
{
    cJSON *array = NULL;
    int32_t i;

    if ((NULL == json) || (NULL == string) || (NULL == strings) || (count <= 0)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    array = cJSON_CreateArray();
    if (NULL == array) {
        TFW_LOGE_UTILS("Failed to create array: %s", string);
        return false;
    }

    for (i = 0; i < count; i++) {
        cJSON *item = cJSON_CreateString(strings[i]);
        if (NULL == item) {
            TFW_LOGE_UTILS("Failed to create string item at index %d", i);
            cJSON_Delete(array);
            return false;
        }

        if (cJSON_AddItemToArray(array, item) == 0) {
            TFW_LOGE_UTILS("Failed to add string item to array at index %d", i);
            cJSON_Delete(item);
            cJSON_Delete(array);
            return false;
        }
    }

    if (cJSON_AddItemToObject(json, string, array) == 0) {
        TFW_LOGE_UTILS("Failed to add array to object: %s", string);
        cJSON_Delete(array);
        return false;
    }

    return true;
}

bool AddNumber16ToJsonObject(cJSON *json, const char * const string, uint16_t num)
{
    cJSON *item = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_CreateNumber(num);
    if (NULL == item) {
        TFW_LOGE_UTILS("Failed to create number item: %s", string);
        return false;
    }

    if (cJSON_AddItemToObject(json, string, item) == 0) {
        TFW_LOGE_UTILS("Failed to add number item to object: %s", string);
        cJSON_Delete(item);
        return false;
    }

    return true;
}

bool AddNumberToJsonObject(cJSON *json, const char * const string, int32_t num)
{
    cJSON *item = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_CreateNumber(num);
    if (NULL == item) {
        TFW_LOGE_UTILS("Failed to create number item: %s", string);
        return false;
    }

    if (cJSON_AddItemToObject(json, string, item) == 0) {
        TFW_LOGE_UTILS("Failed to add number item to object: %s", string);
        cJSON_Delete(item);
        return false;
    }

    return true;
}

bool AddNumber64ToJsonObject(cJSON *json, const char * const string, int64_t num)
{
    cJSON *item = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_CreateNumber((double)num);
    if (NULL == item) {
        TFW_LOGE_UTILS("Failed to create number item: %s", string);
        return false;
    }

    if (cJSON_AddItemToObject(json, string, item) == 0) {
        TFW_LOGE_UTILS("Failed to add number item to object: %s", string);
        cJSON_Delete(item);
        return false;
    }

    return true;
}

bool AddBoolToJsonObject(cJSON *json, const char * const string, bool value)
{
    cJSON *item = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_CreateBool(value);
    if (NULL == item) {
        TFW_LOGE_UTILS("Failed to create bool item: %s", string);
        return false;
    }

    if (cJSON_AddItemToObject(json, string, item) == 0) {
        TFW_LOGE_UTILS("Failed to add bool item to object: %s", string);
        cJSON_Delete(item);
        return false;
    }

    return true;
}

bool GetJsonObjectInt32Item(const cJSON *json, const char * const string, int32_t *target)
{
    // 实现与GetJsonObjectNumberItem相同
    return GetJsonObjectNumberItem(json, string, target);
}

char *GetDynamicStringItemByJsonObject(const cJSON * const json, const char * const string, uint32_t limit)
{
    const cJSON *item = NULL;
    char *result = NULL;

    if ((NULL == json) || (NULL == string)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return NULL;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return NULL;
    }

    if (!cJSON_IsString(item)) {
        TFW_LOGE_UTILS("Item is not a string: %s", string);
        return NULL;
    }

    if ((limit > 0) && (strlen(item->valuestring) >= limit)) {
        TFW_LOGE_UTILS("String exceeds limit: %s", string);
        return NULL;
    }

    result = (char *)malloc(strlen(item->valuestring) + 1);
    if (NULL == result) {
        TFW_LOGE_UTILS("Failed to allocate memory for string: %s", string);
        return NULL;
    }

    strcpy(result, item->valuestring);
    return result;
}

bool AddIntArrayToJsonObject(cJSON *json, const char *string, const int32_t *array, int32_t arrayLen)
{
    cJSON *cjsonArray = NULL;
    int32_t i;

    if ((NULL == json) || (NULL == string) || (NULL == array) || (arrayLen <= 0)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    cjsonArray = cJSON_CreateArray();
    if (NULL == cjsonArray) {
        TFW_LOGE_UTILS("Failed to create array: %s", string);
        return false;
    }

    for (i = 0; i < arrayLen; i++) {
        cJSON *item = cJSON_CreateNumber(array[i]);
        if (NULL == item) {
            TFW_LOGE_UTILS("Failed to create number item at index %d", i);
            cJSON_Delete(cjsonArray);
            return false;
        }

        if (cJSON_AddItemToArray(cjsonArray, item) == 0) {
            TFW_LOGE_UTILS("Failed to add number item to array at index %d", i);
            cJSON_Delete(item);
            cJSON_Delete(cjsonArray);
            return false;
        }
    }

    if (cJSON_AddItemToObject(json, string, cjsonArray) == 0) {
        TFW_LOGE_UTILS("Failed to add array to object: %s", string);
        cJSON_Delete(cjsonArray);
        return false;
    }

    return true;
}

bool GetJsonObjectIntArrayItem(const cJSON *json, const char *string, int32_t *array, int32_t arrayLen)
{
    const cJSON *item = NULL;
    const cJSON *arrayItem = NULL;
    int32_t i;
    int32_t size;

    if ((NULL == json) || (NULL == string) || (NULL == array) || (arrayLen <= 0)) {
        TFW_LOGE_UTILS("Invalid parameter");
        return false;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, string);
    if (NULL == item) {
        TFW_LOGE_UTILS("Item not found: %s", string);
        return false;
    }

    if (!cJSON_IsArray(item)) {
        TFW_LOGE_UTILS("Item is not an array: %s", string);
        return false;
    }

    size = cJSON_GetArraySize(item);
    if (size != arrayLen) {
        TFW_LOGE_UTILS("Array size mismatch: expected %d, got %d", arrayLen, size);
        return false;
    }

    for (i = 0; i < size; i++) {
        arrayItem = cJSON_GetArrayItem(item, i);
        if (NULL == arrayItem) {
            TFW_LOGE_UTILS("Failed to get array item at index %d", i);
            return false;
        }

        if (!cJSON_IsNumber(arrayItem)) {
            TFW_LOGE_UTILS("Array item at index %d is not a number", i);
            return false;
        }

        array[i] = arrayItem->valueint;
    }

    return true;
}
