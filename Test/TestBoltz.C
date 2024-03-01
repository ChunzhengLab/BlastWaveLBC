#include "TF1.h"

const double MassPion = 0.13957018; // GeV/c^2
const double MassProton = 0.9382720813; // GeV/c^2

const double temp = 0.1; // GeV

void TestBoltz()
{
  TString func = "10000.0*x*TMath::Sqrt(x*x-[0]*[0])*TMath::Exp(-x/[1])";
  TF1* fBoltz = new TF1("Boltz", func.Data(), MassProton, 3);
  fBoltz->SetParameter(0, MassProton);
  fBoltz->SetParameter(1, temp);

  fBoltz->Draw();
  for (int i = 0; i < 10; i++){
    cout<<fBoltz->GetRandom()<<endl;
  }

  cout<<"when x = MassProton, fBoltz = "<<fBoltz->Eval(MassProton+0.0000000001)<<endl;
  
}