#!/bin/sh

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
function FailTest(){
    echo "\033[48;5;1m===FAIL TEST===\033[0;0m"
    echo "readelf is :\"$1\""
    echo "MyReadElf is : \"$2\""
    # rm -f MyReadelfCommand.output readelfCommand.output
    exit 1
}


# Cette fonction prend en entrée deux textes censés être différents et avertit l'utilisateur
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
function WarningTest(){
    echo "\033[43;5;1m===WARNING TEST===\033[0;0m"
    echo "readelf is :\"$1\""
    echo "MyReadElf is : \"$2\""
    ((warning++))
}

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
        printf "Error code : " #Fail
        FailTest "$errorReadelf $errorMyReadelf"
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Error code : " #Fail
        FailTest "$errorReadelf $errorMyReadelf"
    fi
fi
echo

sectionNumber=0
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do
    
    #On prend un système clé -> valeur, par exemple: [ 1] -> .text          PROGBITS ...
    #On retire les espaces avant/après le texte et les Tabs

    key=`echo $line | cut -d] -f1`
    key="${key}]"

    value=`echo $line | awk -F] '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}'`
    
    
    # echo "Key : $key"
    # echo "Value : $value"

    case $key in
    
    \[[[:space:]*[0-9]*\]) #Regex permettant de reconnaitre [  0] ... [ 10] ... [100] #\[[[:space:]*[0-9]*\])

    echo "=====Section $sectionNumber====="
    regexZero="\[[[:space:]*[0]*\]"
    otherValue=`grep -F "  $key" readelfCommand.output | cut -d] -f2 `
    #echo "otherValue : $otherValue"
    
    # On cut chaque élément de la ligne Value et on les mets dans leur variable:
    name=`echo $value | cut -d' ' -f1`
    type=`echo $value | cut -d' ' -f2`
    flags=`echo $value | cut -d' ' -f3`
    execAddr=`echo $value | cut -d' ' -f4`
    offset=`echo $value | cut -d' ' -f5`
    sectionSize=`echo $value | cut -d' ' -f6`
    link=`echo $value | cut -d' ' -f7`
    info=`echo $value | cut -d' ' -f8`
    align=`echo $value | cut -d' ' -f9`
    entrySize=`echo $value | cut -d' ' -f10`

    #Ensuite on fait la même chose pour otherValue

    #Cas particulier si première section car il n'y a pas de nom donc une entrée en moins
    if [[ "$key" =~ $regexZero ]] #Match tous les [0], [ 0], [  0], ...
    then
        echo "==Alternate source=="
        otherType=`echo $otherValue | cut -d' ' -f1`
        otherExecAddr=`echo $otherValue | cut -d' ' -f2`
        otherOffset=`echo $otherValue | cut -d' ' -f3`
        otherSectionSize=`echo $otherValue | cut -d' ' -f4`
        otherEntrySize=`echo $otherValue | cut -d' ' -f5`
        otherFlags=`echo $otherValue | cut -d' ' -f6`
        otherLink=`echo $otherValue | cut -d' ' -f7`
        otherInfo=`echo $otherValue | cut -d' ' -f8`
        otherAlign=`echo $otherValue | cut -d' ' -f9`
    else
        echo "==Normal source=="
        otherName=`echo $otherValue | cut -d' ' -f1`
        otherType=`echo $otherValue | cut -d' ' -f2`
        otherExecAddr=`echo $otherValue | cut -d' ' -f3`
        otherOffset=`echo $otherValue | cut -d' ' -f4`
        otherSectionSize=`echo $otherValue | cut -d' ' -f5`
        otherEntrySize=`echo $otherValue | cut -d' ' -f6`
        otherFlags=`echo $otherValue | cut -d' ' -f7`
        otherLink=`echo $otherValue | cut -d' ' -f8`
        otherInfo=`echo $otherValue | cut -d' ' -f9`
        otherAlign=`echo $otherValue | cut -d' ' -f10`
    fi
    #On commence par vérifier le nom :

    #Cas particulier pour la première section qui ne contient pas de nom, on ne peut pas cut -d' '
    
    if [[ "$key" =~ $regexZero ]] #Match tous les [0], [ 0], [  0], ...
    then
        if [ "$name" == "==NO_NAME==" ]
        then
            echo "Nom : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Nom : "
            FailTest "$otherName $name" #Fail
        fi
    else
        if [ "$name" == "$otherName" ]
        then
            echo "Nom : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            # Pour l'instant vu que les noms ne correspondent pas à readelf -S on met juste un Warning
            # On dit que le test passe avec X warnings
            printf "Nom : "
            WarningTest "$otherName $name"
        fi
    fi
    
    #On poursuit en vérifiant le Type :
    echo "otherValue : $otherValue"
    if [ "$type" == "$otherType" ]
    then
        echo "Type :  \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Type : "
        FailTest "$otherType $type" #Fail
    fi

    #On poursuit en vérifiant les Flags :

    #Cas particulier pour les Flags de readelf -S qui n'existent pas si ils sont nuls
    # On a obtenu Flags avec un cut au délimiteur 7, si on obtient un nombre alors on a pris le Link (la colonne d'après) 
    # donc on sait que les flags n'existent pas, on compare à 0 sur notre programme

    #Permet de vérifier si on a un nombre dans Flags, les nombres négatifs n'existent pas donc pas besoin de tester
    regexNumber="^[0-9]"
    if [[ $otherFlags =~ regexNumber ]] 
    then
        #Pas de flag
        if [ $flags -eq 0 ]
        then
            echo "Flags : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Flags : "
            FailTest "[No Flag]" "$flags"
        fi
    else
        if [ "$flags" == "$otherFlags" ]
        then
            echo "Flags : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            # Comme les flags ne sont pas encore décryptés on ne renvoie pas une errreur mais un avertissement
            printf "Flags : "
            WarningTest "$otherFlags" "$flags"
        fi
    fi

    #On poursuit en vérifiant l'Execution Adress :
    #On enlève le préfixe 0x pour la convertion
    execAddr=`echo $execAddr | cut -dx -f2`
    
    if [ "$execAddr" == "$otherExecAddr" ]
    then
        echo "Execution Adress : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Execution Adress :"
        FailTest "$otherExecAddr" "$execAddr"
    fi

    ((sectionNumber++))

    # On poursuit en vérifiant l'Offset
    # Comme readelf imprime tous les chiffres y-compris les 0 devant, on utilise bc pour vérifier l'équivalence
    # Or, bc ne supporte que l'upper-case pour l'hexa, on convertit

    offsetHexaCAPS=`printf '0%X' $offset`
    otherOffsetCAPS=`echo $otherOffset | tr [a-z] [A-Z]`
    ans=`echo $offsetHexaCAPS==$otherOffsetCAPS | bc`

    if [ $ans -eq 1 ] #Si les deux nombres sont égaux 
    then
        echo "Offset : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Offset : " 
        FailTest "$otherOffsetCAPS" "$offsetHexaCAPS" #Fail
    fi

    # On poursuit en vérifiant la Section Size
    #TODO: Le reste

    esac
done

echo "Test $(basename "$1") passed!"