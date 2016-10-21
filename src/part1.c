#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <err.h>
#include "iso9660.h"
#include "aux.h"

struct name *n;

void strput(char *dest, char *s)
{
  int i = 0;
  while (s[i])
  {
    dest[i] = s[i];
    ++i;
  }
  dest[i] = '\0';
}

char *clean(char *s)
{
  int i = 0;
  while (s[i] == ' ' || s[i] == '\t')
    i++;
  return s + i;
}

int clean2(char *s)
{
  int si = 0;
  while (s[si])
    si++;
  int i = 0;
  while ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <='Z'))
    i++;
  while (s[i] == ' ' || s[i] == '\t')
    i++;
  return (i == si);
}

void show_help(void)
{
  printf("help\t: display command help\n");
  printf("info\t: display volume info\n");
  printf("ls\t: display directory content\n");
  printf("cd\t: change current directory\n");
  printf("tree\t: display the tree of the current directory\n");
  printf("get\t: copy file to local directory\n");
  printf("cat\t: display file content\n");
  printf("quit\t: program exit\n");
}


struct iso_dir *move_to_root(struct iso_prim_voldesc *v)
{
  void *p = v;
  char *c = p;
  c += (v->root_dir.data_blk.le - ISO_PRIM_VOLDESC_BLOCK) * ISO_BLOCK_SIZE;
  p = c;
  struct iso_dir *d = p;
  return d;
}

void swap(struct iso_dir **a, struct iso_dir **b)
{
  printf("Changing to '%s' directory\n", n->prev);
  struct iso_dir *tmp = *a;
  *a = *b;
  *b = tmp;
}



int checkiso(int iso, char **argv, struct iso_prim_voldesc **v)
{
  if (iso == -1)
  {
    warnx("%s: %s: No such file directory", argv[0], argv[1]);
    return 1;
  }
  struct stat st;
  fstat(iso, &st);
  size_t size = st.st_size;
  *v = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, iso, 16 * ISO_BLOCK_SIZE);
  if (!(*v))
    return 1;
  if (size < sizeof (struct iso_prim_voldesc)
      || strncmp((*v)->std_identifier, "CD001", 5))
  {
    warnx("%s: %s: invalid ISO9660 file size", argv[0], argv[1]);
    return 1;
  }
  return 0;
}

void interactive(struct iso_prim_voldesc *v)
{
  struct iso_dir *d = move_to_root(v);
  struct iso_dir *oldd = d;
  while (42)
  {
    char buff[255] = "";
    printf("> ");
    fflush(stdout);
    read(STDIN_FILENO, buff, 255);
    removereturn(buff);
    if (!strcmp(buff, "quit"))
      return;
    exec(clean(buff), v, &d, &oldd);
    *buff = '\0';
  }
}

int runfile(char *buff, struct iso_prim_voldesc *v)
{
  struct iso_dir *d = move_to_root(v);
  struct iso_dir *oldd = d;
  char cmd[65];
  int i = 0;
  int global = 0;
  while (buff[global])
  {
    cmd[i] = buff[global];
    if (buff[global] == '\n')
    {
      cmd[i] = '\0';
      if (i >= 2)
        exec(clean(cmd), v, &d, &oldd);
      i = 0;
    }
    else ++i;
    ++global;
  }
  return 0;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    warnx("usage: %s FILE", argv[0]);
    return 1;
  }
  if (argc == 2)
  {
    n = mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    n->curr = mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    n->prev = mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    strput(n->curr, "root dir");
    strput(n->prev, "root dir");
    int iso = open(argv[1], O_RDONLY);
    struct iso_prim_voldesc *v = NULL;
    if (checkiso(iso, argv, &v))
      return 1;
    if (v == NULL)
      return 1;
    struct stat st;
    fstat(STDIN_FILENO, &st);
    if (st.st_size != 0)
    {
      char buff[st.st_size + 1];
      buff[st.st_size] = '\0';
      read(STDIN_FILENO, &buff,  st.st_size);//TODO - des truc
      fflush(stdin);
      if (iso != -1)
        runfile(buff, v);
      else if (isatty(STDIN_FILENO))
        interactive(v);
    }
    else if (iso != -1 && isatty(STDIN_FILENO))
      interactive(v);
    else
      return 1;
  }
  return 0;
}
