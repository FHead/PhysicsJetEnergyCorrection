#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"

#include "SetStyle.h"
#include "CommandLine.h"
#include "PlotHelper4.h"

#include "JetCorrector.h"

int main(int argc, char *argv[]);
void SetHistograms(vector<vector<TProfile *>> &V);
void WriteHistograms(vector<vector<TProfile *>> &V);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string InputFileName      = CL.Get("Input");
   string JECFileName        = CL.Get("JEC", "");
   string OutputFileName     = CL.Get("Output", "Result.pdf");
   string OutputRootFileName = CL.Get("OutputRoot", "Result.root");
   double Fraction           = CL.GetDouble("Fraction", 1.00);
   bool PlotPT               = CL.GetBool("PlotPT", true);
   bool PlotPU               = CL.GetBool("PlotPT", true);
   bool PlotPhi              = CL.GetBool("PlotPhi", true);

   JetCorrector JEC(JECFileName);

   vector<double> EtaBins = {-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191};
   // vector<double> EtaBins = {-3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139};
   
   vector<double> PTBins = {10, 20, 50, 100, 200, 500, 1000, 10000};
   vector<double> RhoBins = {5, 15, 25, 35, 45, 100};
   vector<double> PUBins = {0, 10, 20, 30, 40, 50, 60, 80, 100};

   double LogPTBins[101];
   for(int i = 0; i <= 100; i++)
      LogPTBins[i] = exp((log(6000) - log(1)) / 100 * i);

   TFile OutputFile(OutputRootFileName.c_str(), "RECREATE");

   vector<vector<TProfile *>> PGoodOOAVsRho;
   vector<vector<TProfile *>> PGoodOOAVsPT;
   vector<vector<TProfile *>> PBadOOAVsRho;
   vector<vector<TProfile *>> PBadOOAVsPT;
   vector<vector<vector<TProfile *>>> POOAVsPhi;

   for(int iEta = 0; iEta + 1 < (int)EtaBins.size(); iEta++)
   {
      PGoodOOAVsRho.push_back(vector<TProfile *>());
      PGoodOOAVsPT.push_back(vector<TProfile *>());
      PBadOOAVsRho.push_back(vector<TProfile *>());
      PBadOOAVsPT.push_back(vector<TProfile *>());
      POOAVsPhi.push_back(vector<vector<TProfile *>>(PTBins.size()));
      
      for(int iPT = 0; iPT + 1 < (int)PTBins.size(); iPT++)
      {
         string Name = Form("PGoodOOAVsRho_%d_%d", iEta, iPT);
         string Title = Form("(Out) Eta: [%.3f, %.3f], PT [%.1f, %.1f];Rho;<OOA>",
            EtaBins[iEta], EtaBins[iEta+1], PTBins[iPT], PTBins[iPT+1]);
         PGoodOOAVsRho[iEta].push_back(new TProfile(Name.c_str(), Title.c_str(), 100, 0, 50));
         
         Name = Form("PBadOOAVsRho_%d_%d", iEta, iPT);
         Title = Form("(In) Eta: [%.3f, %.3f], PT [%.1f, %.1f];Rho;<OOA>",
            EtaBins[iEta], EtaBins[iEta+1], PTBins[iPT], PTBins[iPT+1]);
         PBadOOAVsRho[iEta].push_back(new TProfile(Name.c_str(), Title.c_str(), 50, 0, 50));
      }
      for(int iPU = 0; iPU + 1 < (int)PUBins.size(); iPU++)
      {
         string Name = Form("PGoodOOAVsPT_%d_%d", iEta, iPU);
         string Title = Form("(Out) Eta: [%.3f, %.3f], PU [%.1f, %.1f];PT;<OOA>",
            EtaBins[iEta], EtaBins[iEta+1], PUBins[iPU], PUBins[iPU+1]);
         PGoodOOAVsPT[iEta].push_back(new TProfile(Name.c_str(), Title.c_str(), 100, LogPTBins));
         
         Name = Form("PBadOOAVsPT_%d_%d", iEta, iPU);
         Title = Form("(In) Eta: [%.3f, %.3f], PU [%.1f, %.1f];PT;<OOA>",
            EtaBins[iEta], EtaBins[iEta+1], PUBins[iPU], PUBins[iPU+1]);
         PBadOOAVsPT[iEta].push_back(new TProfile(Name.c_str(), Title.c_str(), 50, LogPTBins));
      }
      for(int iPT = 0; iPT + 1 < (int)PTBins.size(); iPT++)
      {
         for(int iPU = 0; iPU + 1 < (int)PUBins.size(); iPU++)
         {
            string Name = Form("POOAVsPhi_%d_%d_%d", iEta, iPT, iPU);
            string Title = Form("Eta: [%.3f, %.3f], PU [%.1f, %.1f], PT [%.1f, %.1f];Phi;<OOA>",
               EtaBins[iEta], EtaBins[iEta+1], PUBins[iPU], PUBins[iPU+1], PTBins[iPT], PTBins[iPT+1]);
            POOAVsPhi[iEta][iPT].push_back(new TProfile(Name.c_str(), Title.c_str(), 40, -M_PI, M_PI));
         }
      }
   }

   SetHistograms(PGoodOOAVsRho);
   SetHistograms(PGoodOOAVsPT);
   SetHistograms(PBadOOAVsRho);
   SetHistograms(PBadOOAVsPT);
   for(auto &v : POOAVsPhi)
      SetHistograms(v);

   double HEMMax = -2.5 / 9 * M_PI;
   double HEMMin = -4.5 / 9 * M_PI;
   double Padding = 0.2;

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("HEM L1 Fun");

   TFile File(InputFileName.c_str());

   TTree *Tree = (TTree *)File.Get("OutputTree");

   double RefPT;     Tree->SetBranchAddress("RefPT", &RefPT);
   double RefEta;    Tree->SetBranchAddress("RefEta", &RefEta);
   double RefPhi;    Tree->SetBranchAddress("RefPhi", &RefPhi);
   double NoPUPT;    Tree->SetBranchAddress("NoPUPT", &NoPUPT);
   double NoPUEta;   Tree->SetBranchAddress("NoPUEta", &NoPUEta);
   double NoPUPhi;   Tree->SetBranchAddress("NoPUPhi", &NoPUPhi);
   double PUPT;      Tree->SetBranchAddress("PUPT", &PUPT);
   double PUEta;     Tree->SetBranchAddress("PUEta", &PUEta);
   double PUPhi;     Tree->SetBranchAddress("PUPhi", &PUPhi);
   double Rho;       Tree->SetBranchAddress("Rho", &Rho);
   double NPU;       Tree->SetBranchAddress("NPU", &NPU);
   double OOA;       Tree->SetBranchAddress("OOA", &OOA);

   int EntryCount = Tree->GetEntries() * Fraction;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      double Area = (PUPT - NoPUPT) / OOA;   // I didn't store area in the trees!  Hack for now

      JEC.SetJetPT(PUPT);
      JEC.SetJetArea(Area);
      JEC.SetJetPhi(PUPhi);
      JEC.SetJetEta(PUEta);
      JEC.SetRho(Rho);

      double NewPUPT = JEC.GetCorrectedPT();
      if(NewPUPT < 0)
         NewPUPT = PUPT;

      double NewOOA = (NewPUPT - NoPUPT) / Area;

      int iEta = upper_bound(EtaBins.begin(), EtaBins.end(), RefEta) - EtaBins.begin() - 1;
      int iPT = upper_bound(PTBins.begin(), PTBins.end(), RefPT) - PTBins.begin() - 1;
      int iRho = upper_bound(RhoBins.begin(), RhoBins.end(), Rho) - RhoBins.begin() - 1;
      int iPU = upper_bound(PUBins.begin(), PUBins.end(), NPU) - PUBins.begin() - 1;

      if(iEta < 0 || iEta >= (int)EtaBins.size() - 1)   // Eta out of range
         continue;
      if(iPT < 0 || iPT >= (int)PTBins.size() - 1)      // PT out of range
         continue;
      if(iRho < 0 || iRho >= (int)RhoBins.size() - 1)   // Rho out of range
         continue;
      if(iPU < 0 || iPU >= (int)PUBins.size() - 1)   // PU out of range
         continue;

      POOAVsPhi[iEta][iPT][iPU]->Fill(PUPhi, NewOOA);
      if(RefPhi > HEMMax + Padding || RefPhi < HEMMin - Padding)
      {
         PGoodOOAVsRho[iEta][iPT]->Fill(Rho, NewOOA);
         PGoodOOAVsPT[iEta][iPU]->Fill(RefPT, NewOOA);
      }
      if(RefPhi < HEMMax && RefPhi > HEMMin)
      {
         PBadOOAVsRho[iEta][iPT]->Fill(Rho, NewOOA);
         PBadOOAVsPT[iEta][iPU]->Fill(RefPT, NewOOA);
      }
   }

   File.Close();

   for(int iEta = 0; iEta + 1 < (int)EtaBins.size(); iEta++)
   {
      PdfFile.AddTextPage(Form("Eta: [%.3f, %.3f]", EtaBins[iEta], EtaBins[iEta+1]));

      if(PlotPU == true)
      {
         for(int iPT = 0; iPT + 1 < (int)PTBins.size(); iPT++)
            PdfFile.AddPlot(PGoodOOAVsRho[iEta][iPT], "");
         for(int iPT = 0; iPT + 1 < (int)PTBins.size(); iPT++)
            PdfFile.AddPlot(PBadOOAVsRho[iEta][iPT], "");
      }
      if(PlotPT == true)
      {
         for(int iPU = 0; iPU + 1 < (int)PUBins.size(); iPU++)
            PdfFile.AddPlot(PGoodOOAVsPT[iEta][iPU], "", false, false, false, true);
         for(int iPU = 0; iPU + 1 < (int)PUBins.size(); iPU++)
            PdfFile.AddPlot(PBadOOAVsPT[iEta][iPU], "", false, false, false, true);
      }
      if(PlotPhi == true)
      {
         for(int iPT = 0; iPT + 1 < (int)PTBins.size(); iPT++)
            for(int iPU = 0; iPU + 1 < (int)PUBins.size(); iPU++)
               PdfFile.AddPlot(POOAVsPhi[iEta][iPT][iPU], "");
      }
   }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   OutputFile.cd();

   WriteHistograms(PGoodOOAVsRho);
   WriteHistograms(PGoodOOAVsPT);
   WriteHistograms(PBadOOAVsRho);
   WriteHistograms(PBadOOAVsPT);
   for(auto &v : POOAVsPhi)
      WriteHistograms(v);

   OutputFile.Close();

   return 0;
}

void SetHistograms(vector<vector<TProfile *>> &V)
{
   for(auto v : V)
   {
      for(auto P : v)
      {
         if(P != nullptr)
         {
            P->SetStats(0);
         }
      }
   }
}

void WriteHistograms(vector<vector<TProfile *>> &V)
{
   for(auto v : V)
      for(auto P : v)
         if(P != nullptr)
            P->Write();
}



