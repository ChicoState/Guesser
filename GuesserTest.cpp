/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

// distance() tests
TEST(GuesserTest, distance)
{
	Guesser g("A");
	int r = g.distance("A");
	ASSERT_EQ(r, 0);
}
TEST(GuesserTest, no_guess)
{
        Guesser g("A");
        int r = g.distance("");
        ASSERT_EQ(r, 1);
}
TEST(GuesserTest, less)
{
        Guesser g("AAA");
        int r = g.distance("A");
        ASSERT_EQ(r, 2);
}
TEST(GuesserTest, more)
{
        Guesser g("A");
        int r = g.distance("AAA");
        ASSERT_EQ(r, 2);
}
TEST(GuesserTest, too_long)
{
        Guesser g("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
        int r = g.distance("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEF");
        ASSERT_EQ(r, 0);
}

// match() tests
TEST(GuesserTest, match)
{
        Guesser g("A");
        bool r = g.match("A");
        ASSERT_EQ(r, 1);
}
TEST(GuesserTest, offBy1)
{
        Guesser g("A");
        bool r = g.match("AB");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, offBy2)
{
        Guesser g("A");
        bool r = g.match("ABC");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, offBy3)
{                                                                                                               Guesser g("A");
        bool r = g.match("BCD");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, no_guess_match)
{                                                                                                               Guesser g("A");
        bool r = g.match("");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, too_many_tries)
{                                                                                                               Guesser g("A");
        bool r = g.match("B");
	r = g.match("B");
	r = g.match("B");
	r = g.match("B");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, too_many_tries_then_correct)
{                                                                                                               Guesser g("A");
        bool r = g.match("B");
        r = g.match("B");
        r = g.match("B");
        r = g.match("A");
        ASSERT_EQ(r, 0);
}
TEST(GuesserTest, way_off)
{                                                                                                               Guesser g("ABCDE");
	bool r = g.match("BKDSLNKL");
        r = g.match("B");
        r = g.match("B");
        r = g.match("A");
        ASSERT_EQ(r, 0);
}
