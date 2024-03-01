const double fMass = 0.13957; // GeV

void TestBoltz0() {
  TF1* fBoltz1[9];
  double TempVal[9] = {91.3442,86.9569,84.7781,87.3677,91.6305,95.1377,98.1375,108.2,110}; // MeV
  double TempErr[9] = {3.5084, 3.49799, 4.8779, 4.7532, 3.8270, 3.3291, 2.8827, 3.2, 5.0};
  for(int i=0; i<9; i++) {
    TempVal[i] += 20;
    TempVal[i] = TempVal[i]; // GeV
    TempErr[i] = TempErr[i];
    fBoltz1[i] = new TF1("Boltz1","x*TMath::Sqrt(x*x-[0]*[0])*TMath::Exp(-x/[1])",0.15,10);
    fBoltz1[i]->SetParameter(0,fMass);
    fBoltz1[i]->SetParameter(1,TempVal[i]);
  }


  for (size_t i = 0; i < 10; i++) {
    cout<<fBoltz1[4]->GetRandom()<<endl;
  }
}