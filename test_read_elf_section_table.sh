#!/bin/sh

#On vérifie qu'on dispose d'un fichier pour le test
if [ $# -eq 0 ]
then
    echo "Missing argument !"
    echo "Usage: test_read_elf_header <File>"
    exit 1
fi

#On vérifie que le fichier existe
if [ ! -f $1 ]
then
    echo "No such file !"
    exit 1
fi

# Dans ce script, on appelle la commande built-in readelf "Readelf"
# Le programme écrit dans le cadre du projet s'appelle "MyReadelf"

#On exécute les deux commandes à comparer et on écrit la sortie standard sur des fichiers temporaires
#On vérifie aussi les codes d'erreurs
readelf -S $1 &> readelfCommand.output
errorReadelf=$?
./read_elf_section_table $1 &> MyReadelfCommand.output
errorMyReadelf=$?

# readelf renvoie 1 si le fichier n'est pas un fichier ELF, même comportement pour notre programme
# On vérfie que les codes d'erreurs correspondent
if [ $errorReadelf -eq 1 ]
then
    if [ $errorMyReadelf -eq 1 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Error code : \033[48;5;1m===FAIL TEST===\033[0;0m" #Fail
        exit 1
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Error code : \033[48;5;1m===FAIL TEST===\033[0;0m" #Fail
        exit 1
    fi
fi
echo

cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do
    #On prend un système clé -> valeur, par exemple: [ 1] -> .text          PROGBITS ...
    #On retire les espaces avant/après le texte et les Tabs

    key=`echo $line | cut -d] -f1`
    key="${key}]"

    value=`echo $line | awk -F] '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}'`
    
    
    echo "Key : $key"
    echo "Value : $value"

    case $key in
    #\[[[:space:]*[0-9]*\])
    
    \[[[:space:]*[0-9]*\]) #Regex permettant de reconnaitre [  0] ... [ 10] ... [100]
        otherValue=`grep -F "  $key" readelfCommand.output | cut -d] -f2 `
        echo "otherValue : $otherValue"

        #On commence par vérifier le nom :

        name=`echo $value | cut -d' ' -f1`
        echo "name : $name"
        otherName=`echo $otherValue | cut -d' ' -f1`
        echo "otherName : $otherName"

        echo
    esac
done