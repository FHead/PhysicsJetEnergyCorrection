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
vector<double> DoEtaBin(PdfFileHelper &PdfFile, TTree *Tree, double Fraction, double PTMin, double PTMax, double EtaMin, double EtaMax, string ReferenceJECFileName, bool Skip);
void PlotComparison(PdfFileHelper &PdfFile, JetCorrector &JEC, TF3 &F, double Eta, double PT, double Rho, double Phi, int Scan);
double HEMFunction(const double *Parameters, double Rho, double PT, double Phi);
string GetFunction(string Phi, double PTMin, bool RhoOnly = false);

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
   vector<double> HEMDoFit(PdfFileHelper &PdfFile,
      bool FixOutside = false, double FixP0 = 1, double FixP1 = 1, double FixP2 = 0);
   void AppendFitCheckPlots(PdfFileHelper &PdfFile, double *Parameters);
   void PlotFitCheck(PdfFileHelper &PdfFile, double *Parameters,
      const double FixPT, const double FixRho, const double FixPhi, const int Scan);
   double HEMLogLikelihood(const double *Parameters) const;
};

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   double PTMin = CL.GetDouble("PTMin", 8);
   double PTMax = CL.GetDouble("PTMax", 6500);
   string InputFileName = CL.Get("Input");
   double Fraction = CL.GetDouble("Fraction", 1.00);
   string OutputFileName = CL.Get("Output", "JEC.pdf");
   string ReferenceJECFileName = CL.Get("JEC", "");
   string OutputJECFileName = CL.Get("OutputJEC", "JEC.txt");

   bool CopyMode = CL.GetBool("CopyMode", false);
   double CopyEtaMin = CL.GetDouble("CopyEtaMin", -3.000 - 0.4);
   double CopyEtaMax = CL.GetDouble("CopyEtaMax", -1.392 + 0.4);

   cout << ReferenceJECFileName << endl;

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("JEC testing");

   ofstream outfull("Full" + OutputJECFileName);
   ofstream out(OutputJECFileName);

   string Function = GetFunction("t", PTMin, false);
   string FunctionRhoOnly = GetFunction("t", PTMin, true);
   
   outfull << "{1 JetEta 4 Rho JetPt JetA JetPhi ";
   outfull << "max(0.0001,1-(z/y)*(" << Function << "))";
   outfull << " Correction L1FastJet}" << endl;
   
   out << "{1 JetEta 4 Rho JetPt JetA JetPhi ";
   out << "max(0.0001,1-(z/y)*(" << FunctionRhoOnly << "))";
   out << " Correction L1FastJet}" << endl;

   TFile InputFile(InputFileName.c_str());

   vector<double> EtaBins = {-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191};
   // vector<double> EtaBins = {-1.83, -1.74};

   TTree *Tree = (TTree *)InputFile.Get("OutputTree");
   Assert(Tree != nullptr, "Error getting input tree");
   
   for(int i = 0; i + 1 < (int)EtaBins.size(); i++)
   {
      cout << "Processing eta bin [" << EtaBins[i] << ", " << EtaBins[i+1] << "]" << endl;

      bool Skip = false;
      if(CopyMode == true)
      {
         if(EtaBins[i+1] < CopyEtaMin)
            Skip = true;
         if(EtaBins[i] > CopyEtaMax)
            Skip = true;
      }

      vector<double> Result =
         DoEtaBin(PdfFile, Tree, Fraction, PTMin, PTMax, EtaBins[i], EtaBins[i+1], ReferenceJECFileName, Skip);

      outfull << EtaBins[i] << " " << EtaBins[i+1] << " " << 8 + Result.size();
      outfull << " 0 70 0 6500 0 10 -3.1416 3.1416 ";
      for(int j = 0; j < (int)Result.size(); j++)
         outfull << " " << Result[j];
      outfull << endl;

      out << EtaBins[i] << " " << EtaBins[i+1] << " " << 8 + Result.size() - 3;
      out << " 0 70 0 6500 0 10 -3.1416 3.1416 ";
      for(int j = 0; j < (int)Result.size(); j++)
      {
         if(j == 3 || j == 5 || j == 6)
            continue;
         out << " " << Result[j];
      }
      out << endl;
   }
   
   InputFile.Close();

   out.close();
   outfull.close();

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

vector<double> DoEtaBin(PdfFileHelper &PdfFile, TTree *Tree, double Fraction, double PTMin, double PTMax, double EtaMin, double EtaMax, string ReferenceJECFileName, bool Skip)
{
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

   if(Skip == true)
      Fraction = 0.01;

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

   vector<double> Fit = Data.HEMDoFit(PdfFile, true, P0, P1, P2);

   string Function = GetFunction("z", PTMin);
   if(Skip == true)
      Fit[7] = M_PI / 9;

   TF3 F("F", Function.c_str(), 0, 100, 0, 10000);
   for(int i = 0; i < 9; i++)
      F.SetParameter(i, Fit[i]);
   // Graph.Fit(&F);

   F.Print();

   PdfFile.AddTextPage(Form("eta = [%.3f, %.3f]", EtaMin, EtaMax));
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

   return Fit;
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
   TGraph G5;   // Rho-only
   
   TGraph G4;   // OOA from HEM function

   double *P = F.GetParameters();
   double PRho[9];
   for(int i = 0; i < 9; i++)
      PRho[i] = P[i];
   PRho[3] = 0;
   PRho[5] = 0;
   PRho[6] = 0;

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
         G4.SetPoint(i, X, HEMFunction(P, X, PT, Phi));
         G5.SetPoint(i, X, max(0.0001, 1 - HEMFunction(PRho, X, PT, Phi) * JetArea / PT));
      }
      if(Scan == SCAN_PT)
      {
         double Y = exp((log(Max) - log(Min)) * i / N + log(Min));
         JEC.SetJetPT(Y);
         
         G1.SetPoint(i, Y, max(0.0001, 1 - F.Eval(Rho, Y, Phi) * JetArea / Y));
         G2.SetPoint(i, Y, JEC.GetCorrection());
         G3.SetPoint(i, Y, max(0.0001, 1 - HEMFunction(P, Rho, Y, Phi) * JetArea / Y));
         G4.SetPoint(i, Y, HEMFunction(P, Rho, Y, Phi));
         G5.SetPoint(i, Y, max(0.0001, 1 - HEMFunction(PRho, Rho, Y, Phi) * JetArea / Y));
      }
      if(Scan == SCAN_PHI)
      {
         double Z = (Max - Min) * i / N + Min;
         JEC.SetJetPhi(Z);

         G1.SetPoint(i, Z, max(0.0001, 1 - F.Eval(Rho, PT, Z) * JetArea / PT));
         G2.SetPoint(i, Z, JEC.GetCorrection());
         G3.SetPoint(i, Z, max(0.0001, 1 - HEMFunction(P, Rho, PT, Z) * JetArea / PT));
         G4.SetPoint(i, Z, HEMFunction(P, Rho, PT, Z));
         G5.SetPoint(i, Z, max(0.0001, 1 - HEMFunction(PRho, Rho, PT, Z) * JetArea / PT));
      }
   }

   G1.SetLineWidth(2.5);
   G2.SetLineWidth(2.0);
   G3.SetLineWidth(1.5);
   G5.SetLineWidth(1.2);

   G1.SetLineColor(kRed);
   G2.SetLineColor(kBlue);
   G3.SetLineColor(kGreen);
   G5.SetLineColor(kMagenta);

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

   G1.GetYaxis()->SetTitle("Correction");
   G1.GetYaxis()->SetRangeUser(0.0, 2.0);
   G4.GetYaxis()->SetTitle("Fitted OOA");

   TCanvas Canvas;

   G1.Draw("al");
   G2.Draw("l");
   G3.Draw("l");
   G5.Draw("l");

   if(SCAN_PT == Scan)
      Canvas.SetLogx();

   TLatex Latex;
   Latex.SetNDC();
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.025);
   Latex.SetTextAlign(33);
   Latex.DrawLatex(0.80, 0.15, "#color[2]{Function} #color[4]{Reference JEC} #color[3]{Delta} #color[6]{Rho}");

   PdfFile.AddCanvas(Canvas);

   G4.Draw("al");

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

vector<double> FitClass::HEMDoFit(PdfFileHelper &PdfFile,
   bool FixOutside, double FixP0, double FixP1, double FixP2)
{
   ROOT::Math::Minimizer *Worker = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");

   Worker->SetPrintLevel(1);
   Worker->SetMaxFunctionCalls(1e9);
   Worker->SetMaxIterations(1e9);
   // Worker->SetTolerance(1e-10);
   Worker->SetStrategy(2);   // 0 = speed, 2 = robustness, 1 is in the middle

   ROOT::Math::Functor Function(this, &FitClass::HEMLogLikelihood, 9);
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
   Worker->SetLimitedVariable(3, "HEMP0", -0.06, 0.01, -200, 200);
   // Worker->SetFixedVariable(3, "HEMP0", 0);
   Worker->SetLimitedVariable(4, "HEMP1", 1.28, 0.01, -10, 10);
   Worker->SetLimitedVariable(5, "HEMP2", 0.00526, 0.01, -1000, 1000);
   Worker->SetLimitedVariable(6, "HEMP3", 0, 0.01, -1000, 1000);
   // Worker->SetFixedVariable(5, "HEMP2", 0);
   // Worker->SetFixedVariable(6, "HEMP3", 0);
   Worker->SetFixedVariable(7, "Offset", 0.055);
   // Worker->SetFixedVariable(8, "Width", 0.4 / sqrt(2));
   Worker->SetLimitedVariable(8, "Width", 0.4 / sqrt(2), 0.01, 0.1, 0.4);
   
   for(int i = 0; i < 10; i++)
      Worker->Minimize();

   const double *X = Worker->X();
   vector<double> FitResult(X, X + 9);

   double P[9] = {0};
   for(int i = 0; i < 9; i++)
      P[i] = X[i];

   AppendFitCheckPlots(PdfFile, P);

   return FitResult;
}

void FitClass::AppendFitCheckPlots(PdfFileHelper &PdfFile, double *Parameters)
{
   PlotFitCheck(PdfFile, Parameters, 40, 10, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 40, 30, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 40, 50, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 100, 10, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 100, 30, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 100, 50, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 300, 10, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 300, 30, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 300, 50, -1.22, SCAN_PHI);
   PlotFitCheck(PdfFile, Parameters, 40, 10, -1.22, SCAN_RHO);
   PlotFitCheck(PdfFile, Parameters, 100, 10, -1.22, SCAN_RHO);
   PlotFitCheck(PdfFile, Parameters, 300, 10, -1.22, SCAN_RHO);
   PlotFitCheck(PdfFile, Parameters, 100, 10, -1.22, SCAN_PT);
   PlotFitCheck(PdfFile, Parameters, 100, 30, -1.22, SCAN_PT);
   PlotFitCheck(PdfFile, Parameters, 100, 50, -1.22, SCAN_PT);
}

void FitClass::PlotFitCheck(PdfFileHelper &PdfFile, double *Parameters,
   const double FixPT, const double FixRho, const double FixPhi, const int Scan)
{
   double DeltaPT = 0.1;
   double DeltaRho = 10;
   double DeltaPhi = 0.4;

   double MinPT  = FixPT * (1 - DeltaPT / 2);
   double MaxPT  = FixPT * (1 + DeltaPT / 2);
   double MinRho = FixRho - DeltaRho / 2;
   double MaxRho = FixRho + DeltaRho / 2;
   double MinPhi = FixPhi - DeltaPhi / 2;
   double MaxPhi = FixPhi + DeltaPhi / 2;

   if(Scan == SCAN_PT)    MinPT = 8, MaxPT = 6500;
   if(Scan == SCAN_RHO)   MinRho = 0, MaxRho = 70;
   if(Scan == SCAN_PHI)   MinPhi = -M_PI, MaxPhi = M_PI;
   
   double Min, Max;
   if(Scan == SCAN_PT)    Min = MinPT, Max = MaxPT;
   if(Scan == SCAN_RHO)   Min = MinRho, Max = MaxRho;
   if(Scan == SCAN_PHI)   Min = MinPhi, Max = MaxPhi;

   double JetArea = 0.4 * 0.4 * M_PI;

   string PTString = Form("PT [%.1f, %.1f]", MinPT, MaxPT);
   string RhoString = Form("Rho [%.1f, %.1f]", MinRho, MaxRho);
   string PhiString = Form("Phi [%.2f, %.2f]", MinPhi, MaxPhi);

   string Title = "";
   if(Scan == SCAN_PT)    Title = RhoString + ", " + PhiString + ";PT;Extra OOA";
   if(Scan == SCAN_RHO)   Title = PTString + ", " + PhiString + ";Rho;Extra OOA";
   if(Scan == SCAN_PHI)   Title = PTString + ", " + RhoString + ";Phi;Extra OOA";

   int NBin = 50;
   double Bins[51] = {0};
   for(int i = 0; i <= 50; i++)
   {
      if(Scan != SCAN_PT)
         Bins[i] = (Max - Min) / NBin * i + Min;
      if(Scan == SCAN_PT)
         Bins[i] = exp((log(Max) - log(Min)) / NBin * i + log(Min));
   }

   TGraph G;   // OOA from HEM function
   TProfile P("P", Title.c_str(), 50, Bins);   // actual fit data
   P.SetStats(0);

   int N = 100;
   for(int i = 0; i <= N; i++)
   {
      double TempPT = FixPT;
      double TempRho = FixRho;
      double TempPhi = FixPhi;

      double X;

      if(Scan == SCAN_RHO)
      {
         TempRho = (Max - Min) * i / N + Min;
         X = TempRho;
      }
      if(Scan == SCAN_PT)
      {
         TempPT = exp((log(Max) - log(Min)) * i / N + log(Min));
         X = TempPT;
      }
      if(Scan == SCAN_PHI)
      {
         TempPhi = (Max - Min) * i / N + Min;
         X = TempPhi;
      }
         
      G.SetPoint(i, X, HEMFunction(Parameters, TempRho, TempPT, TempPhi));
   }

   int DataCount = OOA.size();
   for(int i = 0; i < DataCount; i++)
   {
      if(MinPT >= PT[i] || PT[i] >= MaxPT)
         continue;
      if(MinRho >= Rho[i] || Rho[i] >= MaxRho)
         continue;
      if(MinPhi >= Phi[i] || Phi[i] >= MaxPhi)
         continue;

      double X;
      if(Scan == SCAN_PT)    X = PT[i];
      if(Scan == SCAN_RHO)   X = Rho[i];
      if(Scan == SCAN_PHI)   X = Phi[i];

      P.Fill(X, OOA[i]);
   }

   TCanvas Canvas;

   P.Draw();
   G.Draw("l");
   
   if(Scan == SCAN_PT)
      Canvas.SetLogx();

   PdfFile.AddCanvas(Canvas);
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
   double HEMMin = -4.5 * M_PI / 9 + Parameters[7];
   double HEMMax = -2.5 * M_PI / 9 - Parameters[7];

   // We are doing PU on PU, outside is 0 by definition
   // double OOAOut = Parameters[1] * (Rho - Parameters[0]) * (1 + Parameters[2] * log(PT / 15));
   double OOAOut = 0;
   
   // Standard
   // double OOAIn = (Parameters[4] * Rho - Parameters[3]) * (1 + Parameters[5] * log(PT / 15));

   // Test 22674 (quadratic)
   // double OOAIn = Parameters[4] * (Rho - Parameters[3]) * (1 + Parameters[5] * log(PT / 15) + Parameters[6] * log(PT/15) * log(PT/15));
   
   // Modified 22675
   // double OOAIn = Parameters[3] + Parameters[4] * Rho + Parameters[5] * log(PT / 15) + Parameters[6] * Rho * log(PT / 15);
   
   // Modified 22711
   // double OOAIn = Parameters[3] + Parameters[4] * Rho + Parameters[5] * log(PT / 15) + Parameters[6] * log(PT/15) * log(PT/15);
   
   // Modified 22715
   // double OOAIn = Parameters[3] * pow(PT, Parameters[4]) * exp(-Parameters[5] * PT) + Parameters[6] * Rho;
   
   // Modified 22715 - decoupled
   // double OOAIn = Parameters[3] + Parameters[4] * Rho + Parameters[5] * PT + Parameters[6] * log(PT/15);
   
   // Modified 22715 - rho only
   // double OOAIn = Parameters[3] + Parameters[4] * Rho;
   
   // Modified 22725
   double OOAIn = Parameters[3] + Parameters[4] * Rho + Parameters[5] * log(PT/15) + Parameters[6] * log(PT/15) * log(PT/15);

   double Y = OOAIn
      + (OOAOut - OOAIn) * (erf((Phi - HEMMax) / Parameters[8]) + 1) / 2
      + (OOAOut - OOAIn) * (erf((HEMMin - Phi) / Parameters[8]) + 1) / 2;

   return Y;
}

string GetFunction(string Phi, double PTMin, bool RhoOnly)
{
   string FOut = "([1]*(x-[0])*(1.+[2]*log(y/15.)))";
   string FIn  = Form("([3]+[4]*x+[5]*log(y/15)+[6]*log(y/15)*log(y/15))");
   string ERF1 = "((TMath::Erf((-1.570796-" + Phi + "+[7])/[8])+1)/2)";
   string ERF2 = "((TMath::Erf((" + Phi + "+0.872665+[7])/[8])+1)/2)";
   
   if(RhoOnly == true)
   {
      FIn  = Form("([3]*x)");
      ERF1 = "((TMath::Erf((-1.570796-" + Phi + "+[4])/[5])+1)/2)";
      ERF2 = "((TMath::Erf((" + Phi + "+0.872665+[4])/[5])+1)/2)";
   }

   string Function = FOut + "-" + FIn + "*(" + ERF1 + "+" + ERF2 + "-1)";
   
   return Function;
}
