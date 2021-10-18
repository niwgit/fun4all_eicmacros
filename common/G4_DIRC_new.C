#ifndef MACRO_G4DIRCNEW_C
#define MACRO_G4DIRCNEW_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4eicdirc/G4EicDircSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <cmath>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool DIRC_NEW = false;
  bool DIRC_NEW_OVERLAPCHECK = false;
}  // namespace Enable

/*namespace G4DIRC_NEW
{
  double radius;
  double zOffset;
  double halflength;
}  // namespace G4DIRC_NEW
*/
void DIRC_newInit()
{
  //G4DIRC_NEW::radius = 80.0;
  //G4DIRC_NEW::zOffset = -40.0;
  //G4DIRC_NEW::halflength = 218.0;

  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4DIRC_NEW::outer_skin_radius);
  //BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4DIRC_NEW::z_start);
  //BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, G4DIRC_NEW::z_end);
}

void DIRC_newSetup(PHG4Reco *g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::DIRC_NEW_OVERLAPCHECK;

  G4EicDircSubsystem *dirc;
  dirc = new G4EicDircSubsystem("DIRC");

  eicdirc->set_double_param("NBars", 10);
  eicdirc->set_double_param("Radius", 72.96 * cm);
  eicdirc->set_double_param("Prizm_width", 35.135 * cm);
  eicdirc->set_double_param("Prizm_length", 30.0 * cm);
  eicdirc->set_double_param("Prizm_height_at_lens", 5.0 * cm);
  eicdirc->set_double_param("Bar_thickness", 1.725 * cm);
  eicdirc->set_double_param("Bar_width", 3.5 * cm);
  eicdirc->set_double_param("BarL_length", 122.5 * cm);
  eicdirc->set_double_param("BarS_length", 56.0 * cm);
  eicdirc->set_double_param("Mirror_height", 2.0 * cm);
  eicdirc->set_double_param("z_shift", -43.75 * cm);
  eicdirc->set_int_param("Geom_type", 0); // 0-whole DIRC, 1-one bar box                                                        
  eicdirc->set_int_param("Lens_id", 3); // 3- 3-layer spherical lens                                                            
  eicdirc->set_int_param("MCP_rows", 6);
  eicdirc->set_int_param("MCP_columns", 4);
  eicdirc->set_int_param("NBoxes", 12); // number of bar boxes                                                                 
  eicdirc->set_int_param("Bar_pieces", 4); // pieces glued in one bar

  g4Reco->registerSubsystem(dirc); 
}
#endif
