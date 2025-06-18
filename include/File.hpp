# pragma once

# ifndef _File_HPP_
# define _File_HPP_ 1
# define _FILE_OFFSET_BITS 64

# include <iostream>
# include <filesystem>
# include <string>
# include <fstream>

# include "Result.hpp"


class File {

public:

    enum class Mode {
        Read,
        Write
    };

private:

    std::string file_name_;

    Mode mode_;

    std::ifstream curr_file_;

    std::uintmax_t file_size_;

public:

    File(const std::string &file_name, Mode mode)
    :   file_name_(file_name), mode_(mode)
    {
        std::ios_base::openmode mode__ = (mode == Mode::Read) ? std::ios::in : std::ios::out | std::ios::trunc;
        this->curr_file_.open(file_name, mode__);
        if (!this->curr_file_.is_open()) {
            throw std::runtime_error("Failed to open file: " + file_name);
        }
        this->file_size_ = std::filesystem::file_size(file_name);
    }

    ~File() {
        this->close();
    }

    const std::string &get_file_name() const {
        return this->file_name_;
    }

    std::uintmax_t file_size() const {
        return this->file_size_;
    }

    Result<std::streamsize> read(void *buffer, std::streamsize size, std::streamoff offset = -1) {
        if (this->mode_ != Mode::Read) {
            return Result<std::streamsize>::error("File not opened in read mode");
        }
        
        if (offset >= 0) {
            this->curr_file_.seekg(offset, std::ios::beg);
            if (this->curr_file_.fail()) {
                return Result<std::streamsize>::error("Failed to seek in file");
            }
        }
        
        std::streamsize bytes_read = this->curr_file_.readsome(static_cast<char *>(buffer), size);
        if (bytes_read < size || bytes_read < 0) {
            return Result<std::streamsize>::error("Error reading from file");
        }
        return bytes_read;
    }

    void close() {
        if (this->curr_file_.is_open()) {
            this->curr_file_.close();
        }
    }

};

# endif // _FILE_HPP_
