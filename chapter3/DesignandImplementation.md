###设计与实现

    数据结构的设计是程序构造过程的中心环节。一旦数据结构安排好了，算法就像是瓜熟蒂落，编码也比较容易。
    程序设计语言的选择在整个设计过程中，相对而言，并不是那么重要。
    
    我们准备做的就是产生一些随机的可以读的英语文本。
    
<br>
####1.马尔可夫链算法

        完成这种处理有一种非常漂亮的方法，那就是使用一种称为马尔可夫链算法的技术。们可以把输入想像成由一些互相重叠的短语
    构成的序列，而该算法把每个短语分割为两个部分：一部分是由多个词构成的前缀，另一部分是只包含一个词的后缀。马尔可夫链算
    法能够生成输出短语的序列，其方法是依据 (在我们的情况下)原文本的统计性质，随机性地选择跟在前缀后面的特定后缀。采用三个
    词的短语就能够工作得很好——利用连续两个词构成的前缀来选择作为后缀的一个词：
        设置w1和w2为文本的前两个词
        输出w1和w2
        循环：
            随机的选出w3，它是文本中w1w2的后缀中的一个
            打印w3
            把w2和w3分别换成w2和w3
            重复循环
    为了说明问题，假设我们要基于本章开头的引语里的几个句子生成一些随机文本。这里采用的是两词前缀：
        Show your flowcharts and conceal your tables and I will be
        mystified. Show your tables and your flowcharts will be
        obvious. (end)
        下面是一些输入词对和跟随它们之后的词：
            输入前缀                        跟随的后缀词
            Show your                   flowcharts tables
            your flowercharts           and will
            flowcharts and              conceal
            flowcharts will             be
            your tables                 and and
            will be                     mystifid. obvious.
            be mystifid.                Show
            be obvious.                 (end)
        处理这个文本的马尔可夫算法将首先打印出Show your，然后随机取出flowcharts或tables。如果选中了前者，那么现在前缀
    就变成了your flowcharts，而下一个词应该是and或will。如果它选取tables，下一个词就应该是and。这样继续下去，知道产生
    出足够多的输出，后者在找后缀是遇到了结束标志。
        我们的程序将读入一段英语文本，并利用马尔可夫链算法，基于文本中固定长度的短语的出现频率，产生一段新文本。前缀
    中词的数目是个参数，上面用的是 2。如果将前缀缩短，产生出来的东西将趋向于无聊词语，更加缺乏内聚力；如果加长前缀，
    则趋向于产生原始输入的逐字拷贝。对于英语文本而言，用两个词的前缀选择第三个是一个很好的折衷方式。看起来它既能重现
    输入的风味，又能加上程序的古怪润饰。

<br>
####2.数据结构的选择

        我们需要有一种数据结构，它能较好地表示前缀以及与之相关联的后缀。程序将分两部分，第一部分是输入，它构造表示短
    语的整个数据结构；第二部分是随后的输出，它使用这个数据结构，生成随机的输出。这两部分都需要(快速地)查询前缀：输入
    过程中需要更新与前缀相关的后缀；输出时需要对可能后缀做随机选择。这些分析提醒我们使用一种散列结构，其关键码是前缀，
    其值是对应于该前缀的所有可能后缀的集合。
        为了描述的方便，我们将假定采用二词前缀，在这种情况下，每个输出词都是根据它前面的一对词得到的。前缀中词的个数
    对设计本身并没有影响，程序应该能对付任意的前缀长度，但给定一个数能使下面的讨论更具体些。我们把一个前缀和它所有可
    能后缀的集合放在一起，称其为一个状态，这是马尔可夫算法的标准术语。
        对于一个特定前缀，我们需要存储所有能跟随它的后缀，以便将来取用。这些后缀是无序的，一次一个地加进去。我们不知
    道后缀将会有多少，因此，需要一种能容易且高效地增长的数据结构，例如链表或者动态数组。在产生输出的时候，我们要能从
    关联于特定前缀的后缀集合中随机地选出一个后缀。还有，数据项绝不会被删除。
        如果一个短语出现多次，那么又该怎么办？例如，短语“ might appear twice”可能在文本里出现两次，而“might appear
    once”只出现了一次。这个情况有两种可能的表示方式：或者在“might appear”的后缀表里放两个“twice”；或者是只放一个，
    但还要给它附带一个计数值为2的计数器。我们对用或不用计数器的方式都做过试验。不用计数器的情况处理起来比较简单，因
    为在加入后缀时不必检查它是否已经存在。试验说明这两种方式在执行时间上的差别是微不足道的。
    
        总结一下：每个状态由一个前缀和一个后缀链表组成。所有这些信息存在一个散列表里，以前缀作为关键码。每个前缀是一
    个固定大小的词集合。如果一个后缀在给定前缀下的出现多余一次，则每个出现都单独包含在有关链表里。
    
<br>
####3.在C中构造数据结构

        现在开始考虑C语言中的实现。首先定义一些常数：
        enum {
            NPREF   = 2,        // number of prefix words
            NHASH   = 4093,     // size of state hash table array
            MAXGEN  = 10000     // maximum words generated
        };
    这个声明定义了前缀中词的个数（NPREF），散列表数组的大小（NHASH），生成词数的上界（MAXGEN）。如果NPREF是个编译时的
    常数而不是运行时的变量，程序里的存储管理将会更简单些。数组的规模设的相当大，因为我们预计程序可能处理很大的输入文
    件，或许是整本书。选择NHASH=4093，这样，即使输入里有10000个不同前缀（词对），平均链长仍然会很短，大约两个到三个前
    缀。数组越大，链的期望长度越短，查询进行得也越快。实际上，这个程序还仍然是个摆设，因此其性能并不那么关键。另一方
    面，如果选用的数组太小，程序将无法在合理时间里处理完可能的输入。而如果它太大，又可能无法放进计算机的存储器中。
        前缀可以用词的数组方式存储。散列表的元素用State（状态）数据类型表示，它是前缀与Suffix（后缀）链表的关联：
        typedef struct State State;
        typedef struct Suffix Suffix;
        struct State { /* prefix + suffix list */
            char    *pref[NPREF];   // prefix words
            Suffix  *suf;           // list of suffixes
            State   *next;          // next in hash table
        };
        
        struct Suffix { /* list of suffixes */
            char    *word;          // suffix
            Suffix  *next;          // next in list of suffixes
        };
        
        State *statetab[NHASH];
        现在看一下图示，整个数据结构将具有下面的样子：
![](https://github.com/suliutree/ThePracticeOfProgramming/blob/master/Image/01.png)
        我们需要一个作用于前缀的散列函数，前缀的形式是字符串数组，显然不难对第2章的字符串散列函数做一点修改，使之可用
    于字符串的数组。下面的函数对数组里所有字符串的拼接做散列：
        /* hash: compute hash value for array of NPREF strings */
        unsigned int hash(char *s[NPREF])
        {
            unsigned int h;
            unsigned char *p;
            int i;
            
            h = 0;
            for (i = 0; i < NPREF; i++)
                for (i = 0; i < NPREF; i++)
                    h = MULTIPLIER * h + *p;
            return h % NHASH;
        }
        再加上对检索函数的简单修改，散列表的实现就完成了：
        /* lookup: search for prefix; create if requested. */
        /* creation doesn't strdup so strings mustn't change later. */
        State* lookup(char *prefix[NPREF], int create)
        {
            int i, h;
            State *sp;
            
            h = hash(prefix);
            for (sp = statetab[h]; sp != NULL; sp = sp->next) {
                for (i = 0; i < NPREF; i++)
                    if (strcmp(prefix[i], sp->pref[i]) != != 0)
                        break;
                if (i == NPREF)
                    return sp;
            }
            
            if (create) {
                sp = (State *)emalloc(sizeof(State));
                for (i = 0; i < NPREF; i++)
                    sp->pref[i] = prefix[i];
                sp->suf = NULL;
                sp->next = statetab[h];
                statetab[h] = sp;
            }
            return sp;
        }
    注意，lookup在建立新状态时并不做输入字符串的拷贝。它只是向sp->pref[]里存入一个指针。这实际上要求调用lookup的程序
    必须保证这些数据不会被覆盖。例如，如果字符串原来存放在I/O缓冲区里，那么在调用lookup前必须先做一个拷贝。负责后面的
    输入就会把散列表指针所指的数据覆盖掉。对于跨越某个界面的共享资源，常常需要确定它的拥有者到底是谁。
        作为下一步，我们需要在读入文件的同时构造散列表：
        void build(char *prefix[NPREF], FILE *f)
        {
            char buf[100], fmt[10];
            
            /* create a format string; %s could overflow buf */
            sprintf(fmt, "%%%ds", sizeof(buf) - 1);
            while (fscanf(f, fmt, buf) != EOF)
                add(prefix, estrdup(buf));
        }
        对sprintf的调用有些奇怪，这完全是为了避免fscanf的一个非常烦人的问题，而从其他方面看，使用fscanf都是很合适的。
    如果以s%作为格式符调用fscanf，那就是要求把文件的下一个空白界定的词读入缓冲区。但是，假如在这种情况下没有长度限制，
    特别长的词就可能导致输入缓冲区溢出，从而酿成大祸。假设缓冲区的长度为100个字节（这远远超出正常文本中可能出现的词的
    长度），我们可以用%99s（留一个字节给串的结束符'\0'），这是告诉fsanf读到99个字符就结束。这样做有可能把过长的词分成
    段，虽然是不幸的，但确实安全的。我们可以声明：
        ?   enum { BUFSIZE = 100 };
        ?   char fmt[] = "%99s";
    但是这里又出现了由一个带随意性的决定（缓冲区大小）导出的两个常数，并要求维护他们之间的关系。这个问题可以一下子解
    决：只要利用sprintf动态的建立格式串，也就是上面程序采用的方式。
        函数build有两个参数，一个是prefix数组，用于保存前面的NPREF个输入词；另一个是个FILE指针。函数把prefix和读入词
    的一个拷贝送给add，该函数在散列表里加入一个新项，并更新前缀数组：
        void add(char *prefix[NPREF], char *suffix)
        {
            State *sp;
            
            sp = lookup(prefix, 1); /* create if not found */
            addsuffix(sp, suffix);
            memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
            prefix[NPREF-1] = suffix;
        }
    对memmove的调用是在数组里做删除的一个惯用法。该操作把前缀数组里从1到NPREF-1的元素向下搬，移到从0到NPREF-2的位置。
    这也删去了第一个前缀次，并为新来的一个在后面腾出了位置。
        函数addsuffix把一个新的后缀加进去：
        void addsuffix(State *sp, char *suffix)
        {
            Suffix *suf;
            suf = (Suffix *)emalloc(sizeof(Suffix));
            suf->word = suffix;
            suf->next = sp->suf;
            sp->suf = suf;
        }
    这里的状态更新操作分由两个函数实现：add完成给有关前缀加入一个后缀的一般性工作，addsufffix做的是由特定实现方式决定
    的动作，把一个词具体的加进后缀链表里。函数add由build调用，而addsuffix只在add内部使用，因为这里牵涉到的是一个实现
    细节，这个细节今后也可能会变化。所以，虽然该操作只在这一个地方用，最好也将它建立为一个独立的函数。
    
<br>
####4.生成输出

        数据结构构造好之后，下一步就是产生输出。这里的基本思想与前面类似：给定一个前缀，随机地选出它的某个后缀，打印
    输出并更新前缀。当然，这里说的是处理过程的稳定状态，还需要弄清算法应该如何开始和结束。如果我们已经记录了文中第一
    个前缀，操作就非常简单了：直接从它们起头。结束也容易。我们需要一个标志字来结束算法，在所有正常输入完成之后，我们
    可以加进一个结束符，一个保证不会在任何输入里出现的“词” ：
        build(prefix, stdin);
        add(prefix, NONWORD);
    NONWORD应该是某个不可能在正规输入里遇到的值。由于输入词是由空白界定的，一个空白的“词”总能扮演这个角色，比如用一个
    换行符号：
        char NONWORD[] = "\n";  /* cannot appear as real word */
        还有一件事也需要考虑：如果输入根本就不够启动程序，那么又该怎么办呢？处理这类问题有两种常见方式：或是在遇到输
    入不足时立即退出执行；或是通过安排使得输入总是足够的，从而就完全不必再理会这个问题了。对这里的程序而言，采用后一
    种方式能够做得很好。
        我们可以用一个伪造的前缀来初始化数据结构构造和输出生成过程，这样就能保证程序的输入总是足够的。在做循环准备时，
    我们把前缀数组装满NONWORD词。这样做有一个非常好的效果：输入文件里的第一个词将总是这个伪造前缀的第一个后缀 。这样，
    生成循环要打印的全都是它自己生成的后缀。
        如果输出非常长，我们可以在产生了一定数目的词之后终止程序；另一种情况是程序遇到了后缀 NONWORD。最终看哪个情况
    先出现。
        在数据的最后加上几个 NONWORD，可以大大简化程序的主处理循环。这是一种常用技术的又一个实例：给数据加上哨卫，用
    以标记数据的界限。
        作为编程的一个规则，我们总应该设法处理数据中各种可能的非正常情况、意外情况和特殊情况。编出正确代码很不容易，
    因此应该尽量使控制流简单和规范。
        函数generate采用的就是前面已经给出了轮廓的算法。它产生每行一个词的输出，用文字处理程序可以把它们汇成长的行。
    第 9章将给出一个能完成这个工作的简单格式化程序fmt。
        借助于数据开始和结束的NONWORD串，generate也很容易开始和结束：
        /* generate: produce output, one word per line */
        void generate(int nwords)
        {
            State *sp;
            Suffix *suf;
            char *prefix[NPREF], *w;
            int i, nmatch;
            
            for (i = 0; i < NPREF; i++) /* reset inital prefix */
                prefix[i] = NONWORD;
            
            for (i = 0; i < nwords; i++) {
                sp = lookup(prefix, 0);
                nmatch = 0;
                for (suf = sp->suf; suf != NULL; suf = suf->next)
                    if (rand() % ++nmatch == 0) // prob = 1/nmatch
                        w = suf->word;
                if (strcmp(w, NONWORD) == 0)
                    break;
                printf("%s\n", w);
                memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
                prefix[NPREF-1] = w;
            }
        }
    注意，算法需要在不知道到底有多少个项的情况下随机地选取一个项。变量nmatch用于在扫描后缀表的过程中记录匹配的个数。
    表达式：
        rand() % ++nmatch == 0
    增加nmatch的值，而且使它为真的概率总是 1/nmatch。这样，第一个匹配的项被选中的概率为1，第二个将有1/2的概率取代它，
    第3个将以1/3的概率取代前面选择后留下的项，依此类推。在任何时刻，前面匹配的k个项中的每一个都有1/k的被选概率。
        在算法开始时，p r e f i x已经被设为初始值，可以保证散列表中一定有它。这样得到的第一个suffix值就是文件里的第一
    个词，因为它是跟随初始前缀的惟一后缀。在此之后，算法随机地选择后缀：循环中调用lookup，查出当前prefix对应的散列表
    项，然后随机地选出一个对应后缀，打印它并更新前缀。
        如果选出的后缀是NONWORD，则工作完成，因为已经选到了对应输入结束的状态。如果后缀不是NONWORD，则打印它，然后调
    用memmove丢掉前缀的第一个词，把选出的后缀升格为前缀的最后一个词，并继续循环下去。
        现在，我们可以把所有东西放到一起，装进一个main函数里，它从标准输入流读入，生成至多有指定个数的词序列：
        int main(void)
        {
            int i, nwords = MAXGEN;
            char *prefix[NPREF];            // current input prefix
            for (i = 0; i < NPREF; i++)     // set up initial prefix
                prefix[i] = NONWORD;
            build(prefix, stdin);
            add(prefix, NONWORD);
            generate(nwords);
            return 0;
        }
        
<br>
####6.C++

        实际上，前面C版本的markkov也是一个合法的C++程序。对C++而言，更合适的用法应该是定义一些类，建立起程序中需要的
    各种对象，或多或少像我们写Java程序是所做的那样，这样可以隐蔽起许多实现细节。我们在这里希望更前进一步，使用C++的
    Standard Template Library（标准模板库），即STL。因为STL提供许多内部机制，能完成我们需要做的许多事情。
        deque是一种双端队列，它正好能符合我们对前缀操作的需要：可以用它存放NPREF个元素，丢掉最前面的元素并在后面添加
    一个新的，这都是O(1)的操作。实际上，STL的deque比我们需要的东西更一般，它允许在两端进行压入和弹出，而执行性能方面
    的保证是我们选择它的原因。
        STL还提供了一个map容器，其内部实现基于平衡树。在map中可以存储关键码-值对。map的实现方式保证，从任何关键码出发
    提取相关值的操作都是O(logn)。虽然map可能不如O(1)散列表效率高，但是，直接使用它就可以不必写任何代码，这样也很不错
    （某些非标准的C++库提供了hash或hash_map容器，它的性能可能更好些）。
        我们也可以使用内部提供的比较函数，用于对前缀中各字符串做比较。
        手头有了这些组件，有关代码可以流畅的做出来了。下面是有关声明：
        typedef deque<string> Prefix;
        map<Prefix, vector<string> > statetab;
    STL提供了deque的模板，记法deque<string>将它指定为以字符串为元素的deque。由于这个类型将在程序里多次出现，在这里用
    一个typedef声明，将它另外命名为Prefix。这里声明了一个map类型的变量statetab，它是从前缀到后缀向量的映射。在这里工
    作比在 C或Java中更方便，根本不需要提供散列函数或者equal方法。
        main函数对前缀做初始化，读输入（对于标准输入，调用C++ iostream里的cin），在输入最后加一个尾巴，然后产生输出。
    和前面各个版本完全一样。
        int main(void)
        {
            int nwords = MAXGEN;
            Prefix prefix;
            
            for (int i = 0; i < NPREF; i++)
                add(prefix, NONWORD);
            build(prefix, cin);
            add(prefix, NONWORD);
            generate(nwords);
            return 0;
        }
        函数build使用iostream库，一次读入一个词：
        // build: read input words, build state table
        void build(Prefix& prefix, istream& in)
        {
            string buf;
            while (in >> buf)
            add(prefix, buf);
        }
    字符串buf能够根据输入词的长度需要自动增长。
        函数add能够进一步说明使用STL的优越性：
        // add: add word to suffix list, update prefix
        void add(Prefix& prefix, const string& s)
        {
            if (prefix.size() == NPREF) {
                statetab[prefix].push_back(s);
                prefix.pop_front();
            }
            prefix.pop_back(s);
        }
    这几个非常简单的语句确实做了不少事情。map容器类重载了下标运算符 ( [ ]运算符)，使它在这里成为一种查询运算。表达式       
    statetable[prefix]在statetab里完成一次查询，以prefix作为查询的关键码，返回对于所找到的项的一个引用。如果对应的向
    量不存在，这个操作将建立一个新向量。vector和deque类的push_back函数分别把一个新字符串加到向量或deque的最后；
    pop_front从deque里弹出头一个元素。
        输出生成与前面版本类似：
        void generate(int nwords)
        {
            Prefix prefix;
            int i;
            for(i = 0; i < NPREF; i++)
                add(prefix, NONWORD);
            for (i = 0; i < nwords; i++) {
                vector<string>& suf = statetab[prefix];
                const string& w = suf[rand() % suf.size()];
                if (w == NONWORD)
                    break;
                cout << w << "\n";
                prefix.pop_front();
                prefix.push_back(w);
            }
        }
        总的来说，这个版本看起来特别清楚和优雅——代码很紧凑，数据结构清晰，算法完全一目了然。可惜的是，在这里也要付出
    一些代价：这个版本比原来的 C版本慢得多，虽然它还不是最慢的。
        
        
        
    
        
        
        
        
        
        
        
        
        
        
        
