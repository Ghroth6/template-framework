# TempleFrameWork (TFW) - 妯℃澘妗嗘灦

涓€涓敮鎸丆/C++鍙屾帴鍙ｇ殑鏋佺畝寮€鍙戞鏋舵ā鏉匡紝閲囩敤鍒嗗眰鏃ュ織鏋舵瀯鍜岀粺涓€鐨勯敊璇爜绯荤粺銆?
## 椤圭洰缁撴瀯

```
template-framework/
鈹溾攢鈹€ interface/                    # 鎺ュ彛瀹氫箟
鈹?  鈹溾攢鈹€ TFW_errorno.h            # 缁熶竴閿欒鐮佸畾涔?鈹?  鈹溾攢鈹€ TFW_types.h              # 鍩虹绫诲瀷瀹氫箟
鈹?  鈹溾攢鈹€ TFW_interface.h          # C++鎺ュ彛瀹氫箟
鈹?  鈹斺攢鈹€ TFW_c_interface.h        # C鎺ュ彛瀹氫箟
鈹溾攢鈹€ core/                         # 鏍稿績妯″潡
鈹?  鈹溾攢鈹€ TFW_core.h               # 鏍稿績澶存枃浠?鈹?  鈹溾攢鈹€ TFW_core.cpp             # 鏍稿績瀹炵幇
鈹?  鈹溾攢鈹€ TFW_interface.cpp        # 鎺ュ彛瀹炵幇
鈹?  鈹溾攢鈹€ TFW_c_interface.cpp      # C鎺ュ彛瀹炵幇
鈹?  鈹溾攢鈹€ TFW_core_log.h           # 鏍稿績妯″潡鏃ュ織瀹?鈹?  鈹斺攢鈹€ CMakeLists.txt           # 鏋勫缓閰嶇疆
鈹溾攢鈹€ utils/                        # 宸ュ叿妯″潡
鈹?  鈹溾攢鈹€ include/
鈹?  鈹?  鈹斺攢鈹€ TFW_log.h            # 鍩虹鏃ュ織妗嗘灦
鈹?  鈹溾攢鈹€ log/
鈹?  鈹?  鈹斺攢鈹€ TFW_log.c            # 鏃ュ織瀹炵幇
鈹?  鈹溾攢鈹€ TFW_utils_log.h          # 宸ュ叿妯″潡鏃ュ織瀹?鈹?  鈹斺攢鈹€ CMakeLists.txt           # 鏋勫缓閰嶇疆
鈹溾攢鈹€ docs/                         # 鏂囨。
鈹?  鈹斺攢鈹€ 缂栫爜瑙勮寖.md               # 缂栫爜瑙勮寖
鈹溾攢鈹€ CMakeLists.txt                # 鏍规瀯寤洪厤缃?鈹斺攢鈹€ README.md                     # 鏈枃妗?```

## 鏍稿績鐗规€?
### 1. **鍙屾帴鍙ｆ敮鎸?*
- 鍚屾椂鎻愪緵C++鍜孋璇█鎺ュ彛
- 瀹屽叏鍏煎鐨凙PI璁捐

### 2. **鍒嗗眰鏃ュ織鏋舵瀯**
- **鍩虹灞?*: `utils/include/TFW_log.h` - 鏍稿績鏃ュ織鍔熻兘鍜岀瓑绾у畯
- **妯″潡灞?*: 鍚勬ā鍧椾笓鐢ㄦ棩蹇楀ご鏂囦欢 - 鎻愪緵妯″潡鐗瑰畾鏃ュ織瀹?- **搴旂敤灞?*: 鍚勬ā鍧楁簮鏂囦欢 - 浣跨敤鏃ュ織瀹忚褰曟棩蹇?
### 3. **缁熶竴閿欒鐮佺郴缁?*
- 瀹屽叏C鍏煎鐨勯敊璇爜鏋氫妇
- 妯″潡鍖栭敊璇爜鍒嗛厤锛堟瘡涓ā鍧楅鐣?000涓敊璇爜浣嶏級
- 绫诲瀷瀹夊叏鐨勯敊璇鐞?
### 4. **妯″潡鍖栬璁?*
- 娓呮櫚鐨勬ā鍧楀垎绂诲拰渚濊禆绠＄悊
- 鏄撲簬鎵╁睍鍜岀淮鎶ょ殑鏋舵瀯

## 鏃ュ織妗嗘灦

### 鏃ュ織绛夌骇
- `TFW_LOGT` - 璺熻釜淇℃伅 (TRACE)
- `TFW_LOGD` - 璋冭瘯淇℃伅 (DEBUG)
- `TFW_LOGI` - 涓€鑸俊鎭?(INFO)
- `TFW_LOGW` - 璀﹀憡淇℃伅 (WARNING)
- `TFW_LOGE` - 閿欒淇℃伅 (ERROR)
- `TFW_LOGF` - 鑷村懡閿欒 (FATAL)

### 浣跨敤鏂瑰紡

**鍩虹鐢ㄦ硶锛堟帹鑽愶級锛?*
```cpp
#include "../utils/include/TFW_log.h"

TFW_LOGI(TFW_LOG_MODULE_CORE, "Core妯″潡淇℃伅");
TFW_LOGE(TFW_LOG_MODULE_UTILS, "Utils妯″潡閿欒: %s", error_msg);
```

**妯″潡涓撶敤瀹忥細**
```cpp
#include "TFW_core_log.h"

TFW_LOGI_CORE("Core妯″潡淇℃伅");
TFW_LOGE_CORE("Core妯″潡閿欒: %s", error_msg);
```

## 閿欒鐮佺郴缁?
### 閿欒鐮佽寖鍥?- ** -1000 璧?**: 閫氱敤閿欒鐮侊紙棰勭暀1000浣嶏級
- ** -2000 璧?**: CORE妯″潡閿欒鐮侊紙棰勭暀1000浣嶏級
- ** -3000 璧?**: UTILS妯″潡閿欒鐮侊紙棰勭暀1000浣嶏級
- ** -4000 璧?**: SDK妯″潡閿欒鐮侊紙棰勭暀1000浣嶏級
- ** -5000 璧?**: SYSTEM妯″潡閿欒鐮侊紙棰勭暀1000浣嶏級

**褰撳墠宸插畾涔夌殑閿欒鐮侊細**
- **-1000**: `TFW_ERROR` (鏍囧噯閿欒)
- **-999 鍒?-996**: 閫氱敤閿欒鐮侊紙鑷姩璁＄畻锛?- **-200**: `TFW_ERROR_TASK_FAIL` (浠诲姟澶辫触)
- **-199 鍒?-198**: 浠诲姟鐩稿叧閿欒锛堣嚜鍔ㄨ绠楋級
- **-100**: `TFW_ERROR_SYSTEM` (绯荤粺閿欒)
- **-99 鍒?-97**: 绯荤粺鐩稿叧閿欒锛堣嚜鍔ㄨ绠楋級
- **0**: `TFW_SUCCESS` (鎴愬姛)

### 涓昏閿欒鐮?- `TFW_SUCCESS = 0` - 鎴愬姛
- `TFW_ERROR = -1000` - 鏍囧噯閿欒
- `TFW_ERROR_INVALID_PARAM = -999` - 鏃犳晥鍙傛暟
- `TFW_ERROR_NOT_INIT = -998` - 鏈垵濮嬪寲
- `TFW_ERROR_OPERATION_FAIL = -996` - 鎿嶄綔澶辫触

## 蹇€熷紑濮?
### 1. 鏋勫缓涓婚」鐩?```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 2. 鏋勫缓绀轰緥椤圭洰
```bash
# Windows
.\build_simple.bat

# Linux/macOS
chmod +x build_simple.sh
./build_simple.sh
```

### 2. 浣跨敤C++鎺ュ彛
```cpp
#include "TFW_core.h"
#include "TFW_interface.h"
#include "TFW_core_log.h"

using namespace TFW;

// 鍒濆鍖栨牳蹇?auto result = Core::GetInstance().Initialize();

// 浣跨敤鎺ュ彛
auto interface = CoreInterface::GetInstance();
auto configResult = interface.ExecuteOperation("test");

// 璁板綍鏃ュ織
TFW_LOGI_CORE("Core妯″潡鍒濆鍖栨垚鍔?);
```

### 3. 浣跨敤C鎺ュ彛
```c
#include "TFW_c_interface.h"

// 鍒濆鍖栨牳蹇?int result = TFW_CORE_INITIALIZE();

// 鎵ц鎿嶄綔
int actionResult = TFW_CORE_ACTION("test");

// 妫€鏌ョ粨鏋?if (result == TFW_SUCCESS) {
    printf("鎿嶄綔鎴愬姛\n");
} else {
    printf("鎿嶄綔澶辫触锛岄敊璇爜: %d\n", result);
}
```

## 妯″潡寮€鍙?
### 鍒涘缓鏂版ā鍧?1. 鍦ㄦ牴鐩綍鍒涘缓妯″潡鏂囦欢澶?2. 鍒涘缓妯″潡涓撶敤鏃ュ織澶存枃浠讹紙濡?`TFW_newmodule_log.h`锛?3. 瀹炵幇妯″潡鍔熻兘
4. 鏇存柊CMakeLists.txt

### 妯″潡鏃ュ織瀹忕ず渚?```cpp
// TFW_newmodule_log.h
#ifndef TFW_NEWMODULE_LOG_H
#define TFW_NEWMODULE_LOG_H

#include "../utils/include/TFW_log.h"

#define TFW_LOGI_NEWMODULE(fmt, ...) \
    TFW_LOGI(TFW_LOG_MODULE_NEWMODULE, fmt, ##__VA_ARGS__)

#define TFW_LOGE_NEWMODULE(fmt, ...) \
    TFW_LOGE(TFW_LOG_MODULE_NEWMODULE, fmt, ##__VA_ARGS__)

#endif
```

## C鎺ュ彛鍏煎鎬?
鏈鏋舵彁渚涘畬鏁寸殑C璇█鍏煎鎺ュ彛锛屾敮鎸侊細

- **鏍稿績鎿嶄綔**: 鍒濆鍖栥€侀€€鍑恒€佺姸鎬佹鏌?- **閿欒澶勭悊**: 缁熶竴鐨勯敊璇爜浣撶郴
- **鍐呭瓨瀹夊叏**: 瀹夊叏鐨勫瓧绗︿覆澶勭悊

## 缂栫爜瑙勮寖

璇峰弬鑰?`docs/缂栫爜瑙勮寖.md` 浜嗚В璇︾粏鐨勭紪鐮佽鑼冿紝鍖呮嫭锛?
- 鍛藉悕瑙勮寖
- 浠ｇ爜鏍煎紡
- 閿欒澶勭悊
- C鎺ュ彛鍏煎鎬ц鑼?- 澶存枃浠剁鐞?- 鏃ュ織浣跨敤瑙勮寖

## 璁稿彲璇?
鏈」鐩噰鐢∕IT璁稿彲璇併€?