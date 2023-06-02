#include "Book.h"
#include <unordered_set>
#include <mutex>

using namespace std;

unordered_set<string> Book::getMovies() const
{
    unordered_set<string> movieNames {};
    for (auto const& theatre : theatres) {
        movieNames.insert(theatre.second.movie);
    }

    return movieNames;
}

unordered_set<string> Book::getTheatres(string const& movieName) const
{
    unordered_set<string> theatreNames {};
    for (auto const& theatre : theatres) {
        if (theatre.second.movie == movieName) {
            theatreNames.insert(theatre.first);
        }
    }

    return theatreNames;
}

vector<bool> Book::getSeats(string const& theatreName) const
{
    auto&& it { theatres.find(theatreName) };
    if (it != theatres.end()) {
        return it->second.seats;
    } else {
        return vector<bool> {};
    }
}

bool Book::bookSeats(string const& theatreName, unordered_set<int> const& seatIds)
{
    auto success { false };

    auto&& it { theatres.find(theatreName) };
    if (it != theatres.end()) {
        success = true;
        auto& theatreSeats { it->second.seats };

        {
            // Avoid overbooking: prevent concurrent access to the seats of a specific theatre
            lock_guard<mutex> lock(it->second.seatsMutex);

            for (auto const& id : seatIds) {
                if (theatreSeats[id]) {
                    success = false;
                    break;
                }
            }

            if (success) {
                for (auto const& id : seatIds) {
                    theatreSeats[id] = true;
                }
            }
        }
    }

    return success;
}
