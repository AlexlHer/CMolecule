// Auteur : Alexandre l'Heritier

#ifndef MOLECULE_H
#define MOLECULE_H

#include <iostream>
#include <vector>
#include <omp.h>

#include "EspeceMoleculaire.hpp"

/*
	Molecule.
	Une molécule est juste le stockage des paramètres de la molécule, pas de calcul.
*/
class Molecule
{

	private:
		float posX, posY, posZ, vitesse;
		int posI, posJ, posK;
		EspeceMoleculaire *params;
		bool reaction;
		int id;
		bool aSuppr;

	public:
		Molecule() : id(0), params(nullptr), posX(0), posY(0), posZ(0), posI(-1), posJ(-1), posK(-1), vitesse(0), reaction(false), aSuppr(false){};
		Molecule(EspeceMoleculaire *e, int idd) : id(idd), params(e), posX(0), posY(0), posZ(0), posI(-1), posJ(-1), posK(-1), vitesse(0), reaction(false), aSuppr(false){};

		// Position dans l'espace.
		void setPosX(float x) { posX = x; };
		void setPosY(float y) { posY = y; };
		void setPosZ(float z) { posZ = z; };

		// Position dans les cases.
		void setPosI(int i) { posI = i; };
		void setPosJ(int j) { posJ = j; };
		void setPosK(int k) { posK = k; };

		void setEM(EspeceMoleculaire *e) { params = e; };
		void setReaction(bool b) { reaction = b; };

		void setId(int i) { id = i; };
		void setASuppr(bool i) { aSuppr = i; };

		float getPosX() const { return posX; };
		float getPosY() const { return posY; };
		float getPosZ() const { return posZ; };

		int getPosI() const { return posI; };
		int getPosJ() const { return posJ; };
		int getPosK() const { return posK; };

		bool getReaction() const { return reaction; };

		int getId() const { return id; };
		bool getASuppr() const { return aSuppr; };

		int getTaille() const { return params->getTaille(); };
		float getVitesse() const { return params->getVitesse(); };
		EspeceMoleculaire *getEM() const { return params; };

		bool operator==(Molecule *m) const { return id == m->getId(); };
};

#endif