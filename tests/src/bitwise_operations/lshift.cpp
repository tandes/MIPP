#include <exception>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <mipp.h>
#include <catch.hpp>

template <typename T, int n = (sizeof(T) * 8 -1)>
struct reg_lshift
{
	static void test()
	{
		reg_lshift<T,n-1>::test();

		T inputs1[mipp::N<T>()];
		std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);

		std::mt19937 g;
		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);

		mipp::reg r1 = mipp::load<T>(inputs1);
		mipp::reg r2 = mipp::lshift<T>(r1, n);

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			T res = inputs1[i] << n;
			REQUIRE(*((T*)&r2 +i) == res);
		}
	}
};

template <typename T>
struct reg_lshift<T,-1>
{
	static void test() {}
};

#ifndef MIPP_NO
TEST_CASE("Binary left shift - mipp::reg", "[mipp::lshift]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { reg_lshift<int64_t>::test(); }
#endif
	SECTION("datatype = int32_t") { reg_lshift<int32_t>::test(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { reg_lshift<int16_t>::test(); }
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
	SECTION("datatype = int8_t") { reg_lshift<int8_t >::test(); }
#endif
#endif
#endif
}
#endif

template <typename T, int n = (sizeof(T) * 8 -1)>
struct Reg_lshift
{
	static void test()
	{
		Reg_lshift<T,n-1>::test();

		T inputs1[mipp::N<T>()];
		std::iota(inputs1, inputs1 + mipp::N<T>(), (T)0);

		std::mt19937 g;
		std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);

		mipp::Reg<T> r1 = inputs1;
		mipp::Reg<T> r2 = r1 << n;

		for (auto i = 0; i < mipp::N<T>(); i++)
		{
			T res = inputs1[i] << n;
			REQUIRE(r2[i] == res);
		}
	}
};

template <typename T>
struct Reg_lshift<T,-1>
{
	static void test() {}
};

TEST_CASE("Binary left shift - mipp::Reg", "[mipp::lshift]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { Reg_lshift<int64_t>::test(); }
#endif
	SECTION("datatype = int32_t") { Reg_lshift<int32_t>::test(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { Reg_lshift<int16_t>::test(); }
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
	SECTION("datatype = int8_t") { Reg_lshift<int8_t >::test(); }
#endif
#endif
#endif
}

template <typename T, int n = mipp::N<T>()>
struct msk_lshift
{
	static void test()
	{
		msk_lshift<T,n-1>::test();

		constexpr int N = mipp::N<T>();
		bool inputs1[N], inputs2[N];
		std::mt19937 g;
		std::uniform_int_distribution<unsigned char> dis(0, 1);

		for (auto t = 0; t < 100; t++)
		{
			for (auto i = 0; i < N; i++)
				inputs1[i] = dis(g) ? true : false;

			std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);

			mipp::msk m1 = mipp::set<N>(inputs1);
			mipp::msk m2 = mipp::lshift<N>(m1, n);

			std::fill(inputs2, inputs2 + N, 0);
			for (auto i = 0; i < N - n; i++)
				inputs2[i +n] = inputs1[i];

			mipp::reg r = mipp::toreg<N>(m2);

			for (auto i = 0; i < N; i++)
			{
				if (inputs2[i])
					REQUIRE(*((T*)&r +i) != (T)0);
				else
					REQUIRE(*((T*)&r +i) == (T)0);
			}
		}
	}
};

template <typename T>
struct msk_lshift<T,-1>
{
	static void test() {}
};

#ifndef MIPP_NO
#if !defined(MIPP_AVX) && !defined(__clang__)
TEST_CASE("Binary left shift - mipp::msk", "[mipp::lshift]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { msk_lshift<int64_t>::test(); }
#endif
	SECTION("datatype = int32_t") { msk_lshift<int32_t>::test(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { msk_lshift<int16_t>::test(); }
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
	SECTION("datatype = int8_t") { msk_lshift<int8_t >::test(); }
#endif
#endif
#endif
}
#endif
#endif

template <typename T, int n = mipp::N<T>()>
struct Msk_lshift
{
	static void test()
	{
		Msk_lshift<T,n-1>::test();

		constexpr int N = mipp::N<T>();
		bool inputs1[N], inputs2[N];
		std::mt19937 g;
		std::uniform_int_distribution<unsigned char> dis(0, 1);

		for (auto t = 0; t < 100; t++)
		{
			for (auto i = 0; i < N; i++)
				inputs1[i] = dis(g) ? true : false;

			std::shuffle(inputs1, inputs1 + mipp::N<T>(), g);

			mipp::Msk<N> m1 = inputs1;
			mipp::Msk<N> m2 = m1 << n;

			std::fill(inputs2, inputs2 + N, 0);
			for (auto i = 0; i < N - n; i++)
				inputs2[i +n] = inputs1[i];

			for (auto i = 0; i < N; i++)
			{
				REQUIRE(m2[i] == inputs2[i]);
			}
		}
	}
};

template <typename T>
struct Msk_lshift<T,-1>
{
	static void test() {}
};

#if !defined(MIPP_AVX) && !defined(__clang__)
TEST_CASE("Binary left shift - mipp::Msk", "[mipp::lshift]")
{
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
#if defined(MIPP_64BIT)
	SECTION("datatype = int64_t") { Msk_lshift<int64_t>::test(); }
#endif
	SECTION("datatype = int32_t") { Msk_lshift<int32_t>::test(); }
#if defined(MIPP_BW)
	SECTION("datatype = int16_t") { Msk_lshift<int16_t>::test(); }
#if !defined(MIPP_AVX) || (defined(MIPP_AVX) && MIPP_INSTR_VERSION >= 2)
	SECTION("datatype = int8_t") { Msk_lshift<int8_t >::test(); }
#endif
#endif
#endif
}
#endif
