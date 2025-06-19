# pragma once

# ifndef _File_HPP_
# define _File_HPP_ 1

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

    File(const std::string_view &file_name, Mode mode)
    :   file_name_(file_name), mode_(mode)
    {
        std::ios_base::openmode mode__ = std::ios::in | std::ios::binary; //(mode == Mode::Read) ? std::ios::in | std::ios::trunc : std::ios::out | std::ios::trunc;
        this->curr_file_.open(file_name.data(), mode__);
        if (!this->curr_file_.is_open()) {
            throw std::runtime_error("Failed to open file: " + this->file_name_);
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
        
        if (offset > 0) {
            this->curr_file_.seekg(offset, std::ios::beg);
            if (this->curr_file_.tellg() != offset) {
                return Result<std::streamsize>::error("Failed to seek in file:: " + std::to_string(offset) + "+" + std::to_string(size));
            }
        }
        // char buffer_[8192];
        // std::streamsize bytes_read = 
        this->curr_file_.read(static_cast<char *>(buffer), 100);
        // if (bytes_read < size || bytes_read < 0) {
        //     return Result<std::streamsize>::error("Error reading from file:: " + std::to_string(bytes_read) + " bytes read, expected " + std::to_string(size));
        // }
        std::cout << "Read " << ((std::uint64_t *)buffer)[0] << std::endl;
        return 8192;// bytes_read;
    }

    void close() {
        if (this->curr_file_.is_open()) {
            this->curr_file_.close();
        }
    }

    void print() const {
        std::cout << "File Name: " << this->file_name_ << "\n"
                  << "File Size: " << this->file_size_ << " bytes\n"
                  << "Mode: " << (this->mode_ == Mode::Read ? "Read" : "Write") << std::endl;
    }

};

# endif // _FILE_HPP_
