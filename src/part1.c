#define _POSIX_C_SOURCE 200809L
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
  printf("changing to '' directory\n"); //TODO set name
  struct iso_dir *tmp = *a;
  *a = *b;
  *b = tmp;
}

void exec(char *str, struct iso_prim_voldesc *v, struct iso_dir **d,
   struct iso_dir **oldd)
{
  if (!strcmp(str, "help"))
    show_help();
  else if (!strcmp(str, "info"))
    info(v);
  else if (!strcmp(str, "ls"))
    ls(*d);
  else if (!strcmp(str,  "cd -"))
    swap(d, oldd);
  else if (!strncmp(str, "cd ", 3))
    *d = cd(*d, str + 3, v);
  else if (!strcmp(str, "cd"))
  {
    printf("changing to '/' directory\n");//TODO check that
    *d = move_to_root(v);
  }
  else if (!strcmp(str, "tree"))
    printf("do tree\n");
  else if (!strcmp(str, "get"))
    printf("do get\n");
  else if (!strcmp(str, "cat"))
    printf("do cat\n");
  else
    warnx("my_read_iso: %s: unknown command", str);
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
  struct iso_dir *oldd = d; //TODO test that
  while (1)
  {
    char buff[255] = "";
    printf(">");
    fflush(stdout);
    read(STDIN_FILENO, buff, 255);
    removereturn(buff);
    if (!strcmp(buff, "quit"))
      return;
    exec(buff, v, &d, &oldd);
    *buff = '\0';
  }
}

int runfile(int iso,  char *buff)
{
  printf("I should run the file\n");
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
    int iso = open(argv[1], O_RDONLY);
    struct iso_prim_voldesc *v = NULL;
    if (checkiso(iso, argv, &v))
      return 1;
    if (v == NULL)
      printf("non\n");
    struct stat st;
    fstat(STDIN_FILENO, &st);
    if (st.st_size != 0)
    {
      char buff[st.st_size];
      read(STDIN_FILENO, &buff,  st.st_size);//TODO - des truc
      fflush(stdin);
      printf("size = %zu piped:\n%smyEOF\n",st.st_size,  buff);//return 3;
      if (iso != -1)
        runfile(iso, buff);
      else //TODO TTY
        interactive(v);
    }
    else if (iso != -1 && isatty(STDIN_FILENO))
      interactive(v);
    else
      return 1;
  }
  return 0;
}
