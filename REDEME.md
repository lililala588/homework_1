1. 打开visual studio->继续但是无需代码
2. 文件-> 打开-> CMake
3. 选择CMakeLists.txt
4. 生成->全部生成

build目录下就会有GenerateGraph.exe
用法: GenerateGraph.exe  输入文件  函数名
例如： GenerateGraph.exe ../testData/remove510.txt remove

目录结构：
- googletest: 所使用的单元测试库
- src: 源代码
- test: 单元测试代码
- testData: 测试数据
- CMakeLists.txt: CMake文件
- main.cc: main函数文件
- REDEME.md： 说明文件
