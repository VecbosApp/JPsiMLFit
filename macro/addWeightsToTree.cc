#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <iostream>

using namespace std;

void addWeights(const char* filename, float weight) {

  cout << "Adding weight branch to file " << filename << " with weight " << weight << endl;
  
  TFile *fileOrig = 0;
  TTree *treeOrig = 0;
  
  fileOrig = TFile::Open(filename);
  if( fileOrig ) {
    fileOrig->cd();
    treeOrig = (TTree*)fileOrig->Get("T1");
  } else {
    cout << "File " << filename << " not existing !" << endl;
    return;
  }

  if ( treeOrig ) {
  int nentriesOrig = treeOrig->GetEntries();

  TFile *fileNew = TFile::Open(filename,"recreate");
  TTree *treeNew = new TTree("T1","tree with only selected events");

  Float_t invMass;
  Float_t trackerInvMass;
  Float_t gsfTrackInvMass;
  Float_t deltaR;
  Float_t eleEta;
  Float_t posEta;
  Int_t eleTrackerDriven;
  Int_t eleEcalDriven;
  Int_t posTrackerDriven;
  Int_t posEcalDriven;
  treeOrig->SetBranchAddress("invMass",          &invMass);
  treeOrig->SetBranchAddress("trackerInvMass",   &trackerInvMass);
  treeOrig->SetBranchAddress("gsfTrackInvMass",  &gsfTrackInvMass);
  treeOrig->SetBranchAddress("deltaR",           &deltaR);
  treeOrig->SetBranchAddress("eleEta",           &eleEta);
  treeOrig->SetBranchAddress("posEta",           &posEta);
  treeOrig->SetBranchAddress("eleTrackerDriven", &eleTrackerDriven);
  treeOrig->SetBranchAddress("eleEcalDriven",    &eleEcalDriven);
  treeOrig->SetBranchAddress("posTrackerDriven", &posTrackerDriven);
  treeOrig->SetBranchAddress("posEcalDriven",    &posEcalDriven);

  // copy branches
  treeNew->Branch("invMass",           &invMass,           "invMass/F");  
  treeNew->Branch("trackerInvMass",    &trackerInvMass,    "trackerInvMass/F");  
  treeNew->Branch("gsfTrackInvMass",   &gsfTrackInvMass,   "gsfTrackInvMass/F");  
  treeNew->Branch("deltaR",            &deltaR,            "deltaR/F");  
  treeNew->Branch("eleEta",            &eleEta,            "eleEta/F");  
  treeNew->Branch("posEta",            &posEta,            "posEta/F");  
  treeNew->Branch("eleTrackerDriven",  &eleTrackerDriven,  "eleTrackerDriven/I");  
  treeNew->Branch("eleEcalDriven",     &eleEcalDriven,     "eleEcalDriven/I");  
  treeNew->Branch("posTrackerDriven",  &posTrackerDriven,  "posTrackerDriven/I");  
  treeNew->Branch("posEcalDriven",     &posEcalDriven,     "posEcalDriven/I");  

  float event = -1;
  treeNew->Branch("event",  &event,  "event/F");
  treeNew->Branch("weight", &weight, "weight/F");

  int j =0;

  for(int i=0; i<nentriesOrig; i++) {
    if (i%1000 == 0) std::cout << ">>> Weighting event # " << i << " / " << nentriesOrig << " entries" << std::endl;
    treeOrig->GetEntry(i);

    event = (float)j;
    treeNew->Fill();
  }
  
  fileNew->cd();
  treeNew->Write();
  fileNew->Close();
  
  fileOrig->cd();
  fileOrig->Close();

  } else {
    cout << "Tree T1 not present in the file " << filename << endl;
    return;
  }

}
