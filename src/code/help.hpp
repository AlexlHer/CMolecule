// Auteur : Alexandre l'Heritier

// Le const char* de l'aide.
const char *help = "\
-----Affichage de l'aide-----\n\
Usage : ./ter [Fichier Modele] [Options]\n\n\
-----Liste des options-----\n\
-1\n\
	Utilisation de l'algorithme ECP (Entité Centrée en mode Parallèle). (Algorithme par défaut si non précisé)\n\n\
-2\n\
	Utilisation de l'algorithme ECS (Entité Centrée en mode Séquentiel).\n\n\
-3\n\
	Utilisation de l'algorithme PC (Population Centrée).\n\n\
-o [Fichier de sortie]\n\
	Permet d'écrire les logs dans un fichier.\n\n\
-v [Fichier de sortie.csv]\n\
	Permet d'écrire les résultats de la simulation en format CSV. (Si non précisé, par défaut : out.csv)\n\n\
{ Au choix :\n\
-t [Nombre de secondes]\n\
	Le nombre de secondes à simuler. (1 seconde = 10000 pas) (Si non précisé, par défaut : 1)\nOU\n\
-T [Nombre de pas]\n\
	Le nombre de pas à simuler. (1 pas = 100 micro-secondes). (Si non précisé, par défaut : 10000)\n\
}\n\n\
-p [Nombre de pas entre deux lignes dans csv]\n\
	Le nombre de pas avant écriture dans le csv. (Si non précisé, par défaut : 100)\n\n\
-r [Nombre de threads]\n\
	Le nombre de threads à utiliser. (Pour l'algo ECP uniquement / Si non précisé, par défaut : Déterminé par l'algorithme selon la configuration du CPU)\n\n\
-c [sqrt3(Nombre de case dans vésicule)]\n\
	Le nombre de case dans la vésicule à étudier parallèlement (Attention, ce nombre sera mis à la puissance 3 car vésicule en 3D, donc si '-c 3' alors nombre de case = 3*3*3 = 27) (Pour l'algo ECP uniquement / Si non précisé, par défaut : Déterminé par l'algorithme selon nombre de thread)\n\n\
-d [Nombre de pas diffusion]\n\
	Le nombre de pas à effectuer en diffusion au début. (N'est pas inclut dans le '-t nombre de pas') (Si non précisé, par défaut : 100)\n\n\
-g\n\
	Génération de l'affichage des courbes avec gnuplot et ouverture à la fin (nécessite gnuplot d'installé).\n\n\
-s\n\
	Suivi de la progression dans la console. (Peut ralentir le calcul si Pas petit)\n\n\
-h\n\
	Pour afficher cette aide.\n\n\
-----Exemples de commandes-----\n\
On a le modèle \"modele.txt\".\n\
On veux le résultat de l'algo ECP (-1), avec affichage du suivi (-s), avec le graphique résultat (-g),\n\
avec le résultat dans result.csv (-v result.csv), avec les log (-o log.log), avec un temps simulé de 10 sec (-t 10),\n\
avec un enregistrement du résultat tous les 100 pas (-p 100), avec 16 threads (-r 16) et avec 27 cases (-c 3).\n\
	./ter modele.txt -1 -s -g -v result.csv -o log.log -t 10 -p 100 -r 16 -c 3 \n\n\
On a le modèle \"modele.txt\".\n\
On veux le résultat de l'algo PC (-2), avec affichage du suivi (-s), avec le graphique résultat (-g),\n\
avec le résultat dans result2.csv (-v result2.csv), avec les log (-o log.log), avec 100000 pas (10 sec) (-T 100000) et\n\
avec un enregistrement du résultat tous les 100 pas (-p 100).\n\
	./ter modele.txt -2 -s -g -v result2.csv -o log.log -T 100000 -p 100\n";
