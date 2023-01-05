#!/bin/bash
# Même remarque que pour test_read_elf_header.sh on utilise aussi Shell

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
FailTest() {
    echo -e "\033[41;1;1m===FAIL TEST===\033[0;0m"
    echo "  • readelf is :\"$1\""
    echo "  • MyReadElf is : \"$2\""
    rm -f MyReadelfCommand.output readelfCommand.output
    #TOFIX: Comme le while utilise un subprocess, on peut kill le processus, mais on aura pas de valeur de retour
    #Il faut essayer d'utiliser zsh mais on peut avoir comme erreur "readelf command not found"...
    kill -KILL $3
    exit 1
}

# Cette fonction prend en entrée deux textes censés être différents et avertit l'utilisateur
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
WarningTest() {
    echo -e "\033[43;1;1m===WARNING TEST===\033[0;0m"
    echo "  • readelf is :\"$1\""
    echo "  • MyReadElf is : \"$2\""
}

#On vérifie qu'on dispose d'un fichier pour le test
if [ $# -lt 1 ]; then
    echo "Missing file !"
    echo "Usage: test_read_elf_section_table <File>"
    exit 1
fi

#On vérifie que le fichier existe
if [ ! -f $1 ]; then
    echo "No such file !"
    exit 1
fi

# Dans ce script, on appelle la commande built-in readelf "Readelf"
# Le programme écrit dans le cadre du projet s'appelle "MyReadelf"

#On exécute les deux commandes à comparer et on écrit la sortie standard sur des fichiers temporaires
#On vérifie aussi les codes d'erreurs
readelf -s $1 &>readelfCommand.output
errorReadelf=$?
./readelf_symbol_table $1 &>MyReadelfCommand.output
errorMyReadelf=$?

# readelf renvoie 1 si le fichier n'est pas un fichier ELF, même comportement pour notre programme
# On vérfie que les codes d'erreurs correspondent
if [ $errorReadelf -eq 1 ]; then
    if [ $errorMyReadelf -eq 1 ]; then
        echo -e "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
        echo
        echo -e "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
        rm -f MyReadelfCommand.output readelfCommand.output
        exit 0
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf"
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]; then
        echo -e "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf"
    fi
fi
echo

sectionNumber=0
regexNumberDot="[0-9]+:"
regex2Blanks="/[[:blank:]]{2,}/g"
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do

    #On prend un système clé -> valeur, par exemple: 0: -> 00 00 00 00 	0	LOCAL	NO TYPE		0	0
    #On retire les espaces avant/après le texte et les Tabs

    key=`echo $line | cut -d: -f1 | awk '{gsub(/^[ \t]+|[ \t]+$/, "", $1) ; print $1}'`
    key="${key}:"
    value=`echo $line` #| awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}'`



    case $key in

        [0-9]*:)
            echo "Key : <$key>"
            echo "Value : $value"


            otherValue=`grep "^     $key" readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}'`
            otherValue=`echo $otherValue | awk '{gsub(/$regex2Blanks/, "", $0) ; print $0}'`
            echo "Other Value : $otherValue"
        ;;
		##########TEMP DO DELETE AFTER NAME FIX##########
			symbolName=`echo $otherValue | cut -d
        if [ "$value" == "$otherValue" ]
		then

		fi
		#################################################
    esac

done
