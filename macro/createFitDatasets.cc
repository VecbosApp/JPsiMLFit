#include <TChain.h>
#include <TMath.h>
#include <TFile.h>
#include <TSystem.h>

#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooCategory.h>

#include <iostream>

using namespace std;

void createSingleDataset(const char *treefile, const char *roofitfile);

void createAll() {

  cout << "CREATING ROODATSETS FOR EE CHANNEL..." << endl;
  // createSingleDataset("results/datasets_trees/JPsi_ee.root","results/datasets/JPsi_ee.root");
  // createSingleDataset("results/datasets_trees/QCD_EMDoubleEnriched.root","results/datasets/QCD_EMDoubleEnriched.root");
  createSingleDataset("results/datasets_trees/JPsi_ee_data.root","results/datasets/JPsi_ee_data.root");
}

void createSingleDataset(const char *treefile, const char *roofitfile) {

  cout << "roofitting file " << treefile << " in " << roofitfile << endl;

  RooRealVar *invMass         = new RooRealVar("invMass",         "m(l^{+}l^{-})",   1.5,4.,"GeV");
  RooRealVar *trackerInvMass  = new RooRealVar("trackerInvMass",  "m(l^{+}l^{-})",   1.5,4.,"GeV");
  RooRealVar *gsfTrackInvMass = new RooRealVar("gsfTrackInvMass", "m(l^{+}l^{-})",   1.5,4.,"GeV");
  RooRealVar *eleEta          = new RooRealVar("eleEta",          "electron #eta",  -1.5,1.5);
  RooRealVar *posEta          = new RooRealVar("posEta",          "positron #eta",  -1.5,1.5);
  RooRealVar *weight          = new RooRealVar("weight",          "weight",          0, 10000);
  RooRealVar *event           = new RooRealVar("event",           "event number",    0,1e+07);

  RooArgSet setJPsi(*invMass,*trackerInvMass,*gsfTrackInvMass,*eleEta,*posEta,*weight,*event);

  TFile *file = TFile::Open(treefile);
  TTree *tree = (TTree*)file->Get("T1");

  RooDataSet *data = new RooDataSet("T1","dataset",tree,setJPsi);

  TFile *roofitFile = TFile::Open(roofitfile,"recreate");
  data->Write();
  roofitFile->Close();
}
