# pragma once

# ifndef _FILE_HH_
# define _FILE_HH_ 1

# include <filesystem>

namespace File {

    // Function to check if a file exists
    inline bool exists(const std::filesystem::path& path) {
        return std::filesystem::exists(path);
    }

    // Function to get the size of a file
    inline std::uintmax_t size(const std::filesystem::path& path) {
        return std::filesystem::file_size(path);
    }

} // namespace File



# endif

