#ifndef TFW_CORE_INTERFACE_H
#define TFW_CORE_INTERFACE_H

#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// TFW core module C interface declaration
// ============================================================================

/**
 * initialize TFW core module
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceInit(void);

/**
 * deinitialize TFW core module
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceDeinit(void);

/**
 * Get all configuration items from the TFW core module
 * @param config_array pointer to array that will hold the configuration items
 * @param count pointer to variable that will hold the number of configuration items
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count);

/**
 * Free all configuration items allocated by TFW_CoreInterfaceGetAllConfigItems
 * @param config_array pointer to the configuration items array to be freed
 */
void TFW_CoreInterfaceFreeAllConfigItems(TFW_ConfigItem *config_array);

/**
 * Get integer configuration value by key
 * @param key configuration key to retrieve
 * @param value pointer to store the retrieved integer value
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetIntConfigValue(TFW_ConfigKey key, int32_t *value);

/**
 * Set integer configuration value by key
 * @param key configuration key to set
 * @param value integer value to set
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceSetIntConfigValue(TFW_ConfigKey key, int32_t value);

/**
 * Get float configuration value by key
 * @param key configuration key to retrieve
 * @param value pointer to store the retrieved float value
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetFloatConfigValue(TFW_ConfigKey key, float *value);

/**
 * Set float configuration value by key
 * @param key configuration key to set
 * @param value float value to set
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceSetFloatConfigValue(TFW_ConfigKey key, float value);

/**
 * Get boolean configuration value by key
 * @param key configuration key to retrieve
 * @param value pointer to store the retrieved boolean value
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetBoolConfigValue(TFW_ConfigKey key, bool *value);

/**
 * Set boolean configuration value by key
 * @param key configuration key to set
 * @param value boolean value to set
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceSetBoolConfigValue(TFW_ConfigKey key, bool value);

/**
 * Get string configuration value by key
 * @param key configuration key to retrieve
 * @param value pointer to store the retrieved string value
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetStringConfigValue(TFW_ConfigKey key, const char **value);

/**
 * Set string configuration value by key
 * @param key configuration key to set
 * @param value string value to set
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceSetStringConfigValue(TFW_ConfigKey key, const char *value);

/**
 * Get configuration value type by key
 * @param key configuration key to query
 * @param type pointer to store the configuration value type
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceGetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type);

#ifdef __cplusplus
}
#endif

#endif // TFW_CORE_INTERFACE_H
