#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef unsigned long int ulli;
typedef short int bool;

struct String {
  ulli *length;
  struct String *next;
  struct String *before;

  ulli pos;
  char this;
};

typedef struct String String;

void debug(int num) {
  printf("debug -> %d\n", num);
}

void printStringData(String *str) {
  printf("this: %c, pos: %ld, length: %ld, now: %p, next: %p, before: %p\n", str->this, str->pos, *str->length, str, str->next, str->before);
}

void printData(String *str) {
  String *now;
  for (now = str; now != NULL; now = now->next) {
    printStringData(now);
  }
}

String *newChar(char this, ulli pos, ulli length, String *before, String *next) {
  String *ret = malloc(sizeof(String));
  
  ret->this = this;
  ret->before = before;
  ret->next = next;
  ret->length = malloc(sizeof(ulli));
  *ret->length = length;
  ret->pos = pos;

  return ret;
}

String *emptyString() {
  return newChar('\0', 0, 0, NULL, NULL);
}

bool isEmptyString(String *str) {
  return *str->length == 0 ? TRUE : FALSE;
}

String *runBack(String *str) {
  String *ret;
  for (ret = str; ret->before != NULL; ret = ret->before);
  return ret;
}

char *toCharArr(String *str) {
  char *ret = malloc(sizeof(char) * (*str->length));

  ulli count = 0;

  for(str; str != NULL; str = str->next) {
    ret[count] = str->this;
    count += 1;
  }
  return ret;
}

String *newString(char *str) {
  if (str == NULL) {
    return emptyString();
  } else {
    ulli len = (ulli)(strlen(str));

    String *now = emptyString();
    String *before = emptyString();

    for(ulli i = 0; i < len; i += 1) {
      if (isEmptyString(now)) {
        now = newChar(str[i], i, len, NULL, NULL);
      } else {
        before = now;

        now = newChar(str[i], i, len, NULL, NULL);

        now->before = before;

        before->next = now;        
      }

    }
    return runBack(now);
  }
}

ulli len(String *str) {
  return *str->length;
}

char charAt(String *str, ulli index) {
  for (str; str-> pos != index; str = str->next);
  return str->this;
}

int main(void) {
  String *str = newString("teste");
  printf("%c\n", charAt(str, 0));
}