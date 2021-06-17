{
   TFile File("SynchTree/Result_AK4PFCHS.root");

   // OutputTree->Draw("OOA:PUPT>>htemp(50,0,100)",
   //    "Rho > 15 && Rho < 45 && PUEta > -2.14 && PUEta < -1.54 && PUPhi > -1.4 && PUPhi > 1.0 && PUPT < 1000 && PUPT > 8",
   //    "prof");
   OutputTree->Draw("OOA:PUPT>>htemp(50,0,1000)",
      "Rho > 15 && Rho < 45 && PUEta > -2.14 && PUEta < -1.54 && PUPhi > -1.4 && PUPhi < -1.0 && PUPT < 1000 && PUPT > 8",
      "prof");

   // TF1 F("F", "[0]*(1+[1]*log(x/15))*(30-3.06118)", 0, 6500);
   TF1 F("F", "[0]+[1]*log(x/15)+[2]*x*x", 0, 6500);
   // TF1 F("F", "pol2", 0, 6500);

   // F.SetParameters(0.348269, -0.438033);
   F.SetParameters(2.02672e+02, -1.72185e+02, 0);

   // F.Draw("same");
}
