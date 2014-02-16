#使用微积分割圆法计算圆周率

利用公式：

$$\pi = \int_{0}^{1}{\frac{4}{1+x^2}}$$

```{.cpp .numberLines}
 unsigned long i = start; 
 double x = 0.0, sum = 0.0;
 for ( ; i < end; i++) {
     x = (i + 0.5)*step;
     sum = sum + 4.0/(1.0 + x*x);
 } 
```

这是一个非常好的演示并行计算的例子。