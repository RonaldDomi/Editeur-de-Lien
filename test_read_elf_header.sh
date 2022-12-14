#!/bin/bash
# On ne peut pas utiliser bash, la boucle While utilise un sous-processus, donc le scope des variables dans le while
# N'est pas le même que autre part dans le script.
# Concrètement, on ne peut pas faire un compteur d'erreurs qui s'incrémente dans la boucle while.
# À la sortie de la boucle, le compteur serait à zéro.
# Le problème est plus détaillé ici : https://stackoverflow.com/a/16854326/14702675
# La commande exit ne termine que le sous-processus et non le script en entier
# On utilise donc shell à la place qui n'a pas ce comportement

# Dans ce script, on appelle la commande built-in readelf "Readelf"
# Le programme écrit dans le cadre du projet s'appelle "MyReadelf"

# Cette fonction prend en entrée un texte et supprime tout ce qui n'est pas un chiffre.
# Par exemple: "25 Headers" devient "25"
# On utilise une Regex NOT[0-9] avec le Flag Global
SupprimerLettres() {
    local ans=$(echo $1 | sed '{s/[^0-9]//g;}')
    echo "$ans"
}

#Cette fonction prend en entrée un texte et supprime tout ce qui n'est pas une lettre
#Par exemple: "2 Executable File" devient "Executable File"
#On utilise une Regex NOT[A-Z]*
SupprimerChiffres() {
    local ans=$(echo $1 | sed '{s/[^A-Z]*//;}')
    echo "$ans"
}

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
FailTest() {
    echo -e "\033[101;1;1m===FAIL TEST===\033[0;0m"
    echo "readelf is :\"$1\""
    echo "MyReadElf is : \"$2\""
    rm -f MyReadelfCommand.output readelfCommand.output
    #TOFIX: Comme le while utilise un subprocess, on peut kill le processus, mais on aura pas de valeur de retour
    #Il faut essayer d'utiliser zsh mais on peut avoir comme erreur "readelf command not found"...
    kill -KILL $3
    exit 1
}

echo
#On vérifie qu'on dispose d'un fichier pour le test
if [ $# -eq 0 ]; then
    echo "Missing argument !"
    echo "Usage: test_read_elf_header <File>"
    exit 1
fi

#On vérifie que le fichier existe
if [ ! -f $1 ]; then
    echo "No such file !"
    exit 1
fi

#On exécute les deux commandes à comparer et on écrit la sortie standard sur des fichiers temporaires
#On vérifie aussi les codes d'erreurs
readelf -h $1 &>readelfCommand.output
errorReadelf=$?
./readelf_all --file $1 --type h &>MyReadelfCommand.output
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
        FailTest "$errorReadelf" "$errorMyReadelf" "$$"
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]; then
        echo -e "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf" "$$"
    fi
fi

# On compare la sortie de notre programme avec celle de readelf, on le fait dans ce sens car cela permet d'ignorer
# des données non-pertinentes telle que OS/ABI où Version qui est en double...

#Pour chaque ligne de sortie notre programme :
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do
    #On prend un système clé -> valeur, par exemple: Machine -> ARM
    #On retire les espaces avant/après le texte et les Tabs
    key=$(echo $line | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $1) ; print $1}')
    value=$(echo $line | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')

    # On fait un switch de la clé, suivant la caractéristique qu'on veut comparer (Version, Machine, ...)
    # On va chercher la ligne correspondante dans la sortie de readelf
    # La ligne à rechercher est codée en dur, par exemple on sait qu'on doit chercher "  Magic" pour trouver
    # le Magic Number de readelf c.f. ligne 102
    # On cherche avec une Regex, "^  Field", donc tout ce qui commence (^) par deux espaces et Field qui est codé en dur
    case $key in

    Magic\ number)
        #Other value sera donc la valeur à la sortie de readelf, on enlève les trailing whitespaces et les tabs avec awk & gsub()
        otherValue=$(grep '^  Magic' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        if [ "$value" = "$otherValue" ]; then                     #Si les deux valeurs dénudées des trailing whitespaces & tabs correspondent
            echo -e "Magic Number : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Magic Number : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Type\ objet)
        otherValue=$(grep '^  Type' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        # Pour le type objet, on ne veut récupérer que ce qu'il y a dans des parenthèses
        # On transforme donc "EXEC (Executable File)" -> "Executable File"
        # On le fait avec deux Regex .*( && ).*
        otherValue=$(echo $otherValue | sed '{s/.*(//; s/).*//;}')
        #On fait appel à notre fonction pour supprimer tous les caractères autres que les lettres
        value=$(SupprimerChiffres "$value")

        if [ "$value" = "$otherValue" ]; then
            echo -e "Object Type : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Object Type : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Type\ machine)
        otherValue=$(grep '^  Machine' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerChiffres "$value")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Machine Type : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Machine Type : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Version)
        # readelf contient deux champs "Version"... On ne prend que le premier avec head -1, on ignore le second
        otherValue=$(grep '^  Version' readelfCommand.output | head -1 | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')

        otherValue=$(SupprimerLettres "$otherValue")
        value=$(SupprimerLettres "$value")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Version : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Version : "
            FailTest "$otherValue" "$value" "$$"
        fi
        ;;

    Entry\ point\ adress)
        # Les adresses sont affichées au même format donc pas besoin de traiter le texte
        # Attention au format étrange de readelf:
        # Les adresses sont affichées sur 7 bits, donc tous les octets sont affichés avec 2 chiffre *sauf* le bit de poid fort si il est à zéro
        # Notre programme affiche dans le même format, par exemple: 0x a bc de f0 00
        otherValue=$(grep '^  Entry point address' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')

        #Encore un autre cas particulier, si une adresse est nulle, readelf affichera 0x0 au lieu de 0x0000000, on force donc le changement
        if [ $otherValue = "0x0" ]; then
            otherValue="0x0000000"
        fi

        if [ "$value" = "$otherValue" ]; then
            echo -e "Entry Point Adress : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Entry Point Adress : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Program\ header\ offset)
        otherValue=$(grep '^  Start of program headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Program Header Offset : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Program Header Offset : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Section\ header\ offset)
        otherValue=$(grep '^  Start of section headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Section Header Offset : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Section Header Offset : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Processor\ flags)
        otherValue=$(grep '^  Flags' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        # On prend simplement l'adresse et on la compare, on ignore les détails de readelf
        # Chaque détail est séparé par une virgule, on cut et on prend le premier champ (celui de l'adresse)
        value=$(echo $value | cut -d, -f1)
        otherValue=$(echo $otherValue | cut -d, -f1)
        # Ici aussi si il n'y a aucun flag l'adresse est 0x0 au lieu de 0x0000000. On force le changement.
        if [ $otherValue = "0x0" ]; then
            otherValue="0x0000000"
        fi

        if [ "$value" = "$otherValue" ]; then
            echo -e "Processor Flags : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Processor Flags : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Header\ size)
        otherValue=$(grep '^  Size of this header' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Header Size : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Header Size : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Program\ header\ size)
        otherValue=$(grep '^  Size of program headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Program Header Size : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Program Header Size : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Number\ of\ program\ headers)
        otherValue=$(grep '^  Number of program headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Number Of Programs Headers : \033[48;5;2mOK TEST\033[0;0m"

        else
            printf "Number Of Programs Headers : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Size\ of\ section\ header)
        otherValue=$(grep '^  Size of section headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Size of Section Header : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Size of Section Header : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Number\ of\ section\ headers)
        otherValue=$(grep '^  Number of section headers' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        value=$(SupprimerLettres "$value")
        otherValue=$(SupprimerLettres "$otherValue")
        if [ "$value" = "$otherValue" ]; then
            echo -e "Number Of Section Headers : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Number Of Section Headers : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;

    Index\ section\ string\ table)
        otherValue=$(grep '^  Section header string table index' readelfCommand.output | awk -F: '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}')
        if [ "$value" = "$otherValue" ]; then
            echo -e "Index Section String Table : \033[48;5;2mOK TEST\033[0;0m"
        else
            printf "Index Section String Table : "
            FailTest "$otherValue" "$value" "$$" #Fail
        fi
        ;;
    esac

done

echo
# Si tous les tests ont réussi, alors le test passe, on supprime les fichiers temporaires
echo -e "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
rm -f MyReadelfCommand.output readelfCommand.output
exit 0
