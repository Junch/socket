##pi: 使用微积分割圆法计算圆周率

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

##exam2: 打印0 1 2 1 0 1 2 1 0 ...
据说这是金山公司的一道面试题目：

四个线程a,b,c,d. 线程a,b对变量i加一. 线程c,d对变量i减去一.四个线程顺序执行, 每个线程每次只执行一次.i的初始值为0， 打印结果0 1 2 1 0 1 2 1 0 1 2... 

##exam3: 生产者－消费者: 采用单链表 

##exam4: 打印ABCABC...
编写一个程序，开启3个线程，这3个线程的ID分别为A、B、C，每个线程将自己的ID在屏幕上打印10遍，要求输出结果必须按ABC的顺序显示；如：ABCABC….依次递推。

##exam5: 生产者－消费者: 采用环形队列
可以参考[\** Semaphores **][c1]

**Note**: Mac doesn’t support unnamed semaphores (sem_init() and sem_destroy()).[Semaphores in Mac OS X][c2]

[c1]: http://pages.cs.wisc.edu/~remzi/Classes/537/Fall2008/Notes/threads-semaphores.txt

[c2]: http://heldercorreia.com/blog/semaphores-in-mac-os-x