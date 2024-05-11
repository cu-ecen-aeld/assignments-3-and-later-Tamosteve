#!/bin/bash

# Tester script for assignment 1 and assignment 2
# Author: Steve Tamo

if [ "$#" -ne 2 ];then
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

#recuperer les fichiers
writefile="$1"
writestr="$2"

# Vérifier si le premier argument est un chemin valide
if [ -z "$writefile" ]; then
    echo "Erreur : the directory file do not exist"
    exit 1
fi

mkdir -p "$(dirname "$writefile")"

echo "$writestr" > "$writefile"

cat "$writefile"

exit 0