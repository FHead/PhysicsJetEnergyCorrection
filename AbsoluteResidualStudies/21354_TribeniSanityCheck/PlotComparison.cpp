#include <iostream>
using namespace std;

#include "TFile.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLatex.h"

#include "CommandLine.h"
#include "SetStyle.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[]);
void AddGraphComparison(PdfFileHelper &PdfFile, TFile &F1, TFile &F2, string Name,
   string Title, double XMin, double XMax, double YMin, double YMax, bool LogY = false, bool LogX = false);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string FileName1 = CL.Get("FileName1");
   string FileName2 = CL.Get("FileName2");
   string OutputFileName = CL.Get("OutputFileName");

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Comparison Lucas vs Tribeni");

   TFile File1(FileName1.c_str());
   TFile File2(FileName2.c_str());

   AddGraphComparison(PdfFile, File1, File2, "resp_PtBalchs_a30_eta00_08",
      "resp_PtBalchs_a30_eta00_08", 20, 2000, 0.5, 1.5, false, true);
   AddGraphComparison(PdfFile, File1, File2, "resp_MPFchs_a30_eta00_08",
      "resp_MPFchs_a30_eta00_08", 20, 2000, 0.5, 1.5, false, true);

   File2.Close();
   File1.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void AddGraphComparison(PdfFileHelper &PdfFile, TFile &F1, TFile &F2, string Name,
   string Title, double XMin, double XMax, double YMin, double YMax, bool LogY, bool LogX)
{
   TH2D HWorld("HWorld", Title.c_str(), 100, XMin, XMax, 100, YMin, YMax);
   HWorld.SetStats(0);

   TGraphErrors *G1 = (TGraphErrors *)F1.Get(Name.c_str());
   TGraphErrors *G2 = (TGraphErrors *)F2.Get(Name.c_str());

   TCanvas Canvas;

   HWorld.Draw("axis");

   if(G1 != nullptr)   G1->Draw("lp");
   if(G2 != nullptr)   G2->Draw("lp");

   G1->SetMarkerColor(kBlue);
   G1->SetLineColor(kBlue);
   G2->SetMarkerColor(kRed);
   G2->SetLineColor(kRed);

   if(LogX)   Canvas.SetLogx();
   if(LogY)   Canvas.SetLogy();

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.03);
   Latex.SetNDC();
   Latex.DrawLatex(0.10, 0.91, "#color[2]{Tribeni} #color[4]{Lucas}");

   PdfFile.AddCanvas(Canvas);
}


