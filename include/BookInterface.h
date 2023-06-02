#pragma once
#include <unordered_set>
#include <vector>
#include <string>

class BookInterface
{
public:
    /* Default destructor. */
    virtual ~BookInterface() = default;

    /**
     * Gets the playing movies.
     * @return The names of the movies as a set of movie names.
     */
    virtual std::unordered_set<std::string> getMovies() const = 0;

    /**
     * Gets the threatres playing a specific movie.
     * @param movieName The name of the movie as a string.
     * @return The names of the theatres as a set of theatre names.
     */
    virtual std::unordered_set<std::string> getTheatres(std::string const &movieName) const = 0;

    /**
     * Gets the status of all seats in a theatre. Note that all theatres have 20 seats with
     * identifiers ranging from 0 to 19.
     * @param theatreName The name of the theatre as a string.
     * @return A vector of seat statuses: free (false) or taken (true). Empty vector if the theatre
     * does not exist.
     */
    virtual std::vector<bool> getSeats(std::string const &theatreName) const = 0;

    /**
     * Books seats in a theatre. The seats will be all booked if they are available or none will be
     * booked at all. There are no partial bookings. Note that all theatres have 20 seats identified
     * by integers from 0 to 19.
     * @param theatreName The name of the theatre as a string.
     * @param seatIds The seats to book as a set of seat identifiers.
     * @return The success of the booking.
     */
    virtual bool bookSeats(std::string const &theatreName,
                           std::unordered_set<int> const &seatIds) = 0;
};
