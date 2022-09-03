// Auteur : Alexandre l'Heritier

#ifndef GESTIONENTITECENTRE_H
#define GESTIONENTITECENTRE_H

#include <iostream>
#include <vector>
#include <fstream>

#include "EspeceMoleculaire.hpp"
#include "Reaction.hpp"
#include "Molecule.hpp"
#include "Gestion.hpp"
#include "Case.hpp"

#define _C_VIDE_ 0
#define _C_INAC_ 1
#define _C_OCCU_ 2

/*
	GestionEntiteCentre (GEC)
	GEC est une implémentation de l'algorithme Entité Centrée présenté en cours.
	Cette classe permet de gérer la simulation et permet de sortir un résultat sous forme csv.
*/
class GestionEntiteCentre : public Gestion
{
	private:
		std::vector<EspeceMoleculaire *> *especesMoleculaires;
		std::vector<Reaction *> *reactions;
		int diametreVesicule, nbMolecules, lastId;
		bool reussiteCreation;

		Case *q;
		Case **p;
		Case ***grille;
		int tailleGrille, centreCube, tailleCase, nbCase;
		bool nbMolIsSynchro;
		bool diffusion;
		int nbCasesParThread;
		int toursSimu;

		Case *allCaseEntre;
		Case **caseEntre;
		int nbCaseEntre;
		int taillePGMolecule;
		bool modePrecision;

		void deplacementMolecules(Case &) const;
		void collisionsReactions(Case &, bool);
		bool collision(Molecule *, Molecule *) const;
		bool reactionBimol(Molecule *, Molecule *, bool);
		void reactionMonomol(Molecule *);

		int posEspaceToPosGrille(float) const;
		inline bool isInVesicule(float, float, float, float) const;
		void synchroNbIndividu();
		void entreDeux(Molecule *);

	public :
		GestionEntiteCentre(std::vector<EspeceMoleculaire *> *, std::vector<Reaction *> *, int, std::ofstream &, int, int);
		~GestionEntiteCentre();
		void simulation(int);
		void print();
		void sortieCsv(std::ofstream &);
		void initCsv(std::ofstream &) const;
		void sortieGnp(std::ofstream &, std::string, std::string, bool) const;

		void setDiffusion(bool dif){diffusion = dif;};
};

#endif