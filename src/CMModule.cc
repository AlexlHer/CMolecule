// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-

#include "CMModule.hh"
#include "code/Systeme.hpp"

void CMModule::
startInit()
{
	info() << "----------------------------------------------";
	info() << "-----Projet TER : Simulation Stochastique-----";
	info() << "-------------Alexandre l'Heritier-------------";
	info() << "----------------------------------------------";
  readArc();
}

void CMModule::
readArc()
{
	info() << "-----Début analyse LEX/YACC-----";

  Systeme systeme;

  systeme.setDiametreVesicule(options()->getDiametre());

  Integer num_especes = options()->espece().size();

  for(Integer i = 0; i < num_especes; i++)
  {
    
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