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

void print_name(struct iso_dir *d)
{
  void *p = &(d->idf_len) + 1;
  char *c = p;
  uint8_t pad = 0;
  if (d->type != ISO_FILE_ISDIR)
    pad = 2;
  if (d->idf_len == 1 && *c == '\0')
    printf(".");
  else if (d->idf_len == 1 && *c == 1)
    printf("..");
  else
    for (int i = 0; i < d->idf_len - pad; i++)
      printf("%c", c[i]);
  printf("\n");
}

void ls(struct iso_dir *d, struct iso_prim_voldesc *v)
{
  struct iso_dir *dir = d;
  while (dir->dir_size > 0)
  {
    print_name(dir);
    p = dir;
    c = p;
    c = c + dir->dir_size;
    p = c;
    dir = p;
  }
}
