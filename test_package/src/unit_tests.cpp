#include "gtest/gtest.h"
#include "Book.h"
#include "unordered_set"
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <array>
#include <algorithm>

using namespace std;
using namespace chrono;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//
// UNIT TESTS
//
TEST(Movies, GetOne)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");

    EXPECT_TRUE(booker.getMovies() == unordered_set<string> { "movie1" });
}

TEST(Movies, GetMany)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.addTheatre("theatre2", "movie2");
    booker.addTheatre("theatre3", "movie3");

    EXPECT_TRUE((booker.getMovies() == unordered_set<string> { "movie1", "movie2", "movie3" }));
}

TEST(Movies, OneDuplicate)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.addTheatre("theatre2", "movie2");
    booker.addTheatre("theatre3", "movie1");

    EXPECT_TRUE((booker.getMovies() == unordered_set<string> { "movie1", "movie2" }));
}

TEST(Movies, ManyDuplicates)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.addTheatre("theatre2", "movie2");
    booker.addTheatre("theatre3", "movie1");
    booker.addTheatre("theatre4", "movie3");
    booker.addTheatre("theatre5", "movie3");
    booker.addTheatre("theatre6", "movie3");
    booker.addTheatre("theatre7", "movie4");

    EXPECT_TRUE(
        (booker.getMovies() == unordered_set<string> { "movie1", "movie2", "movie3", "movie4" }));
}

TEST(Theatres, GetOne)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");

    EXPECT_TRUE(booker.getTheatres("movie1"s) == unordered_set<string> { "theatre1" });
}

TEST(Theatres, GetMany)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.addTheatre("theatre2", "movie2");
    booker.addTheatre("theatre3", "movie1");

    EXPECT_TRUE(
        (booker.getTheatres("movie1"s) == unordered_set<string> { "theatre1", "theatre3" }));
}

TEST(Seats, Get)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");

    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, false, false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, false }));
}

TEST(Book, OneSeat)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.bookSeats("theatre1", unordered_set<int> { 1 });

    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, false }));
}

TEST(Book, ManySeats)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.bookSeats("theatre1", unordered_set<int> { 1, 3, 19 });

    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, true,  false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, true }));
}

TEST(Book, FailedOneSeat)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.bookSeats("theatre1", unordered_set<int> { 1 });

    EXPECT_TRUE(booker.bookSeats("theatre1"s, unordered_set<int> { 1 }) == false);
    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, false }));
}

TEST(Book, FailedManySeats)
{
    Book booker {};
    booker.addTheatre("theatre1", "movie1");
    booker.bookSeats("theatre1", unordered_set<int> { 1, 3, 19 });

    EXPECT_TRUE(booker.bookSeats("theatre1"s, unordered_set<int> { 1, 2, 3, 4 }) == false);
    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, true,  false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, true }));
}

TEST(Book, ConcurrentOneSeat)
{
    auto numBooks { 0 };
    mutex numBooksMutex {};

    Book booker {};
    booker.addTheatre("theatre1", "movie1");

    // Tools to generate random delays uniformly distributed between 0.1 and 5 ms
    // std::mt19937: 32-bit Mersenne twister with 19.937 bits of internal state
    random_device rd {};
    array<int, mt19937::state_size> seed_data {};
    generate(begin(seed_data), end(seed_data), ref(rd));
    seed_seq seq(begin(seed_data), end(seed_data));
    auto mtgen { mt19937 { seq } };
    auto udistrib { uniform_int_distribution<> { 100, 5000 } };

    // Start 100 threads with random delays to force booking collisions
    auto numThreads { 100 };
    vector<thread> threads(numThreads);
    for (int i { 0 }; i < numThreads; ++i) {
        auto delay { udistrib(mtgen) };
        threads[i] = thread { [&]() {
            this_thread::sleep_for(microseconds(delay));
            bool success = booker.bookSeats("theatre1", unordered_set<int> { 1 });

            {
                lock_guard<mutex> lock(numBooksMutex);
                if (success) {
                    numBooks++;
                }
            }
        } };
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_TRUE(numBooks == 1);
    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, false }));
}

TEST(Book, ConcurrentManySeat)
{
    auto numBooks { 0 };
    mutex numBooksMutex {};

    Book booker {};
    booker.addTheatre("theatre1", "movie1");

    // Tools to generate random delays uniformly distributed between 0.1 and 5 ms
    // std::mt19937: 32-bit Mersenne twister with 19.937 bits of internal state
    random_device rd {};
    array<int, mt19937::state_size> seed_data {};
    generate(begin(seed_data), end(seed_data), ref(rd));
    seed_seq seq(begin(seed_data), end(seed_data));
    auto mtgen { mt19937 { seq } };
    auto udistrib { uniform_int_distribution<> { 100, 5000 } };

    // Start 100 threads with random delays to force booking collisions
    auto numThreads { 100 };
    vector<thread> threads(numThreads);
    for (int i { 0 }; i < numThreads; ++i) {
        auto delay { udistrib(mtgen) };
        threads[i] = thread { [&]() {
            this_thread::sleep_for(microseconds(delay));
            bool success = booker.bookSeats("theatre1", unordered_set<int> { 1, 3, 19 });

            {
                lock_guard<mutex> lock(numBooksMutex);
                if (success) {
                    numBooks++;
                }
            }
        } };
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_TRUE(numBooks == 1);
    EXPECT_TRUE(
        (booker.getSeats("theatre1"s) ==
         vector<bool> { false, true,  false, true,  false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, true }));
}
