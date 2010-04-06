#include <iostream>
#include <string>

using namespace std;

double weight(string sample, double ngen, double xsec, double filtereff, double lumi = 1, double prescale = 1);

int main(int argc, char* argv[]) {
  
  double wantLumi = 1.0; //pb-1  
  double w;

  std::cout << "sample" << "\t" << "xsec" << "\t" << "prescale" << "\t" << "weight" << std:: endl;
  
  w = weight("JPsiee",                          83458.,     38000., 1.0, wantLumi);
  w = weight("QCD_EMDoubleEnriched_Pt20",    10654167.,  70000000., 1.0, wantLumi);
  w = weight("QCD_EMDoubleEnriched_Pt6to20",  9986186., 480000000., 1.0, wantLumi);

  return 0;

}

double weight(string sample, double ngen, double xsec, double filtereff, 
	   double lumi, double prescale) {

  double W = xsec * filtereff * lumi * prescale / ngen;

  std::cout << sample << "\t" << xsec << "\t" << prescale << "\t" << W << std:: endl;

  return W;

}
