//===========================================================
// utils.h
//-----------------------------------------------------------
// copyright by William McCarthy
//===========================================================

#ifndef __utils_h__
#define __utils_h__


class Utils {
public:
  static char* strconvert(char* s, int (*fp)(int)) {   // strconvert(s, toupper);   // strconvert(s, tolower);
    char* p = s;
    while (*p != '\0') { *p = fp(*p);  ++p; }
    return s;
  }

  static char* strstrip(char* s) {   // "1, 2, 3, 4"     ==>  // "1 2 3 4"
    char* p = s;
    while (*p != '\0') {
      if ((isalnum(*p) || isspace(*p)) && !ispunct(*p)) { *s++ = *p++; }
      else { ++p; }
    }
    *s = '\0';
    return s;
  }
  
  static const char* yes_or_no(bool condition) { return (condition? "YES" : "no"); }
};

#endif

