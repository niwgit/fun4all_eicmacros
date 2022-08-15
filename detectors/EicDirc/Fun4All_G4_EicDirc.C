#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
//#include <g4eval/PHG4DSTReader.h>
//#include <g4histos/G4HitNtuple.h>
#include <g4eicdirc/G4DIRCTree.h>
#include <g4eicdirc/G4LUTTree.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <g4main/PHG4Reco.h>
#include <phool/recoConsts.h>
#include <g4eicdirc/G4EicDircSubsystem.h>
#include <Geant4/G4SystemOfUnits.hh>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eicdirc.so)

//int Fun4All_G4_EicDirc(const int nEvents = 1000, const int LUT = 0, const double eta = 0., const int pid = 211, const string &outputFile = "G4DIRCTree.root", const string &outputPath = ".", const char *DSToutfile = NULL)

//int Fun4All_G4_EicDirc(const int nEvents = 1000, const int Barnumber = 0, const double eta = 0., const int pid = 211, const string &outputFile = "G4DIRCTree.root", const string &outputPath = ".", const char *DSToutfile = NULL)

int Fun4All_G4_EicDirc(const int nEvents = 1000, const int Barnumber = 0, const string &outputFile = "G4DIRCTree.root", const string &outputPath = ".", const char *DSToutfile = NULL)

{
  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("RANDOMSEED", 12345); // if you want to use a fixed seed
  // PHG4ParticleGenerator generates particle
  // distributions in eta/phi/mom range

  /*  PHG4ParticleGun *gun = new PHG4ParticleGun();
  gun->set_name("opticalphoton");  
  gun->set_vtx(72.96, 1.75, -255.44);
  //gun->set_mom(1.89982e-09, 1.41771e-09 , -2.11971e-09); // top-left
  //gun->set_mom(-0.495767e-09, -2.36652e-09 , -2.06548e-09); // bottom-right
  //gun->set_mom(0.960917e-09, 2.37134e-09 , -1.88833e-09); // left
  //gun->set_mom(0.120717e-09, -2.38017e-09 , -2.10538e-09); // right
  gun->set_mom(-1.94054e-09, -2.19653e-09, -1.23367e-09); // pathid = 95341
  //gun->set_mom(-0.300677e-09, 1.84231e-09, -2.57447e-09); // pathid = 932
  //gun->set_mom(2.10111e-09, -0.59692e-09, -2.31115e-09); // pathid = 94
  se->registerSubsystem(gun);
  */
  
  //if(LUT)
  {
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      gen->add_particles("opticalphoton", 1);
      gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform, PHG4SimpleEventGenerator::Uniform, PHG4SimpleEventGenerator::Uniform);
      //gen->set_vertex_distribution_mean(72.96, 1.7575, -255.44);                                                      
      gen->set_vertex_distribution_mean(72.96, -15.8175 + (3.515 * Barnumber), -255.44); 
      gen->set_vertex_distribution_width(0., 0., 0.);
      gen->set_theta_range(90.0*(TMath::Pi()/180.), 270.0*(TMath::Pi()/180.));      
      gen->set_phi_range(-M_PI, M_PI);
      gen->set_p_range(3.18e-09, 3.18e-09, 0.);
      se->registerSubsystem(gen);
  }

    //else
  /*{
      PHG4ParticleGenerator *gen = new PHG4ParticleGenerator("PGENERATOR");
      gen->set_pid(pid);
      //gen->set_vtx(0, -15.8175 + (3.515 * Barnumber), 0);
      gen->set_vtx(0,0,0);
      gen->set_eta_range(eta, eta);
      gen->set_z_range(0., 0.);
      //gen->set_phi_range(0., 0.);
      //gen->set_phi_range(-15*TMath::DegToRad(),15*TMath::DegToRad());
      gen->set_phi_range(0., 2*TMath::Pi());
      gen->set_mom_range(6.0, 6.0); // GeV/c
      se->registerSubsystem(gen);
      }*/
  
  PHG4Reco *g4Reco = new PHG4Reco();
  //g4Reco->set_field(1.5);  // 1.5 T solenoidal field
  g4Reco->set_field(0.);
  //g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"),PHFieldConfig::kField2D);

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
  eicdirc->set_int_param("Geom_type", 1); // 0-whole DIRC, 1-one bar box
  eicdirc->set_int_param("Lens_id", 3); // 3- 3-layer spherical lens
  eicdirc->set_int_param("MCP_rows", 6);
  eicdirc->set_int_param("MCP_columns", 4);
  eicdirc->set_int_param("NBoxes", 12); // number of bar boxes
  eicdirc->set_int_param("Bar_pieces", 4); // pieces glued in one bar

  eicdirc->set_int_param("disable_photon_sim", 0);  // if true, disable photon simulations

  eicdirc->SuperDetector("hpDIRC");
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
  
  //if(LUT) 
  {
      G4LUTTree *lut_tree = new G4LUTTree("G4LUTTree", outputPath + "/" + outputFile);
      lut_tree->AddNode("hpDIRC",0);
      se->registerSubsystem(lut_tree);
  }
  
    //else
    /*{
      G4DIRCTree *dirc_tree = new G4DIRCTree("G4DIRCTree", outputPath + "/" + outputFile);
      dirc_tree->AddNode("hpDIRC",0);
      se->registerSubsystem(dirc_tree);
      }*/

  //---------------------------
  // output DST file for further offlien analysis
  //---------------------------
  if (DSToutfile)
  {
    Fun4AllOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputPath + "/" + DSToutfile);
    se->registerOutputManager(out);
  }

  // save a comprehensive  evaluation file
  /*PHG4DSTReader *ana = new PHG4DSTReader("DSTReaderOutput.root");
  ana->set_save_particle(true);
  ana->set_load_all_particle(true);
  ana->set_load_active_particle(true);
  ana->set_save_vertex(true);

  ana->Verbosity(1);
  ana->AddNode("hpDIRC");
  se->registerSubsystem(ana);
  */
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
