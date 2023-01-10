# GROUPE 6:
• Altun Zeynel  
• Domi Ronald
• Levasseur Bastien  
• Maillard-Simon Émilien  
• Marmey Loïc  
• Régnier Léo  


# Editeur de Lien

Ce projet est une implémentation en C d'une des parties d'un compilateur complet.  

Le compilateur dispose 4 phases  
    > Preprocessor  
    > Compiler  
    > Assembler  
    > Linker  

A partir de plusieurs fichiers relocables .o, l'editeur de lien a 2 doit effectuer deux actions. Fusioner tout les fichiers relocatables dans un relocatable 
unique, ou il n'y a pas de symboles indefinis, et créer un exécutable à partir de ce relocatable final.
Concrétement, on cherche à fusionner 2 fichiers relocatables.

# Getting Started
Pour lancer le projet il faut premierement générer le Makefile :
`autoreconf -vif`
`./configure`  
`make`  
// Pas besoin de `for dir in Examples_fusion/*; do autoreconf -vif $dir; done`

L'execution de Ces lignes est necessaires chaque fois qu'on change le repertoire.   
Comme par exemple: Changement de branches ou 'git pull' 

Pour lancer les programmes:
Partie 1:

Etape 1: `./readelf_all --file <ELFFile> --type h`
Etape 2: `./readelf_all --file <ELFFile> --type s`
Etape 3: `./readelf_all --file <ELFFile> --type x --arg <Number | Name>`
Etape 4: `./readelf_all --file <ELFFile> --type S`
Etape 4: `./readelf_all --file <ELFFile> --type r`

Partie 2: `./mergeelf <File1> <File2>

Tous les tests fournis sont des fichiers ELF dans le répertoire tests/
## Outils
La progression du travail est disponible sur le lien [Trello](https://trello.com/b/Ac6JsYPy/editeur-de-lien) 

# Fonctionnalitées :
## Partie 1: 
• Etape 1 : Lecture/Affichage de l'en-tête d'un fichier ELF
• Etape 2 : Lecture/Affichage d'une table de sections d'un fichier ELF
• Etape 3 : Lecture/Affichage du contenu d'une section
• Etape 4 : Lecture/Affichage de la table des symboles d'un fichier ELF
• Etape 5 : Lecture/Affichage de la table des réimplémentations d'un fichier ELF

## Partie 2:
• Etape 6 : Fusion de deux sections de deux fichiers ELF
• Etape 7 : Fusion, renumérotation et correction des symboles de deux fichiers ELF
• Etape 8 : Fusion, renumérotation et correction des tables de réimplémentations d'un fichier ELF
• Etape 9 : Création d'un fichier ELF

## Bugs notables : 
• Le projet entier est conçu pour la notation Little Endian, l'utilisation de tests Big Endian fausse donc tous les programmes


# Notes pour développeurs
## Processus de dévelopement
Puisque dans ce projet, tout changement doit passer par une Pull Request, il est nécessaire d'ouvrir une branche pour chaque changement.

> Se mettre a jour branche main  
`git fetch`  
`git pull`  
`git log --oneline` 
<hr>

> Ouvrir la branche et developer  
`git branch branch_name`  
`git commit -m '1'`   
`git commit -m '2'`   
`git commit -m '3'`   
<hr>

> Prendre en compte les possibles changements de main. 
On suppose que pendant que on travaille sur notre branche, 'main' va changer.
Il faut donc se mettre à jour
`git checkout main`  
`git fetch, git pull`  
`git checkout brachname`  
`git merge main`  
<hr> 

> Il peut probablement y avoir des conflits. Un conflit apparaît quand un même fichier est change par 2 personnes differents. 
Pour le régler, il faut aller sur les fichiers qui ont conflits, les résoudre et puis : 
git add *, git commit
<hr>

> `git push origin branch_name`   
Créer le Pull Request au Github  
Mettre des Reviewers pour que la PR soit approuvée

<br>
