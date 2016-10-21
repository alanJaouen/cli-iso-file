#!/bin/sh

green='\033[1;32m'
red='\033[1;31m'
ret='\033[0m'
bold='\033[1;1m'

bin='../my_read_iso'
bin2="$1"

print_color()
{
  printf "%b%s%b" "$1" "$2" "$ret"
}

cd tests

$(mkisofs -allow-leading-dots -o iso.iso iso)

for dir in *; do
  [ ! -d "$dir" ] && continue;
  [ "$dir" = "iso" ] && continue;

  print_color "$bold" "========== $dir =========="
  echo

  $($bin iso.iso < "$dir/commands" > "/tmp/ACUmulateur")
  $($bin2 iso.iso < "$dir/commands" > "/tmp/ACUmulateur2")

  if [ "$(diff "/tmp/ACUmulateur2" "/tmp/ACUmulateur")" = "" ]; then
    printf '%s - %s\n' "$(print_color "$green" "OK")" "$dir"
  else
    printf '%s - %s\n' "$(print_color "$red" "KO")" "$dir"
    diff "/tmp/ACUmulateur2" "/tmp/ACUmulateur" -u
  fi

  printf "\n"
done

rm iso.iso
