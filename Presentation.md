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
 • Etape 9 : Création *partielle* d'un fichier ELF

## Bugs notables : 
 • Le projet entier est conçu pour la notation Little Endian, l'utilisation de tests Big Endian fausse donc tous les programmes
 • La création du fichier ELF n'est pas complète


# Description du code :

## Structure globale :

Chaque étape correspond à un ou plusieurs modules, ces modules sont importés entre-eux ou dans nos fonctions mains readelf\_all / mergeelf.

Le programme crée une grande structure pouvant contenir toutes les informations du fichier ELF, cette structure est primordiale pour l'étape 2 et permet de limiter les freaad() & fseek() dans le fichier ELF.
La structure créée est définie dans le fichier modules/CustomElf.h en fin de fichier lignes 3928 à 3945. Elle est définie plus en détail dans la section suivante de cette présentation.

Les fonctions de remplissage de la structure sont séparées des fonctions d'affichage. 
Donc, quand on exécute un affichage (Étape 1 à 5), on utilise d'abord les fonctions de remplissage de la structure (qui elles contiennent les fread() et éventuellement les fseek()) et en seconde partie les fonctions d'affichage qui vont lire de la structure précédemment remplie.
La séparation du remplissage et de l'affichage permet d'utiliser toutes les fonctions sans risquer d'afficher une étape antérieure.
Comme dans le projet, toute étape *n* est dépendante de l'étape *n-1*, cette séparation est très utile.

La fonction principale de la Partie 1 est contenue dans le fichier readelf_all.c. Suivants les options et arguments donnés, la fonction executera la partie correspondante.

Pour la Partie 2, une autre fonction principale est dans le fichier mergeelf.c et regroupe les 4 étapes en une.
L'utilisation de la Partie 1 appelle toutes les fonctions de remplissage de la Partie 1 et utilise la structure remplie.

<br>

## Détails sur la structure utilisée : 

Au plus haut niveau, la structure est une Elf32\_Info contenant :  
	• Une unique en-tête de fichier ELF : Header  
	• Une structure de toutes les tables de sections du fichier : AllSections  
	• Une structure de toutes les tables de symboles du fichier : AllSymbol  

AllSections est une autre structure contenant :   
	• Un entier contenant le nombre de sections : nbSections  
	• Un tableau de taille 50 de toutes les sections : TabAllSec  
	• Un tableau de taille 50 de tout le contenu de toutes les sections : TabAllSecContent  
	• Un tableau de taille 50 de toutes les reimplémentations de type Rel de toutes les sections : TabAllRel  
	• Un tableau de taille 50 de toutes les reimplémentations de type Rela de toutes les sections : TabAllRela  

AllSymbol est de type Elf32_Sym qui est une structure déjà intégrée à CustomElf.h.  
<br>
La taille 50 est arbitraire, un ELF de plus de 50 sections entrainera certainement une erreur de segmentation, mais durant la programmation, aucun fichier ELF n'a produit plus de 50 secontions, le maximum était firmware.elf et ne contenait que 22 sections.

<br>

## Détails sur la structure des tests :

Chaque test est un script Bash qui exécute le programme et la commande *readelf* et compare le résultat ligne par ligne.

Le script ne prend que les lignes qui l'intéresse, on enlève le bruit comme les en-têtes en début d'affichage où des détails qui n'ont pas grande importance.

Le script compare donc uniquement les données. Une erreur est renvoyée lors de la première différence entre les deux programmes.
Parfois, un avertissement est renvoyé à la place d'une erreur, cela arrive si on trouve une différence sur un nom, comme par exemple sur le test de la table des symboles.  
Les avertissements ne font pas échouer le test, il sera quand même considéré comme valide (supposant qu'aucune erreur ne soit renvoyée).  
Cela peut mener à des faux positifs où des données ET le nom peuvent êtres fausses et le test ne renvoie qu'un avertissement à cause du nom. Mais ces faux positifs n'ont jamais étés remarqués dans nos fichiers de tests.  

Tous les scripts sont commentés pour comprendre leur fonctionnement.