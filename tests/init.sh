#!/bin/sh

green='\033[1;32m'
red='\033[1;31m'
ret='\033[0m'
bold='\033[1;1m'

bin='my_read_iso'

name1="../ac_read_iso"

print_color()
{
  printf "%b%s%b" "$1" "$2" "$ret"
}

cd tests

$(mkisofs -allow-leading-dots -o iso.iso iso)

nb=0
fa=0

for dir in *; do
  [ ! -d "$dir" ] && continue;
  [ "$dir" = "iso" ] && continue;

  print_color "$bold" "========== $dir =========="
  echo ""

  ((nb=$nb + 1))
  $($name1 iso.iso < "$dir/commands" > "$dir/res")

done

rm iso.iso
