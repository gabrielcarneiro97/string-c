#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define ASCIIFIX 32

typedef long long int lli;
typedef unsigned short int bool;

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

char this(String *str) {
  return str->this;
}

lli len(String *str) {
  return *str->length;
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
  if (isEmptyString(str)) return emptyString();

  String *new = emptyString();

  for(str; str != NULL; str = str->next) {
    char c = this(str);
    if (len(new) == 0) new = newChar(c, 0, 1, NULL, NULL, NULL);
    else {
      String *newC = newChar(c, new->pos + 1, (*new->length) + 1, new->length, new, NULL);
      new->next = newC;
      new = newC;
    }
  }

  return roolBack(new);
}

char *toCharArr(String *str) {
  String *s = copyString(str);
  char *ret = malloc(sizeof(char) * len(s));

  lli l = len(str);

  lli count = 0;

  for(str; str != NULL; str = str->next) {
    ret[count] = this(str);
    count += 1;
  }
  return ret;
}

String *pushChar(String *str, char c) {
  String *now = copyString(str);
  String *new;

  printf("%lld\n", len(now));
  if (isEmptyString(now)) {
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
}

void printString(String *str) {
  printf("%s\n", toCharArr(str));
}

char charAt(String *str, lli index) {
  for (str; str->pos != index; str = str->next);
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
  return roolBack(ret);
}

String *substr(String *str, lli start, lli size) {
  lli end;

  if (size == 0 || start + size > len(str)) end = len(str);
  else end = start + size;

  char *arr = toCharArr(str);

  String *ret = emptyString();

  if (start < 0 || end < 0) return ret;

  for (lli i = start; i < end; i += 1) {
    ret = pushChar(ret, arr[i]);
  }

  free(arr);
  return roolBack(ret);
}

lli indexOf(String *str, String *search) {
  String *s = copyString(str);
  String *src = copyString(search);
  String *srcBeg = src;

  lli pos = -1;
  for (s; s != NULL; s = s->next) {
    if (this(s) == this(src)) {
      if (src->pos == 0) {
        pos = s->pos;
      }
      src = src->next;

      if (src == NULL) return pos;
    } else {
      src = srcBeg;
      pos = -1;
    }
  }

  freeString(s);
  freeString(src);
  return pos;
}

lli lastIndexOf(String *str, String *search) {
  String *s = copyString(str);
  String *src = copyString(search);

  for (src; src->next != NULL; src = src->next);

  String *srcEnd = src;

  lli pos = -1;
  for(s; s->next != NULL; s = s->next);

  for (s; s != NULL; s = s->before) {
    if (this(s) == this(src)) {
      if (src->pos == 0) {
        pos = s->pos;
      }
      src = src->before;

      if (src == NULL) return pos;
    } else {
      src = srcEnd;
      pos = -1;
    }
  }

  freeString(s);
  freeString(src);
  return pos;
}

bool endsWith(String *str, String *search) {
  String *s = copyString(str);
  String *src = copyString(search);

  lli lastId = lastIndexOf(s, src);

  if (lastId == -1) return FALSE;
  if (lastId != len(s) - len(search)) return FALSE;

  return TRUE;
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

String *repeat(String *str, lli count) {
  if (count <= 0) return emptyString();
  else if (count == 1) return str;

  String *ret = emptyString();

  for (lli i = 0; i < count; i += 1) {
    ret = concat(ret, str);
  }

  return ret;
}

bool isEquals(String *str1, String *str2) {
  bool s1E = isEmptyString(str1);
  bool s2E = isEmptyString(str2);

  if (s1E != s2E) return FALSE;
  if (s1E == TRUE && s2E == TRUE) return TRUE;

  String *s1 = copyString(str1);
  String *s2 = copyString(str2);

  for (s1; s1 != NULL; s1 = s1->next) {
    if (this(s1) != this(s2)) return FALSE;
    s2 = s2->next;
  }

  freeString(s1);
  freeString(s2);

  return TRUE;
}

char *_(String *str) {
  return toCharArr(str);
}

int main(void) {
  String *str1 = newString("Apple, Banana, Kiwi");
  String *str2 = newString("Banana");
  String *str3 = slice(str1, indexOf(str1, str2), 0);

  // printData(str3);
  // printString(str3);

  printString(str1);
  printString(str3);

  freeString(str1);
  freeString(str2);
  freeString(str3);

}
