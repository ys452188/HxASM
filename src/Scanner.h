#ifndef HXASM_SCANNER_H
#define HXASM_SCANNER_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
wchar_t* readLine(FILE* stream, int* err) {
  if (!err) return NULL;
  *err = 0;
  if (!stream) {
    *err = -1;
    return NULL;
  }
  wchar_t ch = L'\0';
  int index = 0;
  int size = 0;
  wchar_t* line = (wchar_t*)calloc(4, sizeof(wchar_t));
  if (!line) {
    *err = -1;
    return NULL;
  }
  size = 4;
  while ((ch = fgetwc(stream)) != WEOF) {
    if (ch == L'\n') {
      break;
    }
    if (index >= size) {
      int oldSize = size;
      size += 4;
      wchar_t* temp = (wchar_t*)realloc(line, size * sizeof(wchar_t));
      if (!temp) {
        *err = -1;
        free(line);
        return NULL;
      }
      line = temp;
      memset((void*)(line + oldSize), 0, (size - oldSize) * sizeof(wchar_t));
    }
    line[index] = ch;
    index++;
  }
  if (index >= size) {
    size++;
    wchar_t* temp = (wchar_t*)realloc(line, size * sizeof(wchar_t));
    if (!temp) {
      *err = -1;
      free(line);
      return NULL;
    }
    line = temp;
    line[index] = L'\0';
  }
  return line;
}
#endif