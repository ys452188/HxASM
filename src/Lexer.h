#ifndef HXASM_LEXER_H
#define HXASM_LEXER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
typedef struct Token {
  enum {
    NUMBER,
    ID,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOV,
    OP_OUT,
    OP_PUSH,
    OP_POP
  } type;
  wchar_t* value;
} Token;
int lex(wchar_t* src, Token** tokens, int* size, int* count) {
  wprintf(L"正在进行词法分析\n");
  if (!src || !tokens || !size || !count) return -1;
  // 输入为空
  if (wcslen(src) == 0) return 0;
  if (wcslen(src) == 1) {
    if (src[0] == L'\n' || src[0] == L'\r') return 0;
  }

  if (*tokens == NULL) {
    *tokens = (Token*)calloc(4, sizeof(Token));
    if (!(*tokens)) return -1;
    *size = 4;
  }
  int token_index = 0;
  int index = 0;

  while (index < wcslen(src)) {
    if (src[index] == L'\t' || src[index] == L' ') {  // 跳过空白字符
      index++;
      continue;
    } else {
      int begin = index;
      while (index < wcslen(src)) {
        index++;
        if (src[index] == L'\t' || src[index] == L' ') break;
      }
      int end = index;
      int len = end - begin;
      if (token_index >= *size) {
        void* temp = realloc(*tokens, (*size + 4) * sizeof(Token));
        if (!temp) return -1;
        *tokens = (Token*)temp;
        memset((void*)((*tokens) + *size), 0, sizeof(Token) * 4);
        (*size) += 4;
      }
      (*tokens)[token_index].value = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
      if (!((*tokens)[token_index].value)) return -1;
      wcsncpy((*tokens)[token_index].value, &(src[begin]), len);
      // wprintf(L"复制：%ls\n", (*tokens)[token_index].value);

      if (wcscmp((*tokens)[token_index].value, L"add") == 0) {
        (*tokens)[token_index].type = OP_ADD;
      } else if (wcscmp((*tokens)[token_index].value, L"sub") == 0) {
        (*tokens)[token_index].type = OP_SUB;
      } else if (wcscmp((*tokens)[token_index].value, L"mul") == 0) {
        (*tokens)[token_index].type = OP_MUL;
      } else if (wcscmp((*tokens)[token_index].value, L"div") == 0) {
        (*tokens)[token_index].type = OP_DIV;
      } else if (wcscmp((*tokens)[token_index].value, L"mov") == 0) {
        (*tokens)[token_index].type = OP_MOV;
      } else if (wcscmp((*tokens)[token_index].value, L"out") == 0) {
        (*tokens)[token_index].type = OP_OUT;
      } else if (wcscmp((*tokens)[token_index].value, L"push") == 0) {
        (*tokens)[token_index].type = OP_PUSH;
      } else if (wcscmp((*tokens)[token_index].value, L"pop") == 0) {
        (*tokens)[token_index].type = OP_POP;
      } else if (iswdigit((*tokens)[token_index].value[0])) {
        (*tokens)[token_index].type = NUMBER;
      } else {
        (*tokens)[token_index].type = ID;
      }

      token_index++;
      (*count)++;
      index++;
    }
  }
  wprintf(L"词法分析完成\n");
  return 0;
}
void freeToken(Token* token) {
  if (token->value) {
    // wprintf(L"%ls\n", token->value);
    free(token->value);
    token->value = NULL;
  }
  return;
}
void freeTokens(Token** tokens, int size) {
  if (!tokens) return;
  if (!(*tokens)) return;
  for (int i = 0; i < size; i++) {
    freeToken(&((*tokens)[i]));
  }
  free(*tokens);
  *tokens = NULL;
  wprintf(L"已释放Tokens\n");
  return;
}
#endif