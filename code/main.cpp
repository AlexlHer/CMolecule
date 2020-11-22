// Auteur : Alexandre l'Heritier
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "help.hpp"
#include "y.tab.hpp"
#include "Gestion.hpp"
#include "Systeme.hpp"
#include "GestionEntiteCentre.hpp"
#include "GestionPopulationCentre.hpp"
#include <chrono>
#include <fstream>

std::ifstream *fichier;

/**
 * Fonction main.
 */
int main(int argc, char **argv)
{
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "-----Projet TER : Simulation Stochastique-----" << std::endl;
	std::cout << "-------------Alexandre l'Heritier-------------" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;

	// Partie lecture des paramètres.
	// -------------
	char opt = getopt(argc, argv, ":o:v:t:T:p:123r:c:d:gsh");

	// Si on appelle le prog sans args, on affiche help et on return.
	if (argc == 1)
	{
		std::cout << help << std::endl;
		return 0;
	}

	// Quelques variables pour utiliser les args après.
	bool graph = false, hasLog = false, suivi = false, isSec = true;
	std::string entre = "", log = "", csv = "out.csv";
	int tour = 10000, pas = 100, nbThread = -1, decoupe = -1, diff = 100, algo = 1;

	// Pour récupérer les args.
	do
	{
		switch (opt)
		{
			case 'o':
				std::cout << "log : " << optarg << std::endl;
				log = optarg;
				hasLog = true;
				break;

			case 'v':
				std::cout << "csv : " << optarg << std::endl;
				csv = optarg;
				break;

			case 't':
				std::cout << "temps : " << optarg << std::endl;
				tour = atoi(optarg) * 10000;
				if (tour <= 0)
				{
					std::cout << "Nombre de secondes invalide." << std::endl;
					return 1;
				}
				break;

			case 'T':
				std::cout << "tours : " << optarg << std::endl;
				tour = atoi(optarg);
				isSec = false;
				if (tour <= 0)
				{
					std::cout << "Nombre de pas invalide." << std::endl;
					return 1;
				}
				break;

			case 'p':
				std::cout << "pas : " << optarg << std::endl;
				pas = atoi(optarg);
				if (pas <= 0)
				{
					std::cout << "Nombre de pas invalide." << std::endl;
					return 1;
				}
				break;

			case '1':
				std::cout << "algo 1" << std::endl;
				algo = 1;
				break;

			case '2':
				std::cout << "algo 2" << std::endl;
				algo = 2;
				break;

			case '3':
				std::cout << "algo 3" << std::endl;
				algo = 3;
				break;

			case 'r':
				std::cout << "threads : " << optarg << std::endl;
				nbThread = atoi(optarg);
				if (nbThread <= 0)
				{
					std::cout << "Nombre de thread invalide." << std::endl;
					return 1;
				}
				break;

			case 'c':
				std::cout << "cases : " << optarg << std::endl;
				decoupe = atoi(optarg);
				if (decoupe <= 0)
				{
					std::cout << "Nombre de case invalide." << std::endl;
					return 1;
				}
				break;

			case 'd':
				std::cout << "diffusion : " << optarg << std::endl;
				diff = atoi(optarg);
				if (diff < 0)
				{
					std::cout << "Nombre de tour en diffusion invalide." << std::endl;
					return 1;
				}
				break;

			case 'g':
				std::cout << "graphique" << std::endl;
				graph = true;
				break;

			case 's':
				std::cout << "suivi" << std::endl;
				suivi = true;
				break;

			case 'h':
				std::cout << help << std::endl;
				return 0;

			case ':':
				std::cout << "Erreur : Un argument est manquant." << std::endl;
				return 1;

			case '?':
				std::cout << "Erreur : Un argument est inconnu : " << optopt << std::endl;
				break;
		}
		// Arg suivant.
		opt = getopt(argc, argv, ":o:v:t:T:p:123r:c:d:gsh");

	} while(opt != -1);

	// S'il n'y a pas le nom du fichier à lire, on return.
	if(optind >= argc) return 1;

	// On récupère le nom du fichier à lire.
	entre = argv[optind];

	std::cout << "Lecture du fichier : " << entre << std::endl;

	// On ouvre un flux pour lire le fichier
	fichier = new std::ifstream(entre, std::ifstream::in);

	// Si le fichier ne s'ouvre pas.
	if(!fichier->is_open())
	{
		std::cout << "Erreur : Ouverture du fichier d'entrée impossible." << std::endl;
		return 1;
	}

	std::cout << "-----Début analyse LEX/YACC-----" << std::endl;

	// Si yacc détécte une erreur.
	if (yyparse())
	{
		std::cout << "-----Erreur LEX/YACC-----" << std::endl;
		std::cout << "Le fichier d'entrée n'est pas correct." << std::endl;
		return 1;
	}
	else std::cout << "-----Analyse LEX/YACC : Ok-----\n" << std::endl;

	// On récupère le système généré par lex/yacc.
	// recupSysteme() se trouve dans analyse.y
	Systeme systeme = recupSysteme();

	// On ouvre un flux de sortie pour les logs.
	std::ofstream outLog;

	// Si l'utilisateur veut un log, on ouvre le fichier log.
	if (hasLog)
	{
		outLog = std::ofstream(log);

		// On met la description du système dans les logs.
		outLog << systeme.toString() << std::endl;
	}

	// On crée la Gestion demandée par l'utilisateur.
	Gestion *gestion;
	switch (algo)
	{
	case 2:
		gestion = systeme.createNewGestionPopulationCentre(outLog);
		break;

	case 3:
		// On met un thread et une case pour forcer le séquentiel. Pas besoin de niveau de Précision.
		gestion = systeme.createNewGestionEntiteCentre(outLog, 1, 1);
		break;

	default:
		gestion = systeme.createNewGestionEntiteCentre(outLog, decoupe, nbThread);
		break;
	}

	// On ouvre le fichier csv.
	std::ofstream outCsv;

	// On initialise le csv (les titres sur la première ligne) et on écrit l'état du système à t=0.
	outCsv = std::ofstream(csv);
	gestion->initCsv(outCsv);
	gestion->sortieCsv(outCsv);
	

	std::cout << "-----Début simulation-----" << std::endl;

	// if (hasLog) gestion->print();

	// On démarre un chrono.
	std::chrono::_V2::steady_clock::time_point start = std::chrono::steady_clock::now();

	// L'algorithme Population Centrée n'a pas besoin de diffusion.
	if(algo != 2)
	{
		if (suivi && diff > 0) std::cout << "Diffusion pendant " << diff << " tours." << std::endl;

		// On diffuse le nombre de fois voulu.
		gestion->simulation(diff);

		// On désactive la diffusion pour après.
		gestion->setDiffusion(false);

		if (suivi && diff > 0) std::cout << "Fin de la diffusion." << std::endl;
	}

	// On fait tous les tours demandés, avec une "pause écriture" tous les i="pas".
	// "tour - (tour % pas)" permet d'éviter de sauter des tours si "tour" n'est pas un multiple de "pas".
	for (int i = 0; i < tour - (tour % pas); i += pas)
	{
		// On fait "pas" pas de simulation.
		gestion->simulation(pas);

		// if (hasLog) gestion->print();

		// On écrit l'état du système dans le csv.
		gestion->sortieCsv(outCsv);
		if (suivi) std::cout << "Nombre de tours effectués : " << i + pas << " / " << tour << std::endl;
	}

	// On fait les tours qui manque.
	if(tour % pas)
	{
		gestion->simulation(tour % pas);
		gestion->sortieCsv(outCsv);
		if (suivi) std::cout << "Nombre de tours effectués : " << tour << " / " << tour << std::endl;
	}

	// On stop le chrono.
	std::chrono::_V2::steady_clock::time_point stop = std::chrono::steady_clock::now();
	std::chrono::milliseconds timeN = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "-----Fin simulation-----" << std::endl << std::endl;

	std::cout << "Temps d'execution : " << timeN.count()/1000 << " secondes." << std::endl;

	// Si l'utilisateur a demandé un graphique.
	if(graph)
	{
		// On crée un fichier GNP avec un nom contenant le nom du csv.
		std::ofstream outGnp = std::ofstream("plot_" + csv + ".gnp");

		// On construit le fichier GNP.
		gestion->sortieGnp(outGnp, entre, csv, isSec);

		// On ferme le fichier.
		outGnp.close();

		// On execute gnuplot avec le GNP généré.
		std::cout << std::endl << "-----Ouverture de gnuplot-----" << std::endl;
		std::cout << "gnuplot plot_" + csv + ".gnp" << std::endl;
		if(std::system(("gnuplot plot_" + csv + ".gnp").c_str()) == -1)
			std::cout << "-----Echec de l'ouverture de gnuplot-----" << std::endl;
		else
			std::cout << "-----Fermeture de gnuplot-----" << std::endl;
	}

	// On ferme les fichiers ouverts.
	outCsv.close();
	if(hasLog) outLog.close();

	// if (hasLog) gestion->print();

	std::cout << std::endl << "-----Fin programme-----" << std::endl;
	return 0;
}