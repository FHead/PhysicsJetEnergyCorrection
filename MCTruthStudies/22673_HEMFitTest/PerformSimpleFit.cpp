#include <iostream>
using namespace std;

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TF3.h"
#include "TLatex.h"

#include "CommandLine.h"
#include "CustomAssert.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

#include "JetCorrector.h"

#define SCAN_PT 0
#define SCAN_RHO 1
#define SCAN_PHI 2

class FitClass;
int main(int argc, char *argv[]);
void PlotComparison(PdfFileHelper &PdfFile, JetCorrector &JEC, TF3 &F, double Eta, double PT, double Rho, double Phi, int Scan);
double HEMFunction(const double *Parameters, double Rho, double PT, double Phi);

class FitClass
{
public:
   vector<double> PT;
   vector<double> Rho;
   vector<double> Phi;
   vector<double> OOA;
public:
   FitClass() {}
   ~FitClass() {}
   void DoFit();
   double LogLikelihood(const double *Parameters) const;
   vector<double> HEMDoFit(bool FixOutside = false, double FixP0 = 1, double FixP1 = 1, double FixP2 = 0);
   double HEMLogLikelihood(const double *Parameters) const;
};

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

   /*
   double P[] = {3.061176301, 0.33, -0.4380334449, 3.061176301, 0.34, -0.4380334449, 0, 0.4 / sqrt(2)};
   TH1D H("H", "H", 100, -M_PI, M_PI);
   TH1D H2("H2", "H2", 100, -M_PI, M_PI);
   for(int i = 1; i <= 100; i++)
   {
      double OOAOut = P[1] * (20 - P[0]) * (1 + P[2] * log(100.0 / 15));
      H2.SetBinContent(i, OOAOut);
      
      double x = H.GetBinCenter(i);
      double y = HEMFunction(P, 20, 100, x);
      H.SetBinContent(i, y);
   }
   PdfFile.AddPlot(H);
   PdfFile.AddPlot(H2);
   */
   
   TFile InputFile(InputFileName.c_str());

   TTree *Tree = (TTree *)InputFile.Get("OutputTree");
   Assert(Tree != nullptr, "Error getting input tree");

   double TreeRefEta;
   double TreeRefPT;
   double TreeOOA;
   double TreeRho;
   double TreePUPT;
   double TreePUPhi;

   Tree->SetBranchAddress("RefEta", &TreeRefEta);
   Tree->SetBranchAddress("RefPT", &TreeRefPT);
   Tree->SetBranchAddress("OOA", &TreeOOA);
   Tree->SetBranchAddress("Rho", &TreeRho);
   Tree->SetBranchAddress("PUPT", &TreePUPT);
   Tree->SetBranchAddress("PUPhi", &TreePUPhi);

   FitClass Data;

   TGraph2D Graph;

   int EntryCount = Tree->GetEntries() * Fraction;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      if(TreeRefEta < EtaMin || TreeRefEta >= EtaMax)
         continue;
      if(TreePUPT < PTMin || TreePUPT >= PTMax)
         continue;

      Data.Rho.push_back(TreeRho);
      Data.PT.push_back(TreePUPT);
      Data.Phi.push_back(TreePUPhi);
      Data.OOA.push_back(TreeOOA);

      int N = Graph.GetN();
      Graph.SetPoint(N, TreeRho, TreePUPT, TreeOOA);
   }

   JetCorrector JEC(ReferenceJECFileName);
   double JetEta = (EtaMin + EtaMax) / 2;

   int BinCount = JEC.JEC[0].Formulas.size();
   int Bin = 0;
   for(int iB = 0; iB < BinCount; iB++)
      if(JEC.JEC[0].BinRanges[iB][0] < JetEta && JEC.JEC[0].BinRanges[iB][1] >= JetEta)
         Bin = iB;
   cout << "0 " << JEC.JEC[0].Parameters[Bin][0] << endl;
   cout << "1 " << JEC.JEC[0].Parameters[Bin][1] << endl;
   cout << "2 " << JEC.JEC[0].Parameters[Bin][2] << endl;

   double P0 = JEC.JEC[0].Parameters[Bin][0];
   double P1 = JEC.JEC[0].Parameters[Bin][1];
   double P2 = JEC.JEC[0].Parameters[Bin][2];

   vector<double> Fit = Data.HEMDoFit(true, P0, P1, P2);

   /*
   string FOut = "([1]*(x-[0])*(1.+[2]*log(y/15.)))";
   string FIn  = "([4]*(x-[3])*(1.+[5]*log(y/15.)))";
   string ERF1 = "((TMath::Erf((-1.570796-z+[6])/[7])+1)/2)";
   string ERF2 = "((TMath::Erf((z+0.872665+[6])/[7])+1)/2)";
   string Function = FIn + "+" + "(" + FOut + "-" + FIn + ")*(" + ERF1 + "+" + ERF2 + ")";
   */
   
   string FOut = "([1]*(x-[0])*(1.+[2]*log(y/15.)))";
   string FIn  = "([4]*(x-[3])*(1.+[5]*log(y/15.)))";
   string ERF1 = "((TMath::Erf((-1.570796-z+[6])/[7])+1)/2)";
   string ERF2 = "((TMath::Erf((z+0.872665+[6])/[7])+1)/2)";
   string Function = FIn + "+" + "(" + FOut + "-" + FIn + ")*(" + ERF1 + "+" + ERF2 + ")";

   TF3 F("F", Function.c_str(), 0, 100, 0, 10000);
   for(int i = 0; i < 8; i++)
      F.SetParameter(i, Fit[i]);
   // Graph.Fit(&F);

   F.Print();

   PlotComparison(PdfFile, JEC, F, JetEta, 30,   10, -1.222, SCAN_RHO);
   PlotComparison(PdfFile, JEC, F, JetEta, 100,  10, -1.222, SCAN_RHO);
   PlotComparison(PdfFile, JEC, F, JetEta, 300,  10, -1.222, SCAN_RHO);
   PlotComparison(PdfFile, JEC, F, JetEta, 1000, 10, -1.222, SCAN_RHO);
   PlotComparison(PdfFile, JEC, F, JetEta, 0, 10, -1.222, SCAN_PT);
   PlotComparison(PdfFile, JEC, F, JetEta, 0, 20, -1.222, SCAN_PT);
   PlotComparison(PdfFile, JEC, F, JetEta, 0, 30, -1.222, SCAN_PT);
   PlotComparison(PdfFile, JEC, F, JetEta, 0, 40, -1.222, SCAN_PT);
   PlotComparison(PdfFile, JEC, F, JetEta, 0, 50, -1.222, SCAN_PT);
   PlotComparison(PdfFile, JEC, F, JetEta, 100, 20, 0, SCAN_PHI);
   PlotComparison(PdfFile, JEC, F, JetEta, 100, 40, 0, SCAN_PHI);
   PlotComparison(PdfFile, JEC, F, JetEta, 50, 20, 0, SCAN_PHI);
   PlotComparison(PdfFile, JEC, F, JetEta, 50, 40, 0, SCAN_PHI);

   InputFile.Close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

void PlotComparison(PdfFileHelper &PdfFile, JetCorrector &JEC, TF3 &F, double Eta, double PT, double Rho, double Phi, int Scan)
{
   double Min, Max;
   if(Scan == SCAN_PT)    Min = 8, Max = 6500;
   if(Scan == SCAN_RHO)   Min = 0, Max = 70;
   if(Scan == SCAN_PHI)   Min = -M_PI, Max = M_PI;

   double JetArea = 0.4 * 0.4 * M_PI;

   TGraph G1;   // new fit
   TGraph G2;   // reference from JEC
   TGraph G3;   // from HEM function

   double *P = F.GetParameters();

   int N = 100;
   for(int i = 0; i <= N; i++)
   {
      JEC.SetJetArea(JetArea);
      JEC.SetJetPT(PT);
      JEC.SetJetEta(Eta);
      JEC.SetJetPhi(Phi);
      JEC.SetRho(Rho);

      if(Scan == SCAN_RHO)
      {
         double X = (Max - Min) * i / N + Min;
         JEC.SetRho(X);

         G1.SetPoint(i, X, max(0.0001, 1 - F.Eval(X, PT, Phi) * JetArea / PT));
         G2.SetPoint(i, X, JEC.GetCorrection());
         G3.SetPoint(i, X, max(0.0001, 1 - HEMFunction(P, X, PT, Phi) * JetArea / PT));
      }
      if(Scan == SCAN_PT)
      {
         double Y = exp((log(Max) - log(Min)) * i / N + log(Min));
         JEC.SetJetPT(Y);
         
         G1.SetPoint(i, Y, max(0.0001, 1 - F.Eval(Rho, Y, Phi) * JetArea / Y));
         G2.SetPoint(i, Y, JEC.GetCorrection());
         G3.SetPoint(i, Y, max(0.0001, 1 - HEMFunction(P, Rho, Y, Phi) * JetArea / Y));
      }
      if(Scan == SCAN_PHI)
      {
         double Z = (Max - Min) * i / N + Min;
         JEC.SetJetPhi(Z);

         G1.SetPoint(i, Z, max(0.0001, 1 - F.Eval(Rho, PT, Z) * JetArea / PT));
         G2.SetPoint(i, Z, JEC.GetCorrection());
         G3.SetPoint(i, Z, max(0.0001, 1 - HEMFunction(P, Rho, PT, Z) * JetArea / PT));
      }
   }

   G1.SetLineWidth(2.5);
   G2.SetLineWidth(2.0);
   G3.SetLineWidth(1.5);

   G1.SetLineColor(kRed);
   G2.SetLineColor(kBlue);
   G3.SetLineColor(kGreen);

   if(SCAN_RHO == Scan)
   {
      G1.SetTitle(Form("PT = %.1f, Phi = %.3f", PT, Phi));
      G1.GetXaxis()->SetTitle("Rho");
   }
   if(SCAN_PT == Scan)
   {
      G1.SetTitle(Form("Rho = %.1f, Phi = %.3f", Rho, Phi));
      G1.GetXaxis()->SetTitle("PT");
   }
   if(SCAN_PHI == Scan)
   {
      G1.SetTitle(Form("PT = %.1f, Rho = %.1f", PT, Rho));
      G1.GetXaxis()->SetTitle("Phi");
   }

   G1.GetYaxis()->SetRangeUser(0.0, 2.0);

   TCanvas Canvas;

   G1.Draw("al");
   G2.Draw("l");
   G3.Draw("l");

   if(SCAN_PT == Scan)
      Canvas.SetLogx();

   TLatex Latex;
   Latex.SetNDC();
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.025);
   Latex.SetTextAlign(33);
   Latex.DrawLatex(0.80, 0.15, "#color[2]{Function} #color[4]{Reference JEC} #color[3]{Evaluation}");

   PdfFile.AddCanvas(Canvas);
}

void FitClass::DoFit()
{
   ROOT::Math::Minimizer *Worker = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");

   Worker->SetPrintLevel(1);
   Worker->SetMaxFunctionCalls(1e9);
   Worker->SetMaxIterations(1e9);
   // Worker->SetTolerance(1e-10);
   Worker->SetStrategy(1);   // 0 = speed, 2 = robustness, 1 is in the middle

   ROOT::Math::Functor Function(this, &FitClass::LogLikelihood, 3);
   Worker->SetFunction(Function);

   Worker->SetLimitedVariable(0, "P0", 1, 0.1, -10, 10);
   Worker->SetLimitedVariable(1, "P1", 1, 0.1, 0, 10);
   Worker->SetLimitedVariable(2, "P2", 0, 0.1, -10, 10);

   Worker->Minimize();
}

double FitClass::LogLikelihood(const double *Parameters) const
{
   double E2 = 0;

   // This is the function "[1]*(x-[0])*(1.+[2]*log(y/15.))"
   
   int N = OOA.size();
   for(int i = 0; i < N; i++)
   {
      double Y = Parameters[1] * (Rho[i] - Parameters[0]) * (1 + Parameters[2] * log(PT[i] / 15));
      E2 = E2 + (OOA[i] - Y) * (OOA[i] - Y);
   }

   return E2;
}

vector<double> FitClass::HEMDoFit(bool FixOutside, double FixP0, double FixP1, double FixP2)
{
   ROOT::Math::Minimizer *Worker = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");

   Worker->SetPrintLevel(1);
   Worker->SetMaxFunctionCalls(1e9);
   Worker->SetMaxIterations(1e9);
   // Worker->SetTolerance(1e-10);
   Worker->SetStrategy(2);   // 0 = speed, 2 = robustness, 1 is in the middle

   ROOT::Math::Functor Function(this, &FitClass::HEMLogLikelihood, 8);
   Worker->SetFunction(Function);

   if(FixOutside == false)
   {
      Worker->SetLimitedVariable(0, "P0", 1, 0.1, 0, 10);
      Worker->SetLimitedVariable(1, "P1", 1, 0.1, 0, 10);
      Worker->SetLimitedVariable(2, "P2", 0, 0.1, -10, 10);
   }
   else
   {
      Worker->SetFixedVariable(0, "P0", FixP0);
      Worker->SetFixedVariable(1, "P1", FixP1);
      Worker->SetFixedVariable(2, "P2", FixP2);
   }
   Worker->SetLimitedVariable(3, "HEMP0", FixP0, 0.1, -10000, 10000);
   Worker->SetLimitedVariable(4, "HEMP1", FixP1, 0.1, 0, 100);
   Worker->SetLimitedVariable(5, "HEMP2", FixP2, 0.1, -10000, 10000);
   Worker->SetLimitedVariable(6, "Offset", 0, 0.05, -0.3, 0.3);
   Worker->SetFixedVariable(7, "Width", 0.4 / sqrt(2));
   
   for(int i = 0; i < 10; i++)
      Worker->Minimize();

   const double *X = Worker->X();
   vector<double> FitResult(X, X + 8);

   return FitResult;
}

double FitClass::HEMLogLikelihood(const double *Parameters) const
{
   double E2 = 0;

   // This is the function "[1]*(x-[0])*(1.+[2]*log(y/15.))"
   
   int N = OOA.size();
   for(int i = 0; i < N; i++)
   {
      double Y = HEMFunction(Parameters, Rho[i], PT[i], Phi[i]);
      E2 = E2 + (OOA[i] - Y) * (OOA[i] - Y);
   }

   return E2;
}

double HEMFunction(const double *Parameters, double Rho, double PT, double Phi)
{
   double HEMMin = -4.5 * M_PI / 9 + Parameters[6];
   double HEMMax = -2.5 * M_PI / 9 - Parameters[6];

   double OOAOut = Parameters[1] * (Rho - Parameters[0]) * (1 + Parameters[2] * log(PT / 15));
   double OOAIn = Parameters[4] * (Rho - Parameters[3]) * (1 + Parameters[5] * log(PT / 15));
   // double OOAIn = Parameters[4] * (Rho - Parameters[3]) * (1 + Parameters[5] * log(PT / 15) + Parameters[6] * log(PT/15) * log(PT/15));

   double Y = OOAIn
      + (OOAOut - OOAIn) * (erf((Phi - HEMMax) / Parameters[7]) + 1) / 2
      + (OOAOut - OOAIn) * (erf((HEMMin - Phi) / Parameters[7]) + 1) / 2;

   return Y;
}
