#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TF1.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   int Bin = CL.GetInt("Bin", 21);
   string OutputFileName = CL.Get("Output", "OOA.pdf");

   int NPTBin = 7;
   int NPUBin = 7;
   double PTBins[] = {10, 20, 50, 100, 200, 500, 1000, 10000};
   double PUBins[] = {0, 10, 20, 30, 40, 50, 60, 80, 100};

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("<OOA> plots");

   TH2D HOut("HOut",   "Outside;PT;PU",    NPTBin, PTBins, NPUBin, PUBins);
   TH2D HIn("HIn",     "Inside;PT;PU",     NPTBin, PTBins, NPUBin, PUBins);
   TH2D HDiff("HDiff", "Difference;PT;PU", NPTBin, PTBins, NPUBin, PUBins);

   HOut.SetStats(0);
   HIn.SetStats(0);
   HDiff.SetStats(0);

   TFile InputFile(InputFileName.c_str());

   for(int iPT = 0; iPT < NPTBin; iPT++)
   {
      for(int iPU = 0; iPU < NPUBin; iPU++)
      {
         TProfile *P = (TProfile *)InputFile.Get(Form("POOAVsPhi_%d_%d_%d", Bin, iPT, iPU));

         TF1 F("F", "[0]+[1]*exp(-(x+1.222)*(x+1.222)/(2*[2]*[2]))", -M_PI, M_PI);
         F.SetParameters(0, -10, 0.28);

         for(int i = 0; i < 10; i++)
            P->Fit(&F, "W");

         HOut.SetBinContent(iPT + 1, iPU + 1, F.GetParameter(0));
         HIn.SetBinContent(iPT + 1, iPU + 1, F.GetParameter(0) + F.GetParameter(1));
         HDiff.SetBinContent(iPT + 1, iPU + 1, F.GetParameter(1));

         PdfFile.AddPlot(P);
      }
   }

   InputFile.Close();
   
   PdfFile.AddPlot(HOut,  "text00 colz", false, false, false, true);
   PdfFile.AddPlot(HIn,   "text00 colz", false, false, false, true);
   PdfFile.AddPlot(HDiff, "text00 colz", false, false, false, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}



