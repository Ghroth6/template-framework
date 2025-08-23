#ifndef TFW_INTERFACE_H
#define TFW_INTERFACE_H

#include "TFW_errorno.h"
#include "TFW_types.h"
#include <string>

namespace TFW {

// 鏍稿績鎺ュ彛绫?class CoreInterface {
public:
    static CoreInterface* GetInstance();

    // 鍒濆鍖栨帴鍙?    TFW_Result Initialize();

    // 閫€鍑烘帴鍙?    TFW_Result Exit();

    // 妫€鏌ョ姸鎬佹帴鍙?    bool IsInitialized() const;

    // 鎵ц鎿嶄綔鎺ュ彛
    TFW_Result ExecuteOperation(const std::string& operation);

private:
    CoreInterface() = default;
    ~CoreInterface() = default;
    CoreInterface(const CoreInterface&) = delete;
    CoreInterface& operator=(const CoreInterface&) = delete;

    bool initialized_ = false;
    TFW_Type mainType_;
};



} // namespace TFW

#endif // TFW_INTERFACE_H
