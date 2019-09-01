#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ASCIIFIX 32

typedef long long int lli;
typedef short int bool;

struct String {
  lli *length;
  struct String *next;
  struct String *before;

  lli pos;
  char this;
};

typedef struct String String;

void debug(int num) {
  printf("debug -> %d\n", num);
}

void printStringData(String *str) {
  printf("this: %c, pos: %lld, length: %lld, now: %p, next: %p, before: %p\n", str->this, str->pos, *str->length, str, str->next, str->before);
}

void printData(String *str) {
  String *now;
  for (now = str; now != NULL; now = now->next) {
    printStringData(now);
  }
}

String *newChar(char this, lli pos, lli length, String *before, String *next) {
  String *ret = malloc(sizeof(String));
  
  ret->this = this;
  ret->before = before;
  ret->next = next;
  ret->length = malloc(sizeof(lli));
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

char *toCharArr(String *str) {
  char *ret = malloc(sizeof(char) * (*str->length));

  lli count = 0;

  for(str; str != NULL; str = str->next) {
    ret[count] = str->this;
    count += 1;
  }
  return ret;
}

String *roolBack(String *str) {
  String *ret;
  for (ret = str; ret->before != NULL; ret = ret->before);
  return ret;
}

String *newString(char *str) {
  if (str == NULL) {
    return emptyString();
  } else {
    lli len = (lli)(strlen(str));

    String *now = emptyString();
    String *before = emptyString();

    for(lli i = 0; i < len; i += 1) {
      if (isEmptyString(now)) {
        now = newChar(str[i], i, len, NULL, NULL);
      } else {
        before = now;

        now = newChar(str[i], i, len, NULL, NULL);

        now->before = before;

        before->next = now;        
      }

    }
    return roolBack(now);
  }
}

String *stringCopy(String *str) {
  char *arr = toCharArr(str);
  return newString(arr);
}

lli len(String *str) {
  return *str->length;
}

String *pushChar(String *str, char c) {
  String *now = stringCopy(str);

  for (now; now->next != NULL; now = now->next);

  String *new = newChar(c, now->pos + 1, *now->length, now, NULL);
  now->next = new;

  return roolBack(new);
}

void printString(String *str) {
  printf("%s\n", toCharArr(str));
}

char charAt(String *str, lli index) {
  for (str; str->pos != index; str = str->next);
  return str->this;
}

int charCodeAt(String *str, lli index) {
  char c = charAt(str, index);
  return (int) c;
}

String *concat(String *str1, String *str2) {
  String *ret = stringCopy(str1);
  for (str2; str2 != NULL; str2 = str2->next) {
    ret = pushChar(ret, str2->this);
  }

  return ret;
}

String *toLowerCase(String *str) {
  String *ret = stringCopy(str);
  String *beg = ret;
  for (ret; ret != NULL; ret = ret->next) {
    int ascii = (int)ret->this;

    if (ascii >= 65 && ascii <= 90) {
      ret->this = (char)(ascii + ASCIIFIX);
    }
  }
  return beg;
}

String *toUpperCase(String *str) {
  String *ret = stringCopy(str);
  String *beg = ret;
  for (ret; ret != NULL; ret = ret->next) {
    int ascii = (int)ret->this;

    if (ascii >= 97 && ascii <= 122) {
      ret->this = (char)(ascii - ASCIIFIX);
    }
  }
  return beg;
}


int main(void) {
  String *str1 = newString("TESTE");
  String *str2 = newString("teste2 !!");
  String *str3 = newString("teste3");

  printString(str1);
  printf("%c\n", charAt(str1, 0));
  printString(str1);

}