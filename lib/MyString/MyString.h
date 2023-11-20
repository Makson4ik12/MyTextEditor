#pragma once

#ifndef MYSTRING_H
#define MYSTRING_H

#ifdef MYSTRING_EXPORTS
    #define MYSTRING_API __declspec(dllexport)
#else
    #define MYSTRING_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <cstring>

class MYSTRING_API MyString {
private:
    /**
     * pointer to char array of MyString object
    */
    char* mystring_buffer_;

    /**
     *  length of MyString's chars buffer
     */
    size_t buffer_length_;

    /**
     * the current amount of allocated memory for array
     */
    size_t capacity_value_;

    /**
     *  the sizeof one cluster of MyString's buffer
     */
    const size_t cBufferClasterSize = 64;

    /**
     * @return enough amount of bytes to allocate buffer
    */
    size_t initCapacity(size_t buffer_size);

    /**
     * throw exception if expression is false
    */
    void validateArgs(bool args_expression, const char* error_message);

public:
    /**
     * default constructor
    */
    MyString();

    /**
     * char array constructor
     * @param str pointer to char array
     */
    MyString(const char* str);

    /**
     * initializer list constructor
     * @param list initializer_list<char> as a initializing value
     */
    MyString(const std::initializer_list<char> list);

    /**
     * std::string constructor
     * @param str std::string as a initializing value
     */
    MyString(const std::string str);

    /**
     * init class with count characters of  �char string�
     * @param str pointer to char array
     * @param count count characters
     */
    MyString(const char* str, const int count);

    /**
     * init class with count of characters
     * @param count count of characters
     * @param c character
     */
    MyString(const int count, const char c);

    /**
     * copy constructor
     * @param other constructor for copying
     */
    MyString(const MyString& other);

    /**
     * destructor
     */
    ~MyString();

    /**
     * concatenate with Mystring
     * @param other other MyString object to concatenate
     */
    MyString operator+(const MyString& other);

    /**
     * concatenate with char array
     * @param str char array's pointer to concatenate
     */
    MyString operator+(const char* str);

    /**
     * concatenate with std::string
     * @param str std::string to concatenate
     */
    MyString operator+(const std::string str);

    /**
     * assignment concatenate with char array
     * @param str char array's pointer to assignment concatenate
     */
    MyString& operator+=(const char* str);

    /**
     * assignment concatenate with std::string
     * @param str std::string to assignment concatenate
     */
    MyString& operator+=(const std::string str);

    /**
     * assignment concatenate with MyString
     * @param other MyString to assignment concatenate
     */
    MyString& operator+=(const MyString& other);

    /**
     * MyString assignment
     * @param other MyString to assignment
     */
    MyString& operator=(const MyString& other);

    /**
     * char array assignment
     * @param str char array to assignment
     */
    MyString& operator=(const char* str);

    /**
     * std::string assignment
     * @param str std::string to assignment
     */
    MyString& operator=(const std::string str);

    /**
     * char assignment
     * @param c char to assignment
     */
    MyString& operator=(const char c);

    /**
     * index operator
     * @param index index of MyString' char
     */
    const char operator[](const int index);

    /**
     * > compare
     * @param other MyString to compare
     */
    const bool operator>(const MyString& other);

    /**
     * < compare
     * @param other MyString to compare
     */
    const bool operator<(const MyString& other);

    /**
     * >= compare
     * @param other MyString to compare
     */
    const bool operator>=(const MyString& other);

    /**
     * <= compare
     * @param other MyString to compare
     */
    const bool operator<=(const MyString& other);

    /**
     * 'not equal to' compare
     * @param other MyString to compare
     */
    const bool operator!=(const MyString& other);

    /**
     * == compare
     * @param other MyString to compare
     */
    const bool operator==(const MyString& other);

    /**
     * @return a pointer to null-terminated character array
     */
    const char* c_str();

    /**
     * @return a pointer to array data that not required to be null-terminated
     */
    const char* data();

    /**
     * @return the number of char elements in string
     */
    const size_t length();

    /**
     * @return the number of char elements in string
     */
    const size_t size();

    /**
     * @return true if string is empty
     */
    const bool empty();

    /**
     * @return the current amount of allocated memory for array
     */
    const size_t capacity();

    /**
     * reduce the capacity to size
     */
    void shrink_to_fit();

    /**
     * remove all char element in string
     */
    void clear();

    /**
     * << operator
     */
    friend MYSTRING_API std::ostream& operator<<(std::ostream& out, const MyString& input);

    /**
     * >> operator
     */
    friend MYSTRING_API std::istream& operator>>(std::istream& in, MyString& input);

    /**
     * insert count of char in index position
     * @param index index of position
     * @param count amount of char
     * @param c char to insert
     */
    void insert(int index, const int count, const char c);

    /**
     * insert null-terminated char string at index position
     * @param index index of position
     * @param str char* to insert
     */
    void insert(int index, const char* str);

    /**
     * insert count of null-terminated char string at index position
     * @param index index of position
     * @param str char* to insert
     * @param count amount chars from char* to insert
     */
    void insert(int index, const char* str, const int count);

    /**
     * insert std::string at index position
     * @param index index of position
     * @param str std::string to insert
     */
    void insert(int index, const std::string str);

    /**
     * insert count of std::string at index position
     * @param index index of position
     * @param str std::string to insert
     * @param count amount chars from char* to insert
     */
    void insert(int index, const std::string str, const int count);

    /**
     * erase count of char at index position
     * @param index index of position
     * @param count amount chars to erase
     */
    void erase(int index, const int count);

    /**
     * append count of char
     * @param count amount chars to append
     * @param c char to append
     */
    void append(const int count, const char c);

    /**
    * append null-terminated char string
    * @param str char* to append
    */
    void append(const char* str);

    /**
     * append a count of null-terminated char string by index position
     * @param str char* to append
     * @param index index to append
     * @param count amount of char* to append
     */
    void append(const char* str, int index, const int count);

    /**
     * append std::string
     * @param str std::string to append
     */
    void append(const std::string str);

    /**
     * append a count of std::string by index position
     * @param str std::string to append
     * @param index index to append
     * @param count amount of std::string's chars to append
     */
    void append(const std::string str, int index, const int count);

    /**
     * replace a count of char at index by �string�
     * @param index index to append
     * @param count amount of chars to replace
     * @param str char* to replace
     */
    void replace(int index, const int count, const char* str);

    /**
     * replace a count of char at index by std::string
     * @param index index to append
     * @param count amount of chars to replace
     * @param str std::string to replace
     */
    void replace(int index, const int count, const std::string str);

    /**
     * @return a substring starts with index position
     * @param index start of substring
     */
    MyString substr(int index);

    /**
    * @return a count of substring�s char starts with index position
    * @param index start of substring
    * @param count count of substring�s char
    */
    MyString substr(int index, const int count);

    /**
     * @return if founded return the index of substring
     * @param str char* to found
     */
    const int find(const char* str);

    /**
     * @return if founded return the index of substring
     * @param str char* to found
     * @param index index to start search
     */
    const int find(const char* str, int index);

    /**
     * @return if founded return the index of substring
     * @param str std::string to found
     */
    const int find(const std::string str);

    /**
    * @return if founded return the index of substring
    * @param str std::string to found
    * @param index index to start search
    */
    const int find(const std::string str, int index);
};

#endif