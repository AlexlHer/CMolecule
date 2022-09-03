// Auteur : Alexandre l'Heritier

/*
	Méthode de GestionEntiteCentre (GEC)
	GEC est une implémentation de l'algorithme Entité Centrée présenté en cours.
	Cette classe permet de gérer la simulation et permet de sortir un résultat sous forme csv.
*/
#include "GestionEntiteCentre.hpp"
#include <math.h>
#include <random>
#include <ctime>

#define PI 3.14159265358979323846

/**
 * Constructeur.
 * 
 * @param e Le pointeur vers un vector d'EspeceMoleculaire contenant des pointeurs vers des EspeceMoleculaire.
 * @param r Le pointeur vers un vector de Reaction contenant des pointeurs vers des Reaction.
 * @param d Le diamètre de la vésicule.
 * @param log Le fux de sortie de log.
 * @param decoupage Le découpage à appliquer (le nombre de case).
 * @param nbThread Le nombre de thread à utiliser.
 */
GestionEntiteCentre::GestionEntiteCentre(std::vector<EspeceMoleculaire *> *e, std::vector<Reaction *> *r, int d, std::ofstream &log, int decoupage, int nbThread)
{
	log << std::endl << "-----Création de la simulation Entité Centré-----" << std::endl;
	log << std::endl << "Nombre de processeurs logiques disponibles : " << std::endl;

	especesMoleculaires = e;
	reactions = r;
	diametreVesicule = d;

	// Nombre de molécule créés.
	nbMolecules = 0;

	// Si le nombre de molécule dans les Cases est correct (sinon, necessite une synchronisation).
	nbMolIsSynchro = false;

	// Le nombre de tour déjà effectué.
	toursSimu = 0;

	// La taille de la plus grande molécule.
	taillePGMolecule = 0;

	// Le découpage de la vésicule en cases de taille "taillePGMolecule".
	int tailleGrilleVesicule = 0;

	// La liste des molécules créées.
	std::vector<Molecule *> molecules;

	// Si la diffusion est activé.
	diffusion = true;

	// Le nombre de case "entre les cases".
	nbCaseEntre = 0;

	// On détermine le nombre de thread et le découpage de la vésicule selon ce qui nous ai donné.
	// Si on doit décider des deux.
	if(nbThread == -1 && decoupage == -1)
	{
		// sqrt3(nbThreadMax) donne le découpage.
		decoupage = pow(1, 1.0/3.0);

		// On prend le nombre de processeurs logiques du PC comme nbThread.
		nbThread = 1;

		// Une case par thread.
		nbCasesParThread = 1;
	}

	// Si on doit décider du nombre de thread.
	else if(nbThread == -1)
	{
		// On prend le nombre de processeurs logiques du PC comme nbThread.
		nbThread = 1;
		nbCasesParThread = ceil(pow(decoupage, 3) / nbThread);
	}

	// Si on doit décider du nombre de case de la vésicule.
	else if (decoupage == -1)
	{
		// On prend le nbThread comme le nombre de case qu'on souhaite.
		decoupage = pow(nbThread, 1.0 / 3.0);
		nbCasesParThread = ceil(pow(decoupage, 3) / nbThread);
	}

	// Si on nous donne les deux.
	else
	{
		nbCasesParThread = ceil(pow(decoupage, 3) / nbThread);
	}

	log << "Nombre de threads utilisés par la simulation : " << nbThread << std::endl;
	log << "Découpage de la grille de parallélisation : " << decoupage << "x" << decoupage << "x" << decoupage << " cases (soit " << decoupage * decoupage * decoupage << " cases)." << std::endl;
	log << "Répartition des cases dans les threads : " << nbCasesParThread << " case(s) par thread." << std::endl;

	// s'il n'y a qu'une case, pas besoin du mode précision.
	modePrecision = (decoupage == 1 ? false : true);

	// On initialise rand avec time comme graine.
	srand((unsigned int)time(NULL));

	log << "Diamètre de la vésicule : " << d << " nm." << std::endl;
	log << "Mode diffusion " << (diffusion ? "activé." : "désactivé.") << std::endl;

	// On crée toutes les molécules à mettre dans la vésicule.
	// On en profite pour compter les molécules.
	// Pour chaque EspeceMoleculaire.
	for(int i = 0; i < especesMoleculaires->size(); i++)
	{
		// On génère autant de molécules que l'EspeceMoleculaire doit posséder.
		for(int j = 0; j < (*especesMoleculaires)[i]->getNbMolecule(diametreVesicule); j++)
		{
			molecules.push_back(new Molecule((*especesMoleculaires)[i], nbMolecules));
			nbMolecules++;
		}

		// On cherche la taille de la plus grande molécule.
		if ((*especesMoleculaires)[i]->getTaille() > taillePGMolecule)
		{
			taillePGMolecule = (*especesMoleculaires)[i]->getTaille();
		}
	}

	// LastId permet de donner un id unique à chaque molécule pour pouvoir faire ==.
	lastId = nbMolecules;

	log << "Nombre de molécules générées : " << nbMolecules << std::endl;


	// +1 pour être sûr que la vésicule est fermé.
	tailleGrilleVesicule = (diametreVesicule / taillePGMolecule) + 1;

	// Si le découpage fait que deux molécules ne passe pas dans une case, on ajuste.
	if(decoupage > tailleGrilleVesicule*2) 
	{
		log << "Réajustement du découpage : " << decoupage << " -> " << tailleGrilleVesicule * 2 << std::endl;
		decoupage = tailleGrilleVesicule*2;
	}

	// On arrondi au sup.
	if (diametreVesicule / taillePGMolecule - (int)(diametreVesicule / taillePGMolecule) >= 0.5)
	{
		tailleGrilleVesicule++;
	}

	// x * y * z;
	// Première verification avec la taille du "cube" de la vésicule.
	if (tailleGrilleVesicule * tailleGrilleVesicule * tailleGrilleVesicule < nbMolecules)
	{
		std::cerr << "Impossible de créer la simulation : pas possible de mettre toutes les molécules (" << nbMolecules << ") dans la vésicule (moins de " << tailleGrilleVesicule * tailleGrilleVesicule * tailleGrilleVesicule << " cases)." << std::endl;
		return;
	}


	log << "Création de la grille de répartition." << std::endl;

	// Découpage de la vésicule en case.
	tailleCase = diametreVesicule / decoupage;
	tailleGrille = decoupage;
	nbCase = tailleGrille * tailleGrille * tailleGrille;

	log << "Taille de la grille de répartition : " << tailleGrille << "x" << tailleGrille << "x" << tailleGrille << " cases (soit " << tailleGrille * tailleGrille * tailleGrille << " cases)." << std::endl;
	log << "Taille d'une case de la grille de répartition : " << tailleCase << " nm." << std::endl;

	// On récupère la case centrale de la grille.
	centreCube = tailleGrille / 2;

	// On crée les tableaux comme en cours.
	q = new Case[tailleGrille * tailleGrille * tailleGrille];
	p = new Case *[tailleGrille * tailleGrille];
	grille = new Case **[tailleGrille];

	// Création de l'index de q dans p (passage 3D -> 2D).
	for (int i = 0; i < tailleGrille; i++)
	{
		for (int j = 0; j < tailleGrille; j++)
		{
			p[j + i * tailleGrille] = q + (j * tailleGrille + (i * tailleGrille * tailleGrille));
		}
	}

	// Création de l'index de p dans grille (passage 2D -> 1D).
	for (int i = 0; i < tailleGrille; i++)
	{
		grille[i] = p + (i * tailleGrille);
	}

	// On donne la position de la case à la case.
	for(int k = 0; k < tailleGrille; k++)
	{
		for(int j = 0; j < tailleGrille; j++)
		{
			for(int i = 0; i < tailleGrille; i++)
			{
				grille[k][j][i].setIJK(i, j, k);
			}
		}
	}

	log << "Création de la grille de répartition terminée." << std::endl;

	// Placement des molécules dans les cases vide.
	float visionX, visionY, visionZ;
	int posActuelI = 0, posActuelJ = 0, posActuelK = 0;

	// Permet de centrer les molécules dans la "case" IJK.
	float centreEspace = 1/taillePGMolecule - centreCube;

	// Ici, on place les molécules dans la vésicule de manière à ce que les molécules ne se rentre pas dedans dès le début.
	// Algorithme initialement concu pour la création d'une vésicule dans le cube.
	// Le point visionXYZ est le point de la case IJK le plus proche du centre de la vésicule.
	int i = 0;
	for (posActuelK; posActuelK < tailleGrilleVesicule; posActuelK++)
	{
		visionZ = ((posActuelK - centreCube) * taillePGMolecule) + taillePGMolecule * posActuelK / tailleGrilleVesicule;
		for (posActuelJ; posActuelJ < tailleGrilleVesicule; posActuelJ++)
		{
			visionY = ((posActuelJ - centreCube) * taillePGMolecule) + taillePGMolecule * posActuelJ / tailleGrilleVesicule;
			for (posActuelI; posActuelI < tailleGrilleVesicule; posActuelI++)
			{
				visionX = ((posActuelI - centreCube) * taillePGMolecule) + taillePGMolecule * posActuelI / tailleGrilleVesicule;

				// Si on se trouve dans la vésicule.
				if(sqrt(
					visionX * visionX +
					visionY * visionY +
					visionZ * visionZ
				) <= diametreVesicule / 2)
				{
					// On donne la nouvelle position à la molécule.
					molecules[i]->setPosX(posActuelI + centreEspace);
					molecules[i]->setPosY(posActuelJ + centreEspace);
					molecules[i]->setPosZ(posActuelK + centreEspace);

					// On l'a met dans la case correspondante.
					grille[posEspaceToPosGrille(molecules[i]->getPosZ())]
						  [posEspaceToPosGrille(molecules[i]->getPosY())]
						  [posEspaceToPosGrille(molecules[i]->getPosX())]
							  .addMolecule(molecules[i], true, true);

					i++;
				}

				// Si i == nbMolecule, c'est qu'on a réussi à placer toutes les molécules.
				if(i == nbMolecules){
					posActuelJ = tailleGrilleVesicule;
					posActuelK = tailleGrilleVesicule;
					break;
				}
			}
			posActuelI = 0;
		}
		posActuelJ = 0;
	}

	// Si on arrive à la dernière itération des boucles, il n'y a plus de place.
	if (i != nbMolecules)
	{
		std::cerr << "Erreur GEC : Pas assez de place." << std::endl;
		return;
	}	

	log << "Positionnement des molécules dans la grille terminé." << std::endl;
	log << "Création de la liste des cases de la grille non vide terminée." << std::endl;
	log << "Mode précision " <<  (modePrecision ? "activé." : "désactivé.") <<std::endl;

	// Si le mode précision est activé.
	if(modePrecision){
		log << "Créaction des cases \"entre les cases\" pour plus de précision." << std::endl;

		// On donne un ID à chaque case.
		for (int i = 0; i < tailleGrille * tailleGrille * tailleGrille; i++)
		{
			q[i].setId(i);
		}

		// Le nombre de case "entre les cases".
		nbCaseEntre = 3 * (decoupage - 1);

		// On crée les tableaux.
		allCaseEntre = new Case[nbCaseEntre];

		// Les cases "entre les cases" de l'axe X dans tableau n°0, Y dans n°1, Z dans n°2.
		caseEntre = new Case*[3];

		for(int i = 0; i < 3; i++)
		{
			caseEntre[i] = allCaseEntre + (i * (decoupage - 1));
		}
	}


	log << "-----Simulation initialisée-----" << std::endl;
}

/**
 * Méthode permettant de réaliser "nbPas" pas.
 * @param nbPas Le nombre de pas à réaliser.
 */
void GestionEntiteCentre::simulation(int nbPas = 1)
{
	// Ajoute le nombre de tours déjà faits pour la boucle for.
	nbPas += toursSimu;

	// Les nombres de molécules ne sont pas synchronisés.
	nbMolIsSynchro = false;

	// 1 tour = 100 ms
	for (toursSimu; toursSimu < nbPas; toursSimu++)
	{
		// On effectue les déplacements des molécules.
		// On met le nombre de thread défini dans le constructeur.
		// Chaque thread s'occupe d'une Case.
		for (int i = 0; i < nbCase; i++)
		{
			deplacementMolecules(q[i]);
		}

		// On effectue les vérifications des collisions et on réalise les réactions.
		for (int i = 0; i < nbCase; i++)
		{
			collisionsReactions(q[i], false);
		}

		// Si on demande plus de précision.
		// Les cases du mode précision sont remplies par collisionsReactions.
		if(modePrecision)
		{
			// On effectue les collisions/réactions entre les cases.
			for (int i = 0; i < nbCaseEntre; i++)
			{
				collisionsReactions(allCaseEntre[i], true);
			}

			// On delete les molécules qui ont été supprimé à cause d'une réaction.
			Molecule* mi;
			for (int m = 0; m < nbCaseEntre; m++)
			{
				allCaseEntre[m].syncNbMolecule();
				for(int i = 0; i < allCaseEntre[m].getNbMolecule(); i++)
				{
					mi = allCaseEntre[m].getMolecule(i);
					if(mi->getASuppr())
					{
						// On supprime la molécule de toutes les cases où elle est présente.
						// (Elle peut être sur trois cases au max (X, Y, Z) si elle est vraiment au centre de la vésicule).
						for (int n = 0; n < nbCaseEntre; n++)
						{
							allCaseEntre[n].delMolecule(mi, true, false);
						}
						
						delete mi;
						i--;
					}
				}

				// On vide les cases (sans supprimer les molécules qui sont dedans).
				allCaseEntre[m].clsMolecule();
			}
		}
	}
	
	// Si on est en mode diffusion, les pas ne comptent pas.
	if(diffusion) toursSimu -= nbPas;
}

/**
 * Méthode permettant de réaliser les déplacements des molécules.
 * 
 * @param caseDeplac La case où se trouve les molécules à déplacer.
 */
void GestionEntiteCentre::deplacementMolecules(Case &caseDeplac) const
{
	Molecule *mo;
	float newX, newY, newZ, colatitude, longitude;
	int newI, newJ, newK;

	// On synchronise le nombre de molécule de la case.
	caseDeplac.syncNbMolecule();

	// On étudie toutes les molécules.
	for (int j = 0; j < caseDeplac.getNbMolecule(); j++)
	{
		// On récupère le pointeur de la molécule j.
		mo = caseDeplac.getMolecule(j);

		// On profite de la boucle pour "dire" à la molécule qu'elle n'a pas encore fait de réaction.
		mo->setReaction(false);

		// On effectue un déplacement autour de la molécule.
		// Merci https://fr.wikipedia.org/wiki/Coordonn%C3%A9es_sph%C3%A9riques pour les formules !
		colatitude = (rand() % 180) * PI / 180;
		longitude = (rand() % 360) * PI / 180;

		// On calcule la nouvelle position de la molécule dans l'espace.
		newX = mo->getPosX() + (mo->getVitesse() * sin(colatitude) * cos(longitude));
		newY = mo->getPosY() + (mo->getVitesse() * sin(colatitude) * sin(longitude));
		newZ = mo->getPosZ() + (mo->getVitesse() * cos(colatitude));

		// Si la nouvelle position de la molécule ne l'a fait pas sortir de la vésicule, on la déplace.
		if (isInVesicule(mo->getTaille(), newX, newY, newZ))
		{
			newI = posEspaceToPosGrille(newX);
			newJ = posEspaceToPosGrille(newY);
			newK = posEspaceToPosGrille(newZ);

			// Si un déplacement de case à case est à faire.
			if (mo->getPosI() != newI || mo->getPosJ() != newJ || mo->getPosK() != newK)
			{
				// On effectue le changement de case.
				// (C'est Case qui s'occupe les sections critiques).
				caseDeplac.delMolecule(mo, true, true);
				
				// On demande à ce que l'incrémentation ne soit pas faite car le thread gérant 
				// la case de destination ne doit pas redéplacer la molécule.
				grille[newK][newJ][newI].addMolecule(mo, false, true);

				j--;
			}

			mo->setPosX(newX);
			mo->setPosY(newY);
			mo->setPosZ(newZ);
		}
	}
}

/**
 * Méthode permettant de gérer les collisions et les réactions.
 * 
 * @param caseDeplac La case où les collisions/réactions doivent être géré.
 * @param tourDePrecision Si cette méthode est appelé pour plus de précision.
 */
void GestionEntiteCentre::collisionsReactions(Case &caseDeplac, bool tourDePrecision)
{
	// On resychronise le nombre de molécule des cases.
	caseDeplac.syncNbMolecule();

	// S'il n'y a qu'une ou zero molécule dans la case, pas besoin de l'étudier.
	if (caseDeplac.getNbMolecule() >= 2)
	{
		Molecule* mi;
		Molecule* mj;

		for (int i = 0; i < caseDeplac.getNbMolecule(); i++)
		{
			mi = caseDeplac.getMolecule(i);

			for (int j = i + 1; j < caseDeplac.getNbMolecule(); j++)
			{
				mj = caseDeplac.getMolecule(j);

				// Si collision entre mi et mj.
				if (mi->getPosX() + (mi->getTaille() / 2) > mj->getPosX() - (mj->getTaille() / 2) 
				&& mi->getPosX() + (mi->getTaille() / 2) < mj->getPosX() + (mj->getTaille() / 2) 
				|| mi->getPosX() - (mi->getTaille() / 2) < mj->getPosX() + (mj->getTaille() / 2) 
				&& mi->getPosX() - (mi->getTaille() / 2) > mj->getPosX() - (mj->getTaille() / 2))
				{

					// Si on veut prendre en compte les réactions.
					if (!diffusion)
					{
						// Si la réaction déclenche la suppression de mj, on fait j-- (pour éviter de sauter une molécule)
						// et on continue (car si mj n'existe plus, plus besoin de l'étudier).
						if (reactionBimol(mi, mj, tourDePrecision))
						{
							j--;
							continue;
						}
					}

					// Comme les molécules peuvent être dans plusieurs cases lors des tours de précision, on verifie
					// qu'elles ne sont pas déjà supprimé.
					if (!tourDePrecision || (!mi->getASuppr() && !mj->getASuppr()))
					{
						// Comme pour les réactions, pour éviter de sauter une case.
						if (collision(mi, mj))
						{
							j--;
						}
					}
				}
			}

			// Si on veux les réactions monomoléculaire.
			if (!diffusion)
			{
				reactionMonomol(mi);
			}

			// Si on n'est pas dans un tour de précision mais que le
			// mode précision est actif et que la molécule mi n'a pas déjà fait 
			// sa réaction, on regarde si cette molécule est entre deux cases. 
			if (!tourDePrecision && modePrecision && !mi->getReaction())
			{
				entreDeux(mi);
			}
		}
	}
}

/**
 * Méthode permettant de faire un déplacement de molécule suite à une collision.
 * 
 * @param mi La molécule impliquée dans la collision.
 * @param mj La molécule impliquée dans la collision.
 * @return true si cette collision a fait sortir mj de sa Case, false sinon.
 */
bool GestionEntiteCentre::collision(Molecule *mi, Molecule *mj) const
{
	
	float newX, newY, newZ, lvp, colatitude, longitude;
	int newI, newJ, newK;
	bool colli = false;

	// On les repousse.
	// Longueur du vecteur de pénétration.
	newX = mj->getPosX() - mi->getPosX();
	newY = mj->getPosY() - mi->getPosY();
	newZ = mj->getPosZ() - mi->getPosZ();

	lvp = (mi->getTaille() / 2) + (mj->getTaille() / 2) - sqrt(powf(newX, 2) + powf(newY, 2) + powf(newZ, 2));

	// On décale j.
	if (newX > 0)
		newX = mj->getPosX() + lvp;
	else
		newX = mj->getPosX() - lvp;

	if (newY > 0)
		newY = mj->getPosY() + lvp;
	else
		newY = mj->getPosY() - lvp;

	if (newZ > 0)
		newZ = mj->getPosZ() + lvp;
	else
		newZ = mj->getPosZ() - lvp;

	// On fait repartir la molécule dans un autre sens (aléatoire, ça evitera de faire encore plus de calcul).
	// 90° dans la formule de la colatitude pour empécher la molécule de retourner dans l'autre molécule
	// (comme lvp est colinéaire avec le vecteur IJ, pas besoin d'autres calculs).
	colatitude = (rand() % 90) * PI / 180;
	longitude = (rand() % 360) * PI / 180;

	newX += lvp * sin(colatitude) * cos(longitude);
	newY += lvp * sin(colatitude) * sin(longitude);
	newZ += lvp * cos(colatitude);

	// Pareil que dans la partie "Déplacement", si ça sort, on ne fait pas.
	if (isInVesicule(mj->getTaille(), newX, newY, newZ))
	{
		newI = posEspaceToPosGrille(newX);
		newJ = posEspaceToPosGrille(newY);
		newK = posEspaceToPosGrille(newZ);

		mj->setPosX(newX);
		mj->setPosY(newY);
		mj->setPosZ(newZ);

		// Si changement de case.
		if (mj->getPosI() != newI || mj->getPosJ() != newJ || mj->getPosK() != newK)
		{
			grille[mj->getPosK()][mj->getPosJ()][mj->getPosI()].delMolecule(mj, true, true);
			grille[newK][newJ][newI].addMolecule(mj, false, true);

			return true;
		}

	}
	return false;
}

/**
 * Méthode permettant de faire une réaction bimoléculaire.
 * @param mi La première molécule à réagir.
 * @param mj La seconde molécule à réagir.
 * @param tourDePrecision Si on est dans un tour de précision.
 * @return true si la molécule à été delete, false sinon.
 */
bool GestionEntiteCentre::reactionBimol(Molecule *mi, Molecule *mj, bool tourDePrecision)
{
	// Si une des deux molécules a déjà réagit, on ne fait pas une seconde réaction.
	if((mi->getReaction() || mj->getReaction())) return false;

	bool suppr = false;

	// Réalisation des réactions impliquant i et j.
	// On regarde si elles réagissent entre elles.
	for (int k = 0; k < reactions->size(); k++)
	{
		// Si les deux molécules sont dans l'équation et que le rand est inferieur
		// à la proba de la réaction.
		if (((*reactions)[k]->isFstGauche(mi->getEM()) && (*reactions)[k]->isSndGauche(mj->getEM())) 
		|| (*reactions)[k]->isFstGauche(mj->getEM()) && (*reactions)[k]->isSndGauche(mi->getEM()) 
		&& ((float)rand()) / RAND_MAX <= (*reactions)[k]->getProba())
		{

			// [MULTIREA] A commenter si on veux plusieurs réactions en un tour.
			// On signale au molécules qu'elles ont fait leur réaction (pour éviter qu'elle
			// réagisse plusieurs fois).
			mi->setReaction(true);
			mj->setReaction(true);

			// On remplace l'espece moléculaire pointée par la molécule
			// (plus rapide que d'effacer et remettre une molécule dans le vector).
			mi->setEM((*reactions)[k]->getFstDroite());

			// S'il n'y a pas de seconde molécule qui réagit, on supprime j.
			if ((*reactions)[k]->getSndDroite() == nullptr)
			{
				// [MULTIREA] A décommenter si on veux plusieurs réactions en un tour.
				//mj->setReaction(true);

				// On décrémente nbMolecule de 1.
				nbMolecules--;
				
				// On supprime la molécule de la case.
				grille[mj->getPosK()][mj->getPosJ()][mj->getPosI()].delMolecule(mj, true, true);

				// Si on est dans un tour de precision, on ne delete pas la molécule, elle peut
				// encore être présente dans une autre case entre deux (supprimer la molécule
				// de toutes les cases prendrai plus de temps).
				if(tourDePrecision)
				{
					mj->setASuppr(true);
				}

				else
				{
					delete mj;
					suppr = true;
				}
				
			}

			// Sinon, même manip qu'avec i.
			else
			{
				mj->setEM((*reactions)[k]->getSndDroite());
			}
			break;
		}
	}
	return suppr;
}

/**
 * Méthode permettant de faire une réaction monomoléculaire.
 * @param mi La molécule qui peut réagir.
 */
void GestionEntiteCentre::reactionMonomol(Molecule *mi)
{
	// Si la molécule a déjà fait sa réaction, on n'en fait pas une deuxième.
	if(mi->getReaction()) return;

	Molecule *mo;

	// On regarde toutes les réactions.
	for (int k = 0; k < reactions->size(); k++)
	{
		// S'il n'y a pas de membre droit dans le membre gauche de l'équation,
		// que la molécule est dans le membre gauche de l'équation et que
		// le rand est inferieur à la proba de la réaction.
		if ((*reactions)[k]->getSndGauche() == nullptr 
		&& (*reactions)[k]->isFstGauche(mi->getEM()) 
		&& ((float)rand()) / RAND_MAX <= (*reactions)[k]->getProba())
		{
			// On remplace l'espece moléculaire pointée par la molécule.
			mi->setEM((*reactions)[k]->getFstDroite());

			// [MULTIREA] A commenter si on veux plusieurs réactions en un tour.
			// On dit que la molécule a déjà fait sa réaction.
			mi->setReaction(true);

			// S'il y a une seconde molécule dans le membre droit de l'équation,
			// on la créé.
			if ((*reactions)[k]->getSndDroite() != nullptr)
			{
				// On crée une molécule.
				mo = new Molecule((*reactions)[k]->getSndDroite(), lastId);
				lastId++;
				nbMolecules++;

				// On l'a place à coté de l'autre molécule, vers le centre de la vésicule
				// (pour être sûr de ne pas sortir de la vésicule).
				if (mi->getPosX() < 0)
					mo->setPosX(mi->getPosX() + mo->getTaille() + mi->getTaille() / 2 + 1);
				else
					mo->setPosX(mi->getPosX() - mo->getTaille() - mi->getTaille() / 2 + 1);

				if (mi->getPosY() < 0)
					mo->setPosY(mi->getPosY() + mo->getTaille() + mi->getTaille() / 2 + 1);
				else
					mo->setPosY(mi->getPosY() - mo->getTaille() - mi->getTaille() / 2 + 1);

				if (mi->getPosZ() < 0)
					mo->setPosZ(mi->getPosZ() + mo->getTaille() + mi->getTaille() / 2 + 1);
				else
					mo->setPosZ(mi->getPosZ() - mo->getTaille() - mi->getTaille() / 2 + 1);

				// [MULTIREA] A commenter si on veux plusieurs réactions en un tour.
				mo->setReaction(true);
				
				// On l'ajoute dans la case.
				grille
					[posEspaceToPosGrille(mo->getPosZ())]
					[posEspaceToPosGrille(mo->getPosY())]
					[posEspaceToPosGrille(mo->getPosX())]
						.addMolecule(mo, false, true);
			}
			break;
		}
	}
}

/**
 * Méthode permettant d'obtenir la case où se situe la molécule.
 * 
 * @param i La position dans l'espace de la molécule (ça peut être x ou y ou z, comme la vésicule est carré, c'est le même calcul).
 * @return La case de la molécule.
 */
int GestionEntiteCentre::posEspaceToPosGrille(float i) const
{
	// Exemple :
	// - diametreVesicule = 1000
	// - tailleCase = 10

	// -500 -> 500 	| i
	// -50 -> 50 	| i / tailleCase
	// 0 -> 100 	| (i / tailleCase) + centreCube
	int fin = (i / tailleCase) + centreCube;

	// Si le calcul implique une sortie de la grille, on remet la molécule dans la grille.
	// N'est pas censé se produire sauf dans entreDeux().
	if(fin < 0) {
		return 0;
	}

	if(fin >= tailleGrille) {
		return tailleGrille - 1;
	}

	return fin;
}

/**
 * Méthode permettant de savoir si une molécule est toujours dans la vésicule.
 * @param size La taille de la molécule.
 * @param xyz La position de la molécule.
 * @return true si la molécule est toujours dans la vésicule.
 */
inline bool GestionEntiteCentre::isInVesicule(float size, float x, float y, float z) const
{
	return sqrt(x*x + y*y + z*z) + size/2 < diametreVesicule/2;
}

/**
 * Méthode permettant de sychroniser le nombre d'individu de l'especeMoleculaire.
 */
void GestionEntiteCentre::synchroNbIndividu()
{
	// Si le nombre de molécule est déjà synchro, pas besoin de le refaire.
	if(nbMolIsSynchro) return;

	// On met le nombre d'individu de toutes les especeMoleculaire à 0.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		(*especesMoleculaires)[i]->setNbIndividu(0);
	}

	// Pour chaque molécule, on ajoute un individu dans son especeMoleculaire.
	for (int i = 0; i < nbCase; i++)
	{
		q[i].syncNbMolecule();
		for (int j = 0; j < q[i].getNbMolecule(); j++)
		{
			q[i].getMolecule(j)->getEM()->setNbIndividu(q[i].getMolecule(j)->getEM()->getNbIndividu() + 1);
		}
	}

	// C'est maintenant synchro.
	nbMolIsSynchro = true;
}

/**
 * Méthode permettant de voir si la molécule est entre deux cases.
 * @param m La molécxule à étudier.
 * Necessite IJK mis à jour. 
 */
void GestionEntiteCentre::entreDeux(Molecule *m)
{
	// On récupère l'id de la case.
	int idCase = grille[m->getPosK()][m->getPosJ()][m->getPosI()].getId();

	// Si dépassement case x+1.
	// posETG renvoie tailleGrille-1 si sortie, donc le if ne sera pas fait (car =).
	if (m->getPosI() != posEspaceToPosGrille(m->getPosX() + taillePGMolecule / 2))
	{
		// On retrouve la case "entre deux" correspondante.
		caseEntre[0][idCase % tailleGrille].addMolecule(m, true, false);
	}

	// Si dépassement case x-1.
	else if (m->getPosI() != posEspaceToPosGrille(m->getPosX() - taillePGMolecule / 2))
	{
		caseEntre[0][idCase % tailleGrille - 1].addMolecule(m, true, false);
	}

	// Si dépassement case y+1.
	// posETG renvoie tailleGrille-1 si sortie, donc le if ne sera pas fait (car =).
	if (m->getPosJ() != posEspaceToPosGrille(m->getPosY() + taillePGMolecule / 2))
	{
		caseEntre[1][int((idCase / tailleGrille) % tailleGrille)].addMolecule(m, true, false);
	}

	// Si dépassement case y-1.
	else if (m->getPosJ() != posEspaceToPosGrille(m->getPosY() - taillePGMolecule / 2))
	{
		caseEntre[1][int((idCase / tailleGrille) % tailleGrille) - 1].addMolecule(m, true, false);
	}

	// Si dépassement case z+1.
	// posETG renvoie tailleGrille-1 si sortie, donc le if ne sera pas fait (car =).
	if (m->getPosK() != posEspaceToPosGrille(m->getPosZ() + taillePGMolecule / 2))
	{
		caseEntre[2][int(idCase / (tailleGrille * tailleGrille))].addMolecule(m, true, false);
	}

	// Si dépassement case z-1.
	else if (m->getPosK() != posEspaceToPosGrille(m->getPosZ() - taillePGMolecule / 2))
	{
		caseEntre[2][int(idCase / (tailleGrille * tailleGrille)) - 1].addMolecule(m, true, false);
	}
}

/**
 * Méthode permettant de print l'etat de la simulation.
 */
void GestionEntiteCentre::print()
{
	synchroNbIndividu();
	std::cout << "temps : " << toursSimu << " nbMolecules : " << nbMolecules << std::endl;

	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		std::cout << (*especesMoleculaires)[i]->getNom() << " : " << (*especesMoleculaires)[i]->getNbIndividu() << std::endl;
	}
}

/**
 * Méthode permettant d'initialiser le csv.
 * @param fichier Le flux du csv.
 */
void GestionEntiteCentre::initCsv(std::ofstream &fichier) const
{
	// On écrit les titres.
	fichier << "toursSimu;tempsSimu;nbMolecules;";

	// Les titres correspondent aux nom des especeMoleculaire.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		fichier << (*especesMoleculaires)[i]->getNom() << ";";
	}

	fichier << std::endl;
}

/**
 * Méthode permettant d'écrire l'état du système dans le csv.
 * @param fichier Le flux du csv.
 */
void GestionEntiteCentre::sortieCsv(std::ofstream &fichier)
{
	// On synchronise le nombre d'individu.
	synchroNbIndividu();

	// On écrit l'état de la simulation.
	fichier << toursSimu << ";" << ((float)toursSimu / 10000.) << ";" << nbMolecules << ";";

	// Et le nombre d'individu de chaque espece.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		fichier << (*especesMoleculaires)[i]->getNbIndividu() << ";";
	}
	fichier << std::endl;
}

/**
 * Méthode permettant de créer le gnp pour le csv.
 * @param fichier Le flux du gnp.
 * @param nomEntre Le nom du fichier "modèle".
 * @param nomSortie Le nom du fichier csv.
 * @param seconde Si l'utilisateur a choisi des secondes (true) ou un nombre de tour.
 */
void GestionEntiteCentre::sortieGnp(std::ofstream &fichier, std::string nomEntre, std::string nomSortie, bool seconde) const
{
	// On configure le graphique.
	fichier << "set datafile separator ';'" << std::endl
			<< "set grid" << std::endl
			<< "set ylabel \"Nombre de molécules\"" << std::endl
			<< "set title \"Modèle '" << nomEntre << "' / Algorithme Entité Centrée\"" << std::endl;

	// On met le bon label.
	if (seconde)
	{
		fichier << "set xlabel \"Temps (s)\"" << std::endl;
	}
	else
	{
		fichier << "set xlabel \"Tour\"" << std::endl;
	}

	// On plot le nombre de molécule.
	fichier << "plot '" << nomSortie << "' using " << (seconde ? 2 : 1) << ":3 with lines title 'Nb molécules'" << std::endl;

	// On plot le nombre d'individu de chaque espece.
	for(int i = 0; i < especesMoleculaires->size(); i++)
	{
		fichier << "replot '" << nomSortie << "' using " << (seconde ? 2 : 1) << ":" << i + 4 << " with lines title '" << (*especesMoleculaires)[i]->getNom() << "'" << std::endl;
	}

	fichier << "pause -1 'Appuyer sur Entrer pour terminer le programme.'" << std::endl;
}

/**
 * Destructeur.
 */
GestionEntiteCentre::~GestionEntiteCentre()
{
	delete[] q;
}