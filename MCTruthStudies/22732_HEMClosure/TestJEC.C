{
   TF1 F1("F1", "max(0.0001,((x<10)*([9]))+((x>=10)*([0]+([1]/(pow(log10(x),2)+[2]))+([3]*exp(-([4]*((log10(x)-[5])*(log10(x)-[5])))))+([6]*exp(-([7]*((log10(x)-[8])*(log10(x)-[8]))))))))", 0, 1000);
   TF2 F2("F2", "(max(0.0001,((x<10)*([9]))+((x>=10)*([0]+([1]/(pow(log10(x),2)+[2]))+([3]*exp(-([4]*((log10(x)-[5])*(log10(x)-[5])))))+([6]*exp(-([7]*((log10(x)-[8])*(log10(x)-[8])))))))))-((max(0.0001,[10]+((x-[11])*([12]+((x-[11])*([13]+((x-[11])*[14])))))))-(max(0.0001,((x<10)*([9]))+((x>=10)*([0]+([1]/(pow(log10(x),2)+[2]))+([3]*exp(-([4]*((log10(x)-[5])*(log10(x)-[5])))))+([6]*exp(-([7]*((log10(x)-[8])*(log10(x)-[8]))))))))))/0.758844*((TMath::Erf((-1.4708-y+[15])/0.1768)+1)/2+(TMath::Erf((y+0.9727+[15])/0.1768)+1)/2-1)", 0, 1000, -10, 10);

   double P1[] = {0.475351, 4.13219, 3.88749, 0.601361, 0.00794707, 12.5034, -0.329927, 0.661665, 1.05413, 1.20162,  2.02789, 7.82501, -0.12335, 0.0894236, -0.00518767, 0.249066};
   F1.SetParameters(P1);
   F2.SetParameters(P1);

   double P2[] = {0.344202, 4.53972, 2.65655, 0.359932, 6.17017e-07, 15.101, -0.777425, 0.424715, 0.384621, 1.28369,  1.80315, 606.158, 0.00162796, 0.000254724, -1.41614e-06, 0};
   F1.SetParameters(P2);
   F2.SetParameters(P2);

   cout << F1.Eval(366.606) << endl;
   cout << F2.Eval(366.606, 1.06936) << endl;
}
