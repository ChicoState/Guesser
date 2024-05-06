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

TEST(GuesserTest, smoke_test)
{
    ASSERT_EQ( 1+1, 2 );
}

TEST(GuesserTest, matchCorrectGuess) {
    Guesser guesser("open");
    guesser.match("open");
    ASSERT_EQ(guesser.remaining(), 3);
    ASSERT_TRUE(guesser.match("open"));
}

TEST(GuesserTest, matchIncorrectGuess) {
    Guesser guesser("secret");
    guesser.match("wrong");
    ASSERT_EQ(guesser.remaining(), 2);
    ASSERT_FALSE(guesser.match("wrong"));
}

TEST(GuesserTest, allIncorrectGuess) {
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("wrong")); 
    ASSERT_EQ(guesser.remaining(), 2); 
    ASSERT_FALSE(guesser.match("wrong"));
    ASSERT_EQ(guesser.remaining(), 1);
    ASSERT_FALSE(guesser.match("wrong")); 
    ASSERT_EQ(guesser.remaining(), 0);
}

TEST(GuesserTest, distanceGuess) {
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("complex"));
}

TEST(GuesserTest, guesserLock) {
    Guesser guesser("secret");
    guesser.match("wrong1"); 
    guesser.match("wrong2"); 
    guesser.match("wrong3");
    ASSERT_EQ(guesser.remaining(), 0);
    guesser.match("wrong4");
    ASSERT_EQ(guesser.remaining(), 0);
}

