#ifndef AUX_H
#define AUX_H
#include "iso9660.h"

void removereturn(char *s);

void info(struct iso_prim_voldesc *v);

void ls(struct iso_dir *d);

struct iso_dir *cd(struct iso_dir *dir, char *s, struct iso_prim_voldesc *v);

void cat(struct iso_dir *d, char *s, struct iso_prim_voldesc *v);

void tree(struct iso_dir *d, char *prev, struct iso_prim_voldesc *v);

void get(struct iso_dir *d, char *s, struct iso_prim_voldesc *v);

#endif /* !AUX_H */
