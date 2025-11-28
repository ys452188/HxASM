#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#define OP_STACK_SIZE 8  // 操作数栈大小
void initLocale(void) {
    // 设置Locale
    if (!setlocale(LC_ALL, "zh_CN.UTF-8")) {
        if (!setlocale(LC_ALL, "en_US.UTF-8")) {
            setlocale(LC_ALL, "C.UTF-8");
        }
    }
    // 设置宽字符流的定向
    fwide(stdout, 1);  // 1 = 宽字符定向
    return;
}
#include "Lexer.h"
#include "Scanner.h"
#include "VM.h"
int main(int argc, char** argv) {
    initLocale();
    wprintf(L"硫酸铜非常好吃的小项目--HxASM：精简解释型语言\n");
    OpStack op_stack = {0};
    while (1) {
        int err = 0;
        wprintf(L"\33[7;36m--> ");
        wchar_t* src = readLine(stdin, &err);
        wprintf(L"\33[0m");
        if (err) {
            wprintf(L"\33[31m[E] 读取源码时发生错误！\33[0m\n");
            return -1;
        }
        wprintf(L"输入：%ls\n", src);
        Token* tokens = NULL;
        int tokens_size = 0;
        int tokens_counts = 0;  // Tokens实际数量
        err = lex(src, &tokens, &tokens_size, &tokens_counts);
        if (err) {
            wprintf(L"\33[31m[E] 词法分析时发生错误！\33[0m\n");
            return -1;
        }
        free(src);
        if(tokens_counts==0) {
            freeTokens(&tokens, tokens_size);
            wprintf(L"(无输入)\n");
            continue;
        }
        wprintf(L"原始操作数栈：\n");
        displayStack(&op_stack);
        err = interpret(tokens, tokens_counts, &op_stack);
        if (err) {
            wprintf(L"\33[31m[E] 运行时发生错误！\33[0m\n");
        }
        wprintf(L"执行后操作数栈：\n");
        displayStack(&op_stack);
        freeTokens(&tokens, tokens_size);
        wprintf(L"\n");
    }
    return 0;
}