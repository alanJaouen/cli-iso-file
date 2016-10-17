#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int checkiso(char *path);

void show_help(void)
{
  printf("help  : display command help\n");
  printf("info  : display volume info\n");
  printf("ls    : display directory content\n");
  printf("cd    : change current directory\n");
  printf("tree  : display the tree of the current directory\n");
  printf("get   : copy file to local directory\n");
  printf("cat   : display file content\n");
  printf("quit  : program exit\n");
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
  iso += 1;//del
  //size_t fsize = st.st_size;
  //char *instr = mmap()
  return 0;
}

int main(int argc, char **argv)
{
  if (argc != 2)
    return 1;
  if (argc == 2)
  {
    char buff[255] = "";
    int iso = open(argv[1], O_RDONLY);
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
