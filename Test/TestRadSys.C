float RadSys(float rad, float phi, float Rx) {
  return sqrt(pow(rad*cos(phi)/Rx,2.)+pow(rad*sin(phi),2.));
}

float BosPhi1(float phi, float Rx) {
  float phis = TMath::ATan(Rx*Rx*TMath::Tan(phi));
  phis = phis % (2 * TMath::Pi());
  return phis;
}

float BosPhi0(float phi, float Rx) {
  float phis = TMath::ATan(Rx*Rx*TMath::Tan(phi));
  if (phi>TMath::Pi()/2. && phi<TMath::Pi()*3./2.)     phis += TMath::Pi();
  else if (phi<2*TMath::Pi() && phi>TMath::Pi()*3./2.) phis += 2*TMath::Pi();
  return phis;
}


void TestRadSys() {
  // float rads = sqrt(gRandom->Rndm());
  // float phis = 2*TMath::Pi()*gRandom->Rndm();
  // while (RadSys(rads,phis,0.933685)>1.) {
  //   rads = sqrt(gRandom->Rndm());
  //   phis = 2*TMath::Pi()*gRandom->Rndm();
  // }
  // float xs = rads*cos(phis);
  // float ys = rads*sin(phis);

  // TVector2 vec(xs,ys);
  // float a = vec.Mod();
  // float b = RadSys(rads,phis,0.933685);
  // cout << "a = " << a << endl;
  // cout << "b = " << b << endl;


  float Rx = 0.933685;
  float phi = 1.75;
  float bosphi0 = BosPhi0(phi, Rx);
  float bosphi1 = BosPhi1(phi, Rx);

  cout << "bosphi0 = " << bosphi0 << endl;
  cout << "bosphi1 = " << bosphi1 << endl;

}

