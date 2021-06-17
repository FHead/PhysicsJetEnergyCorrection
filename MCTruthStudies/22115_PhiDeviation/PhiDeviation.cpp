#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "ProgressBar.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> FileNames = CL.GetStringVector("input");
   string Algorithm         = CL.Get("algorithm", "ak4pfchs");
   string OutputFileName    = CL.Get("output");
   double EtaMin            = CL.GetDouble("etamin", -3.0);
   double EtaMax            = CL.GetDouble("etamax", -1.5);
   vector<double> PTBins    = CL.GetDoubleVector("pt", vector<double>({10, 15, 20, 30, 50, 70, 100, 150, 200, 300, 500, 1000}));

   string TreeName = Algorithm + "/t";
   int N = PTBins.size() - 1;

   vector<TProfile *> H;
   for(int i = 0; i < N; i++)
      H.push_back(new TProfile(Form("H%d", i), Form("PT = %.1f-%.1f", PTBins[i], PTBins[i+1]), 50, -2.5, 0.0));

   PdfFileHelper PdfFile(OutputFileName);

   for(string FileName : FileNames)
   {
      TFile File(FileName.c_str());

      TTree *Tree = (TTree *)File.Get(TreeName.c_str());

      vector<float> *RefPT = nullptr, *RefEta = nullptr, *RefPhi = nullptr;
      vector<float> *JetPT = nullptr, *JetEta = nullptr, *JetPhi = nullptr;

      Tree->SetBranchAddress("refpt",  &RefPT);
      Tree->SetBranchAddress("refeta", &RefEta);
      Tree->SetBranchAddress("refphi", &RefPhi);
      Tree->SetBranchAddress("jtpt",   &JetPT);
      Tree->SetBranchAddress("jteta",  &JetEta);
      Tree->SetBranchAddress("jtphi",  &JetPhi);

      int EntryCount = Tree->GetEntries();
      ProgressBar Bar(cout, EntryCount);
      Bar.SetStyle(-1);

      cout << "Processing file " << FileName << endl;

      for(int iE = 0; iE < EntryCount; iE++)
      {
         Tree->GetEntry(iE);

         Bar.Update(iE);
         if(EntryCount < 500 || iE % (EntryCount / 300) == 0)
            Bar.Print();

         for(int iJ = 0; iJ < (int)RefPT->size(); iJ++)
         {
            if((*RefEta)[iJ] >= EtaMin && (*RefEta)[iJ] <= EtaMax)
            {
               for(int i = 0; i < N; i++)
                  if((*RefPT)[iJ] >= PTBins[i] && (*RefPT)[iJ] < PTBins[i+1])
                     H[i]->Fill((*RefPhi)[iJ], (*JetPhi)[iJ] - (*RefPhi)[iJ]);
            }
         }
      }

      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();

      File.Close();
   }

   TGraphAsymmErrors GH, GW, GS;

   for(int i = 0; i < N; i++)
   {
      TF1 F("F", "[0]*(exp(-(x-[2])**2/[1]/[1])-exp(-(x-[3])**2/[1]/[1]))");
      F.SetParameters(0.015, 0.25, -0.86, -1.63);
      H[i]->Fit(&F);
      H[i]->SetMaximum(0.1);
      H[i]->SetMinimum(-0.05);
      PdfFile.AddPlot(H[i]);

      double X = (PTBins[i+1] + PTBins[i]) / 2;
      double DX = (PTBins[i+1] - PTBins[i]) / 2;

      GH.SetPoint(i, X, F.GetParameter(0));
      GH.SetPointError(i, DX, DX, F.GetParError(0), F.GetParError(0));
      
      double W = fabs(F.GetParameter(2) - F.GetParameter(3));
      W = W - 2 * M_PI / 9;

      double EW = sqrt(F.GetParError(2) * F.GetParError(2) + F.GetParError(3) * F.GetParError(3));

      GW.SetPoint(i, X, W);
      GW.SetPointError(i, DX, DX, EW, EW);

      GS.SetPoint(i, X, fabs(F.GetParameter(1)));
      GS.SetPointError(i, DX, DX, F.GetParError(1), F.GetParError(1));
   }

   GH.SetNameTitle("GH", "Max deviation");
   GW.SetNameTitle("GW", "Effective width - 40 degrees");
   GS.SetNameTitle("GS", "Sigma");

   GH.GetXaxis()->SetTitle("RefPT");
   GH.GetYaxis()->SetTitle("Max deviation");
   GW.GetXaxis()->SetTitle("RefPT");
   GW.GetYaxis()->SetTitle("Effective width - 40 degrees");
   GS.GetXaxis()->SetTitle("RefPT");
   GS.GetYaxis()->SetTitle("Sigma");

   PdfFile.AddPlot(GH, "", false, false, true, true);
   PdfFile.AddPlot(GW, "", false, false, true, true);
   PdfFile.AddPlot(GS, "", false, false, true, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   for(int i = 0; i < N; i++)
      delete H[i];

   return 0;
}





