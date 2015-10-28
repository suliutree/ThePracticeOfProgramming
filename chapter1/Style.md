###风格

<br>
####1.全局变量使用具有描述意义的名字，局部变量用短名字。

        全局变量可以出现在整个程序中的任何地方，因此它们的名字应该足够长，具有足够的说明性，以便使读者能够记得它们是干什
    么用的。给每个全局变量声明附一个简短注释也非常有帮助：
        int npending = 0; // current length of input queue
    全局函数、类和结构也都应该有说明性的名字，以表明它们在程序里扮演的角色。
        相反，对局部变量使用短名字就够了。

<br>
####2.保持一致性。

        除了太长之外，下面这个Java类中各成员的名字一致性也很差：
        ?    class UserQueue {
        ?        int noOfItemInQ, frontOfTheQueue, queueCapacity;
        ?        public int noOfUsersInQueue() {...}
        ?    }
    这里同一个词“队列（queue）”在名字里被分别写为Q、Queue或queue。由于只能在类型UserQueue里访问，类成员的名字中完全不必提
    到队列，因为存在上下文。所以：
        ?   queue.queueCapacity
    完全是多余的。下面的写法更好：
        class UserQueue {
            int nitems, front, capacity;
            public int nusers() {...}
        }
    因为这时可以如此写：
        queue.capacity++;
        n = queue.nusers();
    这样做在清晰性方面没有任何损失。在这里还有可做的事情。例如items和users实际是同一种东西，同样的东西应该使用一个概念。
    
<br>
####3.函数采用动作性的名字。

        函数名应当用动作性的动词，后面可以跟着名词：
            now = date.getTime();
            putchar('\n');
    对返回布尔类型值（真或者假）的函数命名，应该清楚的反映其返回值情况。下面这样的语句
        ?   if (checkoctal(c)) ...
    是不好的，因为它没有指明什么时候返回真，什么时候返回假。而：
        if (isoctal(c)) ...
    就把事情说清楚了：如果参数是八进制数字则返回真，否则返回假。

<br>
####4.要准确。

        名字不仅是个人标记，它还携带着给读程序人的信息。误用的名字可能引起奇怪的程序错误。
        本书作者之一写过一个名为isoctal的宏，并且发布使用多年，而实际上它的实现是错误的：
        ?   #define isoctal(c) ((c) >= '0' && (c) <= '8')
    正确的应该是：
        #define isoctal(c) ((c) >= '0' && (c) <= '7')
        
        另外一种情况：名字具有正确的含义，而对应的实现确实错的，一个合情合理的名字掩盖了一个害人的实现。
        下面的这个例子，其中名字和实现完全是矛盾的：
        ?   public boolean inTable(Object obj) {
        ?       int j = this.getIndex(obj);
        ?       return (j == nTable);
        ?   }
    函数getIndex如果找到了相关对象，就返回0到nTable-1之间的一个值，否则返回nTable的值。而这里inTable返回的布尔值却正好
    与它名字所说的相反。在写这段代码时，这种写法未必会引起什么问题。但如果后来修改这个程序，很可能是由别的程序员来做，
    这个名字肯定会把人弄糊涂。
    
<br>
####5.以缩行显示程序的结构。

        采用一种一致的缩行风格，是使程序呈现出结构清晰的最省力的方法。

<br>
####6.使用表达式的自然形式。

        表达式应该写的你能大声念出来。含有否定运算的条件表达式比较难理解：
        ?   if (!(block_id < actblks) || !(block_id >= unblocks))
        ?   ...
    在两个测试中都用到否定，而它们都不是必要的。应该改变关系运算符的方向，是测试变成肯定的：
        if ((block_id >= actblks) || (block_id < unblocks))
        ...
    现在代码读起来就自然多了。
    
<br>
####7.用加括号的方式排除二义性。

        括号表示分组，即使有时并不必要，加了括号也可能把意图表示的更清楚。
        
