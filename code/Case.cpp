// Auteur : Alexandre l'Heritier

/*
	Méthodes de Case.
	Une Case est un groupement de Molecule. Des Molecules peuvent être
	ajoutées ou supprimées de la Case. 
	La Case s'assure de son intégrité avec un mutex.
	La Case gère aussi le changement des coordonnées de la Case pour les molécules déplacées.
*/

#include "Case.hpp"
#include <algorithm>

/**
 * Méthode permettant d'ajouter une Molecule dans la Case.
 * 
 * @param m La Molecule à ajouter.
 * @param increm Permet de savoir si nbMolecule doit être incrémenté de 1.
 */ 
void Case::addMolecule(Molecule* m, bool increm, bool synchro)
{
	omp_set_lock(&verrou);

	// Si on veux effectuer le déplacement des positions de la molécule.
	if(synchro)
	{
		// On met la position de cette Case dans la Molecule.
		if(m->getPosI() != -1) std::cout << "Attention : La molécule n'a pas été correctement supprimer de l'ancienne case." << std::endl;
		m->setPosI(i);
		m->setPosJ(j);
		m->setPosK(k);
	}

	molecules.push_back(m);
	if(increm) nbMolecule++;

	omp_unset_lock(&verrou);
}

/**
 * Méthode permettant de supprimer une Molecule de la Case.
 * 
 * @param m La Molecule à supprimer.
 * @param increm Permet de savoir si nbMolecule doit être décrémenté de 1.
 * 
 * @return 0 si la suppression à bien eu lieu, 1 s'il y a eu un problème.
 */
int Case::delMolecule(Molecule *m, bool decrem, bool synchro)
{
	omp_set_lock(&verrou);

	// On recherche la Molecule.
	for (int i = 0; i < molecules.size(); i++)
	{
		// Si on l'a trouvé.
		if(*molecules[i] == m)
		{
			// On l'efface.
			if(decrem) nbMolecule--;
			molecules.erase(molecules.begin() + i);

			// On met des positions -1 pour avertir que la molécule a bien été supprimé.
			if (synchro)
			{
				m->setPosI(-1);
				m->setPosJ(-1);
				m->setPosK(-1);
			}

			omp_unset_lock(&verrou);
			return 0;
		}
	}

	omp_unset_lock(&verrou);

	// Pas trouvé donc 1.
	return 1;
}

/**
 * Méthode permettant de supprimer toutes les Molecule de la Case.
 */
void Case::clsMolecule()
{
	omp_set_lock(&verrou);

	// clear avec vector<Molecule*> n'appelle pas le destructeur des molecules.
	molecules.clear();
	nbMolecule = 0;

	omp_unset_lock(&verrou);
}

/**
 * Méthode permettant de resychroniser le nombre de molécule.
 */
void Case::syncNbMolecule()
{
	nbMolecule = molecules.size();
}

/**
 * Méthode permettant de retourner la molécule à la position "pos".
 * 
 * @param pos La position de la molécule recherchée.
 * @return Le pointeur vers la molécule.
 */
Molecule* Case::getMolecule(int pos)
{
	omp_set_lock(&verrou);

	// Si la position donné n'est pas correct, erreur.
	if (pos < 0 || pos >= nbMolecule)
	{
		std::cerr << "Erreur Case::getMolecule() : Case demandée : " << pos << " sur " << nbMolecule << std::endl;
		omp_unset_lock(&verrou);
		return nullptr;
	}

	else
	{
		omp_unset_lock(&verrou);
		return molecules[pos];
	}
	
}

/**
 * Destructeur.
 */
Case::~Case()
{
	omp_destroy_lock(&verrou);
	molecules.clear();
}