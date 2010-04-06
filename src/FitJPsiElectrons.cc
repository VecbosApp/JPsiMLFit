
// Set Fit Options
MLOptions GetDefaultOptions() {
  MLOptions opts;
  // Fit configuration
  opts.addBoolOption("weightedDataset", "use event weight instead of 1", kTRUE);
  opts.addBoolOption("AllFit",          "Fit all species",               kFALSE);
  opts.addBoolOption("JPsiOnlyFit",     "Fit J/Psi species only",        kFALSE);
  opts.addBoolOption("bkgOnlyFit",      "Fit bkg species only",          kTRUE);

  return opts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

MLFit theFit;

void myFit() {

  MLFit theFit;

  // Various fit options...
  MLOptions opts = GetDefaultOptions();

  // define the structure of the dataset
  RooRealVar* invMass = new RooRealVar("invMass", "inv mass [GeV/c^{2}]" , 1.5, 4.);
  RooRealVar* weight  = new RooRealVar("weight",  "weight",1);

  theFit.AddFlatFileColumn(invMass);
  theFit.AddFlatFileColumn(weight);

  // define a fit model
  theFit.addModel("myFit","JPsitoEE");
  
  // define species
  theFit.addSpecies("myFit", "sig", "Signal Component");
  theFit.addSpecies("myFit", "bkg", "Bkg Component");
  
  // mLL PDF
  theFit.addPdfWName("myFit", "sig" , "invMass",  "TripleGaussian", "sig_mass");
  theFit.addPdfWName("myFit", "bkg" , "invMass",  "Poly2",   "bkg_mass");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fit a sample of JPsi events
void FitJPsiElectrons() {
  
  myFit();
  
  // Various fit options...
  MLOptions opts = GetDefaultOptions();

  // Load the data
  char datasetname[200];
  if(opts.getBoolVal("AllFit"))      sprintf(datasetname,"datasets/jpsiee_data_3X.root");  
  if(opts.getBoolVal("JPsiOnlyFit")) sprintf(datasetname,"datasets/JPsi_ee.root");  
  if(opts.getBoolVal("bkgOnlyFit"))  sprintf(datasetname,"datasets/QCD_EMDoubleEnriched.root");    
  theFit.addDataSetFromRootFile("T1", "T1", datasetname);
  RooDataSet *data = theFit.getDataSet("T1");

  // use event weights
  if(opts.getBoolVal("weightedDataset")) data->setWeightVar("weight");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit...
  if(opts.getBoolVal("AllFit")) {
    char initconfigfile[200];
    sprintf(initconfigfile,"fitconfig/fitJPsi.config");
    theFit.initialize(initconfigfile);
  }
  if(opts.getBoolVal("JPsiOnlyFit")) theFit.initialize("shapesJPsiee/config/JPsiFit-JPsionly.config");
  if(opts.getBoolVal("bkgOnlyFit"))  theFit.initialize("shapesJPsiee/config/JPsiFit-bkgonly.config");

  // Print Fit configuration 
  myPdf->getParameters(data)->selectByAttrib("Constant",kTRUE)->Print("V");  
  myPdf->getParameters(data)->selectByAttrib("Constant",kFALSE)->Print("V");
  
  RooFitResult *fitres =  myPdf->fitTo(*data,theFit.getNoNormVars("myFit"),"MHTER");   
  fitres->Print("V");
  
  // write the config file corresponding to the fit minimum
  char configfilename[200];
  if(opts.getBoolVal("AllFit"))      sprintf(configfilename, "fitres/fitMinimumJPsi.config");
  if(opts.getBoolVal("JPsiOnlyFit")) sprintf(configfilename, "shapesJPsiee/config/fitMinimum-JPsionly.config");
  if(opts.getBoolVal("bkgOnlyFit"))  sprintf(configfilename, "shapesJPsiee/config/fitMinimum-bkgonly.config");
  theFit.writeConfigFile(configfilename);  
  
  // save the fit result in ROOT 
  char rootfilename[200];
  if(opts.getBoolVal("AllFit"))      sprintf(rootfilename, "fitres/fitMinimumJPsi.root");
  if(opts.getBoolVal("JPsiOnlyFit")) sprintf(rootfilename,"shapesJPsiee/root/fitRes-JPsionly.root");
  if(opts.getBoolVal("bkgOnlyFit"))  sprintf(rootfilename,"shapesJPsiee/root/fitRes-bkgonly.root");
  
  TFile *file = new TFile(rootfilename,"recreate");
  fitres->Write();
  file->Close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlotJPsiElectrons(int nbins=19) {

  myFit();

  // Various fit options...
  MLOptions opts = GetDefaultOptions();

  // Load the data
  char datasetname[200];
  if(opts.getBoolVal("AllFit"))      sprintf(datasetname,"datasets/jpsiee_data_3X.root");  
  if(opts.getBoolVal("JPsiOnlyFit")) sprintf(datasetname,"datasets/JPsi_ee.root");  
  if(opts.getBoolVal("bkgOnlyFit"))  sprintf(datasetname,"datasets/QCD_EMDoubleEnriched.root");    
  theFit.addDataSetFromRootFile("T1", "T1", datasetname);
  RooDataSet *data = theFit.getDataSet("T1");

  bool usePoissonError=true;
  // use event weights
  if(opts.getBoolVal("weightedDataset")) {
    data->setWeightVar("weight");
    usePoissonError=false;
  }

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit...
  char configfilename[200];
  if(opts.getBoolVal("AllFit"))      sprintf(configfilename, "fitres/fitMinimumJPsi.config");
  if(opts.getBoolVal("JPsiOnlyFit")) sprintf(configfilename, "shapesJPsiee/config/fitMinimum-JPsionly.config");
  if(opts.getBoolVal("bkgOnlyFit"))  sprintf(configfilename, "shapesJPsiee/config/fitMinimum-bkgonly.config");
  theFit.initialize(configfilename);
  
  TCanvas *c = new TCanvas("c","fitResult");
  RooPlot* MassPlot = MakePlot("invMass", &theFit, data, nbins,usePoissonError);    
  
  MassPlot->SetAxisColor(1,"x");
  MassPlot->SetLabelColor(1, "X");
  MassPlot->SetLabelColor(1, "Y");
  MassPlot->SetXTitle("m_{ee}[GeV/c^{2}]");
  
  MassPlot->SetYTitle("Events");
  MassPlot->Draw();
  
  char epsfilename[200];
  char Cfilename[200];
  
  if(opts.getBoolVal("AllFit")) {
    sprintf(epsfilename,"fit-plots/eps/Mass-data.eps");
    sprintf(Cfilename,"fit-plots/macro/Mass-data.C");
  }
  if(opts.getBoolVal("JPsiOnlyFit")) {
    sprintf(epsfilename,"shapesJPsiee/eps/Mass-JPsionly.eps");
    sprintf(Cfilename,"shapesJPsiee/macro/Mass-JPsionly.C");
  }
  if(opts.getBoolVal("bkgOnlyFit")) {
    sprintf(epsfilename,"shapesJPsiee/eps/Mass-bkgonly.eps");
    sprintf(Cfilename,"shapesJPsiee/macro/Mass-bkgonly.C");
  }
  c->SaveAs(epsfilename);
  c->SaveAs(Cfilename);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Make the plot for a given variable
RooPlot *MakePlot(TString VarName, MLFit* theFit, RooDataSet* theData, int nbins, bool poissonError=true)
{
  RooRealVar* Var = theFit->RealObservable(VarName);
  double min=Var->getMin();
  double max=Var->getMax();
  RooPlot *plot = Var->frame(min,max,nbins);
  
  // plot the data
  if(poissonError)
    theData->plotOn(plot);
  else 
    theData->plotOn(plot, RooFit::DataError(RooAbsData::SumW2) );

  // plot the total likelihood
  RooAbsPdf *thePdf = theFit->getPdf("myFit");
  thePdf->plotOn(plot, RooFit::LineColor(kBlack));

  double Ns = theFit->getRealPar("N_sig")->getVal();
  double Nb = theFit->getRealPar("N_bkg")->getVal();

  // plot (dashed) the bkg component
  theFit->getRealPar("N_sig")->setVal(0.);
  thePdf->plotOn(plot, RooFit::Normalization(Nb/(Ns+Nb)),RooFit::LineColor(kBlack),RooFit::LineStyle(kDashed));

  
  return plot;
}
