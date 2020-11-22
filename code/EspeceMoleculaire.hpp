// Auteur : Alexandre l'Heritier

#ifndef ESPECEMOLECULAIRE_H
#define ESPECEMOLECULAIRE_H

#include <iostream>

#define _UM 0
#define _MM 1
#define _ABS 2

/*
	EspeceMoleculaire
	Une EspeceMoleculaire stocke toutes les propriétés d'une espèce moléculaire.
	Elle permet aussi de stocker le nombre de molécules qui sont de cette espèce.
*/
class EspeceMoleculaire
{
	private:
		std::string nom;
		int taille;
		float vitesse;
		float init;
		int unite;
		int nbIndividu;

	public:
		EspeceMoleculaire() : taille(10), vitesse(1), init(0), unite(_ABS){};
		EspeceMoleculaire(std::string n) : nom(n), taille(10), vitesse(1), init(0), unite(_ABS){};
		
		void initNbIndividu(int);

		void setNom(std::string n){nom = n;};
		int setTaille(int);
		int setVitesse(float);
		int setInit(float, int);
		void setNbIndividu(int u){nbIndividu = u;};

		std::string getNom(){return nom;};
		int getTaille(){return taille;};
		float getVitesse(){return vitesse;};
		float getInit(){return init;};
		int getNbIndividu(){return nbIndividu;};

		int getUnite(){return unite;};
		int getNbMolecule(int) const;

		/**
		 * Opérateurs permettant de comparer deux EspeceMoleculaire.
		 * Comme les noms des especes sont uniques, pas besoin d'aller plus loin.
		 */
		bool operator==(EspeceMoleculaire &a) const{return nom == a.getNom();};
		bool operator==(const std::string &a) const{return nom == a;};

		std::string toString() const;
};

#endif