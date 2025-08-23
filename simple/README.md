# Simple 绀轰緥椤圭洰

鏈洰褰曞寘鍚玊FW妗嗘灦鐨勭畝鍗曠ず渚嬮」鐩紝鍒嗗埆鎻愪緵C鍜孋++鐗堟湰鐨勫疄鐜般€?

## 鐩綍缁撴瀯

```
simple/
鈹溾攢鈹€ c/                    # C璇█鐗堟湰绀轰緥
鈹?  鈹溾攢鈹€ main.c           # 涓荤▼搴?
鈹?  鈹溾攢鈹€ CMakeLists.txt   # C鐗堟湰鏋勫缓閰嶇疆
鈹?  鈹斺攢鈹€ README.md        # C鐗堟湰璇存槑
鈹溾攢鈹€ cpp/                  # C++鐗堟湰绀轰緥
鈹?  鈹溾攢鈹€ main.cpp         # 涓荤▼搴?
鈹?  鈹溾攢鈹€ CMakeLists.txt   # C++鐗堟湰鏋勫缓閰嶇疆
鈹?  鈹斺攢鈹€ README.md        # C++鐗堟湰璇存槑
鈹溾攢鈹€ CMakeLists.txt        # 鏍规瀯寤洪厤缃?
鈹斺攢鈹€ README.md             # 鏈枃妗?
```

## 鏋勫缓璇存槑

### 浠庢牴鐩綍鏋勫缓鎵€鏈夌ず渚嬶紙鎺ㄨ崘锛?
```bash
# Windows
.\build_simple.bat

# Linux/macOS
chmod +x build_simple.sh
./build_simple.sh
```

### 鏋勫缓鐗瑰畾鐗堟湰
```bash
# 鏋勫缓C鐗堟湰
cd simple/c
mkdir build && cd build
cmake ..
cmake --build . --config Release

# 鏋勫缓C++鐗堟湰
cd simple/cpp
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## 杈撳嚭鐩綍

鎵€鏈夋瀯寤轰骇鐗╁皢杈撳嚭鍒伴」鐩牴鐩綍鐨?`build/out/` 鏂囦欢澶逛腑锛?
- `build/out/c/` - C鐗堟湰鍙墽琛屾枃浠?
- `build/out/cpp/` - C++鐗堟湰鍙墽琛屾枃浠?

鏋勫缓瀹屾垚鍚庯紝鏍圭洰褰曚細鑷姩鐢熸垚锛?
- **Windows**: 蹇嵎鏂瑰紡鏂囦欢锛?lnk锛?
- **Linux/macOS**: 杞摼鎺ユ枃浠?

鍙互鐩存帴鍦ㄦ牴鐩綍杩愯锛?
```bash
# Windows
.\tfw_simple_c.exe.lnk
.\tfw_simple_cpp.exe.lnk

# Linux/macOS
./tfw_simple_c
./tfw_simple_cpp
```

## 鍔熻兘璇存槑

涓や釜绀轰緥閮藉疄鐜颁簡鐩稿悓鐨勫姛鑳斤細
1. 鍒濆鍖朤FW鏍稿績
2. 璋冪敤鎺ュ彛鎵ц绠€鍗曟搷浣?
3. 鑾峰彇鎿嶄綔缁撴灉
4. 姝ｅ父閫€鍑?

鍖哄埆鍦ㄤ簬锛?
- **C鐗堟湰**: 浣跨敤C鎺ュ彛锛岄€傚悎C璇█椤圭洰闆嗘垚
- **C++鐗堟湰**: 浣跨敤C++鎺ュ彛锛岄€傚悎C++椤圭洰闆嗘垚
