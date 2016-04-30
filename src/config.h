#pragma once

/*
{"x":176,"y":64,"w":600,"h":400,"name":"video","color":"blue"},
               {"x":0,"y":1,"w":71,"h":57,"name":"logo","color":"grey"},
               {"x":82,"y":0,"w":706,"h":61,"name":"label","color":"cyan"},
               {"x":3,"y":66,"w":166,"h":329,"name":"list","color":"green"},
               {"x":0,"y":405,"w":167,"h":103,"name":"no_smoke","color":"red"},
               {"x":20,"y":522,"w":769,"h":73,"name":"scroll","color":"cyan"}],
 */

typedef  enum {
  video=0,
  logo,
  label,
  list,
  no_smoke,
  scroll  
} e_rect;

typedef struct conf {
  char name[256];
  int width;
  int height;
};


