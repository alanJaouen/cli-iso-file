#include <stddef.h>
#include <string.h>

void removereturn(char *s)
{
  size_t si = 0;
  while (s[si] != '\0')
    ++si;
  if (s[si - 1] == '\n')
    s[si - 1] = '\0';
}
