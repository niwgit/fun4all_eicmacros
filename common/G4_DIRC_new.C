#ifndef MACRO_G4DIRCNEW_C
#define MACRO_G4DIRCNEW_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4eicdirc/G4EicDircSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <cmath>

R__LOAD_LIBRARY(libg4detectors.so)

/*!
 * \file G4_DIRC.C
 * \brief Macro setting up the barrel DIRC
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.3 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

namespace Enable
{
  bool DIRC_NEW = false;
  bool DIRC_NEW_OVERLAPCHECK = false;
}  // namespace Enable

namespace G4DIRC_NEW
{
  double radiator_R = 83.65;
  double length = 400;
  double z_shift = -75;  //115
  double z_start = z_shift + length / 2.;
  double z_end = z_shift - length / 2.;
  double outer_skin_radius = 89.25;
}  // namespace G4DIRC

void DIRC_newInit()
{
  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4DIRC_NEW::outer_skin_radius);
  //BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4DIRC_NEW::z_start);
  //BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, G4DIRC_NEW::z_end);
}

//! Babar DIRC (Without most of support structure)
//! Ref: I. Adam et al. The DIRC particle identification system for the BaBar experiment.
//! Nucl. Instrum. Meth., A538:281-357, 2005. doi:10.1016/j.nima.2004.08.129.
void DIRC_newSetup(PHG4Reco *g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::DIRC_NEW_OVERLAPCHECK;

  G4EicDircSubsystem *dirc;
  dirc = new G4EicDircSubsystem("DIRC");
  g4Reco->registerSubsystem(dirc); 
}
#endif
