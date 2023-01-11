# GROUPE 6 :
• Altun Zeynel  
• Domi Ronald
• Levasseur Bastien  
• Maillard-Simon Émilien  
• Marmey Loïc  
• Régnier Léo  

# Documentation supplémentaire : 

Une documentation décrivant la structure du code, les fonctionnalitées, les bugs est disponible sous le fichier Presentation.md à la racine du projet.

<br>

# Éditeur de Lien :

Ce projet est une implémentation en C d'une des parties d'un compilateur complet.  

Le compilateur dispose de 4 phases  
    > Preprocessor  
    > Compiler  
    > Assembler  
    > Linker  

À partir de plusieurs fichiers relocatables .o, l'éditeur de lien doit effectuer deux actions, fusioner tout les fichiers relocatables dans un relocatable unique, ou il n'y a pas de symboles indéfinis, et créer un exécutable à partir de ce relocatable final.
Concrètement, on cherche à fusionner 2 fichiers relocatables.  

<br>

# Getting Started :
## Instructions de compilation :
Pour lancer le projet il faut premièrement générer le Makefile :  
`autoreconf -vif`  
`./configure`    
`make`    

On peut générer des tests supplémentaires dans le répertoire Examples_fusion :   
`cd Examples_fusion`  
`make`

Tous les programmes sont centralisés dans un seul programme : readelf_all
On peut compiler chaque partie indépendemment en décommentant les lignes 18 à 22 et 39 à 43 dans le fichier Makefile.am
On obtiendra donc plusieurs programmes qui ont tous le préfixe "readelf\_".
Cependant, ces programmes peuvent ne pas être à jour et leur comportement peut être imprévisible par rapport à readelf_all qui est à jour.

<br>

## Instructions d'éxécution :
Pour lancer les programmes:
Partie 1:

Etape 1: `./readelf_all --file <ELFFile> --type h`  
Etape 2: `./readelf_all --file <ELFFile> --type s`  
Etape 3: `./readelf_all --file <ELFFile> --type x --arg <Number | Name>`  
Etape 4: `./readelf_all --file <ELFFile> --type S`  
Etape 4: `./readelf_all --file <ELFFile> --type r`  


Partie 2: `./mergeelf <File1> <File2>`

Tous les tests fournis sont des fichiers ELF dans le répertoire tests/.

<br>

## Tester les Étapes : 

Les étapes 1 à 5 peuvent être testées individuellement avec un script Bash fourni.
Chaque étape teste l'affichage de notre programme et le compare avec la fonction readelf qui est notre oracle pour toute la Partie 1.  

À noter que les tests utilisent le programme *readelf_all* et non pas les programmes individuels.
Les tests comparent ligne par ligne et omettent certains détails comme l'affichage des flags par exemple: "Version5 EABI" pour readelf contre "EABI Version 5" pour notre programme.

Pour lancer un test, exécuter le Script Bash associé suivi d'un fichier test, par exemple :  
`./test_read_elf_header.sh tests/ARM.o` pour tester la partie 1 avec le test ARM.o  

<br>

## Outils
La progression du travail est disponible sur le lien [Trello](https://trello.com/b/Ac6JsYPy/editeur-de-lien) 

<br>
<br>

# Notes pour développeurs :

## Processus de développement
Dans ce projet, tout changement doit passer par une Pull Request, il est nécessaire d'ouvrir une branche pour chaque changement.

> Se mettre à jour branche main :
`git fetch`  
`git pull`  
`git log --oneline` 
<hr>

> Ouvrir la branche et développer :
`git branch branch_name`  
`git commit -m '1'`   
`git commit -m '2'`   
`git commit -m '3'`   
<hr>

> Prendre en compte les possibles changements dans la branche main. 
On suppose que pendant qu'on travaille sur notre branche, 'main' va changer.
Il faut donc se mettre à jour :
`git checkout main`  
`git fetch, git pull`  
`git checkout brachname`  
`git merge main`  
<hr> 

> Il peut probablement y avoir des conflits. Un conflit apparaît quand un même fichier est changé par 2 personnes différentes. 
Pour le régler, il faut aller sur les fichiers qui ont conflits, les résoudre, puis : 
`git add *` 
`git commit`
<hr>

> `git push origin branch_name`   
Créer le Pull Request au GitHub 
Mettre des Reviewers pour que la PR soit approuvée
<br>
