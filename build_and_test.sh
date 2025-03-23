#!/bin/sh
# Requires gcc, g++ and valgrind to be installed.
help='
./mk.sh [-drmh] [argument...]
 -d Compile a debug build. (release build by default)
 -r Run the compiled application.
 -m Perform a memory check. (implies -r)
 -h Print the help message and quit. (further options ignored)
 argument... Arguments passed to the application. (applies to -r or -m)
'
# Adjust settings between the dashed lines. Separate items with newlines.
# -----------------------
# file name of the final program
app_name=cstring
# if not empty, default to -d
do_dbgbld=1
# if not empty, default to -r
do_run=1
# if not empty, default to -m
do_memchk=1
# default arguments (arguments from command line are added)
app_args='
'
# flags for gcc
c_flgs='
-std=gnu90
'
# flags for g++
cpp_flgs='
-std=c++23
'
# flags shared with gcc and g++
cmn_flgs='
-Wall
-Wextra
'
# flags for debug builds only
dbg_flgs='
-Og
-g
'
# flags for release builds only
rel_flgs='
-DNDEBUG
-O2
-flto
-s
'
# -----------------------
# write the number of seconds (incl. fractions) that have passed since the start of the UTC day
tm() {
  echo | LC_ALL=C TZ=UTC0 diff -u /dev/null - | awk -F [[:space:]:] 'NR==2 {printf "%f\n", 3600*($4.)+60*($5.)+$6}'
}
# use the command in $1 to compile each file in list $2, save compiled objects in folder $3, write the list of objects
cmp() {
  for src in $2; do
    obj=$3/${src#*/}.o
    mkdir -p "$(dirname "$obj")"
    (set -xv; $1 -c "$src" -o "$obj") || exit 1
    echo "$obj"
  done
}
# prepare
start=$(tm)
n='
'
IFS=$n
while getopts drmh opt; do case $opt in
  d) do_dbgbld=1;; r) do_run=1;; m) do_memchk=1;; *) echo "$help"; exit 1;;
esac done
shift $((OPTIND - 1))
[ -n "$do_dbgbld" ] && bld=debug bldflgs=$dbg_flgs || bld=release bldflgs=$rel_flgs
appdir=./bin/$bld
app=$appdir/$app_name
(
  # compile
  ccmp=gcc$n$c_flgs$n$bldflgs$n$cmn_flgs
  cppcmp=g++$n$cpp_flgs$n$bldflgs$n$cmn_flgs
  cpplist=$(find . -type f -name '*.cpp')
  objlist=$(
    cmp "$ccmp" "$(find . -type f -name '*.c')" "./obj/$bld"
    cmp "$cppcmp" "$cpplist" "./obj/$bld"
  ) || exit 1
  # link
  [ -n "$cpplist" ] && link=$cppcmp || link=$ccmp
  link=$link$n-o$n$app$n$objlist
  mkdir -p "$appdir"
  (set -xv; $link) || exit 1
  # print information
  span=$(echo "$start $(tm)" | awk '{s=$2-$1; if (s<0) s+=86400; m=int(s/60); printf "%dm%.3fs", m, s-m*60}')
  echo "$n $span, $(wc -c <"$app" | awk '{printf "%.1fKB", $1/1024}')$n" >&2
) && [ -n "$do_run$do_memchk" ] && (
  # run w/ or w/o memory check
  run=$app$n$app_args$n$*
  [ -n "$do_memchk" ] && run=valgrind$n-s$n--leak-check=full$n--track-origins=yes$n$run
  (set -xv; $run)
  echo "$n '$app' returned: $?$n" >&2
)
