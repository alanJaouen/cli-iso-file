#!/bin/sh

green='\033[1;32m'
red='\033[1;31m'
ret='\033[0m'
bold='\033[1;1m'

bin='../my_read_iso'

print_color()
{
  printf "%b%s%b" "$1" "$2" "$ret"
}

cd tests

nb=0
fa=0

for dir in *; do
  [ ! -d "$dir" ] && continue;
  [ "$dir" = "iso" ] && continue;

  print_color "$bold" "========== $dir =========="
  echo


  ((nb=$nb + 1))
  $($bin iso.iso < "$dir/commands" > "/tmp/ACUmulateur")

  if [ "$(diff "$dir/res" "/tmp/ACUmulateur")" == "" ]; then
    printf '%s - %s\n' "$(print_color "$green" "OK")" "$dir"
  else
    ((fa=$fa + 1))
    printf '%s - %s\n' "$(print_color "$red" "KO")" "$dir"
    diff "/tmp/ACUmulateur" "$dir/res" -u
  fi

  printf "\n"
done

if [ $nb -ne 0 ]; then
  printf '%s tests failed on %s tests done\n' "$(print_color "$red" "$fa")" \
    "$(print_color "$green" "$nb")"
fi

rm "$name1" "$name2"
