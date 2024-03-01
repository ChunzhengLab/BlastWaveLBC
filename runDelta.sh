#!/bin/bash

# 输入值
fTScale=1.0
fBetaScale=0.0
fRho2Scale=0.5
vecLBC=(0.0348 0.0372 0.0375 0.0480 0.0580) # Correct array initialization
vecCentrality=(25 35 45 55 65) # Correct array initialization

# 获取数组长度
array_length=${#vecCentrality[@]} # Corrected to vecCentrality

# 遍历数组
for (( i=0; i<$array_length; i++ )); do
    fLBC=${vecLBC[$i]} # Correct array access
    fCentrality=${vecCentrality[$i]} # Correct array access
    # 运行程序
    ./main $fTScale $fBetaScale $fRho2Scale $fLBC $fCentrality &
done

# 等待所有后台任务完成
wait

