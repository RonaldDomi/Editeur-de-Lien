#GROUPE 6:
• Altun Zeynel
• Domi Ronald
• Maillard-Simon Émilien
• Marmey Loïc
• Levasseur Bastien
• Régnier Léo

# Editeur-de-Lien
Editeur de Lien implémenté en C


Les fichiers ELF sont des fichiers objets qui peuvent être:
• Un executable
• Un relocatable object (??)
• Un objet partagé par d'autres fichiers objets

Ces fichiers sont donc au format binaire, on cherche donc à lire octet par octet
#Phase 1:

##Etape 1:

Compiler avec <make -f Makefile_Phase1>
Essayer avec ./read_elf_reader <FichierBinaire>
Les tests automatisés sont disponibles : ./test_read_elf_header.sh <FichierBinaire>

##Etape 2:

Compiler avec <make -f Makefile_Phase1>
Essayer avec ./read_elf_section_table <FichierBinaire>
Les tests automatisés ne sont pas encore disponibles, on peut comparer à la main en attendant
