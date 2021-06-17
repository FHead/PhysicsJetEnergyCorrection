#include <iostream>
using namespace std;

#include "TH2D.h"

#include "Code/DrawRandom.h"
#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

#include "JetCorrector.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string JECFile1 = CL.Get("JEC1");
   string JECFile2 = CL.Get("JEC2");
   string OutputFileName = CL.Get("Output", "JECCheck.pdf");

   double ExcludePhiMin = CL.GetDouble("ExcludePhiMin", -2.1708);
   double ExcludePhiMax = CL.GetDouble("ExcludePhiMax", -0.2727);
   double ExcludeEtaMin = CL.GetDouble("ExcludeEtaMin", -3.4000);
   double ExcludeEtaMax = CL.GetDouble("ExcludeEtaMax", -0.9000);

   int N = CL.GetInt("N", 10000000);

   JetCorrector JEC1(JECFile1);
   JetCorrector JEC2(JECFile2);

   TH2D HC1C2("HC1C2", ";JEC1;JEC2", 200, 0, 1.2, 200, 0, 1.2);
   TH1D HDiff("HDiff", ";JEC1-JEC2", 201, -0.1, 0.1);

   HC1C2.SetStats(0);
   HDiff.SetStats(0);

   for(int i = 0; i < N; i++)
   {
      double JetPT = DrawRandom(10, 1000);
      double JetEta = DrawRandom(-3, 3);
      double JetPhi = DrawRandom(-M_PI, M_PI);
      double JetArea = 0.5;
      double Rho = DrawRandom(0, 70);

      if(JetEta > ExcludeEtaMin && JetEta < ExcludeEtaMax
         && JetPhi > ExcludePhiMin && JetPhi < ExcludePhiMax)
         continue;

      JEC1.SetJetPT(JetPT);
      JEC1.SetJetEta(JetEta);
      JEC1.SetJetPhi(JetPhi);
      JEC1.SetJetArea(JetArea);
      JEC1.SetRho(Rho);
      
      JEC2.SetJetPT(JetPT);
      JEC2.SetJetEta(JetEta);
      JEC2.SetJetPhi(JetPhi);
      JEC2.SetJetArea(JetArea);
      JEC2.SetRho(Rho);

      double C1 = JEC1.GetCorrection();
      double C2 = JEC2.GetCorrection();

      HC1C2.Fill(C1, C2);
      HDiff.Fill(C1 - C2);
   }

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("JEC check");

   PdfFile.AddPlot(HC1C2, "colz");
   PdfFile.AddPlot(HDiff);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}

