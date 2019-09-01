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

  char this;
};

typedef struct String String;

void debug(int num) {
  printf("debug -> %d\n", num);
}

void printStringData(String *str) {
  printf("this: %c, length: %ld, next: %p, before: %p, now: %p\n", str->this, *str->length, str->next, str->before, str);
}

void printData(String *str) {
  String *now;
  for (now = str; now != NULL; now = now->next) {
    printStringData(now);
  }
}

String *newChar(char this, String *before, String *next, ulli length) {
  String *ret = malloc(sizeof(String));
  
  ret->this = this;
  ret->before = before;
  ret->next = next;
  ret->length = malloc(sizeof(ulli));
  *ret->length = length;

  return ret;
}

String *emptyString() {
  return newChar('\0', NULL, NULL, 0);
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
        now = newChar(str[i], NULL, NULL, len);
      } else {
        before = now;

        now = newChar(str[i], NULL, NULL, len);

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

int main(void) {
  String *str = newString("teste");
  printf("%s\n", toCharArr(str));
}