// Auteur : Alexandre l'Heritier


#ifndef SYSTEME_H
#define SYSTEME_H

#include <iostream>
#include <vector>
#include <map>

#include "EspeceMoleculaire.hpp"
#include "Reaction.hpp"
#include "GestionEntiteCentre.hpp"
#include "GestionPopulationCentre.hpp"

/*
	Systeme.
	Systeme est une classe créé sur mesure pour yacc (analyse.y).
	Elle permet de créer le système décrit dans le fichier modèle
	en verifiant un minimum que le fichier est correct (pas deux especeMoleculaire
	avec le même nom).
	Elle permet aussi de générer les Gestion avec ce qui a été récupéré dans le fichier.
	(Pas de copie de especesMols ni de reactions, c'est les pointeurs vers les vector
	qui sont envoyés).
*/
class Systeme
{
	private:
		std::vector<EspeceMoleculaire*> *especesMoleculaires;
		std::vector<Reaction*> *reactions;
		int diametreVesicule;

	public:
		Systeme() : diametreVesicule(1000)
		{
			especesMoleculaires = new std::vector<EspeceMoleculaire *>;
			reactions = new std::vector<Reaction *>;
		};

		~Systeme();
		
		int addEspeceMoleculaire(std::string);
		int addReaction(std::string, std::string, std::string, std::string, float);

		int setDiametreVesicule(int);
		int setTailleEspeceMoleculaire(std::string, int);
		int setVitesseEspeceMoleculaire(std::string, float);
		int setInitEspeceMoleculaire(std::string, float, int);

		GestionEntiteCentre *createNewGestionEntiteCentre(std::ofstream&, int, int);
		GestionPopulationCentre *createNewGestionPopulationCentre(std::ofstream&);

		std::string toString() const;
};

#endif