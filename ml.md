# Machine Learning

## 基础术语

- 特征(feature)
- 标记(label)
- 样例(example)
- 预测(prediction)
- 训练集(training set)
- 交叉验证集(cross validation set)
- 测试集(test set)
- 监督学习(supervised learning)
  - 分类(classification)
  - 回归(regression)
- 无监督学习(unsupervised learning)
  - 聚类(clustering)

## 监督学习

1. 数学模型(model)：

   - 回归(regression)
     $$f_{\vec{w},b}(\vec{x})=\vec{w}\cdot\vec{x}+b$$

   - 二元分类(binary classification)
     $$f_{\vec{w},b}(z)=\frac{1}{1+e^{-z}}=\frac{1}{1+e^{-(\vec{w}\cdot\vec{x}+b)}}$$

   - 多类分类(multi-class classification)
     $$
      \vec{a}(x)=\frac{1}{\sum\limits^{N-1}_{k=0}{e^{z_k}}}
      \begin{bmatrix}
      e^{z_1} \\
      \vdots \\
      e^{z_{N}} \\
      \end{bmatrix}
     $$

2. 代价函数(cost function)：

   - 回归
     $$J(\vec{w},b)=\frac{1}{2m}\sum^{m-1}_{i=0}f_{\vec{w},b}(\vec{x}^{(i)}-y^{(i)})^2$$

   - 二元分类
     $$J(\vec{w},b)=-\frac{1}{m}\sum^{m-1}_{i=0}\left[y^{(i)}\log(f_{\vec{w},b}(\vec{x}^{(i)}))+(1-y^{(i)})\log(1-f_{\vec{w},b}(\vec{x}^{(i)}))\right]$$

   - 多类分类
     $$J(\vec{w},b) = -\frac{1}{m} \left[ \sum_{i=1}^{m} \sum_{j=1}^{N}  1\left\{y^{(i)} == j\right\} \log \frac{e^{z^{(i)}_j}}{\sum_{k=1}^N e^{z^{(i)}_k} }\right]$$

3. 梯度下降(gradient descent)：
   $$
   w_j=w_j-\alpha\frac{\partial J(\vec{w},b)}{\partial w_j}=w_j-\frac{\alpha}{m}\sum\limits^{m-1}_{i=0}(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})x_j^{(i)}+\frac{\lambda}{2m}\sum\limits^{n}_{j=0}w_j^2\newline
   b=b-\alpha\frac{\partial J(\vec{w},b)}{\partial b}=b-\frac{\alpha}{m}\sum\limits^{m-1}_{i=0}(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})+\frac{\lambda}{m}w_j
   $$
   - 特征缩放：$x_j=\frac{x_j-\mu_j}{\sigma_j}$
   - α 的选取：0.001~1(3x)
     - 若 J 下降过慢，则 α 过小
     - 若 J 存在增长，则 α 过大
     - 若导数存在振荡，则 α 过大

## 神经网络

- 层(layer)
- 神经元(neuron)
- 激活函数(activation)
- 迁移学习(transfer learning)

## 模型优化

> 将 bias 与 variance 与 baseline 比较

- 高偏差(high bias): $high\ J_{train}$，即欠拟合(underfit)

  - 增加额外特征
  - 增加多项式特征
  - 减小正则化参数 $\lambda$

- 高方差(high variance): $low\ J_{train}\ and\ high\ J_{cv}$，即过拟合(overfit)

  - 增加训练集数据
  - 减少特征
  - 增大正则化参数 $\lambda$

- F1 score: $2\frac{PR}{P+R}$
  - 查准率(precision): 当目标被预测为阳性时描述预测的准确程度
  - 查全率(recall): 当目标为真阳性时描述预测其为阳性的概率
