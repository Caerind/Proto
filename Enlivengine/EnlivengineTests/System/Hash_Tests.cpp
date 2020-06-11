#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

#include <string_view>

DOCTEST_TEST_CASE("Constexpr Hash")
{
	DOCTEST_SUBCASE("std::string_view == const char*")
	{
		std::string_view azertysv = "AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./1234567890098765432123456789098765432123456789098765432134567892345YTFBHJTRDFGHREDFG891Y1GUIAHZ TZGIUIZJ19YT7UFAJHVBSIASHUSHGC VNZIU HIUG IQHDU QDGYTQGHJBQHUIIGQDIHBJNQKS";
		std::string_view asv = "A";
		std::string_view bsv = "B";
		std::string_view a_sv = "A "; 
		const char* azertyc = "AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./1234567890098765432123456789098765432123456789098765432134567892345YTFBHJTRDFGHREDFG891Y1GUIAHZ TZGIUIZJ19YT7UFAJHVBSIASHUSHGC VNZIU HIUG IQHDU QDGYTQGHJBQHUIIGQDIHBJNQKS";
		const char* ac = "A";
		const char* bc = "B";
		const char* a_c = "A ";

		en::U32 a1 = en::Hash::ConstexprHash(azertysv);
		en::U32 a2 = en::Hash::ConstexprHash(azertyc);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::ConstexprHash(asv);
		a2 = en::Hash::ConstexprHash(ac);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::ConstexprHash(bsv);
		a2 = en::Hash::ConstexprHash(bc);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::ConstexprHash(a_sv);
		a2 = en::Hash::ConstexprHash(a_c);
		DOCTEST_CHECK(a1 == a2);
	}
}

DOCTEST_TEST_CASE("Meow32")
{
	DOCTEST_SUBCASE("Bytes")
	{
		// TODO : Test this
	}

	DOCTEST_SUBCASE("char*")
	{
		const en::U32 a = en::Hash::Meow32("A");
		const en::U32 b = en::Hash::Meow32("B");
		const en::U32 c = en::Hash::Meow32("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
		const en::U32 d = en::Hash::Meow32("A");
		const en::U32 e = en::Hash::Meow32("A ");

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("std::string")
	{
		const en::U32 a = en::Hash::Meow32(std::string("A"));
		const en::U32 b = en::Hash::Meow32(std::string("B"));
		const en::U32 c = en::Hash::Meow32(std::string("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U32 d = en::Hash::Meow32(std::string("A"));
		const en::U32 e = en::Hash::Meow32(std::string("A "));

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("std::string_view")
	{
		const en::U32 a = en::Hash::Meow32(std::string_view("A"));
		const en::U32 b = en::Hash::Meow32(std::string_view("B"));
		const en::U32 c = en::Hash::Meow32(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U32 d = en::Hash::Meow32(std::string_view("A"));
		const en::U32 e = en::Hash::Meow32(std::string_view("A "));

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("char* == std::string == std::string_view")
	{
		const en::U32 a = en::Hash::Meow32("A");
		const en::U32 b = en::Hash::Meow32("B");
		const en::U32 c = en::Hash::Meow32("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
		const en::U32 d = en::Hash::Meow32("A");
		const en::U32 e = en::Hash::Meow32("A ");

		const en::U32 as = en::Hash::Meow32(std::string("A"));
		const en::U32 bs = en::Hash::Meow32(std::string("B"));
		const en::U32 cs = en::Hash::Meow32(std::string("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U32 ds = en::Hash::Meow32(std::string("A"));
		const en::U32 es = en::Hash::Meow32(std::string("A "));

		const en::U32 asv = en::Hash::Meow32(std::string_view("A"));
		const en::U32 bsv = en::Hash::Meow32(std::string_view("B"));
		const en::U32 csv = en::Hash::Meow32(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U32 dsv = en::Hash::Meow32(std::string_view("A"));
		const en::U32 esv = en::Hash::Meow32(std::string_view("A "));

		DOCTEST_CHECK(a == as);
		DOCTEST_CHECK(b == bs);
		DOCTEST_CHECK(c == cs);
		DOCTEST_CHECK(d == ds);
		DOCTEST_CHECK(e == es);
		DOCTEST_CHECK(a == asv);
		DOCTEST_CHECK(b == bsv);
		DOCTEST_CHECK(c == csv);
		DOCTEST_CHECK(d == dsv);
		DOCTEST_CHECK(e == esv);
	}
}

DOCTEST_TEST_CASE("Meow64")
{
	DOCTEST_SUBCASE("Bytes")
	{
		// TODO : Test this
	}

	DOCTEST_SUBCASE("char*")
	{
		const en::U64 a = en::Hash::Meow64("A");
		const en::U64 b = en::Hash::Meow64("B");
		const en::U64 c = en::Hash::Meow64("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
		const en::U64 d = en::Hash::Meow64("A");
		const en::U64 e = en::Hash::Meow64("A ");

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("std::string")
	{
		const en::U64 a = en::Hash::Meow64(std::string("A"));
		const en::U64 b = en::Hash::Meow64(std::string("B"));
		const en::U64 c = en::Hash::Meow64(std::string("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U64 d = en::Hash::Meow64(std::string("A"));
		const en::U64 e = en::Hash::Meow64(std::string("A "));

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("std::string_view")
	{
		const en::U64 a = en::Hash::Meow64(std::string_view("A"));
		const en::U64 b = en::Hash::Meow64(std::string_view("B"));
		const en::U64 c = en::Hash::Meow64(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U64 d = en::Hash::Meow64(std::string_view("A"));
		const en::U64 e = en::Hash::Meow64(std::string_view("A "));

		DOCTEST_CHECK(a != 0);
		DOCTEST_CHECK(b != 0);
		DOCTEST_CHECK(c != 0);
		DOCTEST_CHECK(a != b);
		DOCTEST_CHECK(a != c);
		DOCTEST_CHECK(b != c);
		DOCTEST_CHECK(a == d);
		DOCTEST_CHECK(a != e);
	}

	DOCTEST_SUBCASE("char* == std::string == std::string_view")
	{
		const en::U64 a = en::Hash::Meow64("A");
		const en::U64 b = en::Hash::Meow64("B");
		const en::U64 c = en::Hash::Meow64("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
		const en::U64 d = en::Hash::Meow64("A");
		const en::U64 e = en::Hash::Meow64("A ");

		const en::U64 as = en::Hash::Meow64(std::string("A"));
		const en::U64 bs = en::Hash::Meow64(std::string("B"));
		const en::U64 cs = en::Hash::Meow64(std::string("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U64 ds = en::Hash::Meow64(std::string("A"));
		const en::U64 es = en::Hash::Meow64(std::string("A "));

		const en::U64 asv = en::Hash::Meow64(std::string_view("A"));
		const en::U64 bsv = en::Hash::Meow64(std::string_view("B"));
		const en::U64 csv = en::Hash::Meow64(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
		const en::U64 dsv = en::Hash::Meow64(std::string_view("A"));
		const en::U64 esv = en::Hash::Meow64(std::string_view("A "));

		DOCTEST_CHECK(a == as);
		DOCTEST_CHECK(b == bs);
		DOCTEST_CHECK(c == cs);
		DOCTEST_CHECK(d == ds);
		DOCTEST_CHECK(e == es);
		DOCTEST_CHECK(a == asv);
		DOCTEST_CHECK(b == bsv);
		DOCTEST_CHECK(c == csv);
		DOCTEST_CHECK(d == dsv);
		DOCTEST_CHECK(e == esv);
	}
}

DOCTEST_TEST_CASE("Combine hash")
{
	DOCTEST_SUBCASE("32")
	{
		const en::U32 a = en::Hash::Meow32("A");
		const en::U32 b = en::Hash::Meow32("B");
		const en::U32 c = en::Hash::Meow32("C");
		const en::U32 caa = en::Hash::Combine32(a, a);
		const en::U32 cab = en::Hash::Combine32(a, b);
		const en::U32 cba = en::Hash::Combine32(b, a);
		const en::U32 cbb = en::Hash::Combine32(b, b);

		DOCTEST_CHECK(caa != cab);
		DOCTEST_CHECK(caa != cba);
		DOCTEST_CHECK(caa != cbb);
		DOCTEST_CHECK(cab != cba);
		DOCTEST_CHECK(cab != cbb);
		DOCTEST_CHECK(cba != cbb);
	}

	DOCTEST_SUBCASE("64")
	{
		const en::U64 a = en::Hash::Meow64("A");
		const en::U64 b = en::Hash::Meow64("B");
		const en::U64 c = en::Hash::Meow64("C");
		const en::U64 caa = en::Hash::Combine64(a, a);
		const en::U64 cab = en::Hash::Combine64(a, b);
		const en::U64 cba = en::Hash::Combine64(b, a);
		const en::U64 cbb = en::Hash::Combine64(b, b);

		DOCTEST_CHECK(caa != cab);
		DOCTEST_CHECK(caa != cba);
		DOCTEST_CHECK(caa != cbb);
		DOCTEST_CHECK(cab != cba);
		DOCTEST_CHECK(cab != cbb);
		DOCTEST_CHECK(cba != cbb);
	}
}