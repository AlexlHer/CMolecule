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
   * \brief Méthode appelée lors de l'initialisation.
   */
  void startInit() override;

  /*!
   * \brief Méthode appelée à chaque itération.
   */
  void compute() override;

  void readArc();

  /** Retourne le numéro de version du module */
  VersionInfo versionInfo() const override { return VersionInfo(0, 1, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_CM(CMModule);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
