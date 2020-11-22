
# Projet TER Simulation

Projet TER Simulation (M1S2 - 2019-2020)

&nbsp;

# Changelog

## [Build 200509.1] - 2020-05-09

### Changed

- Suppression des .cpp de Molecule et de Reaction (inutile).
- Ajout de quelques const.
- Le mode précision est maintenant intégré d'office (sauf si une seul case) au vu de la faible surcharge de travail.

### Fixed

- Correction d'une erreur dans GPC qui oubliait des réactions.

&nbsp;
___
&nbsp;


## [Build 200508.1] - 2020-05-08

### TODO

- Les destructeurs
- Voir pour l'échelle de temps PC / EC.
- Voir pourquoi l'opti O3 produit des bugs de suppression.
- Optimiser le code avec des const.
- Commentaires (arret dans GEC) / Rapport

### Added

- Ajout de la génération d'un gnp pour l'affichage des courbes avec gnuplot.
- Ajout de l'ouverture de gnuplot à la fin de la simulation.
- Ajout d'une option pour mettre un temps en seconde.

### Changed

- Suppression de l'algorithme GestionEntiteCentreQuadSeq.
- La classe GestionEntiteCentreParLine devient GestionEntiteCentre.
- Amélioration de l'aide.
- Adaptation des graphiques selon si on choisi "tour" ou "seconde".
- Le mode précision effectue maintenant les déplacements de collisions.
- Case gère maintenant le changement de IJK des molécules.
- Suppression de l'algo de précision parallèle (inefficace, pas assez de cases / difficile à gérer).

### Fixed

- Le nombre de tour demandé est maintenant fait correctement.
- Les temps des deux algos sont maintenant "synchronisé" sur "un tour = 100ms".

&nbsp;
___
&nbsp;


## [Build 200507.1] - 2020-05-07

### TODO

- Suppression de l'algo GECQS.
- Mettre les params precision.

### Added

- Mode précision.
- Découpage du mode précision : Simple (parallèle) et ++ (Séquenciel).

### Changed

- Mode precision :
	- Gestion de la double case pour les molécules.
	- Déplacement de l'appel de entreDeux dans la fin de collisionReaction (comme ça, plus de molécules "fantome").
	- Plusieurs choses pour ne pas perdre de molécules.

### Fixed

- Correction d'une erreur au niveau de reactionMono().
- Correction (normalement) d'un segfault au niveau de reactionMono().

&nbsp;
___
&nbsp;


## [Build 200506.1] - 2020-05-06

### TODO

- Suppression de l'algo GECQS.

### Added

- Mode précision++ (ne fonctionne pas pour l'instant TODO).

### Changed

- Utilisation de mutex pour les Cases.
- Suppression de la grille de placement devenu totalement inutile et bien trop lourd.
- Découpage de la simulation GECLP en plusieurs méthodes.

### Fixed

- Correction d'une erreur dans isInVesicule (diametreVesicule/2). Cela provoquait un SegFault et d'autres erreurs de "disparition" de molécule.
- Correction d'une erreur dans les tours de diffusions.
- Ajout d'un bout de code dans main sous la boucle de simulation pour faire tous les tours demandé.

&nbsp;
___
&nbsp;


## [Build 200415.1] - 2020-04-15

### TODO
- Erreur "bbb" (rare) et "ccc" (extremement rare).
- Trouver comment déclarer des section critical avec nom perso.
- Suppression de l'algo GECQS.

### Fixed

- L'algo GPC fonctionne enfin !

&nbsp;
___
&nbsp;


## [Build 200412.1] - 2020-04-12

### TODO
- Erreur "bbb" (rare) et "ccc" (extremement rare).
- Trouver comment déclarer des section critical avec nom perso.
- Suppression de l'algo GECQS.

### Added

- Ajout d'une méthode GECLP::synchroNbIndividu() pour synchroniser le nombre de molécule de tous les espèces moléculaire (avant, c'était print() qui le faisait).
- Ajout de la méthode GECLP::sortieCsv() permettant d'écrive dans le flux de sortie (dans un .csv ici).
- (Enfin) Ajout de la boucle for pour le nombre de pas de GECLP::simulation().
- Ajout des modifications csv/log à la classe GPC.
- Ajout des arguments à main. Tous les arguments qui me passais par la tête. Implémentation de certains, pour l'instant.
- Ajout l'une classe abstaite Gestion avec quelques méthodes en commun avec toutes les classe de gestion pour pouvoir l'utiliser dans main.
- Ajout de tous les arguments (sauf interface graphique).

### Changed

- Modification du istream en ifstream, pour la lecture du fichier par lex/yacc. 
- Systeme return maintenant des pointeurs de GestionX.
- Suppression de "entreDeux" : inutile et lent.

### Fixed

- Résolution d'un (très vieux) bug : "warning: ISO C++ forbids converting a string constant to ‘char*’".

&nbsp;
___
&nbsp;

## [Build 200404.1] - 2020-04-04

### TODO
- factoriser le code simulation.
- Trouver comment déclarer des section critical avec nom perso.

### Added

- 

### Changed

- Quelques optimisations simples.
- Suppression de l'état de Case (devenu inutile).
- Suppression de explore de Case (devenu inutile).
- Simplification de Case : ce n'est maintenant qu'une classe qui gère un vector<Molecule*>.
- Suppression de tous les critical de GECLP::simulation().
- Corrections dans le code GPC mais c'est pas encore ça...

### Fixed

- Analyse du problème de Case::getMolecule() :
	- Dans tous les cas, c'est dans la partie collisions/réactions.
	- Non lié à "schedule(dynamic, nbCasesParThread)"
	- En mettant des "#pragma omp critical (addMol)" autour des deux getMolecule, ça a l'air d'etre bon mais moins rapide.
	- Sans critical mais avec décalage des mi et mj, ça semble fonctionner sans réaction mono-moléculaire (test sur 100000 tours) : la cause est peut-être le addMolecule de l'algo.
	- Test avec une taille de vector mis dans une variable et géré avec des omp atomic (simplification de Case).
	- Même si le problème arrive moins souvent, il arrive et c'est peut-être à cause de la lecture de nbMolecule : atomic ne l'empeche pas. Essai avec critical, plus radical et peut ralentir mais comme ça, on verra où est le problème.
	- Ne fonctionne pas. Modification de l'algorithme : le problème est nbMoleculesCase donc intégration directement dans Case. Comme le but de nbMoleculesCase est de ne pas compter les nouvelles molécules (car déjà déplacées), il y aura le compteur de molecule dans les cases qui ne sera pas incrémenté de 1 lors de l'ajout d'une molécule mais sera décrémenté de 1 lors de la suppression d'une molécule.
	- Problème à priori résolu mais 

- Retour du problème de non suppression de molécule sndGauche. Résolu : lors de la suppression d'une molécule, on regarde toute la liste des molécules de Case, pas jusqu'a nbMolecule (mais nbMolecule reste lors d'un getMolecule(), si l'algo fonctionne avec la modification d'au-dessus, il ne devrai pas y avoir de problème).

- Ajout d'une vérification de la valeur de decoupage : si elle est plus grande que tailleGrilleVesicule, ça ne peut pas fonctionner puisqu'une case ne pourra pas contenir une seule des molécules avec la plus grande taille.

&nbsp;
___
&nbsp;

## [Build 200330.1] - 2020-03-30

### TODO
- factoriser le code simulation.
- plantage aléatoire : Erreur Case::getMolecule() : Case demandée : 17 sur 17 lorsqu'il y a trop de cases/thread.

### Added

- Ajout de la classe GestionPopulationCentre.
- Ajout du nombre d'individu dans EspeceMoleculaire.
- Ajout de la simulation de GPC (mais ne fonctionne pas du tout).

&nbsp;
___
&nbsp;

## [Build 200329.1] - 2020-03-29

### TODO
- factoriser le code simulation.
- plantage aléatoire : Erreur Case::getMolecule() : Case demandée : 17 sur 17 lorsqu'il y a trop de cases/thread.

### Added

- Ajout d'une partie "seconde chance" dans GECLP::simulation() qui permet de faire la réaction entre les molécules à la limite des cases. Moyennement utile lorsqu'il n'y a pas de diffusion avant réaction (+60 réactions avec 6000 molécules donc 1% plus précis, bof) mais totalement inutile lorsqu'il y a une diffusion initiale (+5 réactions, soit 0.1%).

### Changed

- Pour les réactions, j = i + 1 initialement maintenant.
- Suppression de tous les "casesOccu" (c'était utile quand y'avait des milliers de cases, maintenant il y en a une dizaine, donc inutile).
- Le nombre de thread est maintenant égal au nombre de cases.

### Fixed

- Ajout d'un break a la fin de réaction pour éviter d'avoir plusieurs réactions pour deux mêmes molécules.

### Other

- 

&nbsp;
___
&nbsp;

## [Build 200306.1] - 2020-03-06

### Added

- Ajout des parties OpenMP dans simulPasLineSeq() (qui devient donc simulPasLinePar())

### Changed

- Découpage de la classe GEC en deux version : Une version Quadratique Séquencielle, une autre Linéaire Parallèle.
- Nettoyage de la version QuadSeq.
- Nettoyage de la version LinePar.
- Suppression de la méthode "GECLP::tempConstructCasesOccup()".
- Modification de Systeme pour supporter les deux nouvelles classes.
- Les méthodes simulPasLineSeq() et simulPasLinePar() sont renommées en simulation().
- Mise en com des parties qui ajoutent/suppriment des cases de casesOccu. Test par remplacement par schedule(dynamic, 2).

### Fixed

- Enfin, le problème de non-collision est corrigé (toutes les molécules avait la même position dans l'espace).
- Problème de création de molécule non voulu à cause du parallèlisme : Résolu en donnant le même nom à toutes les sessions critiques.
- [TODO] Un segFault dans la méthode Case::getMolecule() (parallèle) (très rare).

### Other

- Performance algo séq lin : (6000 molécules/diam180/une seule réaction)
	- Une unique case         : 1min8sec
	- 2\*2\*2=8 cases           : 6sec
	- 3\*3\*3=27 cases          : 4sec
- Performance algo par lin : (60000 molécules/diam1000/une seule réaction)
	- 2\*2\*2=8 cases / 1 thread (24% CPU)      : 7min4sec
	- 2\*2\*2=8 cases / 8 threads               : Bug repéré dans les logs : création de molécules non désiré.
- Performance algo par lin : (20000 molécules/diam2000/une seule réaction)
	- 2\*2\*2=8 cases / 1 thread (24% CPU)      : 1min11sec
	- 2\*2\*2=8 cases / 8 threads               : 14sec
	- 1.54
- Performance avec / sans casesOccu dynamique :
	- Avec    : 1min54sec
	- Sans    : 1min49sec
- Performance sans casesOccu dynamique :
	- Rien                    : 1min49sec
	- schedule(dynamic, 2)    : 3min45sec
	- schedule(dynamic, 1)    : 1min50sec

&nbsp;
___
&nbsp;

## [Build 200304.1] - 2020-03-04

### Other

- [A voir] Comment les add supp cases pour avoir le tableau de case non vide fixe. Même effet. Continuer la recherche.
- La problème de l'ancienne build doit venir des cases trop petites, ce qui fait que beaucoup de collisions ne sont pas détectées.
- Après application de la grille de parallélisation, test sur 1, 8 et 27 cases : le 1 est comme le séquenciel, les autres sont plus lent pour faire les réactions. impossible que ce soit le manque de collision entre les cases.

### Added

- Add item

### Changed

- Test pour se passer de la grille vésicule en la remplacant par le calcul de la sortie de vésicule direct (pythagore). Ancienne version : 13sec, nouvelle version : 10sec.
- Remplacement de l'ancienne version par la nouvelle pour réduire la dépendance à la grille. L'algo quad n'a plus besoin de grille.
- Le constructeur de GEC utilise maintenant un tableau de bool local pour placer les molécules dans l'espace (avec 'ancien code donc). La création de la grille est toujours dans le constructeur mais en bas.
- tempConstructCasesOccup() s'occupe de placer les molécules dans la grille maintenant.

### Fixed

- Fixed item

### Removed

- Remove item

### Security

- Security item

## [Build 200212.3] - 2020-02-12

### Added

- Ajout de quelques cout pour avoir des infos sur la simulation.
- Ajout de chrono dans main pour avoir une idée de la performance.
		Algo quadratique séquenciel : 10000 pas en 3159ms.
		Algo linéaire séquenciel : 10000 pas en 663ms.
		Algo linéaire parallèle : Bientôt...

### Fixed

- [A voir] GEC::simulPasLineSeq() fonctionne beaucoup plus rapidement que Quad mais n'a pas l'air de faire grand chose (par rapport à l'algo quad).

&nbsp;
___
&nbsp;

## [Build 200212.2] - 2020-02-12

### Added

- Ajout d'un id molécule, en cas de comparaison de deux molécules.
- Ajout de l'attribut "lastId" dans GEC pour avoir toujours un nouvelle id de molécule (au début, lastId = nbMolecule mais comme nbMolecule est décrémenté dans le code, il ne peut pas être utilisé comme id unique). Permet d'avoir aussi le nombre total de molécule créé (au cas où).

### Changed

- Amélioration du placement des nouvelles molécules créées avec une réaction (création à coté de la molécule source au lieu de même position).

### Fixed

- GEC::simulPasLineSeq() rerepris encore de presque zéro, avec un découpage plus net des étapes (quatre boucles for principales au lieu de deux) et plusieurs variables utilisé en condition de boucle pour rendre le code plus lisible.
- Erreur de duplication de molécule dans GEC::simulPasLineSeq() :
		Après une (longue) recherche, et grâce à la fonction print() qui n'affiche pas les mêmes infos en explorant la grille complètement et en explorant juste casesOccu, c'est juste des cases dupliqués dans casesOccu.
		Donc utilisation d'un bool devenu obselete de la classe Case pour tester une amélioration. L'amélioration fonctionne, plus de bug. Plus qu'a nettoyer le code (plus de cout que de code !).
- Partie réaction amélioré avec l'ajout du fait qu'une molécule n'est plus forcement dans la case qu'on étudie.
- Découverte d'un oubli dans l'algo QuadSeq au niveau des réactions.

&nbsp;
___
&nbsp;

## [Build 200212.1] - 2020-02-12

### Fixed

- GEC::simulPasLineSeq() repris de zéro sans gestion de la liste de molécule, maintenant chaque case a sa liste de molécule. Donc ajout d'une liste de case non vide, pour ne pas explorer toutes les cases à chaque fois.
- Toujours une erreur de duplication de molécule dans GEC::simulPasLineSeq(). 
- Oubli au niveau de la diffusion sans réaction.

&nbsp;
___
&nbsp;

## [Build 200209.1] - 2020-02-09

### Changed

- Liste de molécule dans GEC contient maintenant des pointeurs vers des molecules.

### Fixed

- Dans GEC::simulPasQuadSeq() et GEC::simulPasLineSeq(), ajout de i != j pour les collisions pour éviter de comparer la molécule avec elle-même.
- Dans GEC::simulPasQuadSeq() et GEC::simulPasLineSeq(), correction de la recherche de réaction (E + E pour une formule décrivant une réaction E + S passait).
- GEC::simulPasQuadSeq() / B -> S
- Debuggage approfondi de GEC::simulPasLineSeq() :
		- Manque de cohérence entre les affichages du nb de molécule et la réalité : Disparition dans la boucle de déplacement corrigée.
		- Bug de recherche réaction toujours présent malgré l'application de la même correction que GEC::simulPasQuadSeq(). Problème au niveau de la suppression de la molécule dans la liste molecules : Impossible de maintenir une liste de molécule dans GEC en même temps que plusieurs liste de molécules dans chaque case.

&nbsp;
___
&nbsp;

## [Build 200207.1] - 2020-02-07

### Added

- Les cases contiennent leurs molécules.
- Ajout de la méthode GEC::simulPasLineSeq() (mais segfault dans la partie collision normalement).

### Changed

- Optimisation de la classe Case avec changement d'état automatique (mais les méthodes assurant la compatibilité reste pour l'instant).

### Fixed

- Verifications ajoutées dans quelques accesseurs.

&nbsp;
___
&nbsp;

## [Build 200202.1] - 2020-02-02

### Added

- Collision de molécules.
- Réaction de molécule.
- Ajout d'une classe Case.

### Changed

- IJK sont maintenant mis à jour dans les molécules.
- Grattage de performance sur quelques calculs dans la méthode GEC::simulPasQuad().
- Remplacement de la grille de int par une grille de Case.
- Mise en place d'operators dans Case pour ne rien changer dans GEC.

### Fixed

- La méthode GEC::posEspaceToPosGrille() ne fonctionne pas. Rectification avec brouillon.
- Correction du calcul du déplacement des molécules dans la méthode GEC::simulPasQuad().

&nbsp;
___
&nbsp;

## [Build 200129.3] - 2020-01-29

### Added

- Implémentation de l'algorithme de collision quadratique (en diffusion seulement).

### Fixed

- Vérifications de non sortie de grille dans GEC::posEspaceToPosGrille().

&nbsp;
___
&nbsp;

## [Build 200129.2] - 2020-01-29

### Added

- Création de la vésicule dans le cube !
- Positionnement des molécules dans la vésicule.

&nbsp;
___
&nbsp;

## [Build 200129.1] - 2020-01-29

### Changed

- Passage du remplissage de p dans constructeur GEC d'une simple boucle à une double boucle imbriquée (beaucoup plus simple à débugger).

### Fixed

- Le parcours de la grille fonctionne sans segmentation fault.

&nbsp;
___
&nbsp;

## [Build 200127.1] - 2020-01-27

### Added

- Grille en 3D rajouté.
- Ajout du début de l'algorithme de recherche de cube dans/hors de la vésicule.

&nbsp;
___
&nbsp;

## [Build 200126.1] - 2020-01-26

### Added

- Création de la grille de stockage dans GestionEntiteCentre.
- Ajout de plusieurs attributs pour stocker differents paramètres.

### Changed

- Le calcul du nombre de molécule dans especeMoleculaire necessite maintenant la taille de la vesicule.

&nbsp;
___
&nbsp;

## [Build 200125.1] - 2020-01-25

### Added

- Ajout d'une redéfinition de YY_INPUT dans le lex, pour pouvoir gérer plus tard des fichiers non passés en entrée du programme.
- Ajout de la méthode de calcul du nombre de molécule dans EspeceMoleculaire.

&nbsp;
___
&nbsp;

## [Build 200124.1] - 2020-01-24

### Added

- Ajout de tous les getters dans EspeceMoleculaire.
- Création de la classe GestionEntiteCentre.

### Changed

- Mise en place de pointeurs au lieu de copies dans Reaction.
- especesMoleculaires et reactions dans Systeme sont maintenant des pointeurs.

&nbsp;
___
&nbsp;

## [Build 200123.1] - 2020-01-23

### Added

- Ajout de trois attributs dans Molecule, avec leurs accesseurs.

## [Build 200122.2] - 2020-01-22

### Added

- Méthode toString dans les classes "EspeceMoleculaire", "Reaction" et "Systeme".

### Changed

- Ajout d'un appel à toString dans "main.cpp".

&nbsp;
___
&nbsp;

## [Build 200122.1] - 2020-01-22

### Added

- Ajout des classes "EspeceMoleculaire" et "Reaction".

- Remplissage des differentes méthodes de Gestion en utilisant les nouvelles classes.

### Changed

- Renommage de la classe "Gestion" en "Systeme".

- Meilleure présentation des affichages des étapes.

### Todo

- Voir pour la mise en place d'un HashTable au lieu du vector pour le stockage des especes moleculaires dans "Systeme".

&nbsp;
___
&nbsp;

## [Build 200121.1] - 2020-01-21

### Added

- Ajout d'une classe Gestion avec uniquement des déclarations de méthodes.

- Ajout d'un objet Gestion dans "analyse.y".

- Ajout d'une fonction "recupGestion()" dans "analyse.y" pour récupérer l'objet Gestion créé à partir de la lecture du fichier cible. Ajout de la partie "%code requires" pour importer "Gestion.hpp" et déclarer la méthode "recupGestion()" dans le .hpp créé pour lex/yacc.

- Ajout de main.cpp pour gérer le lex et le yacc. Appelle la méthode recupGestion().

&nbsp;
___
&nbsp;

## [Build 200120.1] - 2020-01-20

### Added

- Makefile.

### Changed

- Amélioration des logs.

### Fixed

- Ajout d'un "strdup()" dans le Lex au niveau de "{ident}" pour résoudre le problème des "IDENT".

&nbsp;
___
&nbsp;

## [Build 200119.1] - 2020-01-19

### Added

- Log dans les fichiers.

### Todo

- Bug au niveau du yacc, les identifients "IDENT" ne fonctionne pas. Récupère des éléments après. Solution non trouvée.

&nbsp;
___
&nbsp;

## [Build 200118.2] - 2020-01-18

### Added

- Analyse syntaxique (analyse.y).

### Changed

- Ajout de l'inclusion du fichier "y.tab.hpp" dans "analyse.l.

&nbsp;
___
&nbsp;

## [Build 200118.1] - 2020-01-18

### Added

- Analyse lexicale (analyse.l).
