#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TF2.h"

// #include "RooRealVar.h"
// #include "RooDataSet.h"
// #include "RooArgSet.h"
// #include "RooGenericPdf.h"
// using namespace RooFit;

#include "CommandLine.h"
#include "CustomAssert.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

#include "JetCorrector.h"

int main(int argc, char *argv[]);
void PlotComparison(PdfFileHelper &PdfFile, JetCorrector &JEC, TF2 &F, double Eta, double Fix, bool FixPT);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   double EtaMin = CL.GetDouble("EtaMin", 0.000);
   double EtaMax = CL.GetDouble("EtaMax", 0.087);
   double PTMin = CL.GetDouble("PTMin", 8);
   double PTMax = CL.GetDouble("PTMax", 6500);
   string InputFileName = CL.Get("Input");
   double Fraction = CL.GetDouble("Fraction", 1.00);
   string OutputFileName = CL.Get("Output", "JEC.pdf");
   string ReferenceJECFileName = CL.Get("JEC", "");

   cout << ReferenceJECFileName << endl;

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("JEC testing");

   TFile InputFile(InputFileName.c_str());

   TTree *Tree = (TTree *)InputFile.Get("OutputTree");
   Assert(Tree != nullptr, "Error getting input tree");

   double TreeRefEta;
   double TreeRefPT;
   double TreeOOA;
   double TreeRho;
   double TreePUPT;

   Tree->SetBranchAddress("RefEta", &TreeRefEta);
   Tree->SetBranchAddress("RefPT", &TreeRefPT);
   Tree->SetBranchAddress("OOA", &TreeOOA);
   Tree->SetBranchAddress("Rho", &TreeRho);
   Tree->SetBranchAddress("PUPT", &TreePUPT);

   TGraph2D Graph;

   /*
   RooRealVar OOA("OOA", "OOA", -1000, 1000);
   RooRealVar Rho("Rho", "Rho", 0, 100);
   RooRealVar PUPT("PUPT", "PUPT", 0, 5000);

   RooArgSet Observables(OOA, Rho, PUPT);

   RooDataSet Data("Data", "Main dataset", Observables);
   */

   int EntryCount = Tree->GetEntries() * Fraction;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      if(TreeRefEta < EtaMin || TreeRefEta >= EtaMax)
         continue;
      if(TreePUPT < PTMin || TreePUPT >= PTMax)
         continue;

      int N = Graph.GetN();
      Graph.SetPoint(N, TreeRho, TreePUPT, TreeOOA);

      /*
      OOA.setVal(TreeOOA);
      Rho.setVal(TreeRho);
      PUPT.setVal(TreePUPT);

      Data.add(Observables);
      */
   }

   /*
   Data.Print("V");

   RooRealVar P0("P0", "P0", 1, 0, 10);
   RooRealVar P1("P1", "P1", 1, 0, 2);
   RooRealVar P2("P2", "P2", 0, -1, 1);

   RooGenericPdf PDF("PDF", "PDF", "P1*(Rho-P0)*(1+P2*log(PUPT/15))", RooArgSet(OOA, Rho, PUPT, P0, P1, P2));
   */

   TF2 F("F", "[1]*(x-[0])*(1.+[2]*log(y/15.))", 0, 100, 0, 10000);
   F.SetParameters(1, 1, 0);
   Graph.Fit(&F);

   JetCorrector JEC(ReferenceJECFileName);

   double JetEta = (EtaMin + EtaMax) / 2;
   PlotComparison(PdfFile, JEC, F, JetEta, 30, true);
   PlotComparison(PdfFile, JEC, F, JetEta, 100, true);
   PlotComparison(PdfFile, JEC, F, JetEta, 300, true);
   PlotComparison(PdfFile, JEC, F, JetEta, 1000, true);
   PlotComparison(PdfFile, JEC, F, JetEta, 10, false);
   PlotComparison(PdfFile, JEC, F, JetEta, 20, false);
   PlotComparison(PdfFile, JEC, F, JetEta, 30, false);
   PlotComparison(PdfFile, JEC, F, JetEta, 40, false);
   PlotComparison(PdfFile, JEC, F, JetEta, 50, false);

   InputFile.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void PlotComparison(PdfFileHelper &PdfFile, JetCorrector &JEC, TF2 &F, double Eta, double Fix, bool FixPT)
{
   double Min = FixPT ? 0 : 8;
   double Max = FixPT ? 70 : 6500;

   double JetArea = 0.4 * 0.4 * M_PI;

   TGraph G1;   // new fit
   TGraph G2;   // reference from JEC

   int N = 100;
   for(int i = 0; i <= N; i++)
   {
      if(FixPT == true)
      {
         double X = (Max - Min) * i / N + Min;

         JEC.SetJetArea(JetArea);
         JEC.SetJetPT(Fix);
         JEC.SetJetEta(Eta);
         JEC.SetJetPhi(0);
         JEC.SetRho(X);

         G1.SetPoint(i, X, max(0.0001, 1 - F.Eval(X, Fix) * JetArea / Fix));
         G2.SetPoint(i, X, JEC.GetCorrection());
      }
      else
      {
         double Y = exp((log(Max) - log(Min)) * i / N + log(Min));

         JEC.SetJetArea(JetArea);
         JEC.SetJetPT(Y);
         JEC.SetJetEta(Eta);
         JEC.SetJetPhi(0);
         JEC.SetRho(Fix);
         
         G1.SetPoint(i, Y, max(0.0001, 1 - F.Eval(Fix, Y) * JetArea / Y));
         G2.SetPoint(i, Y, JEC.GetCorrection());
      }
   }

   G1.SetLineWidth(2);
   G2.SetLineWidth(2);

   G1.SetLineColor(kRed);
   G2.SetLineColor(kBlue);

   if(FixPT == true)
   {
      G1.SetTitle(Form("PT = %.1f", Fix));
      G1.GetXaxis()->SetTitle("Rho");
   }
   else
   {
      G1.SetTitle(Form("Rho = %.1f", Fix));
      G1.GetXaxis()->SetTitle("PT");
   }

   G1.GetYaxis()->SetRangeUser(0.8, 1.2);

   TCanvas Canvas;

   G1.Draw("al");
   G2.Draw("l");

   if(FixPT == false)
      Canvas.SetLogx();

   PdfFile.AddCanvas(Canvas);
}




