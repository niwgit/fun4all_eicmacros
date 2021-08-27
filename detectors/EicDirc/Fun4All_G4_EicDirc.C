#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4histos/G4HitNtuple.h>
#include <g4eicdirc/G4DIRCTree.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <g4main/PHG4Reco.h>
#include <phool/recoConsts.h>
#include <g4eicdirc/G4EicDircSubsystem.h>
#include <Geant4/G4SystemOfUnits.hh>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eicdirc.so)

int Fun4All_G4_EicDirc(const int nEvents = 1000, const char *outfile = NULL)
{
  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();
  //  rc->set_IntFlag("RANDOMSEED", 12345); // if you want to use a fixed seed
  // PHG4ParticleGenerator generates particle
  // distributions in eta/phi/mom range
  
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
  gen->set_name("pi+");
  gen->set_vtx(0, 0, 0);
  gen->set_eta_range(0, 0);
  //gen->set_eta_range(1.317, 1.317); // 30 deg
  gen->set_phi_range(0., 0.);
  gen->set_mom_range(6.0, 6.0); // GeV/c
  se->registerSubsystem(gen);
  
  
  /*PHG4ParticleGun *gun = new PHG4ParticleGun();
  gun->set_name("opticalphoton");  
  gun->set_vtx(75, 0, -255.49 + 0.05);
  gun->set_mom(-2.05e-09, 1.60e-09, -1.83e-09);
  se->registerSubsystem(gun);
  */

  PHG4Reco *g4Reco = new PHG4Reco();
  //g4Reco->set_field(1.5);  // 1.5 T solenoidal field
  g4Reco->set_field(0.);

  G4EicDircSubsystem *eicdirc = new G4EicDircSubsystem();
  eicdirc->set_double_param("NBars", 11);
  eicdirc->set_double_param("Radius", 75.0 * cm);
  eicdirc->set_double_param("Prizm_width", 38.65 * cm);
  eicdirc->set_double_param("Prizm_length", 30.0 * cm);
  eicdirc->set_double_param("Prizm_height_at_lens", 3.7 * cm);
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
  eicdirc->SuperDetector("DIRC");
  eicdirc->SetActive();
  g4Reco->registerSubsystem(eicdirc);
  

  // Black hole swallows everything - prevent loopers from returning
  // to inner detectors
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("BlackHole", 0);
  cyl->set_double_param("radius", 80);        // 80 cm
  cyl->set_double_param("thickness", 0.1); // does not matter (but > 0)
  cyl->SetActive();
  cyl->BlackHole(); // eats everything
  g4Reco->registerSubsystem(cyl);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);

  //---------------------------

  G4HitNtuple *hitntup = new G4HitNtuple();
  hitntup->AddNode("DIRC",0);
  se->registerSubsystem(hitntup);
  
  G4DIRCTree *dirc_tree = new G4DIRCTree();
  dirc_tree->AddNode("DIRC",0);
  se->registerSubsystem(dirc_tree);
    
  //---------------------------
  // output DST file for further offlien analysis
  //---------------------------
  if (outfile)
  {
    Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
    se->registerOutputManager(out);
  }
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

  if (nEvents > 0)
  {
    se->run(nEvents);
    // finish job - close and save output files
    se->End();
    std::cout << "All done" << std::endl;

    // cleanup - delete the server and exit
    delete se;
    gSystem->Exit(0);
  }
  return 0;
}

PHG4ParticleGenerator *get_gen(const char *name = "PGENERATOR")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGenerator *pgun = (PHG4ParticleGenerator *) se->getSubsysReco(name);
  return pgun;
}
