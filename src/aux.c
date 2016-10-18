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

void dispay_name(struct iso_dir *d)
{
  uint8_t *a = &(d->idf_len) + 1;
  void *p = a;
  char *c = p;
  printf("s=%u\n", d->idf_len);
  for (uint32_t i = 0; i < d->idf_len; ++i)
  {
    printf("%c\n", c[i]);
  }
}

void ls(struct iso_dir *d, struct iso_prim_voldesc *v)
{
  void *p = v;
  char *c = p;
  c = c + ISO_BLOCK_SIZE * d->data_blk.le;
  p = c;
  struct iso_path_table_le *i = p;
  p = i->data_blk;
  struct iso_dir *d =

  printf("%c\n", c);
}
