// kami_entry.c — C constructor 入口（dylib 加载时自动执行 Swift 端初始化）
// Swift 无法直接标注 attribute((constructor))，用 C 桥接
#include <stdio.h>

// 声明 Swift 暴露的入口函数
// 由 swiftc -emit-library 导出，函数名 mangling 后为 _kamiEntryPoint
extern void kamiEntryPoint(void);

// dylib 加载时自动调用
__attribute__((constructor))
static void kami_constructor(void) {
    kamiEntryPoint();
}