# Machine Learning

## 基础术语

- 特征 (feature)
- 标记 (label)
- 样例 (example)
- 预测 (prediction)
- 训练集 (training set)
- 交叉验证集 (cross validation set)
- 测试集 (test set)
- 监督学习 (supervised learning)
  - 分类 (classification)
  - 回归 (regression)
- 无监督学习 (unsupervised learning)
  - 聚类 (clustering)

## 监督学习

### 回归

1. 数学模型 (model)

   $$f_{\vec{w},b}(\vec{x})=\vec{w}\cdot\vec{x}+b$$

2. 代价函数 (cost function)

   $$J(\vec{w},b)=\frac{1}{2m}\sum^{m-1}_{i=0}\left[f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)}\right]^2+\frac{\lambda}{2m}\sum\limits^{n}_{j=0}w_j^2$$

3. 梯度下降(gradient descent)
   $$
   w_j=w_j-\alpha\frac{\partial J(\vec{w},b)}{\partial w_j}=w_j-\alpha\left[\frac{1}{m}\sum\limits^{m-1}_{i=0}\left[(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})x_j^{(i)}\right]+\frac{\lambda}{m}w_j\right]\newline
   b=b-\alpha\frac{\partial J(\vec{w},b)}{\partial b}=b-\alpha\frac{1}{m}\sum\limits^{m-1}_{i=0}(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})
   $$

### 二元分类

1. 数学模型 (model)

   $$f_{\vec{w},b}(z)=\frac{1}{1+e^{-z}}=\frac{1}{1+e^{-(\vec{w}\cdot\vec{x}+b)}}$$

2. 代价函数 (cost function)

   $$J(\vec{w},b)=-\frac{1}{m}\sum^{m-1}_{i=0}\left[y^{(i)}\log(f_{\vec{w},b}(\vec{x}^{(i)}))+(1-y^{(i)})\log(1-f_{\vec{w},b}(\vec{x}^{(i)}))\right]+\frac{\lambda}{2m}\sum\limits^{n}_{j=0}w_j^2$$

3. 梯度下降 (gradient descent)
   $$
   w_j=w_j-\alpha\frac{\partial J(\vec{w},b)}{\partial w_j}=w_j-\alpha\left[\frac{1}{m}\sum\limits^{m-1}_{i=0}\left[(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})x_j^{(i)}\right]+\frac{\lambda}{m}w_j\right]\newline
   b=b-\alpha\frac{\partial J(\vec{w},b)}{\partial b}=b-\alpha\frac{1}{m}\sum\limits^{m-1}_{i=0}(f_{\vec{w},b}(\vec{x}^{(i)})-y^{(i)})
   $$

### 多类分类

1. 数学模型 (model)

   $$
    \vec{a}(x)=\frac{1}{\sum\limits^{N-1}_{k=0}{e^{z_k}}}
    \begin{bmatrix}
    e^{z_1} \\
    \vdots \\
    e^{z_{N}} \\
    \end{bmatrix}
   $$

2. 代价函数 (cost function)

   $$J(\vec{w},b) = -\frac{1}{m} \left[ \sum_{i=1}^{m} \sum_{j=1}^{N}  1\left\{y^{(i)} == j\right\} \log \frac{e^{z^{(i)}_j}}{\sum_{k=1}^N e^{z^{(i)}_k} }\right]$$

### 神经网络

- 神经元 (neuron)
- 激活函数 (activation)
- 层 (layer)
  - 输入层 (input layer)
  - 隐含层 (hidden layer)
  - 输出层 (output layer)
- 随机梯度下降 (stochastic gradient descent)
  - 前向传播 (forward propagation)
  - 后向传播 (backward propagation)
- 迁移学习 (transfer learning)

### 决策树

1. 划分特征：

   $$
   p_1=1-p_0\newline
   H(p_1)=-p_1log_2(p_1)-p_0log_2(p_0)=-p_1log_2(p_1)-(1-p_1)log_2(1-p_1)\newline
   information\ gain = H(p_1^{root})-\left(w^{left}H(p_1^{left})+w^{right}H(p_1^{right})\right)
   $$

   - 二元离散特征值：选取最大信息增益的特征
   - 多类离散特征值：one-hot encoding 后同二元离散
   - 连续值特征值：选择信息增益最好的中间值

2. 停止划分

   - 0 熵
   - 达到最大深度
   - 熵低于阈值
   - 样例数低于阈值

3. 树集成
   - 随机森林：取特征子集 $\sqrt{n}$
   - XGBoost：预测错误的样例有更高的放回抽样权重

## 优化技巧

- 梯度下降

  - 特征缩放：$x_j=\frac{x_j-\mu_j}{\sigma_j}$
  - α 的选取：0.001~1(3x)
    - 若 J 下降过慢，则 α 过小
    - 若 J 存在增长，则 α 过大
    - 若导数存在振荡，则 α 过大

- 高偏差 (high bias): $J_{train} \gg J_{baseline}$，即欠拟合 (underfit)

  - 增加额外特征
  - 增加多项式特征
  - 减小正则化参数 $\lambda$

- 高方差 (high variance): $J_{cv} \gg J_{train}$，即过拟合 (overfit)

  - 增加训练集数据
  - 减少特征
  - 增大正则化参数 $\lambda$

- 错误分析 (error analysis): 手动分类并用更多预测错误的数据训练

- F1 score: $2\frac{PR}{P+R}$
  - 查准率(precision): 当目标被预测为阳性时描述预测的准确程度
  - 查全率(recall): 当目标为真阳性时描述预测其为阳性的概率
