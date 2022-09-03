// Auteur : Alexandre l'Heritier

/*
	Méthodes de EspeceMoleculaire.
	Une EspeceMoleculaire stocke toutes les propriétés d'une espèce moléculaire.
	Elle permet aussi de stocker le nombre de molécules qui sont de cette espèce.
*/

#include <math.h>
#include "EspeceMoleculaire.hpp"

#define PI 3.14159265358979323846

/**
 * Méthode permettant d'initialiser le nombre d'individu de l'espece.
 */
void EspeceMoleculaire::initNbIndividu(int diametreVesicule = 1000)
{
	nbIndividu = getNbMolecule(diametreVesicule);
}

/**
 * Méthode permettant de définir la taille de l'EspeceMoleculaire.
 * 
 * @param t La taille de l'EspeceMoleculaire.
 * @return 1 si cette taille n'est pas correct, 0 sinon.
 */
int EspeceMoleculaire::setTaille(int t)
{
	if (t < 0) return 1;
	taille = t;
	return 0;
}

/**
 * Méthode permettant de définir la vitesse de l'EspeceMoleculaire.
 * 
 * @param v La vitesse de l'EspeceMoleculaire.
 * @return 1 si cette vitesse n'est pas correct, 0 sinon.
 */
int EspeceMoleculaire::setVitesse(float v)
{
	if (v < 0) return 1;
	vitesse = v;
	return 0;
}

/**
 * Méthode permettant de définir le nombre d'individu initial de l'EspeceMoleculaire.
 * 
 * @param i Le nombre d'individu initial de l'EspeceMoleculaire.
 * @param u L'unité de ce nombre (ABS : Nombre absolu, UM/MM : Concentration).
 * @return 1 si ce nombre d'individu n'est pas correct, 0 sinon.
 */
int EspeceMoleculaire::setInit(float i, int u)
{
	if(i < 0 || u < 0 || u > 2) return 1;
	init = i;
	unite = u;
	return 0;
}

/**
 * Méthode permettant de calculer le nombre de molécules initials.
 * @param diametreVesicule Le diamètre de la vésicule (pour les concentrations).
 * @return Le nombre de molécules initials.
 */
int EspeceMoleculaire::getNbMolecule(int diametreVesicule = 1000) const
{
	// Si ASB, pas besoin de calcul.
	if(unite == _ABS) return init;

	// Si on a une concentration en microMolaire.
	else if(unite == _UM)
	{
		double volumeVesicule = PI * pow(((double)diametreVesicule / 1000.), 3) / 6;
		int nb = (double)init / (volumeVesicule * 6.02252 * 1e5) * 1000;
		return nb;
	}

	// Si on a une concentration en milliMolaire.
	else //MM
	{
		double volumeVesicule = PI * pow(((double)diametreVesicule / 1000.), 3) / 6;
		int nb = (double)init / (volumeVesicule * 6.02252 * 1e5);
		return nb;
	}
}

/**
 * Méthode permettant de générer une description de l'EspeceMoleculaire.
 */
std::string EspeceMoleculaire::toString() const
{
	std::string fin = "Nom Espèce Moléculaire : " + nom + "\n";
	fin += "Taille(" + nom + ") = " + std::to_string(taille) + " nm\n";
	fin += "Vitesse(" + nom + ") = " + std::to_string(vitesse) + " nm/100µs\n";
	fin += "Init(" + nom + ") = " + std::to_string(init) + " " + (unite==_UM ? "UM" : unite==_MM ? "MM" : "ABS");
	return fin;
}