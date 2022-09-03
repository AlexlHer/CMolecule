// Auteur : Alexandre l'Heritier

#ifndef GESTION_H
#define GESTION_H

#include <iostream>

/*
	Classe virtuel permettant de définir un algo de gestion.
*/
class Gestion
{  
	public:
		virtual ~Gestion() {};
		virtual void simulation(int) = 0;
		virtual void print()=0;
		virtual void sortieCsv(std::ofstream &)=0;
		virtual void initCsv(std::ofstream &) const = 0;
		virtual void sortieGnp(std::ofstream &, std::string, std::string, bool) const = 0;
		virtual void setDiffusion(bool) = 0;
};

#endif