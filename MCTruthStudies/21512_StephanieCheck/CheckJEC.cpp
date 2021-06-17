#include <iostream>
using namespace std;

#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "PlotHelper3.h"
#include "SetStyle.h"

#include "JetCorrector.h"

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> FileNames     = CL.GetStringVector("JEC", {"A", "B", "C", "D"});
   double PT = CL.GetDouble("PT");
   double Eta = CL.GetDouble("Eta");
   double Phi = CL.GetDouble("Phi");
   double Rho = CL.GetDouble("Rho");
   double Area = CL.GetDouble("Area");

   cout << "Input Jet " << endl;
   cout << "   RawPT = " << PT << endl;
   cout << "   Eta = " << Eta << endl;
   cout << "   Phi = " << Phi << endl;
   cout << "   Rho = " << Rho << endl;
   cout << "   Area = " << Area << endl;
   cout << endl;

   int N = FileNames.size();

   vector<JetCorrector> JEC(N);
   for(int i = 0; i < N; i++)
      JEC[i].Initialize(FileNames[i]);

   double TotalCorrection = 1.0;

   for(int i = 0; i < N; i++)
   {
      JEC[i].SetJetPT(PT);
      JEC[i].SetJetEta(Eta);
      JEC[i].SetJetPhi(Phi);
      JEC[i].SetRho(Rho);
      JEC[i].SetJetArea(Area);

      TotalCorrection = TotalCorrection * JEC[i].GetCorrection();

      cout << "Level: " << FileNames[i] << endl;
      cout << "   PT: " << PT << " x " << JEC[i].GetCorrection() << " ==> " << JEC[i].GetCorrectedPT() << endl;
      cout << "   Total correction so far = " << TotalCorrection << endl;
      cout << endl;

      PT = JEC[i].GetCorrectedPT();
   }

   return 0;
}

