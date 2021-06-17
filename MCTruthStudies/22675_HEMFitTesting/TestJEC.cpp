#include <iostream>
using namespace std;

#include "JetCorrector.h"

int main(int argc, char *argv[])
{
   JetCorrector JEC("Correction.txt");

   JEC.SetJetEta(-1.8);
   JEC.SetJetPhi(-1.22);
   JEC.SetJetPT(100);
   JEC.SetJetArea(0.502);
   JEC.SetRho(20);

   cout << JEC.GetCorrection() << endl;

   return 0;
}



