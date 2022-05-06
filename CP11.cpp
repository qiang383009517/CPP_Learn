// CP11.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<string>
#include<assert.h>
#include<vector>
#include<map>
#include<list>
#include<functional>

using namespace std;

/* 1-原始字面量*/
void R_dm()
{
	string str1 = R"(D:\hello\world\test.text)";
	cout << str1 << endl;

	string str2 = R"hello(D:\hello\world\test.text)hello";
	cout << str2 << endl;

#if 0
	//string str3 = R"hello(D:\hello\world\test.text)world";//()两边内容必须一样
#endif

}

/*2-nullptr*/
/*c中NULL为(void*)0，c++中NULL为字面量0,重新引入了nullptr可与各种类型指针进行转换*/

void nullfunc(int *p) {
	cout << "void nullfunc(int *p)" << endl;
}
void nullfunc(int p) {
	cout << "void nullfunc(int p)" << endl;
}
void null_dm() {
	nullfunc(nullptr);//重载函数：nullptr转换为int *p
	nullfunc(12);
}

/*3constexpr const*/
/*constexpr修饰常量 cosnt修饰常量和只读变量*/
/*3-1 cosnt*/
void const_func(const int n) {
	const int m = 10;
	//int arry[n];//n为只读变量，不可用定义数组
	int arry[m]{ 1,2,3 };//m为常量，可定义数组

	int a1 = 2;
	int a2 = 3;
	const int& b = a1;//b为常量引用
	//b = a2;//err b为常量引用，只能绑定a1这块内存
	a1 = 10;
	cout << b << endl;//b绑定a1,a1可以变化

}
/*3-2 constexpr*/
/*修饰常量时,const和constexpr是等价的*/
/*修饰自定义的数据类型时无效struct class*/
//constexpr struct A {};
struct A {
	int id;
	int num;
};
void constexpr_dm01() {
	constexpr struct A a{1,2};
	constexpr int id = a.id;
	constexpr int num = a.num;
	//a.num += 10;  //a为常量类型，其内部num也是常量类型，不可修改
}

/*constexpr函数*/
/*必须要有返回值，且返回值必须是常量，不能出现非常量表达式之外的操作*/
class ConstDm {
	constexpr ConstDm():a(1),b(2){} //constexpr 修饰构造函数，只能使用初始化列表赋值成员变量，函数体为空
public:
	constexpr int func() {
		int a = 10;
		int b = 2;
		/*		
		for (int i = 0; i < 10; i++) {

		}*/

		return a + b;//返回值必须常量
	}
private:
	int a;
	int b;
};

/*constexpr修饰模板函数，满足要求为常量函数，不满足为普通函数*/
template<typename T>
constexpr T display(T t)
{
	return t;
}
void constexpr_dm02()
{
	display(12);//display常量函数
	struct A a{1,3};
	display(a);//a为变量，display普通函数
	constexpr struct A b{2,3};
	constexpr struct A c = display(b);//b为常量，dispaly为常量函数
}

/*4-auto*/
/*auto 必须初始化*/
/*auto 推导变量是引用或指针，则保留const volatile属性,否则不保留const volatile属性*/
void auto_dm01() {
	int tmp = 250;
	const auto a1 = tmp;//a1为 const int类型，auto=int
	//a1 = 10;err
	auto a2 = a1;//表达式非指针或引用，不保留const,a2为int 类型，auto=int 
	a2 = 10;
	cout << a2 << endl;
	const auto &a3 = tmp;//表达式有指针或引用，保留const，a3为const int ,auto=int
	//a3 = 5;//err 
	auto &a4 = a3;//表达式有引用，保留const,a4为const int，auto=const int
	//a4 = 3;//err
	cout << a4 << endl;
	auto a5 = a3;//表达式无指针或引用，不保留const，a5为int,auto=int
	a5 = 2;
	cout << a5 << endl;
}

/*auto的限制*/
/*不能用于函数参数*/
/*int func(auto a, auto b) {s
}*/
/*不能用于非静态成员变量的初始化*/
class Test {
	//auto a1 = 10;//a1 非静态成员
	//static auto b = 1;//静态非常量成员,不能再类内部初始化
	static const auto c = 1;//ok 静态常量成员可在类内部初始化
};
/*auto 不能用于定义数组*/
void auto_dm02() {
	int array[]{ 1,3,5,6 };
	//auto array2[]{ 2,3,4 };//err auto不能用于定义数组
	auto a = array;
	//auto b[] = array;
}

/*auto 不能用于模板参数推导*/
template<typename T>
struct B {
	T t;
};
void template_auto() {
	struct B<int> b { 10 };
	//struct B<auto> b2 = b;//auto 不能推导模板参数类型
}

/*5- decltype*/

//6 类的成员变量初始化：可以在类的内部就地初始化 非静态成员
//CPP11之前
struct Base{
	Base():a(10){}
	Base(int num):a(num){}
	int a;
	int b = 11;//err 不能 就地初始化,只能在构造函数中初始化
	//static int c = 0;//err 类的静态成员，只能在类的外部初始化
	//static const double d = 3.14;//err 静态常量成员，不是整形或枚举，则不能通过编译,只能在类的外部初始化
	//static const char * const e = "hello";//err 静态常量成员，不是整形或枚举，则不能通过编译,只能在类的外部初始化
	const static int f = 1;//ok 静态常量成员，是整形或枚举，可以编译
	
};
//CPP11 类成员初始化
class Base0{
	public:
	Base0(){};
	Base0(int x,int y ,int z):a(x),b(y),c(z){};
	int a = 1;//ok 可就地初始化，但会被列表初始化覆盖
	int b = 2;
	int c = 3;
	double arr[4]{1.1,1.2,1.3,1.4};
	string s{"hello world!\n"};
	static const int d = 8;
	//static const double g = 3.14;//err 
	static const double g;
};
const double Base0::g = 11;
void dm_base(void){
	cout<<"--------------------"<<endl;
	Base b0(12);
	cout<<b0.a<<endl;
	Base0 b1(11,22,33);
	cout<<b1.a<<" "<<b1.b<<" "<<b1.c<<endl;
	for(auto n:b1.arr){
		cout<<n<<endl;
	}
	Base0 b2;
	cout<<b2.a<<" "<<b2.b<<" "<<b2.c<<endl;
	cout<<Base0::d<<endl;
	cout<<Base0::g<<endl;
	cout<<b2.d<<endl;
	cout<<b1.g<<endl;
}

//CPP11:关键字final voerride
//final 修饰函数，只能修饰虚函数，作用：阻止子类重写父类的这个函数
class AA{
	virtual void test(){
		cout<<"AA class..."<<endl;
	}	
};
class AB:public AA{
	void test() final{//关键字 final
		cout<<"AB class..."<<endl;
	}
};
class AC:public AA{
	void test(){//err,不允许重写
		cout<<"AC class..."<<endl;
	}
};
//final 修饰类:则该类不再允许派生新类
class AD{};
class AE final:public AD{};
//class AF:public AE{};//ERR 不允许派生

//7.2 关键字:override 显示声明重写父类函数，可防止重写错误，保证重写的正确性
class Ba{
	virtual void test(){
		cout<<"class Ba"<<endl;
	}
};
class Bb:public Ba{
	void test() override{
		cout<<"class Bb"<<endl;
	}
};
class Bc:public Ba{
	void test(void) override{
		cout<<"class Bc"<<endl;
	}
};
//8 默认模板参数:优先使用推导出来的类型，没有推导出类型则使用默认类型
template<typename R=int,typename N>
R func1(N a){
	return a;
}
void dm08(void){
	auto r1 = func1<int>(520);//R:int N:int
	cout<<"r1:"<<r1<<endl;
	auto r2 = func1<double>(22.2);//R:double N:double
	cout<<"r2:"<<r2<<endl;
	auto r3 = func1<int>(32.2);//R:int N:double
	cout<<"r3:"<<r3<<endl;
	auto r4 = func1<char,double>(39.2);//R：char N:double
	cout<<"r4:"<<r4<<endl;
}
//9 数值与字符串的转换 
//9.1数值转换为字符串 to_string()
//9.2字符串转换为数值 stoi()
void dm9_0(void){
	//to_string()
	string s = to_string(12345);
	cout<<s<<endl;

	//stroi
	string str = "0123abc3456";
	size_t pos = 0;
	int n = stoi(str,&pos);
	cout<<n<<" "<<pos<<endl;
}
//10 断言 assert() 静态断言static_assert()
void dm10(void){
	//assert(expression) :参数是一个表达式，为true时继续执行，false则结束运行
	int a = 12;
	assert(a>10);
	cout<<"assert true"<<endl;
	//static_assert(expression,"false description") //编译阶段即可检查的断言,参数1表达式真假值，false则输出参数2的内容，并终止
	static_assert(sizeof(long)==4,"不是32位系统");
	cout<<"32 os"<<endl;
}
//11 noexcept关键字
//11.1异常 try{}catch(){} 栈解旋(抛出异常前，所有构造的对象均被反顺序自动析构)
//异常接口:1-throw(指定异常类型:int,double) 2-不指定接口:-throw 任意类型 3-throw():不抛出异常
struct MyException{
	MyException(string s):msg(s){}
	string msg;
};
double divisionMethod(int a,int b)throw(MyException,int);//抛出Int异常
double divisionMethod1(int a,int b);//可抛出任意异常
double divisionMethod2(int a,int b)throw();//不抛出异常
double divisionMethod3(int a,int b)noexcept;//CPP11 不抛出异常
//12 关键字，auto
//12.1 auto 的const volatile属性---》左侧初始化的变量为指针或引用类型，保留右侧const volatile属性
void dm12(void){
	int tmp = 50;
	const auto a = tmp;
	//a = 12; //err 
	auto a2 = a;//auto=int,推导auto不保留const属性,auto=int
	a2 = 1;
	const auto &a3 = tmp;//引用类型，auto=const int
	//a3 = 15;//err 有const属性
	auto &a4 = a3;//auto = const int ，引用保留const属性
	//a4 = a2;//err a4有const属性
} 
//12.2 auto的使用限制
class AUTO{
	public:
	//auto a = 1;//err
	//static auto b = 10;//err 静态成员不可再类内部初始化//cpp17中可以
	static const auto c = 3;//静态常量成员可以用auto 在类内部初始化
};

void dm12_1(void){
	//1 不能用类的非静态成员初始化
	//2 不能定义数组
	int arry[3]{1,3,5};
	auto brry = arry;
	//auto crry[3] = {3,2,1};//err
	//auto drry[]{2,2,1};//err
	//3 无法推导模板参数
	vector<int> ints;
	ints.push_back(1);
	ints.push_back(3);
	//vector<auto>ret = ints;//err
	auto ret = ints;//ok
	//12.3 auto的应用
	for(auto i : ret){
		cout<<i<<endl;
	}
	
}
//12.3 auto用于泛型编程
class M1{
	public:
	static int get(void){
		return 10;
	}
};
class M2{
	public:
	static string get(void){
		return "hello world";
	}
};
template<class A>
void GetM(void){
	auto ret = A::get();//auto根据A类型确定返回值
	cout<<ret<<endl;
}
void dm12_3(void){
	GetM<M1>();//A推导为M1,返回int类型
	GetM<M2>();//A推导为M2,返回string类型
}

//13 decltype(表达式)
class Ma{
	public:
	int a = 123;
};
int func_int();
int& func_int_r();
int&& func_int_rr();
const int func_cint();
const int& func_cint_r();
const int&& func_cint_rr();
const Ma func_cMa();
void dm13(void){
	// 普通变量或类表达式->类型一致
	Ma ma;
	decltype(ma) mb;//decltype(Ma)
	int n;
	decltype(n) m =1;//decltype(int)
	//函数则推导类型 = 函数返回值类型
	decltype(func_int()) f1 = 1;//decltype->int
	decltype(func_int_r()) fr = f1;//decltype->int &
	decltype(func_int_rr()) frr = 3;//decltype->int &&
	decltype(func_cint()) fc = 5;//decltype->int ，const属性丢失，原因：func_cint()返回纯右值不携带const volatile属性，类除外
	fc = 12;
	decltype(func_cint_r())fcr = f1;//decltype->const int & 返回值类型不是纯右值
	//fcr = 2;
	decltype(func_cint_rr())fcrr = 12;//decltype->const int &&
	//fcrr = 1;
	decltype(func_cMa()) cma = Ma();//

	//表达式被”()“装饰或是左值，则加上"引用"特性
	const Ma a;
	decltype(a.a) a0;//int
	decltype((a.a)) b = 3;//a0;//int &
	cout<<&a<<" "<<&b<<endl;
	int m1 =0,n1=0;
	decltype(m1+n1)c = 132;//int
	decltype(n1 = m1+n1)d = c;//int & 得到左值

}
//decltype的应用，确定迭代器类型
template<typename T>
class Mm{
	public:
	decltype(T().begin()) iter;//根据T类型推导iter的迭代器类型
	void func(T& C){
		for(iter = C.begin();iter!=C.end();iter++){
			cout<<*iter<<" ";
		}
		cout<<endl;
	}
};
void dm13_1(void){
	const list<int>  a{1,3,5,7,9};
	Mm<const list<int>> ca;
	ca.func(a);
}
//decltype返回值后置推导
int & test(int &i){
	return i;
}
double test(double& i){
	return i;
}
template<typename T>
auto myfunc(T& t)->decltype(test(t))
{
	return test(t);
}
void dm13_2(void){
	int x = 520;
	auto z = myfunc(x);//auto->int
	cout<<z<<endl;
	double y = 13.14;
	auto z2= myfunc(y);//auto->double
	cout<<z2<<endl;
}

//14 基于范围的for循环
template<typename T>
auto getRange(T& v){
	cout<<"getrange ok..."<<endl;
	return v;
}
void dm_14(void){
	vector<int> nums{1,2,3,4,5,6};
	for(auto& num : getRange(nums)){//:getRange()只会执行一次，得到遍历对象，和遍历范围
		cout<<num<<" ";
	}
	cout<<endl;

	map<string,int> msi{{"c",1},{"cpp",2},{"python",3},{"java",4}};
	for(auto& cell : msi){ //使用引用的方式可以改变遍历的元素
		cout<<cell.first.c_str()<<":"<<++cell.second<<" ";
	}
	cout<<endl;
}
//15 关键字 nullptr 可以隐士转换为其他类型指针
void fnl(char *p){
	cout<<"void fnl(char*p)"<<endl;
}
void fnl(int p){
	cout<<"void fnl(int p)"<<endl;
}
void dm15(void){
	int *a = nullptr;
	char* b = nullptr;
	fnl(520);
	fnl(nullptr);
}

// 16 lamada表达式 [capture](params)option->return {body;};
//capture:[]不捕获,[&]按引用捕获,[=]按值捕获，[=,&foo]按引用捕获foo，其他按值捕获，[bar]按值捕获bar,[&bar]按引用捕获bar,[this]捕获当前类中this指针
//option:mutable 可以修改按值传递进来的拷贝
//lamada函数的本质：含operator()的类，即仿函数，可以使用std::function,std::bind接受
class lam_test{
public:
	lam_test(int x,int y){
		//auto f1 = [](){return num+x+y;};//err 
		f2 = [=](){return num+x+y;};
		f3 = [&](){return num+x+y;};
		//auto f4 = [this](){return num+x+y;};//err
		f5 = [this,x,y](){return num+x+y;};
		f6 = [this](int x,int y){return num+x+y;};
		f7 = [this,&x,&y](){return num+(x++)+(y++);};
		f8 =[this,&x](int y){return num+(++x)+y;};
	}
	int num = 1;
	std::function<int(int,int)> f6;
	std::function<int()>f2,f3,f5,f7;
	std::function<int(int)> f8;
};
void dm_16(void){
	lam_test lamd(1,2);
	int f2 = lamd.f2();
	int f3 = lamd.f3();
	int f5 = lamd.f5();
	int f6 = lamd.f6(11,12);
	int f7 = lamd.f7();
	int f8 = lamd.f8(11);
	cout<<"f2:"<<f2<<" f3:"<<f3<<" f5:"<<f5<<
	" f6:"<<f6<<" f7:"<<f7<<" f8:"<<f8<<endl;
	cout<<"f2:"<<lamd.f2()<<" f3:"<<lamd.f3()<<" f5:"<<lamd.f5()<<
	" f6:"<<lamd.f6(10,12)<<" f7:"<<lamd.f7()<<" f8:"<<lamd.f8(11)<<endl;
}
//17 常量表达式constexpr 修饰常量 在编译阶段即被确定
// const 可以修饰只读变量 和 常量 
//constexpr不和修饰 类或结构体的定义
/*constexpr*/ struct cstexpr{
	int a;
	int b;
};
void dm17_01(const int& n){
	constexpr int num = 3;
	int arry[num]{1,2,3};//ok，num是常量
	//int arry2[n];//err，n为只读变量
	int a1 = 510;
	int a2 = 221;
	const int &b1 = a1;
	//b1 = a2;//err b1为a1的常量引用，不能更改
	a1 = 111;
	cout<<b1<<endl;
	//常量的类
	constexpr cstexpr t{1,3};
	constexpr int ta = t.a;
	constexpr int tb = t.b;
	//t.b+=100;//err，常量t.b不可修改
	cout<<ta<<" "<<tb<<endl;
}
//17.1 constexpr 函数 和模板函数 和构造函数
//函数 (1)必须return 常量（2）必须先定义（3）不能使用复杂语句
//模板函数：推导的模板函数不满足常量表达式函数的要求，则被当作普通函数
typedef struct Person{
	const char* name;
	int age;
}Person;
template<typename T>
constexpr T tisplay(T t){
	return t;
}
//修饰构造函数,只能使用初始化列表，且函数体必须为空
class pep{
	public:
	constexpr pep(const char* name,int age):name(name),age(age){}
	const char * name;
	int age;
};
void dm17_02(void){
	Person lili {"lili",18};
	Person ali = tisplay(lili);//返回的是变量，普通函数
	constexpr int  a = tisplay(10);//常量表达式函数
	constexpr Person xili ={"xili",20};
	constexpr Person xli = tisplay(xili);//常量表达式函数
	cout<<xli.name<<" " <<xli.age<<endl;
	constexpr pep p1("xiaoming",22);
	cout<<p1.name<<" "<<p1.age<<endl;
}
//18委托构造函数  链式调用
class Weituo{
	public:
	Weituo(){};
	Weituo(int min):m_min(min){};
	Weituo(int min,int max):Weituo(min){m_max = max;};
	Weituo(int min,int max,int mid):Weituo(min,max){m_mid=mid;};
	private:
	int m_min;
	int m_max;
	int m_mid;
};
//18继承构造函数 using Base::Base 可在子类中使继承父类构造函数，甚至可以使用using Base::func，继承父类同名函数
class JC_Base{
	public:
	JC_Base(int i,double j):mi(i),mj(j){}
	JC_Base(int i,double j,string s):mi(i),mj(j),ms(s){}
	int mi;
	double mj;
	string ms;
	void show(int i){
		cout<<"base class:i"<<i<<endl;
	}
	void show(int i,string s){
		cout<<"base class:i"<<i<<" s:"<<s<<endl;
	}
};
class JC_Child:public JC_Base{
	public:
	using JC_Base::JC_Base;
	using JC_Base::show;
	void show(double j){
		cout<<"child class j:"<<j<<endl;
	}
};
void dm18(){
	//委托构造函数
	Weituo wt(1,2,3);
	//继承构造函数
	string str = "hello cpp";
	JC_Child child(1,2.1,str);
	child.show(1);
	string s = "hi";
	child.show(1,s);
	child.show(2.7);
}

//19 右值引用:不可取地址，纯右值：临时变量 原始字面量 lamada表达式；将亡值：T&&函数返回值,std::move()的返回值
class dm19_obj{
	public:
	dm19_obj():m_num(new int(100)){
		cout<<"dm19_obj:jerry"<<endl;
	}
	//拷贝构造函数，深拷贝
	dm19_obj(dm19_obj& a):m_num(new int(*a.m_num)){
		cout<<"dm19_obj:tom"<<endl;
	}
	//移动构造函数，浅拷贝
	dm19_obj(dm19_obj&& a):m_num(a.m_num){
		a.m_num = nullptr;//原资源被舍弃
		cout<<"yidong dm19_obj:helo"<<endl;
	}
	~dm19_obj(){
		if(m_num){
			delete m_num;
			m_num = nullptr;			
		}
		cout<<"destruct class..."<<endl;
	}
	int * m_num;
};
dm19_obj getobj(){
	return dm19_obj();//返回匿名对象，可直接初始化变量,该匿名对象转正，不会析构
}
dm19_obj getobj2(){
	dm19_obj t;
	return t;//返回临时对象，可直接初始化变量，或移动构造来初始化变量，随后临时对象析构
}
//19.2 右值引用的移动语义，将资源通过浅拷贝转移到另一个对象
void dm19(void){

	int &&a1 = 10;//ok，但a1为有名变量，使用时会被看作左值
	//int &&a2 =a1;//err,左值不能初始化右值
	//dm19_obj& dm = getobj();//err,右值不能初始化左值
	dm19_obj&& dm2 = getobj();//ok,getobj()返回的临时对象，即将亡值的资源被移动到了dm2,延长了该内存的声明周期
	const dm19_obj&& dm3 = getobj();//ok	
}

//19.3 移动构造函数，避免大量的内存申请
void dm19_2(void){
	dm19_obj obj = getobj2();//getobj2()返回临时对象来初始化obj,则调用移动构造函数
	dm19_obj obj2 = dm19_obj(std::move(obj));
	cout<<"obj.num "<<*obj2.m_num<<endl;
}
//19.4 T&& atuo&& 未定型的右值引用推导:只有纯右值推导的T&& 和auto&& 类型为右值引用，其他均推导为左值引用；另外const T&& 为右值引用类型，非未定型
template<typename T>
void f(T&& p){cout<<"r-p"<<endl;}
void f1(const T&& p){cout<<"const r-p"<<endl;}

void dm19_3(void){
	//1--右值引用 templte推导
	f(10);//传入右值-》T&& p推导为右值引用
	int s = 10;
	f(s);//传入左值（非右值）-》T&& p推导为左值引用
	//f1(x);//err,说明：const T&& 不是未定型，是右值引用
	//2--变量
	int x = 520,y = 1314;
	auto&& v1 = x;//x非右值，-》左值引用
	auto&& v2 = 520;//右值=》右值引用
	decltype(x)&& v3  = y;//err=>int && v3 = y,左值不能初始化右值
	const int && s = 100;
	auto&& v4 = s;//s：常量右值引用（非右值）=-》v4：常量左值引用
}
//20 转移和完美转发
//20.1 move 将左值转换为右值
//20.2 forward<T>(t) 转发的t是左值或是右值，取决与T,只有T为左值引用时-->t为左值，其他情况-->t为右值
void dm20(){
	//move
	vector<string> s;
	s.push_back("hello");
	s.push_back("world");
	auto && s2 = move(s);//不需要拷贝
	auto & s3=  s;//需要拷贝，效率低
	//forward<T>
}

int main()
{
	R_dm();
	null_dm();
	const_func(1);          
	dm_base();
	//默认模板参数
	dm08();
	//数值与字符串转换
	dm9_0();
	//assert() static_assert()
	dm10();
	//12 auto
	dm12();
	dm12_1();
	dm12_3();
	//13 decltype
	dm13();
	dm13_1();
	dm13_2();
	//14 range for
	dm_14();
	//15 关键字nullptr
	dm15();
	//lamada
	dm_16();
	//constexpr
	dm17_01(3);
	dm17_02();
	//dm18
	dm18();
	//右值引用
	dm19();
	dm19_2();
	dm19_3();
}


