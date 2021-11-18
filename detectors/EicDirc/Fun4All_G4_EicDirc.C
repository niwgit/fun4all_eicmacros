#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
//#include <g4histos/G4HitNtuple.h>
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

int Fun4All_G4_EicDirc(const int nEvents = 1000, const double eta = 0., const int pid = 211, const string &outputFile = "G4DIRCTree.root", const string &outputPath = ".", const char *DSToutfile = NULL)
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
  //gen->set_name("pi+");
  gen->set_pid(pid);
  gen->set_vtx(0, 1.75, 0);
  gen->set_eta_range(eta, eta);
  //gen->set_eta_range(1.317, 1.317); // 30 deg
  gen->set_z_range(0., 0.);
  gen->set_phi_range(0., 0.);
  //gen->set_phi_range(1.5*(TMath::Pi()/180.), 1.5*(TMath::Pi()/180.));
  //gen->set_phi_range(0., 2*TMath::Pi());
  gen->set_mom_range(6.0, 6.0); // GeV/c
  se->registerSubsystem(gen);
  

  /*PHG4ParticleGun *gun = new PHG4ParticleGun();                                                                               
  gun->set_pid(pid);
  gun->set_vtx(0, 1.75, 0);                                                                                          
  gun->set_mom(100.0, 0, 0);                                                                                 
  se->registerSubsystem(gun);                                                                                                 
  */

  /*PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
  gen->set_pid(11);
  gen->set_vtx(76.085*cm, -1.22*cm, -257*cm);
  //gen->set_mom_range(50.0e-03, 50.0e-03);
  //gen->set_eta_range(-3,3);
  //gen->set_mom(0.2075*MeV, 1.609*MeV, -40.12*MeV);
  //gen->set_mom(0, 0, -50*MeV);
  se->registerSubsystem(gen);
  */
  /*PHG4ParticleGun *gun = new PHG4ParticleGun();
  gun->set_name("opticalphoton");  
  gun->set_vtx(75, 0, -255.49 + 0.05);
  gun->set_mom(-2.05e-09, 1.60e-09, -1.83e-09);
  se->registerSubsystem(gun);
  */

  PHG4Reco *g4Reco = new PHG4Reco();
  //g4Reco->set_field(1.5);  // 1.5 T solenoidal field
  //g4Reco->set_field(0.);
  g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"),PHFieldConfig::kField2D);

  G4EicDircSubsystem *eicdirc = new G4EicDircSubsystem();
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

  eicdirc->set_int_param("disable_photon_sim", 0);  // if true, disable photon simulations

  eicdirc->SuperDetector("DIRC");
  eicdirc->SetActive();
  //eicdirc->OverlapCheck(true);
  eicdirc->Verbosity(1);
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

  /*G4HitNtuple *hitntup = new G4HitNtuple();
  hitntup->AddNode("DIRC",0);
  se->registerSubsystem(hitntup);
  */
  G4DIRCTree *dirc_tree = new G4DIRCTree("G4DIRCTree", outputPath + "/" + outputFile);
  dirc_tree->AddNode("DIRC",0);
  se->registerSubsystem(dirc_tree);
    
  //---------------------------
  // output DST file for further offlien analysis
  //---------------------------
  if (DSToutfile)
  {
    Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputPath + "/" + DSToutfile);
    se->registerOutputManager(out);
  }
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

  //PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  //g4->ApplyCommand("/tracking/verbose 3");

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
