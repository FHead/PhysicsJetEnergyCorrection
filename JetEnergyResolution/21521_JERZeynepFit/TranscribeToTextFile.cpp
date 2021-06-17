#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH2F.h"

#include "CommandLine.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   string HistogramName = CL.Get("Histogram");
   bool EtaReflection = CL.GetBool("EtaReflection", true);
   string OutputFileName = CL.Get("Output");

   TFile File(InputFileName.c_str());

   TH2F *H = (TH2F *)File.Get(HistogramName.c_str());

   if(H == nullptr)
   {
      cerr << "Error!  Histogram not found or file open error!" << endl;
      return -1;
   }

   vector<double> Eta(H->GetNbinsY() + 1);
   for(int i = 1; i <= H->GetNbinsY(); i++)
      Eta[i-1] = H->GetYaxis()->GetBinLowEdge(i);
   Eta[H->GetNbinsY()] = H->GetYaxis()->GetBinUpEdge(H->GetNbinsY());
   
   vector<double> PT(H->GetNbinsX() + 1);
   for(int i = 1; i <= H->GetNbinsX(); i++)
      PT[i-1] = H->GetXaxis()->GetBinLowEdge(i);
   PT[0] = 0;
   PT[H->GetNbinsX()] = 7000;

   ofstream out(OutputFileName);

   out << "{2 JetEta JetPt 0 None ScaleFactor}" << endl;

   for(int iX = 0; iX < H->GetNbinsX(); iX++)
   {
      for(int iY = 0; iY < H->GetNbinsY(); iY++)
      {
         double V = H->GetBinContent(iX + 1, iY + 1);
         double E = H->GetBinError(iX + 1, iY + 1);
         out << Eta[iY] << " " << Eta[iY+1] << " " << PT[iX] << " " << PT[iX+1] << " 3 " << V << " " << V - E << " " << V + E << endl;

         if(EtaReflection == true)
            out << -Eta[iY+1] << " " << -Eta[iY] << " " << PT[iX] << " " << PT[iX+1] << " 3 " << V << " " << V - E << " " << V + E << endl;
      }
   }

   out.close();

   File.Close();

   return 0;
}




