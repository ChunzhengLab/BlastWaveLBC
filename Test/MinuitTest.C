#include <TMinuit.h>
#include <TH1D.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TF1.h>

// 全局变量
TH1D *h1;

// 自定义拟合函数
void fitFunction(int& npar, double* deriv, double& f, double par[], int flag) {
    double chi2 = 0;
    int nbins = h1->GetNbinsX();
    for (int i = 1; i <= nbins; i++) {
        double x = h1->GetBinCenter(i);
        double y = h1->GetBinContent(i);
        double err = h1->GetBinError(i);
        if (err == 0) continue;

        // 假设 a 服从高斯分布，x = a*a + a + const
        double a = sqrt(abs(x - par[2])); // 从x解出a的一个可能值
        double expectedY = par[0] * exp(-0.5 * pow((a - par[1]) / par[3], 2));

        chi2 += pow((y - expectedY) / err, 2);
    }
    f = chi2;
}

int MinuitTest() {
    // 1. 生成高斯分布的TH1D
    TRandom3 rand(0);
    h1 = new TH1D("h1", "Transformed Gaussian Histogram", 100, -5, 15);
    for (int i = 0; i < 10000; i++) {
        double a = rand.Gaus(0, 1);
        double x = a*a + a + 1; // 使用关系 x = a*a + a + const
        h1->Fill(x);
    }

    // 2. 创建TMinuit对象
    TMinuit minuit(4); // 4个参数: Constant, Mean, Const, Sigma
    minuit.SetFCN(fitFunction);

    // 设置参数：名称，初始值，步长，下界，上界
    minuit.DefineParameter(0, "Constant", 100, 1, 0, 1000);
    minuit.DefineParameter(1, "Mean", 0, 0.1, -10, 10);
    minuit.DefineParameter(2, "Const", 1, 0.1, -10, 10);
    minuit.DefineParameter(3, "Sigma", 1, 0.1, 0.1, 10);

    // 3. 使用TMinuit完成拟合
    minuit.Migrad();

    // 打印拟合参数
    double par[4], err[4];
    for (int i = 0; i < 4; i++) {
        minuit.GetParameter(i, par[i], err[i]);
        std::cout << "Parameter " << i << ": " << par[i] << " +/- " << err[i] << std::endl;
    }

    // 绘制直方图和拟合结果
    TCanvas *c1 = new TCanvas("c1", "Fit Example", 800, 600);
    h1->Draw();

    // 注意：这里仅为了绘图目的，实际的拟合函数可能需要更复杂的表达
    TF1 *fit = new TF1("fit", "[0]*exp(-0.5*((sqrt(abs(x-[2]))-[1])/[3])**2)", -5, 15);
    fit->SetParameters(par[0], par[1], par[2], par[3]);
    fit->Draw("same");

    c1->SaveAs("fit_result.png");

    return 0;
}
