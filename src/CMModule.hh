// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-

#include "CM_axl.h"

#include <arcane/ITimeLoopMng.h>

#include "code/Gestion.hpp"
#include "code/Systeme.hpp"
#include "code/GestionEntiteCentre.hpp"
#include "code/GestionPopulationCentre.hpp"

#include <fstream>

using namespace Arcane;

/*!
 * \brief Module CM.
 */
class CMModule
: public ArcaneCMObject
{
 public:
  explicit CMModule(const ModuleBuildInfo& mbi) 
  : ArcaneCMObject(mbi) { }

 public:
  /*!
   * \brief Méthode appelée lors de l'initialisation.
   */
  void startInit() override;

  /*!
   * \brief Méthode appelée à chaque itération.
   */
  void compute() override;

  void readArc();

  void createGestion();

  /** Retourne le numéro de version du module */
  VersionInfo versionInfo() const override { return VersionInfo(0, 1, 0); }

 private:
  Systeme m_systeme;
  Gestion* m_gestion;
	std::ofstream m_outLog;
	std::ofstream m_outCsv;

};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_CM(CMModule);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
