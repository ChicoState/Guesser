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

// Constructor tests

TEST(GuesserTest, constructor_normal_secret)
{
	Guesser guesser("secret");
	ASSERT_TRUE(guesser.match("secret"));
}

TEST(GuesserTest, constructor_empty_secret)
{
	Guesser guesser("");
	ASSERT_TRUE(guesser.match(""));
}

TEST(GuesserTest, constructor_truncates_long_secret)
{
	// Secret longer than 32 chars should be truncated to exactly 32
	string long_secret = "this_is_a_very_long_secret_phrase_that_exceeds_32_characters";
	Guesser guesser(long_secret);
	// Verify the secret was truncated to 32 chars by checking distance
	string first_32 = long_secret.substr(0, 32);
	ASSERT_EQ(0, guesser.distance(first_32));
}

TEST(GuesserTest, constructor_exactly_32_chars)
{
	string secret_32 = "12345678901234567890123456789012"; // exactly 32
	Guesser guesser(secret_32);
	ASSERT_TRUE(guesser.match(secret_32));
}

// distance() tests

TEST(GuesserTest, distance_identical_strings)
{
	Guesser guesser("hello");
	ASSERT_EQ(0, guesser.distance("hello"));
}

TEST(GuesserTest, distance_completely_different)
{
	Guesser guesser("aaaaa");
	ASSERT_EQ(5, guesser.distance("bbbbb"));
}

TEST(GuesserTest, distance_one_char_different)
{
	Guesser guesser("hello");
	ASSERT_EQ(1, guesser.distance("hallo"));
}

TEST(GuesserTest, distance_guess_longer_than_secret)
{
	Guesser guesser("hi");
	ASSERT_EQ(2, guesser.distance("hello")); // capped at secret length
}

TEST(GuesserTest, distance_guess_shorter_than_secret)
{
	Guesser guesser("hello");
	ASSERT_EQ(4, guesser.distance("hi"));
}

TEST(GuesserTest, distance_empty_guess)
{
	Guesser guesser("hello");
	ASSERT_EQ(5, guesser.distance(""));
}

TEST(GuesserTest, distance_much_longer_guess)
{
	Guesser guesser("short");
	// Guess is 20 chars longer, but distance capped at secret length (5)
	ASSERT_EQ(5, guesser.distance("verylongguessthatiswaytoobig"));
}

TEST(GuesserTest, distance_all_matching_prefix)
{
	Guesser guesser("test");
	ASSERT_EQ(0, guesser.distance("test"));
}

TEST(GuesserTest, distance_no_overlap_different_lengths)
{
	Guesser guesser("abc");
	ASSERT_EQ(3, guesser.distance("xyz"));
}

// match() tests - Basic functionality

TEST(GuesserTest, match_correct_guess_first_try)
{
	Guesser guesser("password");
	ASSERT_TRUE(guesser.match("password"));
}

TEST(GuesserTest, match_wrong_guess)
{
	Guesser guesser("secret");
	ASSERT_FALSE(guesser.match("wrong"));
}

TEST(GuesserTest, match_case_sensitive)
{
	Guesser guesser("Secret");
	ASSERT_FALSE(guesser.match("secret")); // lowercase should fail
}

// match() tests - Remaining guesses logic

TEST(GuesserTest, match_three_wrong_guesses_locks)
{
	Guesser guesser("secret");
	// All guesses within distance 2
	ASSERT_FALSE(guesser.match("secrat")); // distance 1, remaining: 2
	ASSERT_FALSE(guesser.match("secrit")); // distance 1, remaining: 1
	ASSERT_FALSE(guesser.match("secrot")); // distance 1, remaining: 0
	// Fourth guess should fail even if correct (locked)
	ASSERT_FALSE(guesser.match("secret"));
}

TEST(GuesserTest, match_correct_after_two_wrong)
{
	Guesser guesser("secret");
	ASSERT_FALSE(guesser.match("secrat")); // distance 1
	ASSERT_FALSE(guesser.match("secrit")); // distance 1
	ASSERT_TRUE(guesser.match("secret"));  // correct on 3rd try
}

// match() tests - Brute force detection (distance > 2)

TEST(GuesserTest, match_brute_force_locks_immediately)
{
	Guesser guesser("secret");
	// Guess with distance > 2 should lock immediately
	ASSERT_FALSE(guesser.match("xxxxxx")); // distance 6 > 2, LOCKED
	// Even correct guess should now fail
	ASSERT_FALSE(guesser.match("secret"));
}

TEST(GuesserTest, match_distance_exactly_2_does_not_lock)
{
	Guesser guesser("abcdef");
	// Distance exactly 2 should NOT lock
	ASSERT_FALSE(guesser.match("XXcdef")); // distance 2
	// Should still be able to guess (not locked)
	ASSERT_TRUE(guesser.match("abcdef"));
}

TEST(GuesserTest, match_distance_3_locks)
{
	Guesser guesser("abcdef");
	ASSERT_FALSE(guesser.match("XXXdef")); // distance 3 > 2, locks
	ASSERT_FALSE(guesser.match("abcdef")); // locked, can't match
}

TEST(GuesserTest, match_brute_force_still_decrements_remaining)
{
	Guesser guesser("secret");
	// First brute force locks it
	ASSERT_FALSE(guesser.match("xxxxxx")); // locks
	// Make two more wrong guesses - remaining should still count down
	ASSERT_FALSE(guesser.match("yyyyyy")); 
	ASSERT_FALSE(guesser.match("zzzzzz"));
	// Even if we somehow unlocked, we'd be out of guesses
	ASSERT_FALSE(guesser.match("secret"));
}

// match() tests - Edge cases

TEST(GuesserTest, match_empty_guess_on_empty_secret)
{
	Guesser guesser("");
	ASSERT_TRUE(guesser.match(""));
}

TEST(GuesserTest, match_empty_guess_on_nonempty_secret)
{
	Guesser guesser("secret");
	// Empty guess has distance 6, should lock
	ASSERT_FALSE(guesser.match(""));
	ASSERT_FALSE(guesser.match("secret")); // locked
}

TEST(GuesserTest, match_very_long_wrong_guess)
{
	Guesser guesser("hi");
	// Very long guess - distance capped at 2 (secret length)
	ASSERT_FALSE(guesser.match("verylongincorrectguess"));
	// Distance is 2, so should NOT lock
	ASSERT_TRUE(guesser.match("hi"));
}

TEST(GuesserTest, match_single_char_secret)
{
	Guesser guesser("x");
	ASSERT_FALSE(guesser.match("y")); // distance 1
	ASSERT_TRUE(guesser.match("x"));
}

// match() tests - Combined scenarios

TEST(GuesserTest, match_alternating_close_guesses)
{
	Guesser guesser("password");
	ASSERT_FALSE(guesser.match("passwerd")); // distance 1
	ASSERT_FALSE(guesser.match("passward")); // distance 1
	ASSERT_FALSE(guesser.match("passwird")); // distance 1
	// Out of guesses
	ASSERT_FALSE(guesser.match("password"));
}

TEST(GuesserTest, match_locked_stays_locked)
{
	Guesser guesser("test");
	// Lock via brute force
	ASSERT_FALSE(guesser.match("xxxx")); // locks
	// Try multiple correct guesses - should all fail
	ASSERT_FALSE(guesser.match("test"));
	ASSERT_FALSE(guesser.match("test"));
	ASSERT_FALSE(guesser.match("test"));
}

TEST(GuesserTest, match_distance_boundary_testing)
{
	Guesser guesser("12345");
	// Distance 1: should NOT lock
	ASSERT_FALSE(guesser.match("X2345"));
	ASSERT_FALSE(guesser.match("1X345")); 
	ASSERT_FALSE(guesser.match("12X45"));
	// Out of guesses but NOT locked (all distance ≤ 2)
	ASSERT_FALSE(guesser.match("12345"));
}

// Comprehensive coverage tests

TEST(GuesserTest, match_special_characters)
{
	Guesser guesser("p@$$w0rd!");
	ASSERT_TRUE(guesser.match("p@$$w0rd!"));
}

TEST(GuesserTest, match_numbers_only)
{
	Guesser guesser("123456");
	ASSERT_FALSE(guesser.match("123457")); // distance 1
	ASSERT_TRUE(guesser.match("123456"));
}

TEST(GuesserTest, match_whitespace_in_secret)
{
	Guesser guesser("hello world");
	ASSERT_TRUE(guesser.match("hello world"));
}

TEST(GuesserTest, distance_partial_overlap)
{
	Guesser guesser("testing");
	// "test" matches first 4, then 3 chars missing
	ASSERT_EQ(3, guesser.distance("test"));
}
