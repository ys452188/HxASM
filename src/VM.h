#ifndef HXASM_VM_H
#define HXASM_VM_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "Lexer.h"
typedef struct OpStack {
    long stack[OP_STACK_SIZE];
    int top;  // 栈项指针
} OpStack;
// 解释器
extern int interpret(Token* tokens, int size, OpStack* stack);
// 显示栈中内容
extern void displayStack(OpStack* stack);
/*辅助函数*/
static int interpret_add(OpStack* stack);
static int interpret_sub(OpStack* stack);
static int interpret_mul(OpStack* stack);
static int interpret_div(OpStack* stack);
static int interpret_pop(OpStack* stack);
static int interpret_push(OpStack* stack, Token* tokens);
static int interpret_out(OpStack* stack);
static int interpret_mov(OpStack* stack);

// 解释器
int interpret(Token* tokens, int size, OpStack* stack) {
    if (!tokens || !stack || size == 0) return -1;
    initLocale();
    switch (tokens[0].type) {
    case OP_ADD: {
        wprintf(L"执行add\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_add(stack);
    }
    case OP_SUB: {
        wprintf(L"执行sub\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_sub(stack);
    }
    case OP_MUL: {
        wprintf(L"执行mul\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_mul(stack);
    }
    case OP_DIV: {
        wprintf(L"执行div\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_div(stack);
    }
    case OP_POP: {
        wprintf(L"执行pop\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_pop(stack);
    }
    case OP_PUSH: {
        wprintf(L"执行push\n");
        if (size != 2) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_push(stack, tokens);
    }
    case OP_OUT: {
        wprintf(L"执行out\n");
        if (size != 1) {
            wprintf(L"\33[31m[E] 指令错误\33[0m\n");
            return -1;
        }
        return interpret_out(stack);
    }
    default: {
        wprintf(L"\33[31m[E] 指令尚不支持\33[0m\n");
        return -1;
    }
    }
    return 0;
}
int interpret_add(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    if (stack->top < 2) {
        wprintf(L"\33[31m[E] 栈中操作数不够\33[0m\n");
        return -1;
    }
    stack->stack[(stack->top - 2)] += stack->stack[(stack->top - 1)];
    stack->top--;
    return 0;
}
int interpret_sub(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    if (stack->top < 2) {
        wprintf(L"\33[31m[E] 栈中操作数不够\33[0m\n");
        return -1;
    }
    stack->stack[(stack->top - 2)] =
        stack->stack[(stack->top - 2)] - stack->stack[(stack->top - 1)];
    stack->top--;
    return 0;
}
int interpret_mul(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    if (stack->top < 2) {
        wprintf(L"\33[31m[E] 栈中操作数不够\33[0m\n");
        return -1;
    }
    stack->stack[(stack->top - 2)] =
        (stack->stack[(stack->top - 2)]) * (stack->stack[(stack->top - 1)]);
    stack->top--;
    return 0;
}
int interpret_div(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    if (stack->top < 2) {
        wprintf(L"\33[31m[E] 栈中操作数不够\33[0m\n");
        return -1;
    }
    stack->stack[(stack->top - 2)] =
        (stack->stack[(stack->top - 2)]) / (stack->stack[(stack->top - 1)]);
    stack->top--;
    return 0;
}
int interpret_pop(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top > 0) stack->top--;
    return 0;
}
int interpret_push(OpStack* stack, Token* tokens) {
    if (!stack || !tokens) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    long value = 0;
    if (tokens[1].type != NUMBER) {
        wprintf(L"\33[31m[E] 指令错误\33[0m\n");
        return -1;
    }
    wchar_t* end = NULL;
    value = wcstol(tokens[1].value, &end, 0);
    stack->stack[stack->top] = value;
    stack->top++;
    return 0;
}
int interpret_out(OpStack* stack) {
    if (!stack) return -1;
    if (stack->top < 0 || stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m[E] 操作数栈错误\33[0m\n");
        return -1;
    }
    if (stack->top < 1) {
        wprintf(L"\33[31m[E] 栈中操作数不够\33[0m\n");
        return -1;
    }
    wprintf(L"%ld\n", stack->stack[stack->top-1]);
    return 0;
}
void displayStack(OpStack* stack) {
    if (!stack) return;
    wprintf(L"----------------------\n");
    wprintf(L" |");
    for (int i = 0; i < OP_STACK_SIZE; i++) {
        wprintf(L"%ls%ld |", (stack->top == i) ? L" \33[7;32mtop\33[0m->" : L" ",
                stack->stack[i]);
    }
    wprintf(L"\n");
    if (stack->top >= OP_STACK_SIZE) {
        wprintf(L"\33[31m此时栈溢出\n\33[0m");
    }
    wprintf(L"----------------------\n");
    return;
}
#endif