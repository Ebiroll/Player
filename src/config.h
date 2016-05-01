#pragma once

/*
{"x":176,"y":64,"w":600,"h":400,"name":"video","color":"blue"},
               {"x":0,"y":1,"w":71,"h":57,"name":"logo","color":"grey"},
               {"x":82,"y":0,"w":706,"h":61,"name":"label","color":"cyan"},
               {"x":3,"y":66,"w":166,"h":329,"name":"list","color":"green"},
               {"x":0,"y":405,"w":167,"h":103,"name":"no_smoke","color":"red"},
               {"x":20,"y":522,"w":769,"h":73,"name":"scroll","color":"cyan"}],
 */
#include <string>

typedef  enum {
  video=0,
  logo,
  label,
  datalist,
  no_smoke,
  scroll,
  explode,
  t_clock
} e_rect;

#ifdef DEFINE_NAMES
const char *names[]={
    "video",
    "logo",
    "label",
    "list",
    "no_smoke",
    "scroll",
    "explode",
    "clock",
}
#endif

typedef struct  {
  int x;
  int y;
  int w;
  int h;
  char name[128];
  char color[128];
} rect;

typedef struct  {
  char name[256];
  int  width;
  int  height;
  rect r[32];
} conf;

extern conf config;


void load_current_config();

std::string readFileToString(std::string filename);
