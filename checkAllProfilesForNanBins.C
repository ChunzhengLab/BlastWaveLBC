#include <TFile.h>
#include <TProfile.h>
#include <TKey.h>
#include <iostream>
#include <TRoot.h>
#include <cmath> // 用于isnan函数

void checkAllProfilesForNanBins() {
    for (int i = 0; i < 100; ++i) {
        // 构造文件名字符串
        TString fileName = Form("result_cent_25_%d.root", i);
        
        // 打开ROOT文件
        TFile *file = TFile::Open(fileName, "READ");
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file: " << fileName << std::endl;
            continue; // 如果文件打不开，跳过当前循环
        }

        TIter next(file->GetListOfKeys());
        TKey *key;
        
        // 遍历文件中的所有键（Key）
        while ((key = (TKey*)next())) {
            // 获取对象的类名
            TClass *cl = gROOT->GetClass(key->GetClassName());
            if (!cl->InheritsFrom("TProfile")) continue; // 如果不是TProfile，跳过

            TProfile *profile = (TProfile*)key->ReadObj();
            bool hasNan = false;

            // 检查每个bin内容是否为nan
            for (int bin = 1; bin <= profile->GetNbinsX(); ++bin) {
                double content = profile->GetBinContent(bin);
                if (std::isnan(content)) { // 检查是否为nan
                    hasNan = true;
                    break; // 找到nan后退出循环
                }
            }

            if (hasNan) {
                std::cout << "File with NaN in profile " << key->GetName() << ": " << fileName << std::endl;
            }
        }

        // 关闭文件
        file->Close();
    }
}
