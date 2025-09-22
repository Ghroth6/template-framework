#include "TFW_core.h"
#include "TFW_core_impl.h"

namespace TFW {

TFW_Core &TFW_Core::GetInstance()
{
    return TFW_Core_Impl::GetInstance();
}

} // namespace TFW
