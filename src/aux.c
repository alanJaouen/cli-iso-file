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

void removereturn(char *s)
{
  size_t si = 0;
  while (s[si] != '\0')
    ++si;
  if (s[si - 1] == '\n')
    s[si - 1] = '\0';
}

void info(struct iso_prim_voldesc *v)
{
  printf("System Identifier: ");
  for (int i = 0; i < ISO_SYSIDF_LEN; i++)
    printf("%c", v->syidf[i]);
  printf("\nVolume Identifier: ");
  for (int i = 0; i < ISO_VOLIDF_LEN; i++)
    printf("%c", v->vol_idf[i]);
  printf("\nBlock count: %u\n", v->vol_blk_count.le);
  printf("Block size: %d\n", ISO_BLOCK_SIZE);
  printf("Creation date: ");
  for (int i = 0; i < ISO_LDATE_LEN; i++)
    printf("%c", v->date_creat[i]);
  printf("\nApplication Identifier: ");
  for (int i = 0; i < ISO_APP_LEN; i++)
    printf("%c", v->app_idf[i]);
  printf("\n");

}

void get_name(struct iso_dir *d, char *str)
{
  void *p = &(d->idf_len) + 1;
  char *c = p;
  int i = 0;
  if (d->idf_len == 1 && *c == '\0')
    str[i++] = '.';
  else if (d->idf_len == 1 && *c == 1)
  {
    str[i++] = '.';
    str[i++] = '.';
  }
  else
    for ( ; i < d->idf_len; i++)
    {
      if (i == d->idf_len - 2 && c[i] == ';')
        break;
      str[i] = c[i];
    }
  str[i] = '\0';
}

int get_year(char *str)
{
  printf("%d%d\n", str[4], str[5]);
  return str[4] * 10 + str[5];
}

void ls(struct iso_dir *d)
{
  while (d->dir_size > 20)
  {
    char name[255];
    get_name(d, name);
    char isdir = (d->type == ISO_FILE_ISDIR || d->type == 3)? 'd': '-';
    char ish = (d->type == ISO_FILE_HIDDEN || d->type == 3)? 'h': '-';
    printf("%c%c %9u %04d/%02d/%02d %02d:%02d %s\n",isdir, ish,
       d->file_size.le, 1900 + d->date[0], d->date[1], d->date[2], d->date[3],
       d->date[4], name);
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
}


struct iso_dir *cd(struct iso_dir *dir, char *s, struct iso_prim_voldesc *v,
  struct name *n)
{
  struct iso_dir *d = dir;
  while (d->dir_size > 0)
  {
    char name[255];
    get_name(d, name);
    if (!strcmp(name, s))
    {
      if (d->type != ISO_FILE_ISDIR && d->type != 3)
      {
        warnx("entry '%s' is not a directory", s);
        return dir;
      }
      void *p = v;
      char *c = p;
      c += (d->data_blk.le - ISO_PRIM_VOLDESC_BLOCK) * ISO_BLOCK_SIZE;
      p = c;
      if (!strcmp(s, ".."))
      {
        printf("Changing to '%s' directory\n", n->prev);
        strput(n->prev, n->curr);
      }
      else if (!strcmp(s, "."))
        printf("Changing to '%s' directory\n", n->curr);
      else
      {
        printf("Changing to '%s' directory\n", s);
        strput(n->prev, n->curr);
        strput(n->curr, s);
      }
      return p;
    }
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
  warnx("unable to find '%s' directory entry", s);
  return dir;
}

struct iso_dir *cd2(struct iso_dir *dir, char *s, struct iso_prim_voldesc *v,
  struct name *n)
{
  struct iso_dir *d = dir;
  while (d->dir_size > 0)
  {
    char name[255];
    get_name(d, name);
    if (!strcmp(name, s))
    {
      void *p = v;
      char *c = p;
      c += (d->data_blk.le - ISO_PRIM_VOLDESC_BLOCK) * ISO_BLOCK_SIZE;
      p = c;
      strput(n->prev, n->curr);
      strput(n->curr, s);
      return p;
    }
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
  warnx("unable to find '%s' directory entry", s);
  return dir;
}

void strappend(char *dest, char *src)
{
  int i = 0;
  while (dest[i])
    ++i;
  int j = 0;
  while (src[j])
  {
    dest[i + j] = src[j];
    j++;
  }
  dest[i + j] = '\0';
}

void tree2(struct iso_dir *d, char *prev, struct iso_prim_voldesc *v, int f)
{
  char name[255];
  get_name(d, name);
  if ((d->type == ISO_FILE_ISDIR || d->type == 3))
  {
    printf("/\n");
    char nprev[255] = "";
    strappend(nprev, prev);
    if (f)
      strappend(nprev, " ");
    strappend(nprev, "   ");
    n->dir++;
    tree(cd2(d, name, v, n), nprev, v, 1);
  }
  else
  {
    printf("\n");
    n->file++;
  }
}

void tree(struct iso_dir *d, char *prev, struct iso_prim_voldesc *v, int f)
{
  if (!f)
    printf(".\n");
  char name[255];
  while (d->dir_size > 20)
  {
    get_name(d, name);
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    struct iso_dir *d2 = p;
    if (strcmp(name, ".") && strcmp(name, ".."))
    {
      if (f)
        printf("|%s", prev);
      if (d2->dir_size > 20)
        printf("|-- %s", name);
      else
        printf("+-- %s", name);
      tree2(d, prev, v, f);
    }
    d = d2;
  }
}


