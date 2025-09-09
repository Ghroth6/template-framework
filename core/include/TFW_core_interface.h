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

#ifdef __cplusplus
}
#endif

#endif // TFW_CORE_INTERFACE_H
