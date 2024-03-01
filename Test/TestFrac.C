#include "TRandom3.h"
void TestFrac() {
  const int n = 100000;
  const float frac = 1.0 / 3.0;

  int iPoint = 0;
  int nLBC = 0;
  while (iPoint < n) {
    iPoint++;
    if(gRandom->Uniform() < frac) {
      iPoint++;
      nLBC += 2;
    }
  }

  int nNormal = n - nLBC;
  float ratio = (float)nLBC / n;
  cout<<"nNormal = "<<nNormal<<endl;
  cout<<"nLBC = "<<nLBC<<endl;
  cout<<"ratio = "<<ratio<<endl;
}