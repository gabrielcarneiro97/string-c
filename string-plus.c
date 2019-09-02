#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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

String *newChar(char this, lli pos, lli length, lli *lengthPointer, String *before, String *next) {
  String *ret = malloc(sizeof(String));

  ret->this = this;
  ret->before = before;
  ret->next = next;
  ret->pos = pos;

  if (lengthPointer != NULL) ret->length = lengthPointer;
  else ret->length = malloc(sizeof(lli));

  *ret->length = length;

  return ret;
}

String *emptyString() {
  return newChar('\0', 0, 0, NULL, NULL, NULL);
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
        now = newChar(str[i], i, len, NULL, NULL, NULL);
      } else {
        before = now;

        now = newChar(str[i], i, len, before->length, NULL, NULL);

        now->before = before;

        before->next = now;
      }

    }
    return roolBack(now);
  }
}

String *copyString(String *str) {
  char *arr = toCharArr(str);
  return newString(arr);
}

lli len(String *str) {
  return *str->length;
}

String *pushChar(String *str, char c) {
  String *now = copyString(str);
  String *new;

  if (len(now) == 0) {
    new = newChar(c, 0, 1, NULL, NULL, NULL);
  } else {
    for (now; now->next != NULL; now = now->next);

    new = newChar(c, now->pos + 1, (*now->length) + 1, now->length, now, NULL);
    now->next = new;
  }

  return roolBack(new);
}

String *popChar(String *str, char *poped) {
  String *now = copyString(str);

  for (now; now->next->next != NULL; now = now->next);

  if (poped != NULL) *poped = now->next->this;

  free(now->next);
  now->next = NULL;

  *now->length -= 1;

  return roolBack(now);
}

void freeString(String *str) {
  String *temp;
  for(str; str != NULL; str = temp) {
    temp = str->next;

    if (temp == NULL) {
      str->length = NULL;
      free(str->length);
    }

    free(str);
  }
  free(temp);
}

void printString(String *str) {
  printf("%s\n", toCharArr(str));
}

char charAt(String *str, lli index) {
  for (str; str->pos != index; str = str->next);
  return str->this;
}

char this(String *str) {
  return str->this;
}

lli pos(String *str) {
  return str->pos;
}

int charCodeAt(String *str, lli index) {
  char c = charAt(str, index);
  return (int) c;
}

String *concat(String *str1, String *str2) {
  String *ret = copyString(str1);
  for (str2; str2 != NULL; str2 = str2->next) {
    ret = pushChar(ret, str2->this);
  }

  return ret;
}

String *toLowerCase(String *str) {
  String *ret = copyString(str);
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
  String *ret = copyString(str);
  String *beg = ret;
  for (ret; ret != NULL; ret = ret->next) {
    int ascii = (int)ret->this;

    if (ascii >= 97 && ascii <= 122) {
      ret->this = (char)(ascii - ASCIIFIX);
    }
  }

  return beg;
}

String *slice(String *str, lli start, lli end) {
  if (end == 0 || end > len(str)) end = len(str);

  char *arr = toCharArr(str);

  String *ret = emptyString();

  if (start < 0 || end < 0) return ret;

  for (lli i = start; i < end; i += 1) {
    ret = pushChar(ret, arr[i]);
  }

  free(arr);
  return ret;
}

String *substr(String *str, lli start, lli size) {
  lli end;

  if (size == 0 || start + size > len(str)) end = len(str);
  else end = start + size;

  char *arr = toCharArr(str);

  String *ret = emptyString();

  if (start < 0 || size < 0) return ret;

  for (lli i = start; i < end; i += 1) {
    ret = pushChar(ret, arr[i]);
  }

  free(arr);
  return ret;
}

lli indexOf(String *str, String *search) {
  String *s = copyString(str);
  String *searchBeg = search;

  lli counter = 0;
  lli pos = -1;
  for (s; s != NULL; s = s->next) {
    if (this(s) == this(search)) {
      if (search->pos == 0) {
        pos = s->pos;
      }
      search = search->next;
      counter += 1;

      if (search == NULL) return pos;
    } else {
      search = searchBeg;
      counter = 0;
      pos = -1;
    }
  }

  freeString(s);
  return pos;
}

String *fixPos(String *str) {
  String *in = copyString(str);
  String *beg = in;

  for (in; in->before != NULL; in = in->before);

  lli count = 0;
  for(in; in != NULL; in = in->next) {
    in->pos = count;
    count += 1;
  }

  return beg;
}

String *removeChar(String *str, lli index) {
  if (index >= len(str)) return str;

  String *in = copyString(str);
  String *beg = in;

  for (in; pos(in) != index; in = in->next);

  if (in->before == NULL) {
    beg = in->next;
    beg->before = NULL;

    free(in);

    *beg->length -= 1;

    return fixPos(beg);
  } else if (in->next == NULL) {
    return popChar(beg, NULL);
  } else {
    in->before->next = in->next;

    free(in);

    *beg->length -= 1;

    return fixPos(beg);
  }
}

String *trim(String *str) {
  String *in = copyString(str);
  String *beg = in;

  int nsBeg = 0, nsEnd = 0;

  for (in; in->before != NULL; in = in->before);

  while(nsBeg == 0) {
    if (isspace(this(in))) {
      in = removeChar(in, in->pos);
      beg = in;
    } else {
      nsBeg = 1;
    }
  }

  for (in; in->next != NULL; in = in->next);

  while(nsEnd == 0) {
    if (isspace(this(in))) {
      in = in->before;
      *in->length -= 1;

      in->next = NULL;
      free(in->next);
    } else {
      nsEnd = 1;
    }
  }

  return beg;
}

char *_(String *str) {
  return toCharArr(str);
}

int main(void) {
  String *str1 = newString("      12345         ");
  String *str2 = newString("teste2 !!");
  String *str3 = newString("teste3");

  printData(trim(str1));

  freeString(str1);
  freeString(str2);
  freeString(str3);

}
