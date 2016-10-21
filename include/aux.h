#ifndef AUX_H
#define AUX_H
#include "iso9660.h"

struct name
{
  char *curr;
  char *prev;
  int dir;
  int file;
};

extern struct name *n;

void removereturn(char *s);

void info(struct iso_prim_voldesc *v);

void ls(struct iso_dir *d);

struct iso_dir *cd(struct iso_dir *dir, char *s, struct iso_prim_voldesc *v, struct name *n);

void cat(struct iso_dir *d, char *s, struct iso_prim_voldesc *v);

void tree(struct iso_dir *d, char *prev, struct iso_prim_voldesc *v, int f);

void get(struct iso_dir *d, char *s, struct iso_prim_voldesc *v);

void get_name(struct iso_dir *d, char *str);

void strput(char *dest, char *s);

void exec(char *str, struct iso_prim_voldesc *v, struct iso_dir **d,
   struct iso_dir **oldd);

int clean2(char *s);

struct iso_dir *move_to_root(struct iso_prim_voldesc *v);

void swap(struct iso_dir **a, struct iso_dir **b);

void show_help(void);

#endif /* !AUX_H */
