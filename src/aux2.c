#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "iso9660.h"
#include "aux.h"


struct iso_dir *get_file(struct iso_dir *d, char *s)
{
  while (d->dir_size > 20)
  {
    char name[255];
    get_name(d, name);
    if (!strcmp(name, s))
      return d;
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
  return NULL;
}

void cat(struct iso_dir *d, char *s, struct iso_prim_voldesc *v)
{
  struct iso_dir *f = get_file(d, s);
  if (!f)
  {
    warnx("file '%s' doesn't exist", s); //TODO check
    return;
  }
  if (f->type != 0 && f->type != 1)
  {
    warnx("file '%s' is not a file", s); //TODO check
    return;
  }
  void *p = v;
  char *c = p;
  c += (f->data_blk.le - ISO_PRIM_VOLDESC_BLOCK) * ISO_BLOCK_SIZE;
  for (uint32_t i = 0; i < f->file_size.le; ++i)
    printf("%c", c[i]);
}

void get(struct iso_dir *d, char *s, struct iso_prim_voldesc *v)
{
  struct iso_dir *f = get_file(d, s);
  if (!f)
  {
    warnx("unable to find '%s' entry", s);
    return;
  }
  if (f->type != 0 && f->type != 1)
  {
    warnx("entry '%s' is a directory", s);
    return;
  }
  int fd = open(s, O_CREAT|O_TRUNC|O_WRONLY, 0666);
  void *p = v;
  char *c = p;
  c += (f->data_blk.le - ISO_PRIM_VOLDESC_BLOCK) * ISO_BLOCK_SIZE;
  write(fd, c, f->file_size.le);
}

void exec(char *str, struct iso_prim_voldesc *v, struct iso_dir **d,
   struct iso_dir **oldd)
{
  if (!strncmp(str, "help", 4) && clean2(str))
    show_help();
  else if (!strcmp(str, "info") && clean2(str))
    info(v);
  else if (!strcmp(str, "ls") && clean2(str))
    ls(*d);
  else if (!strcmp(str,  "cd -"))
    swap(d, oldd);
  else if (!strncmp(str, "cd ", 3))
    *d = cd(*d, str + 3, v, n);
  else if (!strcmp(str, "cd\0"))
  {
    printf("Changing to 'root dir' directory\n");
    *d = move_to_root(v);
    char s[200];
    strput(s, n->curr);
    strput(n->curr, n->prev);
    strput(n->prev,s);
  }
  else if (!strcmp(str, "tree"))
    tree(*d, "", v);
  else if (!strncmp(str, "get ", 4))
    get(*d, str + 4, v);
  else if (!strncmp(str, "cat ", 4))
    cat(*d, str + 4, v);
  else
    warnx("my_read_iso: %s: unknown command", str);
}

void exec2(char *str, struct iso_prim_voldesc *v, struct iso_dir **d,
   struct iso_dir **oldd)
{
  if (!strncmp(str, "help", 4) && clean2(str))
    show_help();
  else if (!strcmp(str, "info") && clean2(str))
    info(v);
  else if (!strcmp(str, "ls") && clean2(str))
    ls(*d);
  else if (!strcmp(str,  "cd -"))
    swap(d, oldd);
  else if (!strncmp(str, "cd ", 3))
    *d = cd(*d, str + 3, v, n);
  else if (!strcmp(str, "cd\0"))
  {
    printf("C:hanging to 'root dir' directory\n");
    *d = move_to_root(v);
  }
  else if (!strcmp(str, "tree"))
    tree(*d, "", v);
  else if (!strncmp(str, "get ", 4))
    get(*d, str + 4, v);
  else if (!strncmp(str, "cat ", 4))
    cat(*d, str + 4, v);
  else
    warnx("my_read_iso: %s: unknown command", str);
}