
// Set Fit Options
MLOptions GetDefaultOptions() {
  MLOptions opts;
  return opts;
}


void Generate(Int_t nexp = 1, UInt_t iseed = 65539, char* outfile= 0)
{
  // Various fit options...
  MLOptions opts = GetDefaultOptions();
  
  const char* e0 = "\033[44;37m";
  const char* en="\033[0m";
  
  TDatime *now = new TDatime();
  int today = now->GetDate();
  int clock = now->GetTime();
  int seed  = today+clock+iseed;

  // Set the random number seed...
  RooRandom::randomGenerator()->SetSeed(seed);

  // define the structure of the dataset
  RooRealVar* invMass = new RooRealVar("invMass",  "inv mass [GeV/c^{2}]" , 1.5, 4.);
  RooRealVar* weight  = new RooRealVar("weight",  "weight",1);

  MLFit theFit;

  theFit.AddFlatFileColumn(invMass);

  // define a fit model
  theFit.addModel("myFit", "JPsiToEE");
  
  // define species
  theFit.addSpecies("myFit", "sig", "Signal Component");
  theFit.addSpecies("myFit", "bkg", "Bkg   Component");
  
  // mLL PDF
  theFit.addPdfWName("myFit", "sig" , "invMass",  "TripleGaussian", "sig_Mass");
  theFit.addPdfWName("myFit", "bkg" , "invMass",  "Poly2",          "bkg_Mass");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit...
  char configfilename[200];
  sprintf(configfilename, "toyconfig/toyJPsi.config");

  theFit.initialize(configfilename);
  MLGenerator theGenerator(theFit, "myFit");
  
  Int_t ngen =
    theFit.getRealPar("N_sig")->getVal()+
    theFit.getRealPar("N_bkg")->getVal();

  // Generate...
  RooArgSet genVars(theFit.getObsList(MLStrList("invMass")));
  MLToyStudy theStudy(theGenerator, genVars, "E", "MTE", 0, theFit.getNoNormVars("myFit"));
  theStudy.addFit(*myPdf);

  theStudy.generateAndFit(nexp,ngen);
  
  char resultfilename[200];
  if (outfile==0) sprintf(resultfilename,"toys/resultsJPsi.dat");
  else sprintf(resultfilename, outfile);
  theStudy._fitParData->write(resultfilename);
  
  char variablesfilename[200];
  sprintf(variablesfilename,"toys/variablesJPsi.root");
  TFile varfile(variablesfilename,"RECREATE");

  RooArgSet *variables = theStudy._fitParData->get();
  variables->setName("variables");
  variables->Write();
  varfile.Close();
}
