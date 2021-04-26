#!/bin/bash

C_FILES=$(find -type f \( -name '*.c' ! -name 'main.c' ! -name 'tests.c' \))
H_FILES=$(find -type f \( -name '*.h' ! -name 'tests.h' ! -name 'linalg_obj.h' \))


echo $C_FILES
echo $H_FILES

mkdir -p single-include
echo "#pragma once" > single-include/linalg.hpp
echo "namespace hola {" >> single-include/linalg.hpp

# linalg_obj.h needs to be first
for file in 'linalg_obj.h' $H_FILES $C_FILES; do
    cat $file | sed 's/#pragma once//g' | sed 's/#include ".*"//g' >> single-include/linalg.hpp
done

echo "} // namespace hola" >> single-include/linalg.hpp

sed -i 's/#define OWNS_MEMORY(object) (((struct linalg_obj\*) object)->owns_memory)/#define OWNS_MEMORY(object) (((struct hola::linalg_obj\*) object)->owns_memory)/g' single-include/linalg.hpp

sed -i 's/#define MEMORY_OWNER(object) (((struct linalg_obj\*) object)->memory_owner)/#define MEMORY_OWNER(object) (((struct hola::linalg_obj*) object)->memory_owner)/g' single-include/linalg.hpp

sed -i 's/#define REF_COUNT(object) (((struct linalg_obj\*) object)->ref_count)/#define REF_COUNT(object) (((struct hola::linalg_obj*) object)->ref_count)/g' single-include/linalg.hpp

sed -i 's/#define DATA(object) (((struct linalg_obj\*) object)->data)/#define DATA(object) (((struct hola::linalg_obj*) object)->data)/g' single-include/linalg.hpp

sed -i 's/double norm = vector_norm(v);/[[maybe_unused]] double norm = vector_norm(v);/g' single-include/linalg.hpp
