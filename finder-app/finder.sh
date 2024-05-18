#!/bin/sh

# Vérifier si le nombre d'arguments est correct
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi
filesdir="$1"
searchstr="$2"

if [ ! -d "$filesdir" ]; then  
    echo "la direction donnée $filesdir existe pas"
    exit 1
fi


# # Initialiser les compteurs
num_files=0
num_matching_lines=0

list_file=$(find "$filesdir" -type f)

# Parcourir chaque fichier et compter les lignes correspondantes
while read -r file; do
    matching_lines=$(grep -c "$searchstr" "$file")
    ((num_matching_lines += matching_lines))
    ((num_files++))
done <<< "$list_file"

echo "The number of files are $num_files and the number of matching lines are $num_matching_lines"


