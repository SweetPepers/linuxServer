### const 
常量
- 常量—变量 const float PI =  3.14f;   定义之后无法修改 //在什么时候？？？
无法修改是 程序编译时期无法修改   运行后可以修改内存


指针 
- 	char * p = “hello world!”; //后面字符串为常量，直接不能被修改
  -> `const char* p = “Hello world!”;`
  -> `char const* p = “hello world !”;`  两种方式一样的
指针指向的内容不能修改  指针本身的值可以改变
- char * const p = “hello World!”; 指针指向的内容可以修改，但是指针的值不能改。
- const char * const p = “hwsada”;  两个都不能修改了。
- int  不能指向 const int   const int n = 5; int *p = &n; 报错
1.需要将const int 强转为 int
2.int * p = const_cast<int*>(&n);  //将一个类型的const修饰去掉

### 默认参数

有默认参数的参数放在后面

###	内联函数
-	inline int getMax(int a , int b);  
一般放在头文件里面  编译器不一定会执行  建议
-	宏定义会有一些错误
例如 #define GETMAX(a, b) (a > b ? a : b) 
执行 GETMAX(n++, 1)；相当于(n++ > 1 ? n++ : 1) n加了两次 
###	引用
-	在函数内部改变函数外部传来的参数
    - 通过指针   易产生程序崩溃 指针指空nullptr
-	引用 int& refN = n; //左值引用   
应用相当于变量的别名 指向相同的内存
```cpp
int fooRef(int& n) { n = 5; }
```
- 使用时  传入n的引用 int &refN = n  即可改变n的值 fooRef(refN); 
或者传入  fooRef(n); 也可以


引用的使用
-	只能把变量赋值给引用
-	引用关系建立后无法修改  发生  int & refN = n;  refN = m 时相当于将m的值赋值给n
-	使用
    -	作为函数参数
    - 作为函数返回值
- 二级引用不存在


引用的本质 : 引用的值是关联对象的地址，本质是限制的指针
###	作用域与数据隐藏
-	全局作用域 名字空间域 (namespace)
-	局部作用域 或者 块作用域
-	类域(class)
-	数据隐藏
```cpp
{ int n =1; { int n = 0} /*这里n = 1*/}  / 
```
- 在不同的作用域可以定义多个相同名字的变量，内部的变量会隐藏外部的变量，内部无变量时，会就近选择外部变量（内->外查找）

###	名字空间定义及作用
`namespace`
使用：
namespace ShellMad{
    int nWidth = 5;
};

ShellMad::nWidth = 8;
-	一中作用域的区分，通常用于项目中的模块或组件
    -	名字控件可以嵌套使用
      `ShellMad::ShellMoney::nMoney;`
    -	名字控件可以取别名
      `namespace n = ShellMad::ShellMoney;`
    -	当在块内使用全局作用域的变量时，如此操作
      `::nWidth = 8;`  前面加个四桶就表示使用的全局的变量了。

名字空间的引用及原理
- 直接使用 名字空间::变量
-	using namespace ShellMad 从这句话之后，当前区域内可以直接使用ShellMad里面的变量。（不推荐）
-	声明只使用名字空间的部分变量或函数 using ShellMad::n;可以直接使用n，其他不能直接使用。

###	函数重载
-	宏实现 缺乏类型检查 但都可以编译通过。
-	写函数 `getMax(int a, int b);` --> 如果不是int的话会不匹配，相似功能还要再写一个函数?
-	函数重载：c++中允许出现同名的函数 // 参数决定
什么时候允许出现重载？？
-	条件（函数要素：函数名，函数参数，函数返回值，函数的条用约定）
    -	函数名必须相同
    -	函数参数个数不同||类型不同||顺序不同(不同类型参数的顺序)
    -	函数返回值不做考虑
    - 调用约定
      int __stdcall getMax();
      int __cdecl getMax();
          调用约定不做考虑。
    - 作用域相同

调用规则及本质
-	优先调用隐式转换最方便的函数。
int getMax(float a, int b)  getMax(1, 1.0) 是合理的。 （隐式转换）
可隐式转换char - int, float - double , float – int 
-	根据函数名找对应的函数作为候选函数。
-	最佳匹配个数等于1才会调用成功，>1会报 二义性

##	面向对象及类
-	对象：object 数据(数据类型)+行为(函数)
-	类：有函数的结构体，联系紧密

###	类及类的访问权限
面向对象特征：
- 封装
-	继承
-	多态


c++对结构体进行了扩充，可以在结构体里添加函数（封装）
结构体和类的区别：默认访问权限的区别，结构体默认公有权限，**类默认私有权限**


访问权限：在当前类域之外访问的规则
-	public：类域里和类域外	均可访问
-	protected：继承中可以使用
-	private：类域里可以，类域外不可以访问

类的标准写法
-	成员(member)私有 命名：m_名字
-	成员函数部分私有
-	声明实现分开，实现前面加上类的作用域
```cpp
class person{
  int m_height;

public:
  int getHeight();
  void modHeight(int h);
}
int person::getHeight(){
  return this.m_height;
}

void person::modHeight(int h){
  this.m_height = h;
}

```


###	this指针
- 同一个类的对象，其成员函数地址是一样的，表示同一个类的对象的成员函数是共用的。
  - 数据是独立的
  - 成员函数是共用的

this指针指向当前的对象	 默认是__thiscalll，通过寄存%器ecx传递

类成员函数指针的调用
```CPP
// class CClock
CClock clock;
CClock *pClock = &clock
PFN_GETHOUR lp = &CClock::GetHour;
(clock.*lp)();
(pClock->*lp)();

```
###	构造函数
构造函数
- 类名作函数名
- 无函数返回值
- 参数 
  CStudent stu(n, name_s);
  CStudent stu = CStudent(n, name_s);

调用
构造函数可以函数重载
关键字
- explicit : 只支持显式调用构造函数，不支持隐式转换调用
- CStudent() = delete;  删除默认构造函数函数，后面用不了了
- CStudent() = default;  使用默认的构造函数
###	析构函数
- 作用：完成资源的反初始化，资源的释放
- 写法：类名前加上~
- 通常是编译器决定调用时机，不需要手动调用
  - 不能有参数和返回值。
  - 不能重载（没有参数）
  - 将其他指针的释放操作放入析构函数中。

调用时机
- 可以手动调用，手动调用后编译器还会调用一次，一般不要手动调用
- 构造和析构一般公有，如果构造私有化，不能创建对象
- 栈上的局部对象调用时机
构造：声明该对象时调用
析构：对象出作用域时调用析构
- 全局对象：
构造：进入到main之前构造
析构：离开main之后析构

###	拷贝构造函数
CStudent stu(“张三”, strlen(“张三”)+1);
CStudent stu2 = stu;
- 构造调用了一次？？最后程序崩溃free(m_szName)调用了两次
stu、stu2 地址相同，stu2没有调用自己编的构造函数，而调用了默认拷贝构造函数（memcpy，浅拷贝）。当用一个对象创建另一个对象的时候。
- 可以自己定义拷贝函数
CStudent(CStudent & obj){};

浅拷贝与深拷贝
- 浅拷贝 使用同一块内存
- 深拷贝：自己编写拷贝函数。


何时需要重写拷贝构造函数？
当资源需要在析构函数里释放的时候，或者说 当对象中的成员函数存在一中需要分配的资源时(堆空间，文件指针等)，为了避免析构时重复释放导致程序崩溃，需重写拷贝函数或者禁用（=delete）。

###	new与delete
- 堆对象 堆上的对象不会调用构造函数和析构函数
c语言char * pBuf = (char *)malloc(10);
CStudent * pStu = (CStdudent *)malloc (sizeof(CStudent));//并没有调用构造函数
- C++ new:分配空间，delete:释放空间 
在对象的使用上替代malloc和free, 可以调用构造函数和析构函数
- 在堆上创建一个对象
CStudent * pStu = new CStudent;
释放对象：delete pStu
- 关于普通的类型使用new和delete仅仅分配空间，没有构造和析构
```cpp
int * pN = new int;
int *pN = new int(123)；赋初值
delete pN;
```

new与delete使用注意事项
- 混用new，delete，malloc，free
int *pN = new int(123);
free pN;
是可以的
- 普通数据类型可以相互替代，对象不行
- 在堆上申请数组
int * pN = new int[10];
delete[] pN;
delete pN; //只调用一次析构函数
new 申请时，内存中创建的空间前面四个字节内容为数组个数，用于delete[]释放时调用析构函数。


###	继承
概念
- 代码重复，类之间存在共同的部分
- 一个类是另一个类的儿子
     子类           父类
     派生类         基类
`class CStudent : public CPerson`


可见性

在子类域里，可以访问父类的public和protected，父类里的private不能被继承
- 公有继承 public
父亲的 public、protected在子类中依然是public、protected
父亲的private子类中不可见
- 保护继承 protected
父亲的public、protected到儿子中变为protected
父亲的private不可见
- 私有继承 private
父亲的protected、public成员到儿子中变为private
父亲的私有不可见

整体来看取两者更隐私的方式

从内存角度看继承
- 子类从父类继承的变量也占用字节（无法访问的private也会存在，占用内存）
- 继承的可见性在何时做检查？？ 
是由编译器在编译时刻做的限制
- 指针转换的安全性 子类转父类是安全的，父类转子类不安全
  主要是指针越界问题。(子类大于等于父类, 自己意会, 大的可以转小的)
  子类指针转换为父类指针
  - CPerson * pPer = &stu; // 可以实现
  父类指针转换为子类指针
  - CStudent * pStu = (CStudent *)&per; // 不可以实现  后面多4个字节越界。

父子类，成员类构造析构顺序 – 析构与构造相反 **最后构造自己**
- 派生类的构造顺序：先父类后子类
- 析构顺序：出了mian()之后开始析构，先子类后父类。
- 朋友类：一个类作为另一个类的成员
- 构造：父亲 朋友 自己
- 析构：自己 朋友 父亲


派生类中的函数隐藏
- 初始化列表
  父亲中构造为有参构造
```cpp
class CStudent : public CPerson{
public:
    CStudent() : CPerson(1){};   // 初始化列表
    CStudent() : CPerson(1), m_StuId(4), n(4){}; // 也可调用子类成员自己的初始化
private:
    int StuId;
    const int n;  //或者上面取消 然后 const int n = 4;
}
```

- 当子类和父类中存在相同变量时，仍然遵循就近原则（调用儿子）
  强行调用父亲成员可以打出四桶 CPerson::m_nGender = 12;
- 子类和父亲中存在相同函数时，也遵循就近原则，但也可以强行调用。
- 隐藏的条件：
  - 函数名相同
  - 参数相同
  - 作用域不同
  - 参数列表和调用约定、返回值均不考虑
会优先在子类作匹配,例如
父类 void Test(int n){};
子类 void Test(){};
子类中使用Test(3) 会和子类中的子类匹配，编译不通过

###	多态和虚函数
```cpp
class CPerson{void speak(){print“说人话”}}
class CChinese : public CPerson{ void speak(){print“说中文”}}
class CEnglish : public CPerson{ void speak(){print“说英语”}}
//一群人说话 存储人  不能用CEnglish ary[4];
CPerson * ary[2]
CChinese chi;
CEnglish eng;
ary[1] = &chi;
ary[2] = &eng;
ary[i] -> speak();
// 这样调用speak 都说人话
// - virtual 虚函数  在CPerson里的speak()前加上 virtual就解决了
// - virtual void speak(){}
```

深入理解虚函数的原理

- 是否加virtual
  三个类占用的内存之前是4个字节(int mType) 
  加了virtual变成了8个字节 
  多了四个字节哪来的？
  - 多存储了一个指针指向函数
- 间接调用（先查虚表地址，再查虚表中的虚函数指针）
  指针->虚表指针->函数
- 增加了virtual关键字的对象头部4个字节是虚表地址（某些情况下单继承）

函数覆盖
- virtual 重写函数
  只修改大括号内的函数具体实现

- 为什么函数名、参数、返回值要一样？？
  ```cpp
  父亲 virtual void speak(){}
  儿子 virtual void speak(int n){} 
  ```
  导致儿子的虚表里有两个虚函数，相当于儿子中重载了这个函数，不再覆盖。
- 虚表大小的确定
  - 由编译器在编译时期确定
  - 在运行时内存中没有个数的表示
  - 虚表中虚函数的顺序：父亲中声明的顺序确定了儿子的顺序。（儿子去父亲的位置覆盖）
  - 重写后，在自己的虚表里覆盖
- 子类中新增父亲没有的虚函数 添加在父亲所有虚函数后面


虚函数的直接调用与间接调用
- 直接调用（编译时确定调用谁）
  根据函数名称直接调用该函数
- 间接调用（在运行时去确定调用谁）
通过查找对象的虚表下标来调用函数的方法 //
指针指谁就查谁的表
  - 使用对象指针   p->foo()
  - 使用对象的引用
- pChs->CChinese::speak() 直接调用，没有查表


函数覆盖重载隐藏的调用技巧
- 3.14和3.14f    3.14默认为double类型  3.14f为float
- 一旦在某一个类域中找到该函数，就会停止向上查找（函数隐藏规则）
- 子类重写了函数就会覆盖

// TODO
函数覆盖重载隐藏的调用技巧2
- 调用者类型决定查找的起点
  - 在调用的类中，查找同名函数
    - 如果没有，往上一层找，均找不到，报错（未定义）
    - 有，停止往上一层找（函数隐藏）
- 在当前可见域里找最佳函数（函数重载）（多个报二义性）
  - 唯一的最佳函数不是虚函数，则直接调用
  - 唯一最佳函数为虚函数，判断调用者是否时指针或引用
    - 是，间接调用（函数覆盖规则）（谁的类型调用谁）
    - 否，直接调用 （谁的类型调用谁）


###	常成员 const
函数
```cpp
int CInterger::GetNumber()
{ return m_nNumber;}
不想修改成员函数  const修饰函数
int CInterger::GetNumber() const{}  // 在函数内不能修改数据成员一般修饰get函数
```
- 原理
`"无法从const CInterger 转换为 CInterger"`
普通类成员函数this指针类型：T \*const（表示指针本身不能被修改）
常成员函数this指针类型 const T\* const 指针不能改，指针内容不能改
int CInterger::GetNumber() const{} 中
this指针类型为const CInterger\* const, 可将其强转为 CInterger* const 之后就可以修改成员函数
- 成员声明之前加上mutabel 可以随便改
mutable int m_nNumber;


常成员变量及初始化列表
- private: const int m_nTest;  报错
要声明时初始化，编译器低级不会通过
- 不通过, 使用初始化列表 // 通常用来初始化有参数的成员对象
构造函数后加上：
  ```cpp
  CInterger():
      m_nTest(123), m_Number(2)
  {}
  ```
- 类的成员为一个类（无默认构造函数），可以使用初始化列表构造


###	静态成员 static
变量 
- public:
    static int m_nstatic;  
链接错误
  - 要单独的把实现写在类外(cpp里)
  int CInterger::m_nstatic;
- 默认用0对该成员进行初始化（int）
- static成员不占内存，取地址发现不同对象的静态成员地址是相同的。
静态成员变量是对象（同一个类）共用的。（跟类绑定）避免命名冲突。
（本质：带类域的全局变量）


函数
public：
    static void foo(){}
- 函数内不能用非静态成员变量，只能访问静态成员变量。
- 静态成员函数里无this指针，所以不能访问某一个对象的成员。
  - 本质：无this指针。带类域的全局函数。避免名称冲突


###	单例模式的原理及实现(维护性好)//对象只能有一个
```cpp
class Singleton{
public:
  ~Singleton(){}
  /*
  static Singleton * CreatObject(){ 
  //造成内存泄漏问题,得考虑对象内存的释放问题
  	if(m_pObject == nullptr)  m_pObject = new Singleton();
  	else return m_pObject;
  }
  */
  //应使用这种写法
  static Singleton &CreateObject(){ 
  //返回引用，防止用户在外面delete导致内存错误。
  	static Singleton obj;
  	return &obj;
  }
private:
	static Singleton * m_pObject;
  Singleton(){printf(“asda”);}
}
```

创建对象时得 Singleton & obj = createObject();
当用户使用 Singleton obj = createObject()时又失去了单例模式，得把这种没事禁用了


###	友元的实现及作用
某一个类有朋友
- 普通的友元函数
  ```cpp
  public：
      friend void foo();
  ```
  作用：朋友函数可以访问类的私有成员
- 友元成员函数
friend void B::foo()  // B的foo是A的朋友
- 友元类
class A{
public:
    friend B;
}
- 有什么用？破坏类的封装性，有时方便


###	虚析构函数
析构函数最好为虚函数
virtual ~CParent();
CTest 为 CParent的子类
- 这样会少一次儿子的析构 
CParent *p = new CTest;
delete p; 
因为
`delete p; <=> p->~CParent(); free(p)  `
- 加了virtual后，
`delete p; <=> p-> ~CTest(); free(p) `
//间接调用，查子类虚表，虚表指向test的析构函数

??TODO
###	虚函数的多态性（间接调用看虚表）
- 构造函数中不存在多态 this->foo() 指向当前类的foo()
进去构造时，会为当前对象赋值为当前类的虚表，所以没有多态
- 普通成员函数有多态  this->foo()
- 析构函数里面也没有多态 why？？？


纯虚函数与抽象类
作为父类，规范子类
- 父类只是为了子类提供接口，将其称为接口类：**规范对外使用的接口**
CAnimal{}
CFish : public CAnimal{}
创建一个鱼对象 CFish fish;
但是创建一个动物对象，生活中并没有确定存在的动物这一对象

引入纯虚函数
- virtual void Run() = 0 //纯虚函数，不需要实现代码，也代表该类不能实例化
- 纯虚函数没有实现，那么包含纯虚函数的类是抽象类，不能实例化
- 抽象类：一定要包含某个纯虚函数，并且没有被实现
- 特例：纯虚析构： virtual ~CAnimal() 得写实现


###	多重继承
```cpp
class CSofa{};
class CBed{};
class CSofaBed : public CSofa, public CBed{};
```
- 指针转换
CSofaBed sbObj;
CSofa * pSofa = &sbObj;
CBed * PBed = &sbObj;
两个指针指向地址不一定相同，指向各自的部分
- 使用组合(推荐)
```cpp
class CSofaBed2{
public:

private:
    CSofa m_sofa;
    CBed m_bed;
};
```

菱形继承与虚继承 // 尽量不用多重继承


###	运算符重载
```cpp
//两个字符串拼接
#include <string>
std::string str = “hello”;
str += “world”;
// char * psz = “hello” + “world”;
```
运算符重载本质是函数的调用
- 特点：
  - 不破坏原有的运算符功能
  - 运算符优先级不变
  - 使用方式不变
- 不能重载： .    ::   ?:   #


cin  cout
- cout 是 类ostream的对象 cin   istream
- endl 回车换行  函数名
- <<    运算符重载了很多基础数据类型和函数等

// TODO modify
编写字符串类
```cpp
class CMyString{
    public:
        CMystring& operator=(char * psz){
          strcpy(m_szBuf, psz);
        }
        friend std::ostrem& operator << (std::ostrem& out , CMystring *str);
    private:
        char m_szBuf[256];
};
//在类的外部写友元全局的运算符重载
std::ostrem& operator << (std::ostrem& out , CMystring *str){
    out << str.m_szBuf;
    return out;
}
int main(){
    CMystring str;
    str = “hello world!”;
    std::cout << str << 123 ;  //  不通过

}
```
- 类成员的运算符重载
CMystring& operator=(char * psz);
- 友元全局的运算符重载

// TODO modify
转换运算符
```cpp
class Cint{
public:
    Cint(int n){ m_int = n; }
    Cint ++operator(){   //前缀++写法
        return ++m_int;
    } 
    //后缀++的写法
    Cint operator++(int){ 
        return m_int++;
    }
    //new 是运算符
    void * operator new(size_t cb){ return new char[cb];  }
    //转换运算符
    operator int(){return m_int; }  // 之后就可以  objN = objN + 1; 
//当使用int才能使用的操作时，系统会将对象转换为int类型
private:
   int m_int;
};

```


###	模板
函数模板
- 函数重载里，区别主要是参数类型不同
- 把某些类型的关键字（int，double）看成变量
  ```cpp
  template<typename T> 
  T GetMin(T a, T b){return a>b?a:b;}
  ```
- 函数的隐式实例化（由编译器猜测类型）上面
- 显式实例化 GetMin<int>(1, 2.0f);
- 模板特例化 GetMin(“hello”, “world”);
`char* GetMin<char*>(char *a, char*b){};  用显式实例化调用`
- 函数在实例化后才会产生函数

类模板
将类里面的变量类型看成一个变量
```cpp
template <typename T, typename K>

private:
    T m_nInt;
    K m_nDouble;

CTest<int, double> t;
```
- 类模板必选显式实例化
- 放头文件里

###	异常处理
```cpp
try{
    int t = m/n;
    // 人为抛出异常
    throw 1；
}
catch(…){
    cout << “error”<< endl;
}
```
- 人为抛异常throw
throw “hello”;
- catch(...) 中... 表示接收任何异常
  catch(int n) 程序接收int类型异常
- 里面接不住，会向外一层抛出异常
- std::exception 是异常的总类
 
