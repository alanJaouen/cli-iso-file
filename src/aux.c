#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "iso9660.h"

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
  uint8_t pad = 0;
  int i = 0;
  if (d->type != ISO_FILE_ISDIR)
    pad = 2;
  if (d->idf_len == 1 && *c == '\0')
    str[i++] = '.';
  else if (d->idf_len == 1 && *c == 1)
  {
    str[i++] = '.';
    str[i++] = '.';
  }
  else
    for ( ; i < d->idf_len - pad; i++)
      str[i] = c[i];
  str[i] = '\0';
}

void ls(struct iso_dir *d)
{
  while (d->dir_size > 0)
  {
    char name[255];
    get_name(d, name);
    printf("%s\n", name);
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
}

struct iso_dir *cd(struct iso_dir *dir, char *s, struct iso_prim_voldesc *v)
{
  struct iso_dir *d = dir;
  while (d->dir_size > 0)
  {
    if (strlen(s) == d->idf_len)
    {
      void *p = &(d->idf_len) + 1;
      char *c = p;
      if (!strncmp(c, s, d->idf_len))
      {
        printf("FIND\n");
        p = v;
        c = p;
        printf("on jump a %u\n", d->data_blk.le);
        c += (d->data_blk.le - 16) * 2048;
        p = c;
        return p;
      }
    }
    void *p = d;
    char *c = p;
    c = c + d->dir_size;
    p = c;
    d = p;
  }
  printf("non\n");

  return dir;

}
