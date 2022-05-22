// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-

#include "CM_axl.h"
#include <arcane/ITimeLoopMng.h>

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
   * \brief Méthode appelée à chaque itération.
   */
  void compute() override;
  /*!
   * \brief Méthode appelée lors de l'initialisation.
   */
  void startInit() override;

  /** Retourne le numéro de version du module */
  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void CMModule::
compute()
{
  info() << "Module CM COMPUTE";

  // Stop code after 10 iterations
  if (m_global_iteration()>10)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void CMModule::
startInit()
{
  info() << "Module CM INIT";
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_CM(CMModule);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
