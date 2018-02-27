#ifndef MACRO_H
#define MACRO_H

#define DEAD 0

#define ACTIVATED -1
#define READY 0

inline static int mod(int a, int base){
  return ((a % base) + base) % base;
}


#endif
