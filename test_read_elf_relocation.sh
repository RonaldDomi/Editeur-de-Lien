#!/bin/bash
# Même remarque que pour test_read_elf_header.sh on utilise aussi Shell

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
FailTest() {
    echo -e "\033[41;1;1m===FAIL TEST===\033[0;0m"
    echo "  • readelf is :\"$1\""
    echo "  • MyReadElf is : \"$2\""
    #rm -f MyReadelfCommand.output readelfCommand.output
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
    echo "Usage: test_read_elf_symbol_table <File>"
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
readelf -r $1 &>readelfCommand.output
errorReadelf=$?
./readelf_relocation $1 &>MyReadelfCommand.output
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

endOfCommand=0
regex2Blanks="/[[:blank:]]{2,}/g" #Match tous les deux whitespaces ou plus e.g: "  ", "     ", ...
regexScope="^relocation"
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do

    #On prend un système clé -> valeur, par exemple: 0: -> 00 00 00 00 	0	LOCAL	NO TYPE		0	0
    #On retire les espaces avant/après le texte et les Tabs

    #Cas particulier si il n'y a aucune relocation
    noReloc="There are no relocations in this file."
    if [ "$line" == "$noReloc" ]
    then
        otherValue=`grep "^There are no relocations" readelfCommand.output`

        if [ "$line" == "$otherValue" ]
        then
            echo -e "No relocations : \033[48;5;2mOK TEST\033[0;0m" #Pass
            echo
            echo -e "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
            rm -f MyReadelfCommand.output readelfCommand.output
            exit 0
        else
            printf "No relocations"
            FailTest "$otherValue" "$line" "$$"
        fi
    fi

    #On doit déterminer le scope 
    if [[ "$line" =~ $regexScope ]]
    then
        scopeKey=1
    else
        scopeKey=0
    fi

    if [[ scopeKey -eq 1 ]]
    then
        scopeName=`echo $line | cut -d\' -f2 | awk '{gsub(/^[ \t]+|[ \t]+$/, "", $1) ; print $1}'`
        otherScopeName=`grep "'$scopeName'" readelfCommand.output | cut -d\' -f2 | awk '{gsub(/^[ \t]+|[ \t]+$/, "", $1) ; print $1}'`

        if [ "$scopeName" == "$otherScopeName" ]
        then
            echo -e "Scope : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Scope : "
            FailTest "$otherScopeName" "$scopeName" "$$"
        fi
    fi

    key=`echo $line | awk '{gsub(/$regex2Blanks/, "", $0) ; print $0}' | cut -c -18`
    key=`echo $key | sed '{s/ /  /;}'`

    case $key in
        [0-9]*)
            value=`echo $line | awk '{gsub(/$regex2Blanks/, "", $0) ; print $0}'`
            otherValue=`grep "^$key" readelfCommand.output | awk '{gsub(/$regex2Blanks/, "", $0) ; print $0}'`
            echo "Key : $key"
            echo "Value : $value"
            echo "OtherValue : <$otherValue>"
            if [ "$value" != "$otherValue" ]
            then
                printf "Relocation : "
                FailTest "$otherValue" "$value" "$$"
            fi
        ;;
    esac
done

echo
# Si tous les tests ont réussi, alors le test passe, on supprime les fichiers temporaires
echo -e "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
rm -f MyReadelfCommand.output readelfCommand.output
exit 0
