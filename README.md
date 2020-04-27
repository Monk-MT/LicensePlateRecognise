# 基于openCV的车牌识别系统
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该车牌识别系统是基于openCV这一开源库进行开发，在参考了网络上的车牌识别系统的情况下，用于学习而开发的一个车牌识别系统。该系统用于识别中文车牌，图片清晰的情况下准确率在60%左右。

## 兼容性与环境配置
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该系统基于openCV3.1和VS2013进行编写，在编译前需要进行相关配置。
1. 如果当前VS2013是x64的，那么需要先在VS2013菜单栏“生成–>配置管理器”中将工程属性更改为x64，更改方式如下：
![配置管理器](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86%E5%99%A8.png)

2. 为项目testPR和LicensePlateRecognise配置opencv包含目录和库文件:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;分别选中项目testPR和LicensePlateRecognise,打开VS2013菜单栏的“项目–>属性“，选择”配置属性–>VC++目录“，分别配置”包含目录“和”库目录“，如下图所示。
![属性管理器](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E5%B1%9E%E6%80%A7%E7%AE%A1%E7%90%86%E5%99%A8.png)


包含目录中添加如下内容：

%\opencv\build\include\opencv2;

%opencv\build\include\opencv;

%\opencv\build\include;

库目录中添加如下内容：

%\opencv\build\x64\vc12\staticlib;

%\opencv\build\x64\vc12\lib;

## 使用说明
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;软件运行后会默认显示单张识别的界面。
![打开](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E6%89%93%E5%BC%80.png)
### 设置选项：
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在设置选项中可以勾选车牌定位的方式和识别图片的来源，默认方式全选，来源未生活照片。
![设置](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E8%AE%BE%E7%BD%AE.png)

### 标签栏：
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在标签栏中可以切换不同的模块，接下来介绍各个模块。
#### 单张检测：
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该模块一次只能检测一张图片，可以显示详细的检测过程。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;选择好要识别的图片后会在源图片处显示，之后点击开始识别就会开始识别过程，识别结果显示在结果栏中。点击观察过程就可以看到识别过程中产生中各个步骤产生的临时图片。
![单张识别](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E5%8D%95%E5%BC%A0%E8%AF%86%E5%88%AB.png)

#### 批量检测：
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该模块可以一次检测一个文件夹的图片，但只有过程和时间显示。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;单机选择文件选择要识别的文件夹，之后单机开始进行识别，过程中会显示识别进度，但是不能打断。识别结束后会显示所有结果和总体的正确率。
![批量识别](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E6%89%B9%E9%87%8F%E8%AF%86%E5%88%AB.png)

#### 训练网络
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该模块可以依靠自己的图片集来进行训练，以符合以提高当前状况的识别正确率。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;首先选择要训练的文件夹（必须符合标准），之后选择要训练的项目即可。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;各个文件夹数据存放标准可以参考resources\train_data文件夹。
![训练网络](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E8%AE%AD%E7%BB%83%E7%BD%91%E7%BB%9C.png)

## 程序流程介绍
### 总流程
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;该车牌识别系统可以分为连个部分：车牌定位和文字识别。车牌识别分为车牌定位、SVM训练、车牌判断三个部分，文字定位分为字符分割，ANN训练，字符识别三个部分，总计有六个模块。其中车牌识别过程为车牌定位、车牌判断、字符分割、字符识别。如图：

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;车牌定位首先筛选出可能是车牌的模块，然后使用SVM判断模型选择出真正的车牌图块。之后把车牌分割为的单个字符的分割图块。再将得到7个字符图块放入训练好的神经网络模型，通过模型来预测每个图块所表示的具体字符。
![总流程图](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E6%80%BB%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

### 车牌定位
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;车牌定位采用了垂直边缘检测和颜色识别两种方法并用。

##### 颜色识别
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;颜色是被采用HSV模型判断。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;蓝色：H：100—140 S：64—255 V:64—255

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;黄色：H：15—40   S：64—255 V:64—255

##### 垂直边缘检测
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;具体如图：
![垂直边缘车牌定位](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E5%9E%82%E7%9B%B4%E8%BE%B9%E7%BC%98%E8%BD%A6%E7%89%8C%E5%AE%9A%E4%BD%8D.png)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;两种方法相互结合可以很好的定位大部分车牌。

### SVM训练
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;把车牌图像（136x36）数据用直方统计，得到共有172（横向136，纵向36）个维度，内核用rbf核，高斯函数作为核函数。

### 字符分割
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;具体如图：
![字符分割](https://github.com/ZuoYiGeHeShang/LicensePlateRecognise/blob/master/show/%E5%AD%97%E7%AC%A6%E5%88%86%E5%89%B2.png)

### ANN训练
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;这里的ANN模型使用的是3层的多层感知器，全连接，只有一个隐藏层，输入层有120个神经元，表示一个样本是由120个特征值组成的特征向量；隐藏层40神经元；输出层的神经元等于需要分类的个数，即65个字符。激活函数使用逻辑S型函数，训练方法采用反向传播算法，权重梯度项的强度默认设为0.1，动量的强度默认设为0.1（梯度下降方法经常会收敛到局部极小值，避免局部极小值的方法是在权值更新公式中加一个动量）

## 目录结构
#### 工程目录
|目录|解释|
|-|-|
| src | 所有源文件 |
| include | 所有头文件 |
| testPR | MFC所有文件 |
| model | 机器学习的模型 |
| resources| 资源文件|

#### src目录中子目录的解释:
|目录|解释|
|-|-|
| core | 核心功能 |
| preprocess | SVM预处理 |
| train | 训练目录，存放模型训练的代码 |
| util | 辅助功能 |

#### src/core目录下一些核心文件的解释与关系:
|目录|解释|
|-|-|
| plate_locate | 车牌定位 |
| plate_judge | 车牌判断 |
| plate_detect | 车牌检测，是车牌定位与车牌判断功能的组合 |
| chars_segment | 字符分割 |
| chars_identify | 字符鉴别 |
| chars_recognise | 字符识别，是字符分割与文字识别功能的组合 |
| plate_recognize | 车牌识别，是车牌检测与字符识别的共有子类 |
| feature | 特征提取回调函数 |
| plate | 车牌抽象 |
| core_func | 共有的一些函数 |

#### core/train目录下文件的解释:
|目录|解释|
|-|-|
| ann_train.cpp | 训练二值化字符 |
| annCh_train.hpp | 训练中文灰度字符 |
| svm_train.hpp | 训练车牌判断 |
| create_data.hpp | 生成合成数据 |

#### testPR目录下文件的解释:
|目录|解释|
|-|-|
| BatchTest.cpp | 批量检测 |
| SingleTest.cpp | 单张检测 |
| TrainTest.cpp | 训练 |
| Setting.hpp | 设置 |