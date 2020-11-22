// Auteur : Alexandre l'Heritier

#ifndef CASE_HPP
#define CASE_HPP

#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include "Molecule.hpp"

/*
	Case
	Une Case est un groupement de Molecule. Des Molecules peuvent être
	ajoutées ou supprimées de la Case. 
	La Case s'assure de son intégrité avec un mutex.
	La Case gère aussi le changement des coordonnées de la Case pour les molécules déplacées.
*/
class Case
{
	private:
		std::vector<Molecule*> molecules;
		int nbMolecule;
		int idCase;
		omp_lock_t verrou;
		std::random_device randd;
		int i, j, k;
	
	public:
		Case() : nbMolecule(0){omp_init_lock(&verrou);};
		~Case();

		void addMolecule(Molecule *, bool, bool);
		int delMolecule(Molecule*, bool, bool);
		void clsMolecule();

		void syncNbMolecule();

		int getNbMolecule(){return nbMolecule;};
		Molecule *getMolecule(int);

		int getI(){return i;};
		int getJ(){return j;};
		int getK(){return k;};
		void setIJK(int ii, int jj, int kk){i=ii; j=jj; k=kk;};

		int getId(){return idCase;};
		int setId(int id){idCase=id;return 0;};
};

#endif