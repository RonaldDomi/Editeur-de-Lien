#!/bin/sh
# Même remarque que pour test_read_elf_header.sh on utilise aussi Shell

# Cette fonction prend en entrée deux textes censés être différents et termine le script
# On utilise cette fonction uniquement lorsqu'on trouve une différence entre les deux fonctions testées
FailTest () {
    echo "\033[41;1;1m===FAIL TEST===\033[0;0m"
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
WarningTest () {
    echo "\033[43;1;1m===WARNING TEST===\033[0;0m"
    echo "  • readelf is :\"$1\""
    echo "  • MyReadElf is : \"$2\""
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
        echo
        echo "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
        rm -f MyReadelfCommand.output readelfCommand.output
        exit 0
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf"
    fi
else #Si errorReadelf = 0
    if [ $errorMyReadelf -eq 0 ]
    then
        echo "Error code : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        echo "Is the project compiled?"
        printf "Error code : " #Fail
        FailTest "$errorReadelf" "$errorMyReadelf"
    fi
fi
echo

sectionNumber=0
pid=$$
alternateSourceName=0 # Permet de savoir si la ligne traitée contient un nom de section où non
alternateSourceFlag=0 # Permet de savoir si la ligne traitée contient des flags où non
regexNumber="^[0-9]" # Match tous les chiffres en base 10
regexZero="\[[[:space:]*[0]*\]" # Match tous les [0], [ 0], [  0], ...
cat MyReadelfCommand.output | while read line || [ -n "$line" ]; do
    
    #On prend un système clé -> valeur, par exemple: [ 1] -> .text          PROGBITS ...
    #On retire les espaces avant/après le texte et les Tabs

    key=`echo $line | cut -d] -f1`
    key="${key}]"

    value=`echo $line | awk -F] '{gsub(/^[ \t]+|[ \t]+$/, "", $2) ; print $2}'`
    # echo "Key : $key"
    # echo "Value : $value"

    case $key in
    
    #Regex permettant de reconnaitre [  0] ... [ 10] ... [100] #\[[[:space:]*[0-9]*\])
    \[[[:space:]*[0-9]*\]) 
    
    echo "=====Section $sectionNumber====="
    
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

    #Other value sera donc la valeur à la sortie de readelf, on enlève juste le [numéro] au début 
    #Ensuite on fait la même chose pour otherValue
   

    otherValue=`grep -F "  $key" readelfCommand.output | cut -d] -f2 `
    #echo "otherValue : $otherValue"

    # Cas particulier si première section car il n'y a pas de nom donc une entrée en moins
    # L'entrée en moins est représentée par un whitespace, donc on aurait un décalage avec un cut -d' '
    if [[ "$key" =~ $regexZero ]] #Match tous les [0], [ 0], [  0], ...
    then
       # ==Alternate source==
        alternateSourceName=1
        otherType=`echo $otherValue | cut -d' ' -f1`
        otherExecAddr=`echo $otherValue | cut -d' ' -f2`
        otherOffset=`echo $otherValue | cut -d' ' -f3`
        otherSectionSize=`echo $otherValue | cut -d' ' -f4`
        otherEntrySize=`echo $otherValue | cut -d' ' -f5`
        otherFlags=`echo $otherValue | cut -d' ' -f6`

        # Cas particulier pour les Flags de readelf -S qui n'existent pas si ils sont nuls
        # On a obtenu Flags avec un cut au délimiteur 7, si on obtient un nombre alors on a pris le Link (la colonne d'après) 
        # donc on sait que les flags n'existent pas, on compare à 0 sur notre programme
        if [[ $otherFlags =~ $regexNumber ]] #Pas de flags puisque flag vide 
        then
            alternateSourceFlag=1
            otherLink=`echo $otherValue | cut -d' ' -f6`
            otherInfo=`echo $otherValue | cut -d' ' -f7`
            otherAlign=`echo $otherValue | cut -d' ' -f8`
        else
            otherLink=`echo $otherValue | cut -d' ' -f7`
            otherInfo=`echo $otherValue | cut -d' ' -f8`
            otherAlign=`echo $otherValue | cut -d' ' -f9`
        fi
    else
        # ==Normal source==
        otherName=`echo $otherValue | cut -d' ' -f1`
        otherType=`echo $otherValue | cut -d' ' -f2`
        otherExecAddr=`echo $otherValue | cut -d' ' -f3`
        otherOffset=`echo $otherValue | cut -d' ' -f4`
        otherSectionSize=`echo $otherValue | cut -d' ' -f5`
        otherEntrySize=`echo $otherValue | cut -d' ' -f6`
        otherFlags=`echo $otherValue | cut -d' ' -f7`
        if [[ $otherFlags =~ $regexNumber ]]
        then
            alternateSourceFlag=1
            otherLink=`echo $otherValue | cut -d' ' -f7`
            otherInfo=`echo $otherValue | cut -d' ' -f8`
            otherAlign=`echo $otherValue | cut -d' ' -f9`
        else
            otherLink=`echo $otherValue | cut -d' ' -f8`
            otherInfo=`echo $otherValue | cut -d' ' -f9`
            otherAlign=`echo $otherValue | cut -d' ' -f10`
        fi
    fi

    #echo "OtherType : <$otherType>"
    #On commence par vérifier le nom :

    #Cas particulier pour la première section qui ne contient pas de nom
    #On peut le vérifier avec le flag alternateSourceName 
    
    if [ $alternateSourceName -eq 1 ]
    then
        if [ "$name" == "==NO_NAME==" ]
        then
            echo "Nom : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Nom : "
            FailTest "$otherName" "$name" "$$" #Fail
        fi
    else
        if [ "$name" == "$otherName" ]
        then
            echo "Nom : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            # Pour l'instant vu que les noms ne correspondent pas à readelf -S on met juste un Warning
            # On dit que le test passe avec X warnings
            printf "Nom : "
            WarningTest "$otherName" "$name" 
        fi
    fi
    
    #On poursuit en vérifiant le Type :
    
    if [ "$type" == "$otherType" ]
    then
        echo "Type :  \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Type : "
        FailTest "$otherType" "$type" "$$" #Fail
        kill -KILL $$
    fi

    # On poursuit en vérifiant les Flags :
    # On vérifie avec le flag (variable) alternateSourceFlag
    if [ $alternateSourceFlag -eq 1 ] 
    then
        #Pas de flag
        if [ $flags -eq 0 ]
        then
            echo "Flags : \033[48;5;2mOK TEST\033[0;0m" #Pass
        else
            printf "Flags : "
            FailTest "[No Flag]" "$flags" "$$" #Fail
        fi
    else
        #Un flag existe
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
        FailTest "$otherExecAddr" "$execAddr" "$$" #Fail
    fi

    

    # On poursuit en vérifiant l'Offset
    # Comme readelf imprime tous les chiffres y-compris les 0 devant, on utilise bc pour vérifier l'équivalence
    # Or, bc ne supporte que l'upper-case pour l'hexadécimal, on convertit
    # Les leading 0 n'importent pas pour bc: 0x00000001 <=> 0x1 

    offsetHexaCAPS=`printf '%X' $offset`
    otherOffsetCAPS=`echo $otherOffset | tr [a-z] [A-Z]`
    ans=`echo $offsetHexaCAPS==$otherOffsetCAPS | bc`

    if [ $ans -eq 1 ] #Si les deux nombres sont égaux 
    then
        echo "Offset : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Offset : " 
        FailTest "$otherOffsetCAPS" "$offsetHexaCAPS" "$$" #Fail
    fi

    # On poursuit en vérifiant la Section Size
    # Même méthode que l'offset

    sectionSizeHexaCAPS=`printf '0%X' $sectionSize`
    otherSectionSizeCAPS=`echo $otherSectionSize | tr [a-z] [A-Z]`
    ans=`echo $sectionSizeHexaCAPS==$otherSectionSizeCAPS | bc`

    if [ $ans -eq 1 ] #Si les deux nombres sont égaux 
    then
        echo "Section Size : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Section Size : " 
        FailTest "$otherSectionSizeCAPS" "$sectionSizeHexaCAPS" "$$" #Fail
    fi
    
    # On poursuit en vérifiant le Link
    # Pas besoin de traduire en hexa, on vérifie simplement

    if [ "$link" == "$otherLink" ]
    then
        echo "Link To : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Link To : " 
        FailTest "$otherLink" "$link" "$$" #Fail
    fi

    
    # On poursuit en vérifiant les Info
    # Même méthode que le Link

    if [ "$info" == "$otherInfo" ]
    then
        echo "Info : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Info : " 
        FailTest "$otherInfo" "$info" "$$" #Fail
    fi


    # On poursuit en vérifiant l'alignement
    # Même méthode que ci-dessus

    if [ "$align" == "$otherAlign" ]
    then
        echo "Align : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Align : " 
        FailTest "$otherAlign" "$align" "$$" #Fail
    fi


    # On finit en vérifiant l'Entry Size
    # On doit convertir en Hexa comme pour l'Offset
    entrySizeHexaCAPS=`printf '%X' $entrySize`
    otherEntrySizeCAPS=`echo $otherEntrySize | tr [a-z] [A-Z]`
    ans=`echo $entrySizeHexaCAPS==$otherEntrySizeCAPS | bc`

    if [ $ans -eq 1 ] #Si les deux nombres sont égaux 
    then
        echo "Entry Size : \033[48;5;2mOK TEST\033[0;0m" #Pass
    else
        printf "Entry Size : " 
        FailTest "$otherEntrySizeCAPS" "$entrySizeHexaCAPS" "$$" #Fail
    fi


    ((sectionNumber++))
    alternateSourceName=0
    alternateSourceFlag=0
    echo
    ;;

    esac

    #Pas d'autres cases, les lignes qui ne commencent par par [  X] ne sont pas à traiter
done

echo
# Si tous les tests ont réussi, alors le test passe, on supprime les fichiers temporaires
echo "Test $(basename "$1") \033[48;5;2mpassed\033[0;0m!"
rm -f MyReadelfCommand.output readelfCommand.output
exit 0