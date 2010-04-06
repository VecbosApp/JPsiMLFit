#! /bin/sh

# usage: ./createToyConfig

echo ""
echo "   [Parameters]   "
echo ""

grep sig_Mass shapesJPsiee/config/fitMinimum-JPsionly.config 
echo ""
grep bkg_Mass shapesJPsiee/config/fitMinimum-bkgonly.config

# yields
if [ $? -eq 0 ] ; then
    
    echo "N_sig =  20359 +/- 220.00  L(-100 - 900000)"
    echo "eff_sig =  0.884 +/- 0.0022246  L(0 - 1)"
    echo ""
    echo "N_bkg =  1364 +/- 50.000  L(-500 - 10000)"
    echo "eff_bkg =  0.377 +/- 0.0010562  L(0 - 1)"

fi

echo ""
echo "   [Observables]"
echo ""
echo "Mass           = 3 C L(0 - 4)      // [GeV]"

