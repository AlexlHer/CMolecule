// Auteur : Alexandre l'Heritier

#ifndef REACTION_H
#define REACTION_H

#include <iostream>
#include "EspeceMoleculaire.hpp"

/*
	Reaction.
	Une réaction est juste le stockage des paramètres de la réaction, 
	pas de calcul (a part les verification d'existance des parties facultative).
*/
class Reaction
{
	private:
		// Réactif 1, Réactif 2, Produit 1, Produit 2.
		EspeceMoleculaire *gauche1, *gauche2, *droite1, *droite2;
		bool isFacuGauche, isFacuDroite;
		float probabilite;

	public:
		Reaction() : isFacuDroite(false), isFacuGauche(false), probabilite(.0){};
		
		// Les réactifs.
		void setEspeceGaucheObli(EspeceMoleculaire *e){gauche1 = e;}
		void setEspeceGaucheFacu(EspeceMoleculaire *e){gauche2 = e;isFacuGauche = true;}

		// Les produits.
		void setEspeceDroiteObli(EspeceMoleculaire *e){droite1 = e;}
		void setEspeceDroiteFacu(EspeceMoleculaire *e){droite2 = e;isFacuDroite = true;}

		void setProba(float f) { probabilite = f; }
		float getProba() const { return probabilite; }

		EspeceMoleculaire *getFstGauche() const { return gauche1; }
		EspeceMoleculaire *getSndGauche() const { return isFacuGauche ? gauche2 : nullptr; }

		EspeceMoleculaire *getFstDroite() const { return droite1; }
		EspeceMoleculaire *getSndDroite() const { return isFacuDroite ? droite2 : nullptr; }

		bool isFstGauche(EspeceMoleculaire *e) const { return *gauche1 == *e; }
		bool isSndGauche(EspeceMoleculaire *e) const { return isFacuGauche && *gauche2 == *e; }

		// Permet de décrire la réaction.
		std::string toString() const
		{
			std::string fin = gauche1->getNom();
			if (isFacuGauche)
				fin += " + " + gauche2->getNom();

			fin += " -> " + droite1->getNom();
			if (isFacuDroite)
				fin += " + " + droite2->getNom();

			fin += " [" + std::to_string(probabilite) + "]";

			return fin;
		};
};

#endif