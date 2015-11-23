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
        char *strstr(char *str1, char *str2);   若str2是str1的子串，则先确定str2在str1中第一次出现的位置，并返回此位置到
                                                str1末尾的所有字符

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

<br>
####6.可增长数组

        下面的代码定义了一个元素为Nameval类型的可增长数组，新元素将被加到有关数组的最后。在必要时数组将自动增大以提供新
    的空间。任何元素都可以通过下标在常数时间里访问。这种东西类似于Java和C++库中的向量类。
        typedef struct Nameval Nameval;
        struct Nameval {
            char *name;
            int value;
        };
        
        struct NVtab {
            int nval;
            int max;
            Nameval *nameval;
        }nvtab;
        
        enum { NVINIT = 1, NVGROW = 2};
        
        /* addname: add new name and value to nvtab*/
        int addname(Nameval newname)
        {
            Nameval *nvp;
            if (nvtab.nameval == NULL) {
                nvtab.nameval = (Nameval *)malloc(NVINIT * sizeof(Nameval));
                if (nvtab.nameval == NULL)
                    return -1;
                nvtab.max = NVINIT;
                nvtab.nval = 0;
            } else if (nvtab.nval >= nvtab.max) {
                nvp = (Nameval *)realloc(nvtab.nameval, (NVGROW * nvtab.max) * sizeof(Nameval));
                if (nvp == NULL)
                    return -1;
                nvtab.max *= NVGROW;
                nvtab.nameval = nvp;
            }
            nvtab.nameval[nvtab.nval] = newname;
            return nvtab.nval++;
        }
    函数addname返回刚加入数组的项的下标，出错的时候返回-1.
        对realloc调用将把数组增长到一个新的规模，并保持已有的元素不变。这个函数返回指向新数组的指针；当存储不够时返回
    NULL。注意，上面的代码中没有采用下面这种方式：
        ?   nvtab.nameval = (Nameval *)realloc(nvtab.nameval, (NVGROW*nvtab.max) * sizeof(Nameval));
        如果采用这种形式，当重新分配失败时原来的数组就会丢失。
        我们开始用一个非常小的初值（NVINIT = 1）确定数组规模。这迫使程序一定要增长其数组，保证这段程序能够被执行。如果这
    段代码放在产品里使用，初始值可以改的大一些。当然由小的初始值引起的代价也是微不足道的。
        按说realloc的返回值不必强制到最后类型，因为C能自动完成对void*的提升。而C++中就不同，在那里必须做类型强制。有人这
    里会争辩，究竟是应该做强制（这样做清晰而认真）还是不做强制（因为强制实际上可能掩盖真正的错误）。我们选择写强制，是因
    为这种写法能同时适用于C和C++，付出的代价是减少了C编译器检出错误的可能性。通过允许使用两种编译器，我们也得到了一点补
    偿。
        删除名字需要一点诀窍，因为必须决定怎样填补删除后数组中留下的空隙。如果元素的顺序并不重要，最简单的办法就是把位于
    数组的最后元素复制到这里。如果还必须保持原有的顺序，我们就只能把空洞后面的所有元素前移一个位置：
        int delname(char *name)
        {
            int i;
            for (i = 0; i < nvtab.nval; i++)
                if (strcmp(nvtab.nameval[i].name, name) == 0) {
                    memmove(nvtab.nameval + i, nvtab.nameval + i + 1, (nvtab.nval - (i+1)) * sizeof(Nameval));
                    nvtab.nval--;
                    return 1;
                }
                return 0;
        }
    这个程序里调用memmove，通过把元素向下移一个位置的方法将数组缩短。memmove是标准库函数，用于复制任意大小的存储区域。
        在ANSI C的标准库里定义了两个相关的函数：memcpy的速度快，但是如果源位置和目标位置重叠，它有可能覆盖掉存储区中的
    某些部分；memmove函数的速度可能慢些，但总能保证复制的正确完成。
        也可以使用下面的饿循环代替程序里对memmove的调用：
        int j;
        for (j = i; j < nvtab.nval - 1; j++)
            nvtab.nameval[j] = nvtab.nameval[j+1];
    我们喜欢用memmove，因为这样可以避免人很容易犯的复制顺序的错误。例如，如果这里要做的是插入而不是删除，那么循环的顺序
    就必须反过来，以避免覆盖掉数组元素。通过调用memmove完成工作就不必为这些事操心了。
        
<br>
####7.表

        在C里通常不是直接定义表的类型list，而是从某种元素开始，例如HTML的元素Nameval，给它加一个指针，以便链接到下一个
    元素：
        trpedef struct Nameval Nameval;
        struct Nameval {
            char *name;
            int value;
            Nameval *next;
        };
        我们无法在编译时初始化一个非空的表，这点也与数组不同。表应该完全动态构造起来的。首先我们需要有方法来构造一个项。
        最直接的方式是定义适当的函数，完成有关的非配工作，这里称它为newitem：
            Nameval *newitem(char *name, int value)
            {
                Nameval *newp;
                
                newp = (Nameval *)emalloc(sizeof(Nameval));
                newp->name = name;
                newp->value = value;
                newp->next = NULL;
                return newp;
            }
        函数emalloc将调用malloc，如果分配失败，它报告一个错误并结束程序的执行。函数的定义代码在第4章给出，现在只要认定
        emalloc是个存储分配函数，它绝不会以失败的情况返回。
            构造表最简单而又快速的方法就是把每个元素都加在表的最前面：
            Nameval *addfront(Nameval *listp, Nameval *newp)
            {
                newp->next = listp;
                return newp;
            }
            当一个表被修改时，结果可能得到另一个首元素，就像addfront里的情况。对表做更新操作的函数必须返回指向新首元素
        的指针，这个指针将被存入保持着这个表的变量里。函数addfront和这组函数里的其它函数都返回指向表中首元素的指针，以
        此作为它们的返回值。函数的典型使用方式是：
            nvlist = addfront(nvlist, newitem("smiley", 0x263A));
        这种设计对于原表为空的情况同样可以用，函数也可以方便的用在表达式里。
            如果要在表末尾加一个元素，这就是一个O(n)的操作，因为函数必须穿越整个表，直到找到了表的末端：
            Nameval *addend(Nameval *listp, Nameval *newp)
            {
                Nameval *p;
                if (listp == NULL)
                    return newp;
                for (p = listp; p->next != NULL; p = p->next)
                    ;
                p->next = newp;
                return listp;
            }
        如果想把addend做成一个O(1)的操作，那么就必须维持另一个独立的指向表尾的指针。除了总需要费心维护表尾指针外，这种
        做法还有另一个缺点：表再不是由一个指针变量表示的东西了。下面我们将坚持最简单的风格。
            要检索具有某个特定名字的项，应该沿着next指针走下去：
            Nameval *lookup(Nameval *listp, char *name)
            {
                for ( ; listp != NULL; listp = listp->next)
                    if (strcmp(name, listp->name) == 0)
                        return listp;
                return NULL;
            }
        如果要打印表里的所有元素，可以直接写一个函数，它穿越整个表并打印每个元素；要计算表的长度，可以写一个函数穿越整
        个表，其中使用一个计数器；如此等等。这里再提出另一种解决问题的方式，那就是写一个名为apply的函数，它穿越表并对表
        的每个元素调用另一个函数。我们可以把apply做的更具一般性，为它提供一个参数，把它传递给apply对表元素调用的那个函
        数。这样，apply就有了3个参数：一个表、一个将要被作用于表里每个元素的函数以及提供给该函数使用的一个参数：
            void apply(Nameval *listp, void (*fn)(Nameval*, void*), void *arg)
            {
                for ( ; listp != NULL; listp = listp->next)
                    (*fn)(listp, arg);
            }
        appaly的第二个参数是一个函数指针，这个函数有两个参数，返回类型是void。这种标准描述方式在语法上很难看：
            void (*fn)(Nameval*, void*)
        这说明了fn是一个指向void函数的指针。也就是说，fn本身是个变量，它将以一个返回值为void的函数地址作为值。被fn指向
        的函数应该有两个参数，一个参数的类型是Nameval*，即表的元素类型；另一个是void*，是个通用指针，它将作为fn所指函
        数的一个参数。
            要使用appaly，例如打印一个表的元素，我们可以写一个简单的函数，其参数包括一个格式描述串：
            void printnv(Nameval *p, void *arg)
            {
                char *fmt;
                fmt = (char *)arg;
                printf(fmt, p->name, p->value);
            }
        它的调用形式是：
            apply(nvlist, printnv, "%s: %x\n");
            为统计表中的元素个数也需要定义一个函数，其特殊参数是一个指向整数的指针，该整数倍用作计数器：
            void inccounter(Nameval *p, void *arg)
            {
                int *ip;
                
                ip = (int *)arg;
                (*ip++);
            }
        对这个函数的调用可以是：
            int n;
            n = 0;
            appaly(nvlist, inccounter, &n);
            printf("%d elements in nvlist\n", n);
            并不是每个表操作都能很方便的以这种方式实现。要销毁一个表就必须特别小心：
            void freeall(Nameval *listp)
            {
                Nameval *next;
                for ( ; listp != NULL; listp = next) {
                    next = listp->next;
                    free(listp);
                }
            }
        当一块存储区被释放之后，程序里就不能再使用它了。因此，在释放listp所指向的元素之前，必须首先把listp->next的值
        保存到一个局部变量(next)里。如果把上面的循环写成下面的样子：
            ?   for ( ; listp != NULL; listp = listp->next)
            ?       free(listp);
        由于listp->next原来的值完全可能被free复写掉，这个代码有可能会失败。









