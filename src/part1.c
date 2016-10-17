#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int checkiso(int iso, char **argv, struct iso_prim_voldesc *v)
{
  if (iso == -1)
  {
    printf("%s: %s: No such file directory\n", argv[0], argv[1]);
    return 1;
  }
  struct stat st;
  fstat(iso, &st);
  v = mmap()//struct en param
  if (st.st_size < 2041 || 1)//TODO 2nd cond
  {
    printf("%s: %s: invalid ISO9660 file\n");
    return 1;
  }
  return 0;
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

void interactive(void)
{
  char buff[255] = "";
  while (1)
  {
    printf(">");
    fflush(stdout);
    read(STDIN_FILENO, buff, 255);
    printf("buff = %s\n", buff);
  }
}

int runfile(int iso, int piped, char *buff)
{
  printf("I should run the file\n");
  struct stat st;
  fstat(piped, &st);
  //size_t fsize = st.st_size;
  //char *instr = mmap()
  return 0;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: %s FILE\n", argv[0]);
    return 1;
  if (argc == 2)
  {
    char buff[255] = "";
    int iso = open(argv[1], O_RDONLY);
    if (checkiso(iso, argv))
      return 1;
    if (read(STDIN_FILENO, buff, 255) != 0)
    {
      fflush(stdin);
      printf("piped: %s\n", buff);//return 3;
      int piped = open(buff,  O_RDONLY);
      if (iso && !isatty(STDIN_FILENO))
        runfile(iso, piped, buff);
      else
        interactive();
    }
    else if (iso != -1)
      interactive();
    else
      return 1;
  }
  return 0;
}
