// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-

#include "CMModule.hh"

void CMModule::
startInit()
{
	info() << "----------------------------------------------";
	info() << "-----Projet TER : Simulation Stochastique-----";
	info() << "-------------Alexandre l'Heritier-------------";
	info() << "----------------------------------------------";

  readArc();
  createGestion();

	m_outCsv = std::ofstream("csv.csv");
	m_gestion->initCsv(m_outCsv);
	m_gestion->sortieCsv(m_outCsv);

	info() << "-----Début simulation-----";
  Integer algo = 1;
  Integer diff = 100;
  Integer tour = 10000, pas = 100;

  if(algo != 2)
	{
		if (diff > 0) info() << "Diffusion pendant " << diff << " tours.";

		// On diffuse le nombre de fois voulu.
		m_gestion->simulation(diff);

		// On désactive la diffusion pour après.
		m_gestion->setDiffusion(false);

		if (diff > 0) info() << "Fin de la diffusion.";
	}

	for (int i = 0; i < tour - (tour % pas); i += pas)
	{
		m_gestion->simulation(pas);
		m_gestion->sortieCsv(m_outCsv);
		info() << "Nombre de tours effectués : " << i + pas << " / " << tour;
	}

	if(tour % pas)
	{
		m_gestion->simulation(tour % pas);
		m_gestion->sortieCsv(m_outCsv);
		info() << "Nombre de tours effectués : " << tour << " / " << tour;
	}
	info() << "-----Fin simulation-----";

  // On crée un fichier GNP avec un nom contenant le nom du csv.
	std::ofstream outGnp = std::ofstream("plot_csv.csv.gnp");

	// On construit le fichier GNP.
	m_gestion->sortieGnp(outGnp, ";", "csv.csv", true);

	// On ferme le fichier.
	outGnp.close();

	// On execute gnuplot avec le GNP généré.
	info() << "-----Ouverture de gnuplot-----";
	info() << "gnuplot plot_csv.csv.gnp";
	info() << "-----Echec de l'ouverture de gnuplot-----";

  m_outCsv.close();
	m_outLog.close();

	info() << "-----Fin programme-----";
}

void CMModule::
readArc()
{
	info() << "-----Début analyse LEX/YACC-----";

  m_systeme.setDiametreVesicule(options()->getDiametre());

  Integer num_especes = options()->espece().size();

  for(Integer i = 0; i < num_especes; i++)
  {
    std::string name = options()->espece()[i]->getName().localstr();
    m_systeme.addEspeceMoleculaire(name);
    m_systeme.setTailleEspeceMoleculaire(name, options()->espece()[i]->getSize());
    m_systeme.setVitesseEspeceMoleculaire(name, options()->espece()[i]->getSpeed());
    m_systeme.setInitEspeceMoleculaire(name, options()->espece()[i]->getInit(), 2); // TODO : Ajouter unités.
  }

  Integer num_reaction = options()->reaction().size();

  for(Integer i = 0; i < num_reaction; i++)
  {
    m_systeme.addReaction(
      options()->reaction()[i]->getFirstLeft().localstr(),
      options()->reaction()[i]->getSecLeft().localstr(),
      options()->reaction()[i]->getFirstRight().localstr(),
      options()->reaction()[i]->getSecRight().localstr(),
      options()->reaction()[i]->getProba()
    );
  }
	info() << "-----Analyse LEX/YACC : Ok-----";

}

void CMModule::
createGestion()
{
  m_outLog = std::ofstream("log.log");

	// On met la description du système dans les logs.
	m_outLog << m_systeme.toString() << std::endl;

	switch (1)
	{
	case 2:
		m_gestion = m_systeme.createNewGestionPopulationCentre(m_outLog);
		break;

	case 3:
		// On met un thread et une case pour forcer le séquentiel. Pas besoin de niveau de Précision.
		m_gestion = m_systeme.createNewGestionEntiteCentre(m_outLog, 1, 1);
		break;

	default:
		m_gestion = m_systeme.createNewGestionEntiteCentre(m_outLog, 1, 1);
		break;
	}
}


void CMModule::
compute()
{
  info() << "Module CM COMPUTE";

  // Stop code after 10 iterations
  if (m_global_iteration()>10)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}