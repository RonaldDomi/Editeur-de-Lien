#!/bin/sh
# Même remarque que pour test_read_elf_header.sh on utilise aussi Shell

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
function FailTest {
    echo "\033[41;1;1m===FAIL TEST===\033[0;0m"
    echo "  • readelf is :\"$1\""
    echo "  • MyReadElf is : \"$2\""
    rm -f MyReadelfCommand.output readelfCommand.output
    #TOFIX: Comme le while utilise un subprocess, on peut kill le processus, mais on aura pas de valeur de retour
    #Il faut essayer d'utiliser zsh mais on peut avoir comme erreur "readelf command not found"...
    kill -KILL $3
    exit 1
}

#On vérifie qu'on dispose d'un fichier pour le test
if [ $# -eq 0 ]
then
    echo "Missing file !"
    echo "Usage: test_read_elf_header <File>"
    exit 1
fi

#On vérifie que le fichier existe
if [ ! -f $1 ]
then
    echo "No such file !"
    exit 1
fi


#####TEMP######
regexNumber="^[0-9]" # Match tous les chiffres en base 10
if [[ ! "$2" =~ $regexNumber ]]
then
    echo "Les recherches par nom sont pour l'instant impossibles dans le programme "
    echo "On ne peut pas tester les recherches par nom pour le momemnt "
    exit 1
fi
###############


# Dans ce script, on appelle la commande built-in readelf "Readelf"
# Le programme écrit dans le cadre du projet s'appelle "MyReadelf"

#On exécute les deux commandes à comparer et on écrit la sortie standard sur des fichiers temporaires
#On vérifie aussi les codes d'erreurs
readelf -x $2 $1 &> readelfCommand.output
errorReadelf=$?
./read_elf_section_content $1 $2 &> MyReadelfCommand.output
errorMyReadelf=$?

# readelf renvoie 1 si le fichier n'est pas un fichier ELF, même comportement pour notre programme
# On vérfie que les codes d'erreurs correspondent
if [ $errorReadelf -eq 1 ]
then
    if [ $errorMyReadelf -eq 1 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
        echo
        echo "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
        rm -f MyReadelfCommand.output readelfCommand.output
        exit 0
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf" "$$"
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf" "$$"
    fi
fi

errors=0
regexBegin="^0x"
regexHex="[0-9]*[a-f]*"
regex2Blanks="/[[:blank:]]{2,}/g"
echo
echo "Testing... This may take up to a minute."
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do
    # On prend un système clé -> valeur, par exemple: 00000010 -> .text          PROGBITS ...
    # On retire les espaces avant/après le texte et les Tabs

    # Cas particulier si il n'y a pas de data, readelf -x renvoie 0
    # On hard code, si notre programme dit qu'il n'y a pas de data (donc la ligne ci-dessous existe)
    
    if [ "$line" == "There is no data to dump." ]
    then
        # Alors si cette ligne existe, on verifie que readelf -x dit la même chose
        # Encore hard code, si dans l'affichage de readelf -x une ligne commence par "Section"
        # on sait que readelf -x dit que cette section n'a pas de données
        # Le message exact est "Section [NOM] has no data to dump." sauf que [NOM] rend la recherche compliquée
        # On simplifie en cherchant seulement "Section" qui est suffisant
        otherValue=`grep '^Section' readelfCommand.output`
        
        if [ ! -z "$otherValue" ] #On vérifie si la variable n'est pas vide, e.g: Si grep a trouvé la ligne
        then
            # Trouvé, readelf -x est d'accord avec notre programme. Pas de tests supplémentaires nécessaires
            echo "No data : \033[48;5;2mOK TEST\033[0;0m" #Pass
            echo
            echo "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
            rm -f MyReadelfCommand.output readelfCommand.output
            kill -KILL $$ #Comme on est dans une boucle While on doit kill le process...
            exit 0
        else
            # Pas trouvé, readelf -x nous contredit, fail test
            printf "No data : " #Fail
            FailTest "$errorReadelf" "$errorMyReadelf" "$$"
        fi
    fi

    # Autre cas particulier, notre programme contient une ligne avec une adresse "Starting at..."
    # On ne veut pas que cette adresse soit une Clé, la Clé serait valide mais pas la valeur associée
    # Donc on décide de ne créer une clé que si une ligne commence par "0x"
    # toutes les lignes pertinentes commencent par l'adresse donc par le préfixe 0x
    if [[ $line =~ $regexBegin ]]
    then
        key=`echo $line | cut -dx -f2 | cut -d' ' -f1`
    else
        key=" "
    fi

    # On hard code la position 12, on sait que toutes les données commencent à partir de la 12ème position.
    # Donc on cut à partir de la 12ème position, on enlève les leading/trailing Whitespaces/Tabs
    value=`echo $line | cut -c 12- | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $1) ; print $1}'`

    # echo "Key : $key"
    # echo "Value : $value"

    case $key in

    [[:xdigit:]]*)
        searchField="  0x$key"
        otherValue=`grep "^  0x$key" readelfCommand.output | cut -c -48 | cut -dx -f2 | cut -c 10-`
        otherValue=`echo $otherValue | awk '{gsub(/$regex2Blanks/, "", $0) ; print $0}'`
        #echo "OtherValue : $otherValue"

        if [ "$otherValue" = "$value" ]
        then # Les résultats correspondent on passe à la ligne suivante
            :
        else #Les résultats ne correspondent pas on renvoie une exception
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
    ;;
    esac
    
done

# Si tous les tests ont réussi, alors le test passe, on supprime les fichiers temporaires
echo "Content : \033[48;5;2mOK TEST\033[0;0m" #Pass
echo
echo "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
rm -f MyReadelfCommand.output readelfCommand.output
exit 0