###算法与数据结构

        每个程序都需要依靠算法和数据结构，但很少有程序依赖于必须发明一批全新的东西。即使是很复杂的程序，比如在编译器或网络
    浏览器里，主要的数据结构也是数组、表、树和散列表等等。如果在一个程序里要求某些更精巧的东西，它多半也是基于这些简单的东
    西构造起来的。因此，对大部分程序员而言，所需要的是知道有哪些合适的、可用的算法和数据结构，知道如何在各种可以相互替代的
    东西中做出选择。
        这里要讲的是一个核桃壳的故事。实际上基本算法只有屈指可数的几个，它们几乎可以出现在每个程序中，可能已经被包含在程序
    库里，这就是基本的检索和排序。与此类似，几乎所有的数据结构都是从几个基本东西中产生出来的。

<br>    
####1.检索

        要存储静态表格式数据当然应该用数组。由于可以做编译时的初始化，构建这种数组非常容易。要检查学生练习中是否废话太多，
    在程序里可能会定义：
        char *flab[] = {
            "actually",
            "just",
            "quite",
            "really",
            NULL
        };
    检索程序必须知道数组里有多少元素。对这个问题的一种处理方法是传递一个数组长度参数。这里采用的是另一种方法，在数组最后放
    一个NULL作为结束标志。
        int lookup(char *word, char *array[])
        {
            int i;
            for (i = 0; array[i] != NULL; i++)
            if (strcmp(word, array[i]) == 0)
                return i;
            return -1;
        }
    在C和C++里，字符串数组参数可以说明为char *array[]或者char **array。虽然这两种形式是等价的，但前一种形式能把参数的使用
    方式更清楚的表现出来。
        标准库提供了一些函数，它们可以处理某些特定类型的数据检索问题。例如，函数strchr和strstr能在C和C++字符串里检索给定的
    字符或子串，Java的String类里有一个indexOf方法，C++的类属算法find几乎能用于任何数据类型。如果对某个数据类型有这种函数，
    我们就应该直接用它。
        char *strchr(const char* s, char c);    查找字符串s中首次出现字符c的位置
        char *strstr(char *str1, char *str2);   若str2是str1的子串，则先确定str2在str1中第一次出现的位置，并返回此位置到str1
                                                末尾的所有字符

        下面的程序来自一个分析HTML的程序，这里有一个具有实际规模的数组，其中为成百个独立的字符定义了文字名：
        typedef struct Nameval Nameval;
        struct Nameval {
            char *name;
            int value;
        };
        
        Nameval htmlchars[] = {
            "AElig", 0x00c6,
            "Aacute", 0x00c1,
            "Acirc", 0x00c2,
            /* ... */
            "zeta", 0x03b6,
        };
        对这种表格的二分检索函数可以写成下面的样子：
        int lookup(char *name, Nameval tab[], int ntab)
        {
            int low, high, mid, cmp;
            
            low = 0;
            high = ntab - 1;
            while (low < high) {
                mid = (low + high) / 2;
                cmp = strcmp(name, tab[mid].name);
                if (cmp < 0)
                    high = mid - 1;
                else if (cmp > 0)
                    low = mid + 1;
                else
                    return mid;
            }
            return -1;
        }
    把这些放在一起，检索htmlchars的操作应写为：
        half = lookup("frac12", htmlchars, NELEMS(htmlchars));
        
        二分检索在每个工作步骤中丢掉一半数据。这样，完成检索需要做的步数相当于对表的长度n反复除以2，直至最后剩下一个元
    素时所做的除法次数。忽略舍入后得到的是logn(底数为2)。如果被检索的数据有1000个，采用二分检索大约需要10步，如果被检索
    的数据项有一百万个，而分检索值需要二十步。可见项目越多，二分检索的优势也就越明显。
    
    <br>
    ####2.排序
    
            快速排序工作方式：
                从数组中取出一个元素（基准值）；
                其他元素分为两组：
                    “小的”是那些小于基准值的元素；
                    “大的”是那些大于基准值的元素。
                递归的对这两个数组进行排序。
        当这个工作结束时，整个数组已经有序了。快速排序非常快，原因是：一旦知道了某个元素比基准值小，它就不必再与那些大的
        元素进行比较了。同样，大的元素也不必再与小的做比较了。
        
            下面的quicksort函数做整数数组的排序：
            void quicksort(int v[], int n)
            {
                int i, last;
                if (n <= 1)
                    return;
                swap(v, 0, rand() % n);
                last = 0;
                for (i = 1; i < n; i++)
                    if (v[i] < v[0])
                        swap(v, ++last, i);
                swap(v, 0, last);
                quicksort(v, last);
                quicksort(v+last+1, n-last-1);
            }
            
            void swap(int v[], int i, int j)
            {
                int temp;
                temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }

<br>
####3.库

        C函数库中排序函数的名字是qsort，在调用qsort时必须为它提供一个比较函数，因为在排序中需要比较两个值。由于这里的
    值可以是任何类型的，比较函数的参数是两个指向被比较数据的void*指针，在函数里应该把指针强制转换到适当的类型，然后提
    取数据值加以比较，并返回结果。
        下面是对字符串数组的排序，它首先对两个参数做强制转换，然后调用strcmp做比较：
        int scmp(const void *p1, const void *p2)
        {
            char *v1, *v2;
            
            v1 = *(char **)p1;
            v2 = *(char **)p2;
            return strcmp(v1, v2);
        }
        我们不能直接用strcmp作为比较函数，因为qsort传递的是数组里元素的地址，也就是说是&str[i](类型为char**)而不是str[i]
    (类型为char*)，如下图所示：
                N个指针的数组
        str[0]      [ ]--->an
        str[1]      [ ]--->array
        str[2]      [ ]--->of
                    .
                    .
                    .
                    [ ]
        str[N-1]    [ ]--->strings
    如果需要对字符串数组元素str[0]到str[N-1]排序，那么就应该以这个数组、数组的长度、被排序元素（数组元素）的大小以及比较
    函数作为参数，调用qsort：
        char *str[N];
        qsort(str, N, sizeof(str[0]), scmp);
        
        下面是一个用于比较整数的类似函数icmp：
        int icmp(const void *p1, const void *p2)
        {
            int v1, v2;
            
            v1 = *(int *)p1;
            v2 = *(int *)p2;
            if (v1 < v2)
                return -1;
            else if (v1 == v2)
                return 0;
            else 
                return 1;
        }
    我们或许想写：
        ?   return v1 - v2;
    但在这种情况下，如果v1是很大的正数而v2是大负数，或者相反，这个计算结果就可能溢出，并由此产生不正确的结果。采用直接比
    较写起来虽然长一点，但是却更安全。
        调用qsort同样要用数组、数组长度、被排序元素的大小以及比较函数做参数：
        int arr[N];
        qsort(arr, N, sizeof(arr[0]), icmp);
        
        ANSI C也定义了一个二分检索函数bsearch。与qsort类似，bsearch也要求一个指向比较函数的指针（常用于qsort相同的函数）。
    bsearch返回一个指针，指向检索到的那个元素；如果没找到有关元素，bsearch返回NULL。下面是用bsearch重写的HTML查询函数：
        int lookup(char *name, Nameval tab[], int ntab)
        {
            Nameval key, *np;
            
            key.name = name;
            key.value = 0;
            np = (Nameval *)bsearch(&key, tab, ntab, sizeof(tab[0]), nvcmp); //比qsort多了一个参数
            if (np == NULL)
                return -1;
            else
                return np - tab;
        }
        与qsort情况相同，比较函数得到的也是被比较项的地址，所以，这里的key也必须具有项的类型。在这个例子里，我们必须专门
    造出一个Nameval的项，将它传给比较函数。函数nvcmp比较两个Nameval项，方法是对他们的字符串部分调用函数strcmp，值部分在
    这里完全不看。
        int nvcmp(const void *va, const void *vb)
        {
            const Nameval *a, *b;
            
            a = (Nameval *)va;
            b = (Nameval *)vb;
            return scrcmp(a->name, b->name);
        }
        
        标准C++库里有一个名字为sort的类属算法，它保证O(nlogn)的执行性质。使用它的代码非常简单，因为这个函数不需要强制转
    换，也不需要知道元素的大小。对于已知排序关系的类型，它甚至不要求显示的比较函数。
        int arr[N];
        sort(arr, arr+N);
