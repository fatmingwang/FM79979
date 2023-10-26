#pragma once

#include <memory>
#include "Tweeny/tweeny.h"
#include "../Core/AllCommonInclude.h"
class cTweenyObject
{
	std::map<unsigned int,std::function<void()>>	m_IDAndFinishFunction;
	std::vector<int>		m_TweenyIDVector;
	void					TweenFinish(unsigned int);
public:
	~cTweenyObject();
	//https://easings.net/zh-tw
	void	AddTweeny(tweeny::easing::enumerated e_easing,Vector2 e_vStart, Vector2 e_vEnd,float e_fDuration, std::function<void(Vector2)> e_Function,std::function<void()> e_FinishFunction = nullptr);
	void	AddTweeny(tweeny::easing::enumerated e_easing,Vector3 e_vStart, Vector3 e_vEnd,float e_fDuration, std::function<void(Vector3)> e_Function, std::function<void()> e_FinishFunction = nullptr);
	void	AddTweeny(tweeny::easing::enumerated e_easing,float e_fStart, float e_fEnd,float e_fDuration, std::function<void(float)> e_Function, std::function<void()> e_FinishFunction = nullptr);
	void	AddTweeny(tweeny::easing::enumerated e_easing,int e_iStart, int e_iEnd,float e_fDuration, std::function<void(int)> e_Function, std::function<void()> e_FinishFunction = nullptr);
};


class  cTweenyManager:public NamedTypedObject
{
protected:
	cTweenyManager();
	virtual ~cTweenyManager();
	static cTweenyManager* m_spInstance;
public:
	cTweenyManager(cTweenyManager const&) = delete;
	cTweenyManager(cTweenyManager*) = delete;
	static void DestroyInstance();
	static cTweenyManager* GetInstance();
	void	Update(float e_fElpaseTime);
	DEFINE_TYPE_INFO();
};

const wchar_t* ValueToString(tweeny::easing::enumerated e_easing);
class cTweenyTestObject
{
	cTweenyObject m_TweenyObject;
	void			TestAll();
	void			Test(tweeny::easing::enumerated e_enumerated);
public:
	std::vector<Vector2> m_vTestVector;
	cTweenyTestObject();
	void	KeyUp();
};

//auto tween = tweeny::from(0, 0.0f).to(2, 2.0f).during(100).onStep([](int i, float f) { printf("i=%d f=%f\n", i, f); return false; });
//while (tween.progress() < 1.0f) tween.step(1);
//return 0;

//
//
///* Prints a line, showing the tween value and a dot corresponding to that value */
//bool print(tweeny::tween<int>&, int p)
//{
//	FMLOG("%+.3d |", p); // 3 digits with sign
//	std::string l_Str;
//	for (int i = 0; i <= 100; i++)
//	{
//		l_Str += i == p ? '.' : '-';
//	}
//	FMLOG(l_Str.c_str());
//	FMLOG("%c\n", p == 100 ? ';' : '|');
//	return false;
//}
//
///* Runs the tween with the designated easing */
//
//template<typename EasingT> void test(tweeny::tween<int>& tween, EasingT easing) {
//	tween.via(0, easing);
//	tween.seek(0);
//	for (int i = 0; i <= 100; i++)
//	{
//		auto l_Current = tween.step(0.01f);
//	}
//}
///* Nacro to help call test() with each type of easing */
//#define EASING_TEST(tween, easing) \
//    FMLOG("%s\n", #easing " In/Out/InOut"); \
//    test(tween, easing ## In); \
//    test(tween, easing ## Out); \
//    test(tween, easing ## InOut)
//
//void	TweenyTest()
//{
//	auto t = tweeny::from(0).to(100).during(100).onStep(print);
//	EASING_TEST(t, tweeny::easing::quadratic);
//	////tween.tcc line 113.
//	////https://easings.net/zh-tw
//	////https://github.com/mobius3/tweeny-demos
//	//auto tween1 = tweeny::from(0.0, 1.0f).to(1.0f, 0.0f).via(tweeny::easing::enumerated::stepped, tweeny::easing::linear);
//	////auto tween1 = tweeny::from(0.0, 1.0f).to(1.0f, 0.0f).via("stepped", "linear");
//	//auto l_Array2 = tween1.step(1);
//	////tween1.step()
//	//auto helloworld = tweeny::from('h', 'e', 'l', 'l', 'o').to('w', 'o', 'r', 'l', 'd').during(50);
//	//for (int i = 0; i < 50; i++)
//	//{
//	//	auto l_str = helloworld.step(1);
//	//	std::string l_strResult = UT::ComposeMsgByFormat("%d: ", i);
//	//	for (char c : l_str)
//	//	{
//	//		l_strResult += c;
//	//	}
//	//	FMLOG(l_strResult.c_str());
//	//}//
//}
