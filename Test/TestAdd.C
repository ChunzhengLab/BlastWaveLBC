void TestAdd() {
  TH1D *h1 = new TH1D("h1", "h1", 100, 0, 100);
  TH1D h2("h2", "h2", 100, 0, 100);

  for (int i = 0; i < 10000; i++) {
    h1->Fill(gRandom->Gaus(50, 10));
    h2.Fill(gRandom->Uniform(70, 80));
  }

  h1->Add(&h2);

  h1->Draw();
}