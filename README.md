# CMolecule
## Par Alexandre l'Heritier
Simulateur stochastique de réactions biochimiques.

TER proposé par Patrick Amar.

## Description

Le principe de ce programme est de simuler les déplacement des molécules et les réactions entre molécule dans une vésicule.

En entrée, on a un fichier qui donne les informations sur les molécules et les réactions possibles entre ces molécules.

En sortie, on a un .csv qui donne la population de chaque type de molécule tous les t temps et un graphique généré avec le .csv par GNUPlot.

Exemple de fichier d'entrée :
```
espece E, S, P, ES;
diametre = 1000;

taille(E) = 7;
vitesse(E) = 0.5;
init(E) = 50;

init(S) = 50;

E + S -> ES [0.3];
ES -> E + S [0.002];
ES -> E + P [0.005];
```

Exemple de fichier de sortie :
```
toursSimu;tempsSimu;nbMolecules;ES;P;S;E;
0;0;100;0;0;50;50;
100;0.01;84;16;8;26;34;
200;0.02;85;15;11;24;35;
300;0.03;87;13;15;22;37;
400;0.04;91;9;23;18;41;
500;0.05;96;4;28;18;46;
600;0.06;94;6;30;14;44;
700;0.07;93;7;31;12;43;
800;0.08;97;3;36;11;47;
900;0.09;99;1;38;11;49;
1000;0.1;99;1;39;10;49;
...
```

## Programme
### Compilation
Commande pour compiler le programme (nécessite make/flex/bison/g++) :
```
cd code
make
```

### Lancement
Pour accéder à l'aide du programme :
```
./projet_ter -h
```

Pour lancer : 
 - Sur l'exemple 1 [./projet_ter test.txt],
   * la simulation Entité Centrée Parallèle [-1],
   * pendant 1 seconde (10 000 pas) [-t 1],
   * avec un enregistrement des résultats tous les 100 pas (toutes les 0.01 secondes) [-p 100],
   * avec un suivi de la progression [-s],
   * avec les résultats dans le fichier "exemple1.csv" [-v exemple1.csv],
   * avec un affichage final dans gnuplot et le fichier de config gnp "plot_exemple1.csv.gnp" [-g],
   * avec un enregistrement des logs dans le fichier "log_exemple1.txt" [-o log_exemple1.txt] :
```
./projet_ter test.txt -1 -t 1 -p 100 -s -v exemple1.csv -g -o log_exemple1.txt
```

 - Sur l'exemple 2 [./projet_ter test2.txt],
   * la simulation Population Centrée [-2],
   * pendant 40 secondes (400 000 pas) [-t 40],
   * avec un enregistrement des résultats tous les 1000 pas (toutes les 0.1 secondes) [-p 1000],
   * sans suivi de la progression (car ralentit la simulation),
   * avec les résultats dans le fichier "exemple2.csv" [-v exemple2.csv],
   * avec un affichage final dans gnuplot et le fichier de config gnp "plot_exemple2.csv.gnp" [-g],
   * avec un enregistrement des logs dans le fichier "log_exemple2.txt" [-o log_exemple2.txt] :
```
./projet_ter test2.txt -2 -t 40 -p 1000 -v exemple2.csv -g -o log_exemple2.txt
```

 - Sur le exemple 3 [./projet_ter test3.txt],
   * la simulation Population Centrée [-2],
   * pendant 15 secondes (150 000 pas) [-t 15],
   * avec un enregistrement des résultats tous les 100 pas (toutes les 0.01 secondes) [-p 100],
   * sans suivi de la progression (car ralentit la simulation),
   * avec les résultats dans le fichier "exemple3.csv" [-v exemple3.csv],
   * avec un affichage final dans gnuplot et le fichier de config gnp "plot_exemple3.csv.gnp" [-g],
   * avec un enregistrement des logs dans le fichier "log_exemple2.txt" [-o log_exemple3.txt] :
```
./projet_ter test3.txt -2 -t 15 -p 100 -v exemple3.csv -g -o log_exemple3.txt
```
