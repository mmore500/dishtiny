#!/bin/bash

################################################################################
echo
echo "running gitget.sh"
echo "-------------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" != 3 )); then
  echo "USAGE: [url] [slug] [revision]"
  exit 1
fi

url="${1}"
echo "url ${url}"
shift

slug="${1}"
echo "slug ${slug}"
shift

revision="${1}"
echo "revision ${revision}"
shift

command -v module >/dev/null && module load git/2.27.0 && module list || :
git --version

################################################################################
echo
echo "running gitget.sh"
echo "-------------------------------"
################################################################################

test -f ~/.cache/gitget/"${revision}.tar.gz" \
  && echo "found cached assets, unzipping" \
  && tar -xf ~/.cache/gitget/"${revision}.tar.gz" \
  && echo "assets unzipped successfully" \
  && exit 0

echo "no cached assets found continuing"

################################################################################
echo
echo "getting assets"
echo "-------------------------------"
################################################################################

echo "setting up pinned project source at revision ${revision}..."
time for retry in {1..20}; do

  rm -rf "${slug}" \
  && mkdir "${slug}" \
  && git -C "${slug}" init \
  && git -C "${slug}" remote add origin "${url}" \
  && git -C "${slug}" fetch --quiet --depth 1 origin "${revision}" \
  && git -C "${slug}" checkout FETCH_HEAD \
  && git -C "${slug}" submodule update --quiet --init --recursive --depth 1 --jobs 16 \
  && echo "  source setup success" \
  && break \
  || (echo "source setup failure (${retry})" && sleep $((RANDOM % 10)))

  if ((${retry}==20)); then echo "source setup fail" && exit 2; fi
done

################################################################################
echo
echo "preparing cache"
echo "-------------------------------"
################################################################################

mkdir -p ~/.cache
mkdir -p ~/.cache/gitget

tempfile="$(mktemp)"
echo "tempfile ${tempfile}"

tar -czf "${tempfile}" "${slug}"
# use mv instead of copy, should be atomic-ish
mv "${tempfile}" ~/.cache/gitget/"${revision}.tar.gz"

################################################################################
echo
echo "done"
echo "-------------------------------"
################################################################################
