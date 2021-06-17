#include <iostream>
#include <cmath>
using namespace std;

#include "TF1.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TGraph.h"

#include "SetStyle.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   double XL = -1.6;
   double XR = -0.9;
   double R0 = 0.8;
   double C0 = 1.0;
   double DC = 0.5;

   TGraph G1, G2, G3, G4;
   G1.SetPoint(0, -M_PI, C0);
   G1.SetPoint(1, +M_PI, C0);
   G2.SetPoint(0, -M_PI, C0 - DC);
   G2.SetPoint(1, +M_PI, C0 - DC);
   G3.SetPoint(0, XL, -100);
   G3.SetPoint(1, XL, +100);
   G4.SetPoint(0, XR, -100);
   G4.SetPoint(1, XR, +100);
   G1.SetLineStyle(kDashed);
   G2.SetLineStyle(kDashed);
   G3.SetLineStyle(kDashed);
   G4.SetLineStyle(kDashed);

   string OneSideFunction
      = "[0] - [1] * (acos(max(-1.0,min(1.0,([3]-x)/[2]))) - ([3]-x)/[2] * sqrt(1-min(1.0,([3]-x)/[2]*([3]-x)/[2]))) / TMath::Pi()";
   string OneSideFunctionPart2
      = "([0]-[1]) + [1] * (acos(max(-1.0,min(1.0,(x-[3])/[2]))) - (x-[3])/[2] * sqrt(1-min(1.0,(x-[3])/[2]*(x-[3])/[2]))) / TMath::Pi()";
   string OneSideFunctionPart3
      = "([0]-[1]) + [1] * (acos(max(-1.0,min(1.0,([3]-x)/[2]))) - ([3]-x)/[2] * sqrt(1-min(1.0,([3]-x)/[2]*([3]-x)/[2]))) / TMath::Pi()";
   string TwoSideFunction
      = "([0]-[1]) + [1] * (acos(max(-1.0,min(1.0,([4]-x)/[2]))) - ([4]-x)/[2] * sqrt(1-min(1.0,([4]-x)/[2]*([4]-x)/[2]))) / TMath::Pi() + [1] * (acos(max(-1.0,min(1.0,(x-[3])/[2]))) - (x-[3])/[2] * sqrt(1-min(1.0,(x-[3])/[2]*(x-[3])/[2]))) / TMath::Pi()";

   TF1 F1("F1", OneSideFunction.c_str(), -M_PI, M_PI);
   F1.SetParameters(C0, DC, R0, XL);
   TF1 F2("F2", OneSideFunctionPart2.c_str(), -M_PI, M_PI);
   F2.SetParameters(C0, DC, R0, XL);
   TF1 F3("F3", OneSideFunctionPart3.c_str(), -M_PI, M_PI);
   F3.SetParameters(C0, DC, R0, XR);
   TF1 F4("F4", TwoSideFunction.c_str(), -M_PI, M_PI);
   F4.SetParameters(C0, DC, R0, XL, XR);
   TF1 F5("F5", "[0]-[1]", -M_PI, M_PI);
   F5.SetParameters(C0, DC);

   PdfFileHelper PdfFile("Functions.pdf");
   PdfFile.AddTextPage("Functions");

   TCanvas Canvas;
   TH2D HWorld("HWorld", ";#phi;", 100, XL - R0 * 1.5, XR + R0 * 1.5, 100, C0 - DC * 1.1, C0 + DC * 0.1);
   HWorld.SetStats(0);

   HWorld.Draw("axis");
   G1.Draw("l");
   G2.Draw("l");
   G3.Draw("l");
   G4.Draw("l");
   F1.Draw("same");
   PdfFile.AddCanvas(Canvas);

   HWorld.Draw("axis");
   G1.Draw("l");
   G2.Draw("l");
   G3.Draw("l");
   G4.Draw("l");
   F2.Draw("same");
   PdfFile.AddCanvas(Canvas);

   HWorld.Draw("axis");
   G1.Draw("l");
   G2.Draw("l");
   G3.Draw("l");
   G4.Draw("l");
   F3.Draw("same");
   PdfFile.AddCanvas(Canvas);

   HWorld.Draw("axis");
   G1.Draw("l");
   G2.Draw("l");
   G3.Draw("l");
   G4.Draw("l");
   F4.Draw("same");
   PdfFile.AddCanvas(Canvas);

   HWorld.Draw("axis");
   G1.Draw("l");
   G2.Draw("l");
   G3.Draw("l");
   G4.Draw("l");
   F5.Draw("same");
   PdfFile.AddCanvas(Canvas);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   return 0;
}




