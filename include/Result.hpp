# pragma once

# ifndef _RESULT_HPP_
# define _RESULT_HPP_ 1

# include <stdexcept>
# include <string>

template<class value_type>
class Result {

private:

    value_type value_;
    
    bool success_;

    std::string error_message_;

    Result()
    :   success_(false), error_message_("")
    {}
    
public:

    Result(const value_type& value)
    :   value_(value), success_(true), error_message_("")
    {}

    Result(const Result<value_type>& other) 
    :   value_(other.value_), success_(other.success_), error_message_(other.error_message_)
    {}

    Result(Result<value_type>&& other) noexcept
    :   value_(std::move(other.value_)), success_(other.success_), error_message_(std::move(other.error_message_))
    {
        other.success_ = false; // Invalidate the moved-from object
    }

    static Result<value_type> error(const std::string_view& error_message) {
        Result<value_type> ret = Result();
        ret.error_message_ = error_message;
        return ret;
    }

    bool is_success() const {
        return success_;
    }

    bool is_error() const {
        return !success_;
    }

    const value_type& get_value() const {
        if (!success_) {
            throw std::runtime_error("Attempted to access value of a failed Result");
        }
        return value_;
    }

    const std::string& get_error() const {
        return error_message_;
    }

};

# endif // Result.hpp
