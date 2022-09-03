// Auteur : Alexandre l'Heritier

/*
	Méthodes de Systeme.
	Systeme est une classe créé sur mesure pour yacc (analyse.y).
	Elle permet de créer le système décrit dans le fichier modèle
	en verifiant un minimum que le fichier est correct (pas deux especeMoleculaire
	avec le même nom).
	Elle permet aussi de générer les Gestion avec ce qui a été récupéré dans le fichier.
	(Pas de copie de especesMols ni de reactions, c'est les pointeurs vers les vector
	qui sont envoyés).
*/

#include "Systeme.hpp"
#include <fstream>

/**
 * Méthode permettant d'ajouter une espèce moléculaire à notre système.
 * 
 * @param nom : Nom de l'espèce moléculaire.
 * @return 0 si tout s'est bien passé, 0 sinon.
 */
int Systeme::addEspeceMoleculaire(std::string nom)
{
	// Si l'especeMoleculaire avec le nom "nom" est déjà présente, erreur.
	for(int i = 0; i < especesMoleculaires->size(); i++)
	{
		if(*(*especesMoleculaires)[i] == nom) return 1;
	}

	// On ajoute cette espece.
	especesMoleculaires->push_back(new EspeceMoleculaire(nom));

	return 0;
}

/**
 * Methode addReaction permettant de rajouter une réaction à notre systeme.
 * 
 * @param a : Nom de l'espece moleculaire obligatoire gauche.
 * @param b : Nom de l'espece moleculaire facultatif gauche.
 * @param c : Nom de l'espece moleculaire obligatoire droit.
 * @param d : Nom de l'espece moleculaire facultatif droit.
 * @param proba : Probabilité de la réaction.
 * @return 0 si tout s'est bien passé, 1 sinon.
 */
int Systeme::addReaction(std::string a, std::string b, std::string c, std::string d, float proba)
{
	// On recherche les espècesMoleculaires qui correspondent
	// aux noms donnés en paramètre.
	int aa = -1, bb = -1, cc = -1, dd = -1;

	// On lance la recherche.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		if (*(*especesMoleculaires)[i] == a)
			aa = i;

		if (*(*especesMoleculaires)[i] == b)
			bb = i;

		if (*(*especesMoleculaires)[i] == c)
			cc = i;

		if (*(*especesMoleculaires)[i] == d)
			dd = i;
	}

	// Si un nom n'a pas été trouvé, erreur.
	// (les seconds réactif et produit n'étant pas indispensable, on ajuste la condition).
	if (aa == -1 || (bb == -1 && b != "") || cc == -1 || (dd == -1 && d != "")){
		std::cout << "AAAAAAAAAAAAAAAAAAA" << std::endl;
		return 1;
	}

	// On crée la Reaction avec les especesMol qu'on a trouvé.
	Reaction *r = new Reaction();

	// Réactifs.
	r->setEspeceGaucheObli((*especesMoleculaires)[aa]);
	if (b != "")
		r->setEspeceGaucheFacu((*especesMoleculaires)[bb]);

	// Produits.
	r->setEspeceDroiteObli((*especesMoleculaires)[cc]);
	if (d != "")
		r->setEspeceDroiteFacu((*especesMoleculaires)[dd]);

	r->setProba(proba);

	reactions->push_back(r);

	return 0;
}

/**
 * Méthode permettant de définir le diamètre de la vésicule du système.
 * 
 * @param diametre : Le nouveau diamètre de la vésicule du système.
 * @return 0 si tout s'est bien passé, 1 sinon.
 */
int Systeme::setDiametreVesicule(int diametre)
{
	if(diametre < 0) return 1;
	diametreVesicule = diametre;
	return 0;
}

/**
 * Méthode permettant de définir la taille de l'espèce moléculaire ayant le nom "nom".
 * 
 * @param nom : Le nom de l'espèce moléculaire à modifier.
 * @param taille : La nouvelle taille de l'espèce moléculaire.
 * @return 0 si tout s'est bien passé, 1 sinon.
 */
int Systeme::setTailleEspeceMoleculaire(std::string nom, int taille)
{
	// On recherche l'especeMol avec le nom donné.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		if (*(*especesMoleculaires)[i] == nom)
			return (*especesMoleculaires)[i]->setTaille(taille);
	}

	// Si on ne trouve pas, erreur.
	return 1;
}

/**
 * Méthode permettant de définir la vitesse de l'espèce moléculaire ayant le nom "nom".
 * 
 * @param nom : Le nom de l'espèce moléculaire à modifier.
 * @param vitesse : La nouvelle vitesse de l'espèce moléculaire.
 * @return 0 si tout s'est bien passé, 1 sinon.
 */
int Systeme::setVitesseEspeceMoleculaire(std::string nom, float vitesse)
{
	// On recherche l'especeMol avec le nom donné.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		if (*(*especesMoleculaires)[i] == nom)
			return (*especesMoleculaires)[i]->setVitesse(vitesse);
	}

	// Si on ne trouve pas, erreur.
	return 1;
}

/**
 * Méthode permettant de définir le nombre d'individu de l'espèce moléculaire ayant le nom "nom".
 * 
 * @param nom : Le nom de l'espèce moléculaire à modifier.
 * @param taille : La nouvelle population de l'espèce moléculaire.
 * @param unit : L'unité de init (um, mm, absolue).
 * @return 0 si tout s'est bien passé, 1 sinon.
 */
int Systeme::setInitEspeceMoleculaire(std::string nom, float init, int unit)
{
	// On recherche l'especeMol avec le nom donné.
	for (int i = 0; i < especesMoleculaires->size(); i++)
	{
		if (*(*especesMoleculaires)[i] == nom)
			return (*especesMoleculaires)[i]->setInit(init, unit);
	}

	// Si on ne trouve pas, erreur.
	return 1;
}

/**
 * Méthode permettant de créer un objet GestionEntiteCentre avec ce qu'on a trouvé dans le fichier.
 * @param log Le fux de sortie de log.
 * @param decoupage Le découpage à appliquer (le nombre de case).
 * @param nbThread Le nombre de thread à utiliser.
 * @param precision Le niveau de precision de l'algorithme.
 */
GestionEntiteCentre *Systeme::createNewGestionEntiteCentre(std::ofstream &log, int decoupage, int nbThread)
{
	return new GestionEntiteCentre(especesMoleculaires, reactions, diametreVesicule, log, decoupage, nbThread);
}

/**
 * Méthode permettant de créer un objet GestionPopulationCentre avec ce qu'on a trouvé dans le fichier.
 * @param log Le fux de sortie de log.
 */
GestionPopulationCentre *Systeme::createNewGestionPopulationCentre(std::ofstream &log)
{
	return new GestionPopulationCentre(especesMoleculaires, reactions, diametreVesicule, log);
}

/**
 * Méthode permettant de faire une description du système actuel.
 * 
 * @return La description.
 */
std::string Systeme::toString() const
{
	std::string fin = "Description Système\n----------\n";

	fin += "Ensemble des espèces : ";
	for(int i = 0; i < especesMoleculaires->size(); i++)
	{
		fin += (*especesMoleculaires)[i]->getNom() + ", ";
	}

	fin += "\nDiamètre vésicule : " + std::to_string(diametreVesicule) + " nm\n\n";

	fin += "Détails espèces :\n-----\n";

	for(int i = 0; i < (*especesMoleculaires).size(); i++)
	{
		fin += (*especesMoleculaires)[i]->toString() + "\n-----\n";
	}

	fin += "\nRéactions :\n-----\n";
	for(int i = 0; i < reactions->size(); i++)
	{
		fin += (*reactions)[i]->toString() + "\n-----\n";
	}
	fin += "Fin Description";
	return fin;
}

/**
 * Destructeur.
 */
Systeme::~Systeme()
{
	delete especesMoleculaires;
}
