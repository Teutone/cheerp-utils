//===---------------------------------------------------------------------===//
//	Copyright 2015 Leaning Technologies
//===---------------------------------------------------------------------===//

#include <tests.h>

// Test 64-bit integer arithmetics

#ifndef PRE_EXECUTE_TEST
template <typename T>
static void dump(T t) {
	long h = (long)((t & (0xffffffffll << 32)) >> 32);
	long l = (long)(t & 0xffffffff);
	cheerp::console_log("highint h:", h, "l:", l);
}
#endif

void testRepresentation() {
	assertEqual((long long) 0, 0x0000000000000000LL, "int64_t representation 1/N");
	assertEqual(-1 == 0xffffffffffffffffLL, true, "int64_t representation 2/N");
	assertEqual((unsigned long long) -1, 0xffffffffffffffffULL, "int64_t representation 3/N");
	assertEqual(9223372036854775807LL, 0x7fffffffffffffffLL, "int64_t representation 4/N");
}

template <typename T>
static void testShiftOps() {
	T a = 0x10000;
	T b = 0x100000000;
	T c = 0x1000000000000;
	T d = 0x7fffffffLL;
	T e = 0x7fffffff00000000LL;

	assertEqual(a <<  0, a, "int64_t shl support 1/N");
	assertEqual(a <<  8, (T)0x01000000, "int64_t shl support 2/N");
	assertEqual(a << 16, b, "int64_t shl support 3/N");
	assertEqual(a << 32, c, "int64_t shl support 4/N");
	assertEqual(d << 32, e, "int64_t shl support 5/N");

	assertEqual(a >>  0, a, "int64_t shr support 1/N");
	assertEqual(a >>  8, (T)0x00000100, "int64_t shl support 2/N");
	assertEqual(b >> 16, a, "int64_t shr support 3/N");
	assertEqual(c >> 32, a, "int64_t shr support 4/N");
	assertEqual(e >> 32, d, "int64_t shr support 5/N");

	if (std::is_unsigned<T>::value) {
		T f = 0xffffffff;
		T g = 0xffffffff00000000;
		assertEqual(g >> 32, f, "int64_t shr support 6a/N");
	} else {
		T f = 0xffffffffffffffff;
		T g = 0xffffffff00000000;
		assertEqual(g >> 32, f, "int64_t shr support 6b/N");
	}
}

template <typename T>
static void testBitwiseOps() {
	T a = 0xffffffffffffffffLL;
	T b = 0x0000000000000000LL;
	T l = 0x00000000ffffffffLL;
	T u = 0xffffffff00000000LL;

	assertEqual(a & l, l, "int64_t and support 1/N");
	assertEqual(a & u, u, "int64_t and support 2/N");

	assertEqual(a | l, a, "int64_t or support 1/N");
	assertEqual(b | l, l, "int64_t or support 2/N");
	assertEqual(a | u, a, "int64_t or support 3/N");
	assertEqual(b | u, u, "int64_t or support 4/N");

	assertEqual(a ^ b, a, "int64_t xor support 1/N");
	assertEqual(a ^ a, b, "int64_t xor support 2/N");
	assertEqual(b ^ b, b, "int64_t xor support 3/N");

	assertEqual(~a, b, "int64_t not support 1/N");
	assertEqual(~b, a, "int64_t not support 2/N");
	assertEqual(~l, u, "int64_t not support 3/N");
}

template <typename T>
static void testAddSubOps() {
	T a = 0x0deadbeaf;
	T b = 0x100000000;
	T c = 0x0ffffffff;
	T d = 0xffffffffffffffff;
	T e = 0xfffffffffffffffe;

	assertEqual(a + b, (T)0x1deadbeaf, "int64_t add support 1/N");
	assertEqual(c + 1, (T)0x100000000, "int64_t add support 2/N");
	assertEqual(d + 1, (T)0x000000000, "int64_t add support 3/N");
	assertEqual(d + d, e, "int64_t add support 4/N");

	assertEqual(a - 1, (T)0xdeadbeae, "int64_t sub support 1/N");
	assertEqual(b - 1, (T)0xffffffff, "int64_t sub support 2/N");
	assertEqual(b - b, (T)0x00000000, "int64_t sub support 3/N");
	assertEqual(c - b, (T)0xffffffffffffffff, "int64_t sub support 4/N");
	assertEqual(d - d, (T)0x00000000, "int64_t sub support 5/N");
	T f = -1;
	assertEqual(f + -1, (T)0xfffffffffffffffe, "int64_t sub support 6/N");
}

template <typename T>
static void testMulDivModOps() {
	T a = 0x00018001;
	assertEqual(a * 2, (T)0x00030002, "int64_t mul support 1/N");

	T b = 0x0000000088888888;
	assertEqual(b * 2, (T)0x0000000111111110, "int64_t mul support 2/N");

	T c = 0x8888888800000000;
	assertEqual(c * 2, (T)0x1111111000000000, "int64_t mul support 3/N");

	T d = 0x1122334455667788;
	T e = 0x1111111111111111;
	assertEqual(d * e, (T)0xcba862fb71c5f808, "int64_t mul support 4/N");

	T f = 0xffffffff;
	assertEqual(f * f, (T)0xfffffffe00000001, "int64_t mul support 5/N");

	assertEqual(f / f, (T)1, "int64_t div support 1/N");
	assertEqual(d / d, (T)1, "int64_t div support 2/N");
	if (std::is_unsigned<T>::value) {
		assertEqual((T)((-d) / d), (T)13, "int64_t div support 3a/N");
	} else {
		assertEqual((T)((-d) / d), (T)-1, "int64_t div support 3b/N");
	}
	T g = 0xf;
	assertEqual(g / 3, (T)5, "int64_t div support 4/N");

	assertEqual(f % f, (T)0, "int64_t mod support 1/N");
	assertEqual((T)(-1) % 1, (T)0, "int64_t mod support 2/N");
	assertEqual(d % e, (T)0x0011223344556677, "int64_t mod support 3/N");
}

template <typename T>
static void testUnaryOps() {
	T a = 0x0;
	T b = 0x1;
	T c = 0x0000000100000000;
	T d = 0xffffffffffffffff;
	T e = 0x1122334455667788;

	assertEqual(-a, (T)0, "int64_t neg support 1/N");
	assertEqual(-b, (T)0xffffffffffffffff, "int64_t neg support 2/N");
	assertEqual(-c, (T)0xffffffff00000000, "int64_t neg support 3/N");
	assertEqual(-d, (T)0x00000001, "int64_t neg support 4/N");
	assertEqual(-e, (T)0xeeddccbbaa998878, "int64_t neg support 5/N");
	assertEqual(-0x7fffffffffffffff == 0x8000000000000001, true, "int64_t neg support 6a/N");

	assertEqual(!a, true, "int64_t lnot support 1/N");
	assertEqual(!b, false, "int64_t lnot support 2/N");
	assertEqual(!c, false, "int64_t lnot support 3/N");
	assertEqual(!d, false, "int64_t lnot support 4/N");
}

template <typename T>
static void testBitwiseCompoundAssignmentOps() {
	T a = 0x0;
	T b = 0x1;
	T c = 0x0000000100000000;
	T d = 0xffffffffffffffff;
	T t;

	t = a; t &= b;
	assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 1/N");
	t = b; t &= c;
	assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 2/N");
	t = c; t &= d;
	assertEqual(t, (T)0x0000000100000000, "int64_t and assign support 3/N");
	t = d; t &= a;
	assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 4/N");

	t = a; t |= b;
	assertEqual(t, (T)0x0000000000000001, "int64_t or assign support 1/N");
	t = b; t |= c;
	assertEqual(t, (T)0x0000000100000001, "int64_t or assign support 2/N");
	t = c; t |= d;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t or assign support 3/N");
	t = d; t |= a;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t or assign support 4/N");

	t = a; t ^= b;
	assertEqual(t, (T)0x0000000000000001, "int64_t xor assign support 1/N");
	t = b; t ^= c;
	assertEqual(t, (T)0x0000000100000001, "int64_t xor assign support 2/N");
	t = c; t ^= d;
	assertEqual(t, (T)0xfffffffeffffffff, "int64_t xor assign support 3/N");
	t = d; t ^= a;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t xor assign support 4/N");
}

template <typename T>
static void testArithmeticCompoundAssignmentOps() {
	T a = 0x0;
	T b = 0x1;
	T c = 0x0000000100000000;
	T d = 0xffffffffffffffff;
	T t;

	t = a; t += b;
	assertEqual(t, (T)0x0000000000000001, "int64_t add assign support 1/N");
	t = b; t += c;
	assertEqual(t, (T)0x0000000100000001, "int64_t add assign support 2/N");
	t = c; t += d;
	assertEqual(t, (T)0x00000000ffffffff, "int64_t add assign support 3/N");
	t = d; t += a;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t add assign support 4/N");

	t = a; t -= b;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t sub assign support 1/N");
	t = b; t -= c;
	assertEqual(t, (T)0xffffffff00000001, "int64_t sub assign support 2/N");
	t = c; t -= d;
	assertEqual(t, (T)0x0000000100000001, "int64_t sub assign support 3/N");
	t = d; t -= a;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t sub assign support 4/N");

	t = a; t %= b;
	assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 1/N");
	t = b; t %= c;
	assertEqual(t, (T)0x0000000000000001, "int64_t mod assign support 2/N");
	t = c; t %= d;
	if (std::is_unsigned<T>::value) {
		assertEqual(t, (T)0x0000000100000000, "int64_t mod assign support 3a/N");
	} else {
		assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 3b/N");
	}
	t = d; t %= b;
	assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 4/N");

	t = a; t *= b;
	assertEqual(t, (T)0x0000000000000000, "int64_t mul assign support 1/N");
	t = b; t *= c;
	assertEqual(t, (T)0x0000000100000000, "int64_t mul assign support 2/N");
	t = c; t *= d;
	assertEqual(t, -c, "int64_t mul assign support 3/N");
	t = d; t *= b;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t mul assign support 4/N");

	t = a; t /= b;
	assertEqual(t, (T)0x0000000000000000, "int64_t div assign support 1/N");
	t = b; t /= c;
	assertEqual(t, (T)0x0000000000000000, "int64_t div assign support 2/N");
	t = c; t /= d;
	if (std::is_unsigned<T>::value) {
		assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 3a/N");
	} else {
		assertEqual(t, -c, "int64_t mod assign support 3b/N");
	}
	t = d; t /= b;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t div assign support 4/N");

	t = b; t <<= 0;
	assertEqual(t, (T)0x0000000000000001, "int64_t shl assign support 1/N");
	t = c; t <<= 16;
	assertEqual(t, (T)0x0001000000000000, "int64_t shl assign support 2/N");
	t = d; t <<= 32;
	assertEqual(t, (T)0xffffffff00000000, "int64_t shl assign support 3/N");

	t = b; t >>= 0;
	assertEqual(t, (T)0x0000000000000001, "int64_t shr assign support 1/N");
	t = c; t >>= 16;
	assertEqual(t, (T)0x0000000000010000, "int64_t shr assign support 2/N");
	t = d; t >>= 32;
	if (std::is_unsigned<T>::value) {
		assertEqual(t, (T)0x00000000ffffffff, "int64_t shr assign support 3a/N");
	} else {
		assertEqual(t, (T)0xffffffffffffffff, "int64_t shr assign support 3b/N");
	}

	T s;
	volatile T r;
	t = a; r = t += a;
	s = r;
	assertEqual(t, s, "int64_t compound assign result support");

}

template <typename T>
static void testComparisonOps() {
	T a = 0x0;
	T b = 0x1;
	T c = 0x0000000100000000;
	T e = 0xffffffffffffffff;
	T t = a;
	T h = 0xf000000000000000;

	assertEqual(a < b, true, "int64_t lt support 1/N");
	assertEqual(a < c, true, "int64_t lt support 2/N");
	assertEqual(e < a, std::is_signed<T>::value, "int64_t lt support 3/N");
	assertEqual(e < c, std::is_signed<T>::value, "int64_t lt support 4/N");
	assertEqual(b < c, true, "int64_t lt support 5/N");
	assertEqual(c < b, false, "int64_t lt support 6/N");
	assertEqual(t < a, false, "int64_t lt support 7/N");
	assertEqual(h < 0, std::is_signed<T>::value, "int64_t lt support 8/N");

	assertEqual(a <= b, true, "int64_t le support 1/N");
	assertEqual(a <= c, true, "int64_t le support 2/N");
	assertEqual(e <= a, std::is_signed<T>::value, "int64_t le support 3/N");
	assertEqual(e <= c, std::is_signed<T>::value, "int64_t le support 4/N");
	assertEqual(b <= c, true, "int64_t le support 5/N");
	assertEqual(c <= b, false, "int64_t le support 6/N");
	assertEqual(t <= a, true, "int64_t le support 7/N");
	assertEqual(h <= 0, std::is_signed<T>::value, "int64_t le support 8/N");

	assertEqual(a > b, false, "int64_t gt support 1/N");
	assertEqual(a > c, false, "int64_t gt support 2/N");
	assertEqual(e > a, std::is_unsigned<T>::value, "int64_t gt support 3/N");
	assertEqual(e > c, std::is_unsigned<T>::value, "int64_t gt support 4/N");
	assertEqual(b > c, false, "int64_t gt support 5/N");
	assertEqual(c > b, true, "int64_t gt support 6/N");
	assertEqual(t > a, false, "int64_t gt support 7/N");
	assertEqual(h > 0, std::is_unsigned<T>::value, "int64_t gt support 8/n");

	assertEqual(a >= b, false, "int64_t ge support 1/N");
	assertEqual(a >= c, false, "int64_t ge support 2/N");
	assertEqual(e >= a, std::is_unsigned<T>::value, "int64_t ge support 3/N");
	assertEqual(e >= c, std::is_unsigned<T>::value, "int64_t ge support 4/N");
	assertEqual(b >= c, false, "int64_t ge support 5/N");
	assertEqual(c >= b, true, "int64_t ge support 6/N");
	assertEqual(t >= a, true, "int64_t ge support 7/N");
	assertEqual(h >= 0, std::is_unsigned<T>::value, "int64_t ge support 8/n");

	bool zeroIsFalse = true;
	if(a)
		zeroIsFalse = false;
	assertEqual(zeroIsFalse, true, "int64_t zero is false 1/N");
}

template <typename T>
static void testCastToFloat() {
	T a = 0x0;
	T b = 0x1;
	T c = 0x0000000100000000;
	T e = 0xffffffffffffffff;

	assertEqual((float) a, 0.f, 1e-6, "int64_t cast to float support 1/N");
	assertEqual((float) b, 1.f, 1e-6, "int64_t cast to float support 2/N");
	assertEqual((float) c, 4.2949673e9f, 1e-6, "int64_t cast to float support 3/N");

	T h = 9223372036854775807LL;
	assertEqual((double) h, 9.223372036854776e18, 1e-10, "int64_t cast to float support 4/N");

	if (std::is_unsigned<T>::value) {
		assertEqual((double) e, 18446744073709551616., 1e4, "int64_t cast to float support 5a/N");
	} else {
		assertEqual((double) e, -1., 1e-6, "int64_t cast to float support 5b/N");
		T d = 0x8000000000000000;
		assertEqual((double) d, -9223372036854776000., 1e-1, "int64_t cast to float support 6b/N");
		T f = 0x7fffffffffffffff;
		assertEqual((double) f, 9223372036854775808., 1e-1, "int64_t cast to float support 7b/N");
	}
}

template <typename T>
static void testCastFromFloat() {
	float a = 0;
	float b = 1;
	float c = 1337.;
	float d = 3.142;
	float e = -1.;

	assertEqual((T) a, (T)0, "int64_t cast from float support 1/N");
	assertEqual((T) b, (T)1, "int64_t cast from float support 2/N");
	assertEqual((T) c, (T)1337, "int64_t cast from float support 3/N");
	assertEqual((T) d, (T)3, "int64_t cast from float support 4/N");

	if (std::is_signed<T>::value) {
		assertEqual((T) e, (T)-1, "int64_t cast from float support 5a/N");

		float f = -3.2e3;
		assertEqual((T) f, (T)-3200, "int64_t cast from float support 6/N");

		float g = -3.2e9;
		assertEqual((T) g, (T)-3200000000, "int64_t cast from float support 7/N");

		T h = 0x7ffffffffffff000;
		double i = 9223372036854771712.;
		assertEqual((T) i, h, "int64_t cast from float support 8/N");
	}
}

template <typename T>
static void testIncrement() {
	T a = 0;
	a++;
	assertEqual(a, (T)1, "int64_t post increment support 1/N");
	a = 0x00000000ffffffff;
	assertEqual(a++, (T)0x0000000100000000, "int64_t post increment support 2/N");
	assertEqual(a, (T)0x0000000100000000, "int64_t post increment support 3/N");

	T b = 0;
	++b;
	assertEqual(b, (T)1, "int64_t pre increment support 1/N");
	b = 0x00000000ffffffff;
	assertEqual(++b, (T)0x0000000100000000, "int64_t pre increment support 2/N");
	assertEqual(b, (T)0x0000000100000000, "int64_t pre increment support 3/N");

	a = 0;
	a--;
	assertEqual(a, (T)0xffffffffffffffff, "int64_t post decrement support 1/N");

	a = 0xffffffffffffffff;
	assertEqual(a--, (T)0xfffffffffffffffe, "int64_t post decrement support 2/N");
	assertEqual(a, (T)0xfffffffffffffffe, "int64_t post decrement support 3/N");

	b = 0;
	--b;
	assertEqual(b, (T)0xffffffffffffffff, "int64_t pre decrement support 1/N");
	b = 0xffffffffffffffff;
	assertEqual(--b, (T)0xfffffffffffffffe, "int64_t pre decrement support 2/N");
	assertEqual(b, (T)0xfffffffffffffffe, "int64_t pre decrement support 3/N");
}

template <typename T>
static void testStructs() {
	typedef struct s_ {
		T a;
		T b;
		int c;
	} s;

	s foo;
	foo.a = 42;
	foo.b = 0x7fffffffffffffff;
	foo.c = 1;

	assertEqual(foo.a < foo.b, true, "int64_t struct support 1/N");
	assertEqual(foo.a > foo.c, true, "int64_t struct support 2/N");
	assertEqual(foo.b > foo.c, true, "int64_t struct support 3/N");
}

template <typename T>
static void increment(T& a) {
	a += 1;
}

template <typename T>
static void testPointerArithmetic() {
	T *a = new T[20];
	memset(a, 0, sizeof(T)*20);
	bool valid = true;
	for (int i = 0; i < 20; i++)
		valid &= (a[i] == 0);
	assertEqual(valid, true, "int64_t list support 1/N");

	for (int i = 0; i < 20; i++) {
		a[i] = ((0xffffffffLL - i) << 32) + i;
	}

	valid = true;
	for (int i = 0; i < 20; i++)
		valid &= (a[i] != 0);
	assertEqual(valid, true, "int64_t list support 2/N");
}

template <typename T>
static void testPointersAndReferences() {
	T a = 42;
	increment<T>(a);
	assertEqual(a, (T)43, "int64_t reference support 1/N");
}

static void testBitfields() {
	struct A {
		unsigned long long a : 3;
		unsigned long long b : 3;
	};
	struct A foo;
	foo.a = 7;
	foo.b = 4;
	assertEqual(foo.a, 7ULL, "int64_t bit fields support 1/N");
	assertEqual(foo.b, 4ULL, "int64_t bit fields support 2/N");

	// Test 0 bit field
	struct B {
		unsigned long long a1 : 3;
		unsigned long long a2 : 3;
		unsigned long long : 0;
		unsigned long long b1 : 3;
		unsigned long long b2 : 3;
	};
	struct B bar;
	bar.a1 = 7;
	bar.a2 = 4;
	bar.b1 = 7;
	bar.b2 = 4;
	assertEqual(bar.a1, 7ULL, "int64_t bit fields support 3/N");
	assertEqual(bar.a2, 4ULL, "int64_t bit fields support 4/N");
	assertEqual(bar.b1, 7ULL, "int64_t bit fields support 5/N");
	assertEqual(bar.b2, 4ULL, "int64_t bit fields support 6/N");

	// Test >32 bit field
	struct C {
		unsigned long long a : 34;
		unsigned long long b : 30;
	};
	struct C baz;
	baz.a = 7;
	baz.b = 4;
	assertEqual(baz.a, 7ULL, "int64_t bit fields support 7/N");
	assertEqual(baz.b, 4ULL, "int64_t bit fields support 8/N");

	// Test signed >32 bit field
	struct D {
		long long a : 34;
		long long b : 30;
	};
	struct D quux;
	quux.a = 7;
	quux.b = -1;
	assertEqual(quux.a, 7LL, "int64_t bit fields support 9/N");
	assertEqual(quux.b, -1LL, "int64_t bit fields support 10/N");
}

template <typename T>
static void testSwitch()
{
	volatile T a = 11;
	bool result = false;
	switch(a)
	{
		case 11:
			result = true;
			break;
		case 10:
			result = false;
			break;
		default:
			result = false;
			break;
	}
	assertEqual(result, true, "int64_t in switch statements");
}

#ifndef PRE_EXECUTE_TEST
static void testDump() {
	long long t = 0xff;
	cheerp::console_log("test dump(0xff):");
	dump(t);
}
#endif

void webMain() {
	testRepresentation();
#ifndef PRE_EXECUTE_TEST
	testDump();
#endif
	testShiftOps<long long>();
	testShiftOps<unsigned long long>();
	testBitwiseOps<long long>();
	testBitwiseOps<unsigned long long>();
	testAddSubOps<long long>();
	testAddSubOps<unsigned long long>();
	testMulDivModOps<long long>();
	testMulDivModOps<unsigned long long>();
	testUnaryOps<long long>();
	testUnaryOps<unsigned long long>();
	testBitwiseCompoundAssignmentOps<long long>();
	testBitwiseCompoundAssignmentOps<unsigned long long>();
	testArithmeticCompoundAssignmentOps<long long>();
	testArithmeticCompoundAssignmentOps<unsigned long long>();
	testComparisonOps<long long>();
	testComparisonOps<unsigned long long>();
	testCastToFloat<long long>();
	testCastToFloat<unsigned long long>();
	testCastFromFloat<long long>();
	testCastFromFloat<unsigned long long>();
	testIncrement<long long>();
	testIncrement<unsigned long long>();
	testStructs<long long>();
	testStructs<unsigned long long>();
	testPointerArithmetic<long long>();
	testPointerArithmetic<unsigned long long>();
	testPointersAndReferences<long long>();
	testPointersAndReferences<unsigned long long>();
	testBitfields();
	//switch with 64-bit values is not supported
	//testSwitch<long long>();
	//testSwitch<unsigned long long>();
}

// vim: noexpandtab
