
#if ( defined(_MSC_VER) && _MSVC_LANG >= 201703L ) || __cplusplus >= 201703L		// if C++17 or later

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <list>
#include <memory>
#include <optional>

#include <boost/test/unit_test.hpp>

#include "StringFormat.h"

using namespace rlib;

BOOST_AUTO_TEST_CASE(stringformat_test)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);	// ログレベル

	BOOST_TEST_MESSAGE("stringformat_test");
	BOOST_CHECK(true);

	using std::cout;
	using std::endl;
	using namespace rlib::string;

	// 以下コードは printf 風の書式しか使っておりませんが、中身は boost::format です。
	// 書式の詳細はそちらのリファレンスもご参照下さい。

	{// 普通の使い方
		cout << format(u8"%s %dSX SR%dDE%s", u8"日産", 180, 20, "T") << endl;	// 日産 180SX SR20DET
	}
	{// std::wstring も可です
		std::wstring r = format(L"BNR%d RB%dDE%s", 32, 26, L"TT");	// L"BNR32 RB26DETT"
		std::wcout << r << endl;	// L"BNR32 RB26DETT"
	}
	{// 適してない型でも例外発生しません
		cout << format("%dSX %s", 240, L"日産") << endl;	// 240SX 00007FF742CC123C(例)
	}
	{// std::string をそのまま書けます。c_str() が不要なので便利。
		const std::string a = u8"トヨタ";
		std::string b = "1JZ-GTE";
		cout << format("%s jzx90 %s", a, b) << endl;	// トヨタ jzx90 1JZ-GTE
	}
#ifdef _MSC_VER
	{// VisualStudio の CString
		CString t0(_T("マツダ"));
		const CString t1(_T("13B-T"));
		auto r = format(_T("%s FC3S %s"), t0, t1);	// マツダ FC3S 13B-T
	}
#endif
	{// std::tuple std::pair は展開します。
		std::tuple<std::string, int, double> a{ "RPS13",1998,11.8 };
		const std::pair<std::string, int> b{ "FR", 1848000 };
		cout << format(			// 形式:RPS13 排気量:1998cc 燃費:11.8km/l 駆動方式:FR 価格:1848000円
			u8"形式:%s 排気量:%dcc 燃費:%.1fkm/l 駆動方式:%s 価格:%d円", a, b) << endl;
	}
	{// std::unique_ptr std::shared_ptr std::optional は中身を出力にします。空(無効)の場合は"(null)"を出力します
		std::unique_ptr<int> u = std::make_unique<int>(1);
		std::shared_ptr<int> s = std::make_shared<int>(2);
		std::optional<int> o(3);
		std::optional<int> e;
		cout << format("%d,%d,%d,%d", u, s, o, e) << endl;	// 1,2,3,(null)
	}
	{// その他コンテナも展開します。(やりすぎかもしれません。素直にコンパイルエラーに振ってもよいかも。)
		cout << format("%d,%d,%d", std::array<int, 3>{ 1, 2, 3 }) << endl;			// 1,2,3
		cout << format("%d,%d,%d", std::vector<int>{4, 5, 6}) << endl;				// 4,5,6
		cout << format("%d,%d,%d", std::list<int>{7, 8, 9}) << endl;				// 7,8,9
		cout << format("%d,%d,%d", std::deque<int>{10, 11, 12}) << endl;			// 10,11,12
		cout << format("%d,%d,%d", std::set<int>{3, 2, 1}) << endl;					// 1,2,3 (取れる順で出力します)
		cout << format("%d,%d,%d", std::map<int, int>{ {1, 2}, { 3,4 }}) << endl;	// 1,2,3
		cout << format("%d,%d,%d", std::multiset<int>{1, 2, 3}) << endl;			// 1,2,3"
		cout << format("%d,%d,%d", std::multimap<int, int>{ {1, 2}, { 3,4 }}) << endl;// 1,2,3
	}
	{// コンテナの中も再帰で展開するので以下みたいな型も可です
		std::map<int, std::pair<std::string, std::vector<int>>> m{
			{1, {"a", {5,6,7}}},
			{8, {"b", {9,10}}}
		};
		cout << format("%s,%s,%s,%s,%s,%s", m) << endl;	// 1,a,5,6,7,8
	}
	{// boost::format なので未対応の型はコンパイルエラーになってくれます
		std::function<void()> f;
//		cout << format("%s", f) << endl;	// コンパイルエラー
	}

}

#endif	// ( define(_MSC_VER) && _MSVC_LANG < 201703L )) || __cplusplus > 201703L
