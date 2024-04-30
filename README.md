# PatchMatch SAD版本

## 文件说明

PatchMatch 算法主程序

- [main.cpp](main.cpp)
- [pm.cpp](pm.cpp)
- [pm.h](pm.h)

视差图转点云脚本

- [disp_to_coud.py](disp_to_coud.py)
- [pfm_util.py](pfm_util.py)

## 编译

编译依赖于OpenCV 4.x版本，3.x不能保存pfm图像

## 运行

```bash
PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\PM3SAD11MF7 11 500 3
```

参数解释：PatchMatch.exe [左图] [右图] [结果保存路径(会自动创建，但只会创建最后一级目录)] [SAD窗口大小] [最大视差] [迭代次数]

## 修改点

- 匹配代价计算修改为SAD，原版论文是颜色+梯度计算的，使用SAD不需要梯度计算，这部分也注释了，可查看plane_match_cost函数
- 视差优化阶段的加权中值滤波修改为中值滤波，对于散斑IR图，加权的方式并不适用

## 部分运行结果

运行结果均在[imgs](imgs)目录下

- 150mm基线琴女

![qinnv-150mm-disp.png](doc%2Fqinnv-150mm-disp.png)

![qinnv-150mm-cloud.png](doc%2Fqinnv-150mm-cloud.png)

- 150mm基线广东青年

![gdqn-150mm-disp.png](doc%2Fgdqn-150mm-disp.png)

![gdqn-150mm-cloud.png](doc%2Fgdqn-150mm-cloud.png)

## Tirck

### 结果文件夹命名

PM3SAD11MF7 表示 PatchMatch迭代3次 SAD窗口大小11 中值滤波窗口大小7

### imgs文件的命名方式参考MiddleBury数据集

使用MiddleBury提供的工具cvkit打开disp0PM3SAD11MF7.pfm，按m键可以伪彩色显示，按p键可以直接显示点云，其它功能按h查询

![dir.png](doc%2Fdir.png)


