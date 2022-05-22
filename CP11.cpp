// CP11.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include<string>
#include<assert.h>
#include<vector>
#include<map>
#include<list>
#include<functional>
#include<type_traits>
#include<memory>
#include<string.h>

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
	int x = 1, y = 1;
	int num = 1;
	
	std::function<int(int,int)> f6;
	std::function<int()>f2,f3,f5,f7;
	std::function<int(int)> f8;
};
void dm_16(void){
	int x = 1, y = 2;
	lam_test lamd(1,2);
	int f2 = lamd.f2();
	int f3 = lamd.f3();
	int f5 = lamd.f5();
	int f6 = lamd.f6(11, 12);
	//int f7 = lamd.f7();
	//int f8 = lamd.f8(11);
	cout<<"f2:"<<f2<<" f3:"<<f3<<" f5:"<<f5<<
	" f6:"<<f6/*<<" f7:"<<f7 << " f8:" << f8 << endl*/;
	/*	cout<<"f2:"<<lamd.f2()<<" f3:"<<lamd.f3()<<" f5:"<<lamd.f5()<<
	" f6:"<<lamd.f6(10,12)<<" f7:"<<lamd.f7()<<" f8:"<<lamd.f8(11)<<endl;*/
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
template<typename T>
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
	//decltype(x)&& v3  = y;//err=>int && v3 = y,左值不能初始化右值
	const int && s1 = 100;
	auto&& v4 = s1;//s：常量右值引用（非右值）=-》v4：常量左值引用
}
//20 转移和完美转发
//20.1 move 将左值转换为右值
//20.2 forward<T>(t) 转发的t是左值或是右值，取决与T,只有T为左值引用时-->t为左值，其他情况-->t为右值
template<typename T>
void printValue(T& t){
	cout<<"l-value:"<<t<<endl;
}
template<typename T>
void printValue(T&& t){
	cout<<"r-value:"<<t<<endl;
}
template<typename T>
void testForward(T &&v){
	printValue(v);//v 有名左值-》T& t-》l-value
	printValue(move(v));//转为右值-》T&& T->r-value
	printValue(std::forward<T>(v));//T:如果为左值引用，则完美转发左值v->T& t->l-value   //T:如果为右值引用，则完美转发右值v->T&& t->r-value
	cout<<endl;
}
void dm20(){
	//move
	vector<string> s;
	s.push_back("hello");
	s.push_back("world");
	auto && s2 = move(s);//不需要拷贝
	auto & s3=  s;//需要拷贝，效率低
	//forward<T>
	testForward(520);//右值传入->v：模板参数：右值引用
	int n = 1314;
	testForward(n);//左值传入v->推导模板参数为左值一引用(注意：模板参数如果为T& 则T只能推导为类型，没有引用属性)
	testForward(std::forward<int>(n));//n完美转发为右值，-》模板参数T:右值引用
	testForward(std::forward<int&>(n));//n完美转发为左值-》模板参数T:左值引用
	testForward(std::forward<int&&>(n));//n完美转发为右值，-》模板参数T:右值引用

}

//21 列表初始化 聚合体 非聚合体 initial_list<>
class CL21 {
public:
	CL21(int n) { cout << "CL21(int n)..." << endl; }
private:
	CL21(CL21& cl) { cout << "CL21(CL21& cl)..." << endl; }
};
CL21 retCL21(int n) {
	//CL21 ret{ n };
	//return ret;//err，因为拷贝构造为私有函数
	return { n };
}

class CL21_0 {
public:
	CL21_0(int n):m_num(n){}
	CL21_0(initializer_list<string> names) {
		for (auto it = names.begin(); it != names.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
private:
	int m_num;
	vector<string> names;
protected:
	int m_m;
public:
	virtual int getm() {
		return m_m;
	}
};

void travel(std::initializer_list<int> a) {
	for (auto it = a.begin(); it != a.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
void dm21(void) {
	//类和变量的列表初始化
	int arry[3]{ 1,3,4 };
	int ar2[3] = { 3,5,6 };
	int n = { 2 }; int n2{ 1 };
	CL21 cl01 = { 12 };
	CL21 cl02{ 1 };
	//CL21 cl03 = 20;//调用构造，不是私有拷贝构造{20}
	//new 内存分配初始化
	int* a = new int{ 100 };
	int* pary = new int[3]{ 1,3,5 };

	//函数返回值-列表初始化
	retCL21(10);

	//聚合体：普通数组；自定义类型：无自定义的构造函数/无私有成员或保护的非静态成员/静态成员不能用列表初始化/无基类/无虚函数/
	CL21_0 c1{ 20 };
	//非聚合体：不满足聚合体条件的都是非聚合体，注意：当一个类内部含有非聚合体类型时，它也可能是一个聚合体
	//initiallizer_list<T> 作为函数参数
	travel({ 1,3,5,7,9 });
	initializer_list<int> b{ 2,4,5,6,7 };
	travel(b);

	//initializer_list<T> 作为构造函数参数与
	CL21_0({ "小明","huahua","dage","沈腾","jaling" });
}
//22 关键字using  和typedef, 相比较using 使用更方便
typedef int(*fc)(int ,double);
using fc2 = int(*)(int,double);
//模板别名
// template<typename T>
// typedef map<int,T> type;//err
template<typename T>
using m_map = map<int,T>;
void dm22(void){
	//type a;//err
	m_map<string> a;
	a.insert({1,"hello"});
	a.insert(pair<int,string>(2,"world"));
	a.insert(make_pair<int,string>(3,"cpp"));

	for(auto && cell : a){
		cout<<cell.first<<" "<<cell.second<<endl;
	}
}
// 23 仿函数 可调用对象包装器std::function 绑定器std::bind
//23.1 可调用对象
//函数指针
int (*func)(int,double) = [](int x,double y)->int{};
using fc1 = void(*)(int,string);
struct dm23_1{
	//成员函数指针或类成员指针
	static void prints(int a,string b){
		cout<<a<<" "<<b<<endl;
	}
	int m_num;
	//operator()成员的类对象,重载()
	void operator()(string s){
		cout<<s<<endl;
	}
	//可被转换为函数指针的类对象
	operator fc1(){
		return this->prints;//prints是类的静态成员函数
	}	
};
struct dm23_2{
	void print(int a,string b){
		cout<<a<<" "<<b<<endl;
	}
};
void dm23_1(void){
	//operator()仿函数
	struct dm23_1 dm23;
	dm23("cpp11_hello");
	//转换为函数指针的类对象
	dm23(1,"hello_cpp11");
	//定义成员函数指针指向类成员函数，并调用
	dm23_2 dm;
	void (dm23_2::*fptr)(int,string) = &dm23_2::print;
	(dm.*fptr)(10,"hahaha");
	//定义类成员指针指向类成员，并调用
	int dm23_1::*nptr = &dm23_1::m_num;
	dm23.*nptr = 11;
	cout<<"nptr:"<<dm23.m_num<<endl;
}
//23.2 可调用对象包装器
void dm23_funtion(){
	//std::function接收函数指针
	std::function<void(int,double)> f1 = func;
	f1(1,3.5);
	//接收静态成员函数
	std::function<void(int,string)> f2 = dm23_1::prints;
	f2(3,"haihaiah");
	//接收仿函数
	struct dm23_1 dm231;
	std::function<void(string)> f3 = dm231;//dm231相当于一个仿函数的指针
	dm231("jjjjjjj");
	//接收类转换为函数指针
	std::function<void(int,string)> f4 = dm231;//dm231相当于一个函数指针
	f4(12,"121212");
}
//23.3 绑定器
void output(int x,int y){
	cout<<x<<" "<<y<<endl;
}
class dm23_3{
	public:
	void output(int x,int y){
		cout<<x<<" "<<y<<endl;
	}
	int m_n;
};
void dm23_bind(){
	//绑定可调用对象和其参数
	bind(output,placeholders::_1,2)(3);
	auto f0 = bind(output,placeholders::_1,2);
	f0(12);
	std::function<void(int)> f1 = std::bind(output,placeholders::_1,2);//placeholders::_1绑定第1个参数 作为第一个参数传入output，2作为第二个参数传入output
	f1(10);///placeholders::_1绑定10
	std::function<void(int,int)>f2 = std::bind(output,placeholders::_2,345);//placeholders::_2绑定第二个参数传入output的第一个参数
	f2(0,123);//placeholders::_2 绑定123；0不使用
	//类对象成员的绑定
	dm23_3 dm3;
	std::function<void(int,int)> f3 = std::bind(&dm23_3::output,&dm3,placeholders::_2,placeholders::_1);
	f3(321,123);//placeholders::_2 绑定123 传入dm3.output函数的第一个参数;placeholders::_1 绑定321传入dm3.output的第二个参数
	std::function<int&(void)> f4 = std::bind(&dm23_3::m_n,&dm3);//bind 绑定对象成员变量
	f4()  = 100;
	cout<<"dm3.m_n:"<<dm3.m_n<<endl;
}

//24 POD类型 plian old data 普通的数据类型==》1 可以安全的进行memset和memcpy;2 安全的在C和C++间进行操作;3 保证静态初始化时安全有效
//24.1 平凡的类或结构体:1.拥有默认的构造函数 和析构函数(什么都不干)或用=defalt来显示的平凡化
					//2.拥有平凡的拷贝构造函数和移动构造函数
					//3.拥有平凡的拷贝复制运算符和移动赋值运算符
					//4.不含虚函数或基类
//24.2 标准的布局类型的类或结构体:1.所有非静态成员有相同的访问权限(都是private protected public)
							//2.基类中有静态成员，派生类中有非静态成员 -- 或者-- 基类中有非静态成员，派生类中无非静态成员
							//3.子类中的第一个非静态成员与基类类型不同 //4.没有虚函数或虚基类
// 对平凡的判断std::is_trival 模板类  对标准布局的判断std::is_standard_layout 模板类
class dm24_a{};
class dm24_b{dm24_b(){}};//false 不能有自定义的构造函数
class dm24_c:dm24_b{};//false 不能有基类
class dm24_d{virtual void fn();};//false 不含虚函数
class dm24_e:virtual public dm24_a{};//不含基类

struct dm24_A{};
struct dm24_B:dm24_A{int i;};
struct dm24_C{public:int a;private:int b;};//flase 访问权限不一致
struct dm24_D1{static int i;};
struct dm24_D2{int i;};
struct dm24_E1{static int i;};
struct dm24_E2{int i;};
struct dm24_D:public dm24_D1,public dm24_E1{int a;};
struct dm24_E:public dm24_D1,public dm24_E2{int a;};//false 基类和子类同时出现非静态成员变量 
struct dm24_F:public dm24_D2,public dm24_E2{static int a;};//false 多继承的基类中同时出现了非静态成员
struct dm24_G:public dm24_A{int foo;dm24_A a;};//根据编译器不同可能结果不同
struct dm24_H:public dm24_A{dm24_A a;int foo;};//false 子类第一个非静态成员类型和基类类型不能相同

void dm24(){
	//std::is_trival
	cout<<"a:"<<is_trivial<dm24_a>::value<<endl;
	cout<<"b:"<<is_trivial<dm24_b>::value<<endl;
	cout<<"c:"<<is_trivial<dm24_c>::value<<endl;
	cout<<"d:"<<is_trivial<dm24_d>::value<<endl;
	cout<<"e:"<<is_trivial<dm24_e>::value<<endl;
	//std::is_standard_layout
	cout<<"A:"<<is_standard_layout<dm24_A>::value<<endl;
	cout<<"B"<<is_standard_layout<dm24_B>::value<<endl;
	cout<<"C"<<is_standard_layout<dm24_C>::value<<endl;
    cout << "D: " << is_standard_layout<dm24_D>::value << endl;
	cout << "H: " << is_standard_layout<dm24_D1>::value << endl;
	cout << "H: " << is_standard_layout<dm24_D2>::value << endl;
    cout << "D1: " << is_standard_layout<dm24_E>::value << endl;
    cout << "E: " << is_standard_layout<dm24_F>::value << endl;
    cout << "F: " << is_standard_layout<dm24_G>::value << endl;
    cout << "G: " << is_standard_layout<dm24_H>::value << endl;
}

//25 关键字 =default(让类或结构体保持POD类型) =delete
class dm25{
	public:
	dm25()=default;//默认无参构造
	dm25(const dm25& obj)=default;//指定默认拷贝构造
	dm25(dm25&& obj)=default;//指定移动构造为默认函数
	dm25& operator=(const dm25& obj)=default;//指定复制赋值操作符为默认函数
	dm25& operator=(dm25&& obj)=default;//指定移动赋值操作符为默认函数
	~dm25()=default;//指定析构函数为默认函数
	//25.2 delete
	dm25(int a)=delete;//不再被使用
	int m;
};
//或者default放在外边，同一类型的构造函数只能有一个时默认的
// dm25::dm25()=default;
// dm25::dm25()=default;//默认无参构造
// dm25::dm25(const dm25& obj)=default;//指定默认拷贝构造
// dm25::dm25(dm25&& obj)=default;//指定移动构造为默认函数
// dm25&  dm25::operator=(const dm25& obj)=default;//指定复制赋值操作符为默认函数
// dm25& dm25::operator=(dm25&& obj)=default;//指定移动赋值操作符为默认函数
void dm25_1(){
	dm25 m25;
	//dm25 mm(10);
}

//26 关键字friend的拓展
//26.1类友元
class dm26_tom;
using tom = dm26_tom;
class dm26_jack{
	//friend class tom;//cpp98
	friend tom;//cpp11
	string name = "jack";//默认私有
	void print(){
		cout<<"name="<<name<<endl;
	}
};
class dm26_tom{
	public:
	void print(){
		//访问友元类的私有成员
		cout<<"my friend name is "<<jack.name<<endl;
		cout<<"my friend's jack print:"<<endl;
		jack.print();
	}
	private:
	string name="tom";
	dm26_jack jack;
};
void dm26_1(){
	tom tm;
	tm.print();
}

//26.2 模板友元类
template<typename T>
class Rct{
	friend T;
	public:
	Rct(int w,int h):width(w),height(h){}
	private:
	int width;
	int height;
};
template<typename T>
class Circle{
	friend T;
	public:
	Circle(int r):radius(r){}
	private:
	int radius;
};
class Verify{
	public:
	void verify_rct(int w,int h,Rct<Verify>& rct){//声明一个Verify友元类的Rct对象
		if(rct.width>w&&rct.height>h){
			cout<<"矩形满足条件"<<endl;
		}else{
			cout<<"不满足矩形条件"<<endl;
		}
	}
	void verify_circle(int r,Circle<Verify>& circle){//声明一个Verify友元类的Circle对象
		if(circle.radius>r){
			cout<<"满足条件的圆形"<<endl;
		}else{
			cout<<"不满足圆形条件"<<endl;
		}
	}
};
void dm26_2(){
	Verify vf;
	Rct<Verify> rct(10,20);//创建一个Verify友元类的Rct对象
	Circle<Verify> cl(10);//创建一个Verify友元类的Circle对象
	vf.verify_rct(3,5,rct);
	vf.verify_circle(12,cl);
}

//27 强枚举类型 namespace::enum
//cpp11:enum class ... //cpp11之前:enum Colors{} 是全局可见的
enum class Colors:char{red,green,blue};// Colors::red 默认类型为Int，这里显示的指定底层类型为char
enum class China{shanghai,beijing,najing};
enum class Japan:char{dongding,daban,fudao};//char 只占1个字节
enum Goods:char{yumi,xiaomai,dami};//cpp11 兼容
void dm27(){
	//int m = shanghai;//err 强枚举类型不能直接使用，必须加上作用域China, 其次 强枚举类型不会进行隐式转换
	//int n = China::beijing;//err 强枚举类型不可隐式转换
	if((int)China::najing > 1){
		cout<<"najing..."<<endl;
	}
	cout<<"size China:"<<sizeof(China::shanghai)<<endl;
	cout<<"size Japan:"<<sizeof(Japan::dongding)<<endl;
	Goods ym = yumi;//ok
	Goods xm = Goods::xiaomai;//ok
}

//28 非受限联合体 任何非引用类型都可以作为非受限联合体的成员
//28_1 静态成员
union Test28{
	int age;
	int id;
	//int& tmp = age;//err 不可有引用成员
	static char c;
	static int print() {//静态成员函数只能访问静态成员
		cout << "value" << c << endl;
		return 0;
	}
};
//28_2 POD成员

char Test28::c = 'c';
void dm28_1() {
	Test28 t;
	Test28 t1;
	t.c = 'b';
	t1.c = 'c';
	t1.age = 10;
	cout << "t1.c" << t1.c << endl;
	cout << "t.c" << t.c << endl;
	cout << "t1.age" << t1.age << endl;
	cout << "t1.id" << t1.id << endl;
	t.print();
	t1.print();

}
//28_2 非POD类型成员（非受限联合体会删除默认构造系列函数，需自定义构造系列函数） POD类型用在非受限联合体时要placement_new即：calssname *ptr = new(内存地址)classname
/*
union Student {
	int id;
	string name;//POD类型，需要放置new
};
*/
class Base28 {
public:
	void setText(string str) {
		notes = str;
	}
	void print() {
		cout << "Base notes:" << notes << endl;
	}
private:
	string notes;
};

union Student {
	Student() {
		new(&name)string;
	}
	~Student(){}
	int id;
	Base28 tmp;
	string name;
};
//28_3 匿名非受限联合体
class Person28 {
public:
	Person28(int i) :id(i) {}
	Person28(string nm):name(nm){}
	~Person28(){}
	void setid(int n) {
		id = n;
		cout << &id << endl;
	}
	void print() {
		cout << "id:" << id << endl;
		cout << &id << endl;
		cout << "name:" << name << endl;
		cout << &name << endl;
	}
private:
	union {
		int id;
		string name;
	};
};
void dm28_2() {
	Student s;
	//s.id = 1000;
	s.name = "alpha";
	s.tmp.setText("我要成为最帅的男银");
	s.tmp.print();
	//s.id = 100;
	cout << "Student name:" << s.name << endl;
	cout << &s.name << endl;
	cout << "Student id:" << s.id << endl;
	cout << &s.id << endl;
	string name = "beta";
	Person28 p(name);
	p.setid(100000);
	p.print();
}
//29 智能指针
//29_1 共享智能指针 std::shared_ptr
//std::shared_ptr 1 构造初始化 拷贝和移动构造 2 std::make_shared初始化函数 
class Test29{
	public:
	Test29(){cout<<"construct test29"<<endl;}
	Test29(int i){cout<<"construct test29 ,x="<<i<<endl;}
	Test29(string s){cout<<"construct test29,str="<<s<<endl;}
	~Test29(){cout<<"destruct test29"<<endl;}
};

//智能指针删除器,std::default_delete<T>()
template<typename T>
shared_ptr<T> make_shared_arry(size_t size){
	return shared_ptr<T>(new T[size],std::default_delete<T[]>());
}

void dm29_1(){
	//1 初始化 shared_ptr<typename T>
	shared_ptr<int> ptr1(new int(100));
	cout<<"ptr1计数:"<<ptr1.use_count()<<endl;
	shared_ptr<char>ptr2(new char[12]);
	cout<<"ptr2计数:"<<ptr2.use_count()<<endl;
	//创建智能指针，不指向任何内存
	shared_ptr<int> ptr3;
	cout<<"ptr3计数:"<<ptr3.use_count()<<endl;//计数为0
	//创建智能指针，初始化为空
	shared_ptr<int>ptr4(nullptr);
	cout<<"ptr4计数:"<<ptr4.use_count()<<endl;// 指针计数为0

	//2 原始指针不能多次初始化不同智能指针
	int *p = new int(10);
	shared_ptr<int> p1(p);
	shared_ptr<int> p2(p);//p 不能再次初始化智能指针，编译不保存，运行会报错

	//3 拷贝构造初始化 移动构造初始化
	shared_ptr<int> ptr01(new int(12));
	cout<<"ptr01.use_count()="<<ptr01.use_count()<<endl;
	shared_ptr<int> ptr02(ptr01);//拷贝构造初始化 计数+1
	cout<<"ptr02.use_count="<<ptr02.use_count()<<endl;
	shared_ptr<int> ptr03(move(ptr02));//移动构造初始化，计数不变
	cout<<"ptr03.use_count="<<ptr03.use_count()<<endl;
	shared_ptr<int> ptr04 = ptr03;//拷贝构造初始化，计数+1
	cout<<"ptr04.use_count="<<ptr04.use_count()<<endl;
	shared_ptr<int> ptr05 = std::move(ptr04);//移动构造初始化，计数不变
	cout<<"ptr05.sue_count="<<ptr05.use_count()<<endl;

	//4 shared_ptr<T> make_shared(Args&&... args)函数初始化 
	shared_ptr<int> p01 = make_shared<int>(18);
	cout<<"p01.count="<<p01.use_count()<<endl;
	shared_ptr<int> p02 = p01;
	cout<<"p02.count="<<p02.use_count()<<endl;

	shared_ptr<Test29> pt1 = make_shared<Test29>();
	cout<<"pt1.count="<<pt1.use_count()<<endl; //默认构造 计数+1
	shared_ptr<Test29> pt2 = make_shared<Test29>(123);//123 传入Test构造参数
	cout<<"pt2.count="<<pt2.use_count()<<endl;
	shared_ptr<Test29> pt3 = make_shared<Test29>("hello test");//参数直接传给Test构造
	cout<<"pt3.count="<<pt3.use_count()<<endl;
	auto pt4 = pt3;
	auto pt5 = pt3;
	cout<<"pt5.count="<<pt4.use_count()<<endl;
	cout<<"----------reset----------------"<<endl;
	//5 std::shared_ptr::reset方法
	pt3.reset();//pt3释放，计数-1
	cout<<"pt4.count="<<pt4.use_count()<<endl;

	pt5.reset(new Test29(132));//reset 重置pt2
	cout<<"pt5.count="<<pt5.use_count()<<endl;
	cout<<"pt4.count="<<pt4.use_count()<<endl;

	//6 获取原始指针 T* get()
	shared_ptr<int> pg1(new int(211));
	*pg1 = 121;
	cout<<"pg1->"<<*pg1.get()<<" "<<"*pg1="<<*pg1<<endl;

	shared_ptr<char>pc(new char[32]);
	char* add = pc.get();
	memset(add,0,32);
	strcpy(add,"hello pc.get()");
	cout<<"pc:"<<pc<<" "<<"add:"<<add<<endl;

	//7 指定删除器，shared_ptr默认删除器不支持数组对象，需要自己指定或使用std::default_delete<T>()
	//7.1 匿名函数的删除器
	shared_ptr<int> pi(new int(103),[](int *p){cout<<"delete p"<<endl;delete p;});
	shared_ptr<int> pary(new int[10],[](int* p){cout<<"delete p[]"<<endl;delete []p;});
	//7.2 std::default_delete<T>()
	shared_ptr<int> pary2 = make_shared_arry<int>(10);
}
//29_2 独占智能指针 std::unique_ptr<T> 
unique_ptr<int> dm29func(){
	return unique_ptr<int>(new int(12));
}
void dm29_2(){
	//1 初始化
	unique_ptr<int> p1(new int(111));
	//unique_ptr<int> p2 = p1;//err 独占智能指针不允许赋值
	unique_ptr<int> p3 = move(p1);
	unique_ptr<int> p4 = dm29func();//匿名对象初始化P4
	//2 reset方法
	p3.reset();
	p4.reset(new int(520));

	//3 获取原始指针shared_ptr::get方法
	int *p = p4.get();
	cout<<"*p:"<<*p<<" "<<"*p4.get():"<<*p4.get()<<endl;

	//4 删除器，独占指针的删除器需要指定删除器的类型
	using dfunc = void(*)(int*);
	unique_ptr<int,dfunc> p5(new int(123),[](int* p){cout<<"delete dfunc p"<<endl;delete p;});
	unique_ptr<int,std::function<void(int*)>> p6(new int(321),[&](int* p){cout<<"delete function p"<<endl;delete p;});//lamamda式中如果捕获外部变量，则不能转换为普通函数，需要std::funtion包装为可调用对象
}

//29_3 弱指针 std::weak_ptr
  

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
	//右值引用 和 完美转发
	dm20();
	//列表初始化
	dm21();
	//using
	dm22();
	//仿函数 可调用对象包装器 可调用对象绑定器
	dm23_1();
	dm23_funtion();
	dm23_bind();
	//POD数据类型
	dm24();
	//friend
	dm26_1();
	dm26_2();
	//强枚举
	dm27();
	// 非受限联合体
	dm28_1();
	dm28_2();
	// 智能指针
	dm29_1();
	dm29_2();
}


