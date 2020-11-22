// Auteur : Alexandre l'Heritier

#ifndef GESTIONPOPULATIONCENTRE_H
#define GESTIONPOPULATIONCENTRE_H

#include <iostream>
#include <vector>
#include <fstream>

#include "EspeceMoleculaire.hpp"
#include "Reaction.hpp"
#include "Molecule.hpp"
#include "Gestion.hpp"
#include "Case.hpp"

/*
	GestionPopulationCentre (GPC)
	GPC est une implémentation de l'algorithme Population Centrée présenté en cours.
	Cette classe permet de gérer la simulation et permet de sortir un résultat sous forme csv.
*/

class GestionPopulationCentre : public Gestion
{
	private:
		std::vector<EspeceMoleculaire *> *especesMoleculaires;
		std::vector<Reaction *> *reactions;
		int diametreVesicule;

		double volumeVesicule;
		const double alpha = 7.4e-7;
		const int tau = 100;

		int toursSimu;
		int nbMolecules;
		bool pairImpair;

		bool nbMolIsSynchro;

		void synchroNbMolecule();

	public :
		GestionPopulationCentre(std::vector<EspeceMoleculaire *> *, std::vector<Reaction *> *, int, std::ofstream&);
		void simulation(int);
		void print();
		void sortieCsv(std::ofstream &);
		void initCsv(std::ofstream &) const;
		void sortieGnp(std::ofstream &, std::string, std::string, bool) const;
		void setDiffusion(bool){};
};

#endif