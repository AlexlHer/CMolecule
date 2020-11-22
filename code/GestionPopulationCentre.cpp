// Auteur : Alexandre l'Heritier

/*
	Méthode de GestionPopulationCentre (GPC)
	GPC est une implémentation de l'algorithme Population Centrée présenté en cours.
	Cette classe permet de gérer la simulation et permet de sortir un résultat sous forme csv.
*/

#include "GestionPopulationCentre.hpp"
#include <math.h>
#include <random>
#include <ctime>
#include <map>

#define PI 3.14159265358979323846

/**
 * Constructeur.
 * 
 * @param e Le pointeur vers un vector d'EspeceMoleculaire contenant des pointeurs vers des EspeceMoleculaire.
 * @param r Le pointeur vers un vector de Reaction contenant des pointeurs vers des Reaction.
 * @param d Le diamètre de la vésicule.
 * @param log Le fux de sortie de log.
 */
GestionPopulationCentre::GestionPopulationCentre(std::vector<EspeceMoleculaire *> *e, std::vector<Reaction *> *r, int d, std::ofstream &log)
{
	log << std::endl << "-----Création de la simulation Population Centré-----" << std::endl;
	especesMoleculaires = e;
	reactions = r;
	diametreVesicule = d;
	// On initialise le nombre d'individu directement.
	for (int i = 0; i < e->size(); i++)
	{
		(*especesMoleculaires)[i]->initNbIndividu(diametreVesicule);
		// log << (*especesMoleculaires)[i]->getNom() << " " << (*especesMoleculaires)[i]->getNbIndividu() << std::endl;
	}

	// L'initialisation de nbMolecules sera faite dans synchroNbMolecule().
	nbMolecules = 0;
	nbMolIsSynchro = false;
	pairImpair = false;

	toursSimu = 0;

	// On initialise les variables.
	volumeVesicule = PI * pow(((double)diametreVesicule / 1000.), 3) / 6.;
	log << "diametreVesicule : " << diametreVesicule << std::endl;
	log << "volumeVesicule : " << volumeVesicule << std::endl;
	srand((unsigned int)time(NULL));

	log << "-----Simulation initialisée-----" << std::endl;
}

/**
 * Méthode permettant de réaliser "nbPas" pas de simulation.
 * @param nbPas Le nombre de pas à réaliser.
 */
void GestionPopulationCentre::simulation(int nbPas = 1)
{
	Reaction* reac;
	double prop;

	// Ajoute le nombre de tours déjà faits pour la boucle for.
	nbPas += toursSimu;

	// Les nombres de molécules ne sont pas synchronisés.
	nbMolIsSynchro = false;

	// 1 tour = 100 ms
	for (toursSimu; toursSimu < nbPas; toursSimu++)
	{
		// Permet de faire un coup de haut en bas, un autre coup de bas en haut.
		pairImpair = !pairImpair;

		// On verifie toutes les réactions.
		for(int r = 0; r < reactions->size(); r++)
		{
			// On regarde si on prend des réactions de bas en haut ou de haut en bas.
			reac = (*reactions)[(pairImpair ? r : reactions->size() - 1 - r)];

			// Bi-mol : Si la seconde especeMol partie gauche (les réactifs) existe.
			if (reac->getSndGauche() != nullptr)
			{
				prop = (alpha * (double)reac->getFstGauche()->getNbIndividu() * (double)reac->getSndGauche()->getNbIndividu() * (double)reac->getProba()) / volumeVesicule;
			}

			// Mono-mol
			else
			{
				prop = (double)reac->getFstGauche()->getNbIndividu() * (double)reac->getProba();
			}

			int EN = prop;
			double FR = prop - EN;

			int n_appl = EN + ( (((float)rand() / RAND_MAX) < FR) ? 1 : 0 );

			// Si le n_appli donnera unnombre d'individu négatif, on l'ajuste.
			if (reac->getSndGauche() != nullptr && 
			(reac->getFstGauche()->getNbIndividu() - n_appl < 0 || reac->getSndGauche()->getNbIndividu() - n_appl < 0))
			{
				// On l'ajuste en prenant le minimum entre les deux nbIndividu.
				n_appl = std::min(reac->getFstGauche()->getNbIndividu(), reac->getSndGauche()->getNbIndividu());
			}
			// Si on est sur du monomol, plus facile.
			else if (reac->getSndGauche() == nullptr && reac->getFstGauche()->getNbIndividu() - n_appl < 0)
			{
				n_appl = reac->getFstGauche()->getNbIndividu();
			}

			// On réduit le nombre d'individu des réactifs.
			reac->getFstGauche()->setNbIndividu(reac->getFstGauche()->getNbIndividu() - n_appl);
			if (reac->getSndGauche() != nullptr)
				reac->getSndGauche()->setNbIndividu(reac->getSndGauche()->getNbIndividu() - n_appl);

			// On augmente le nombre d'individu des produits.
			reac->getFstDroite()->setNbIndividu(reac->getFstDroite()->getNbIndividu() + n_appl);
			if (reac->getSndDroite() != nullptr)
				reac->getSndDroite()->setNbIndividu(reac->getSndDroite()->getNbIndividu() + n_appl);
		}
	}
}

/**
 * Méthode permettant de synchroniser le nombre de molécules.
 */
void GestionPopulationCentre::synchroNbMolecule()
{
	if (nbMolIsSynchro) return;

	nbMolecules = 0;

	// On compte les molécules.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		nbMolecules += (*especesMoleculaires)[i]->getNbIndividu();
	}

	nbMolIsSynchro = true;
}

/**
 * Méthode permettant de print l'etat de la simulation.
 */
void GestionPopulationCentre::print()
{
	synchroNbMolecule();
	std::cout << "tours : " << toursSimu << " nbMolecules : " << nbMolecules << std::endl;

	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		std::cout << (*especesMoleculaires)[i]->getNom() << " : " << (*especesMoleculaires)[i]->getNbIndividu() << std::endl;
	}
}

/**
 * Méthode permettant d'initialiser le csv.
 * @param fichier Le flux du csv.
 */
void GestionPopulationCentre::initCsv(std::ofstream &fichier) const
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
void GestionPopulationCentre::sortieCsv(std::ofstream &fichier)
{
	// On synchronise le nombre de molécules.
	synchroNbMolecule();

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
void GestionPopulationCentre::sortieGnp(std::ofstream &fichier, std::string nomEntre, std::string nomSortie, bool seconde) const
{
	// On configure le graphique.
	fichier << "set datafile separator ';'" << std::endl
			<< "set grid" << std::endl
			<< "set ylabel \"Nombre de molécules\"" << std::endl
			<< "set title \"Modèle '" << nomEntre << "' / Algorithme Population Centrée\"" << std::endl;

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
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		fichier << "replot '" << nomSortie << "' using " << (seconde ? 2 : 1) << ":" << i + 4 << " with lines title '" << (*especesMoleculaires)[i]->getNom() << "'" << std::endl;
	}
	fichier << "pause -1 'Appuyer sur Entrer pour terminer le programme.'" << std::endl;
}