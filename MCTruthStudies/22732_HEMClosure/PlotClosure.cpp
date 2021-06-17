#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TGraph.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

#include "JetCorrector.h"

#define PLOT_PT 0
#define PLOT_ETA 1
#define PLOT_PHI 2

struct JetData;
int main(int argc, char *argv[]);
void PlotClosure(PdfFileHelper &PdfFile, int ToPlot, vector<JetData> &Data, JetCorrector &JEC,
   double PTMin, double PTMax, double EtaMin, double EtaMax, double PhiMin, double PhiMax);

struct JetData
{
   double RefPT;
   double RefEta;
   double RefPhi;
   double Rho;
   double PT;
};

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> JECFileNames = CL.GetStringVector("JEC", vector<string>());
   string InputFileName = CL.Get("Input");
   string OutputFileName = CL.Get("Output", "Closure.pdf");
   double Fraction = CL.GetDouble("Fraction", 0.05);

   TFile File(InputFileName.c_str());
   TTree *Tree = (TTree *)File.Get("OutputTree");

   double RecoDR;

   JetData M;
   Tree->SetBranchAddress("PUPT", &M.PT);
   Tree->SetBranchAddress("RefPT", &M.RefPT);
   Tree->SetBranchAddress("RefEta", &M.RefEta);
   Tree->SetBranchAddress("RefPhi", &M.RefPhi);
   Tree->SetBranchAddress("Rho", &M.Rho);
   Tree->SetBranchAddress("RecoDR", &RecoDR);

   vector<JetData> Data;
   int EntryCount = Tree->GetEntries() * Fraction;
   for(int i = 0; i < EntryCount; i++)
   {
      Tree->GetEntry(i);
      if(RecoDR > 0.2)
         continue;
      Data.push_back(M);
   }

   File.Close();

   JetCorrector JEC(JECFileNames);

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("JEC Closure");

   PlotClosure(PdfFile, PLOT_PT, Data, JEC, 10, 6500, -0.5, 0.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 30, 60, 0, 1.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 100, 200, 0, 1.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 400, 800, 0, 1.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 30, 60, -2.0, -1.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 100, 200, -2.0, -1.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 400, 800, -2.0, -1.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 30, 60, -2.5, -2.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 100, 200, -2.5, -2.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 400, 800, -2.5, -2.0, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 30, 60, -3.0, -2.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 100, 200, -3.0, -2.5, -M_PI, M_PI);
   PlotClosure(PdfFile, PLOT_PHI, Data, JEC, 400, 800, -3.0, -2.5, -M_PI, M_PI);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void PlotClosure(PdfFileHelper &PdfFile, int ToPlot, vector<JetData> &Data, JetCorrector &JEC,
   double PTMin, double PTMax, double EtaMin, double EtaMax, double PhiMin, double PhiMax)
{
   double HistMin = 0;
   double HistMax = 1;

   if(ToPlot == PLOT_PT)    HistMin = 10, HistMax = 6500;
   if(ToPlot == PLOT_ETA)   HistMin = -5.2, HistMax = 5.2;
   if(ToPlot == PLOT_PHI)   HistMin = -M_PI, HistMax = M_PI;

   int NBin = 40;
   double Bins[1001];
   for(int i = 0; i <= NBin; i++)
   {
      double X;
      if(ToPlot == PLOT_PT)
         Bins[i] = exp((log(HistMax) - log(HistMin)) / NBin * i + log(HistMin));
      else
         Bins[i] = (HistMax - HistMin) / NBin * i + HistMin;
   }

   TProfile H1("H1", ";;", NBin, Bins);
   TProfile H2("H2", ";;", NBin, Bins);

   for(int i = 0; i < (int)Data.size(); i++)
   {
      if(Data[i].RefPT < PTMin || Data[i].RefPT > PTMax)
         continue;
      if(Data[i].RefEta < EtaMin || Data[i].RefEta > EtaMax)
         continue;
      if(Data[i].RefPhi < PhiMin || Data[i].RefPhi > PhiMax)
         continue;

      JEC.SetJetPT(Data[i].PT);
      JEC.SetJetEta(Data[i].RefEta);
      JEC.SetJetPhi(Data[i].RefPhi);
      JEC.SetJetArea(0.4 * 0.4 * M_PI);
      JEC.SetRho(Data[i].Rho);
      
      if(JEC.GetCorrectedPT() < 0 || JEC.GetCorrectedPT() != JEC.GetCorrectedPT())
         cout << "ERROR " << Data[i].PT << " " << Data[i].RefEta << " " << Data[i].RefPhi << endl;
      else
      {
         double X = 0;
         if(ToPlot == PLOT_PT)    X = Data[i].RefPT;
         if(ToPlot == PLOT_ETA)   X = Data[i].RefEta;
         if(ToPlot == PLOT_PHI)   X = Data[i].RefPhi;

         H1.Fill(X, Data[i].PT / Data[i].RefPT);
         H2.Fill(X, JEC.GetCorrectedPT() / Data[i].RefPT);

         if(JEC.GetCorrectedPT() / Data[i].RefPT > 5)
            cout << JEC.GetCorrectedPT() << " " << Data[i].RefPT << " " << Data[i].PT << " " << Data[i].RefEta << " " << Data[i].RefPhi << " " << Data[i].Rho << endl;
      }
   }

   H1.SetLineColor(kRed);
   H1.SetMarkerColor(kRed);
   H1.SetLineWidth(2);
   H1.SetMarkerSize(1.2);

   TGraph GLine;
   GLine.SetPoint(0, HistMin, 1.00);
   GLine.SetPoint(1, HistMax, 1.00);

   TH2D HWorld("HWorld", ";;", NBin, Bins, 100, 0.5, 1.5);
   HWorld.SetStats(0);

   if(ToPlot == PLOT_PT)    HWorld.SetTitle(Form("Eta [%.3f,%.3f], Phi [%.3f,%.3f]", EtaMin, EtaMax, PhiMin, PhiMax));
   if(ToPlot == PLOT_ETA)   HWorld.SetTitle(Form("PT [%.1f,%.1f], Phi [%.3f,%.3f]", PTMin, PTMax, PhiMin, PhiMax));
   if(ToPlot == PLOT_PHI)   HWorld.SetTitle(Form("PT [%.1f,%.1f], Eta [%.3f,%.3f]", PTMin, PTMax, EtaMin, EtaMax));

   if(ToPlot == PLOT_PT)    HWorld.GetXaxis()->SetTitle("PT");
   if(ToPlot == PLOT_ETA)   HWorld.GetXaxis()->SetTitle("Eta");
   if(ToPlot == PLOT_PHI)   HWorld.GetXaxis()->SetTitle("Phi");
   
   TCanvas Canvas;

   HWorld.Draw();
   GLine.Draw("l");
   H1.Draw("same");
   H2.Draw("same");

   if(ToPlot == PLOT_PT)
      Canvas.SetLogx();

   PdfFile.AddCanvas(Canvas);
}



