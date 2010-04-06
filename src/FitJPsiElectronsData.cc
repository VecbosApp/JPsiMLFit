// Set Fit Options
MLOptions GetDefaultOptions() {
  MLOptions opts;
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
  RooRealVar* weight  = new RooRealVar("weight",  "weight", 0., 100.);
  
  theFit.AddFlatFileColumn(invMass);
  theFit.AddFlatFileColumn(weight);
  
  // define a fit model
  theFit.addModel("myFit", "JPsitoEE");
  
  // define species
  theFit.addSpecies("myFit", "sig", "Signal Component");
  theFit.addSpecies("myFit", "bkg", "Bkg   Component");
  
  // mLL PDF
  theFit.addPdfWName("myFit", "sig" , "invMass",  "TripleGaussian", "sig_mass");
  theFit.addPdfWName("myFit", "bkg" , "invMass",  "Poly2",          "bkg_mass");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fit a sample of JPsi events
void FitJPsiElectrons() {
  
  myFit();

  // Load the data
  char datasetname[200];
  sprintf(datasetname,"datasets/JPsi_ee_data.root");
  theFit.addDataSetFromRootFile("T1", "T1", datasetname);
  RooDataSet *data = theFit.getDataSet("T1");
  data->setWeightVar("weight");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit...
  char fitconfig[200];
  sprintf(fitconfig,"fitconfig/fitJPsi.config");
  theFit.initialize(fitconfig);
  
  // Print Fit configuration 
  myPdf->getParameters(data)->selectByAttrib("Constant",kTRUE)->Print("V");  
  myPdf->getParameters(data)->selectByAttrib("Constant",kFALSE)->Print("V");
  
  RooFitResult *fitres =  myPdf->fitTo(*data,theFit.getNoNormVars("myFit"),"MHTER");
  fitres->Print("V");
  
  // write the config file corresponding to the fit minimum
  char configfilename[200];
  sprintf(configfilename, "fitres/fitResult-JPsi.config");
  theFit.writeConfigFile(configfilename);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlotJPsiElectrons(int nbins=19) {

  myFit();

  // Load the data
  char datasetname[200];
  sprintf(datasetname,"datasets/JPsi_ee_data.root");
  theFit.addDataSetFromRootFile("T1", "T1", datasetname);
  RooDataSet *data = theFit.getDataSet("T1");
  data->setWeightVar("weight");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");

  // Initialize the fit...
  char configfilename[200];
  sprintf(configfilename, "fitres/fitResult-JPsi.config");
  theFit.initialize(configfilename);

  TCanvas *c = new TCanvas("c","fitResult");
  char rootfilename[200];
  sprintf(rootfilename,"fitres/JPsi.root");
  TFile *output = new TFile(rootfilename,"RECREATE");

  RooPlot* MassPlot = MakePlot("invMass", &theFit, data, nbins, false);    

  MassPlot->SetYTitle("Events");
  MassPlot->Draw();
  char epsfilename[200];
  sprintf(epsfilename,"fitres/JPsi.eps");
  c->SaveAs(epsfilename);
  MassPlot->Write();
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

