#include <cstring>
#include "mystring.h"

// ----------------------------CONSTRUCTORS-------------------------------------//

MyString::MyString() : mystring_buffer_(nullptr), buffer_length_(0), capacity_value_(0) {}

MyString::MyString(const char* str) : MyString::MyString(str, strlen(str)) {}

MyString::MyString(const std::initializer_list<char> list)  : MyString::MyString()
{
    buffer_length_ = list.size();
    capacity_value_ = initCapacity(buffer_length_);
    mystring_buffer_ = new char[capacity_value_];

    for(int i = 0; i < buffer_length_; i++)
        mystring_buffer_[i] = *(list.begin() + i);
    mystring_buffer_[buffer_length_] = '\0';
}

MyString::MyString(const std::string str) : MyString::MyString(str.c_str()) {}

MyString::MyString(const char* str, const int count) : MyString::MyString()
{
    validateArgs (strlen(str) < count, "Exception: length of const char* less than count");

    buffer_length_ = count;
    capacity_value_ = initCapacity(buffer_length_);
    mystring_buffer_ = new char[capacity_value_];

    for (int i = 0; i < buffer_length_; i++) 
        mystring_buffer_[i] = str[i];
    mystring_buffer_[buffer_length_] = '\0';
}

MyString::MyString(const int count, const char c) : MyString::MyString()
{
    validateArgs (count < 0, "Exception: count must be more then or equal zero");

    buffer_length_ = count;
    capacity_value_ = initCapacity(buffer_length_);
    mystring_buffer_ = new char[capacity_value_];

    for (int i = 0; i < buffer_length_; i++)
        mystring_buffer_[i] = c;
    mystring_buffer_[buffer_length_] = '\0';
}

MyString::MyString(const MyString &other): MyString::MyString(other.mystring_buffer_) {}   

MyString::~MyString()
{
    delete[] mystring_buffer_;
}

//-------------------------------OPERATORS--------------------------------------//

MyString MyString::operator+(const MyString& other)
{
    return *this + other.mystring_buffer_;
}

MyString MyString::operator+(const char* str)
{
    MyString mystring_for_return(this->mystring_buffer_);
    mystring_for_return.append(str);
    return mystring_for_return;
}

MyString MyString::operator+(const std::string str)
{
    return *this + str.c_str();
}

MyString& MyString::operator+=(const char* str)
{
   *this = *this + str;
    return *this;
}

MyString& MyString::operator+=(const std::string str)
{
   *this = *this + str;
    return *this;
}

MyString& MyString::operator+=(const MyString& other)
{
    *this = *this + other.mystring_buffer_;
    return *this;
}

MyString& MyString::operator=(const MyString& other)
{
    *this = other.mystring_buffer_;
    return *this;
}

MyString& MyString::operator=(const char* str) 
{
    this->clear();
    this->append(str);
    return *this;
}

MyString& MyString::operator=(const std::string str)
{
    *this = str.c_str();
    return *this;
}

MyString& MyString::operator=(const char c) 
{
    this->clear();
    this->append(1, c);

    return *this;
}

const char MyString::operator[](const int index) 
{
    validateArgs((index < 0) || (index >= buffer_length_), "Exception: index out of range");
    return mystring_buffer_[index];
}

//----------------------------BOOL OPERATORS------------------------------------//

const bool MyString::operator>(const MyString& other) 
{
    return buffer_length_ > other.buffer_length_ ? true : false;
}

const bool MyString::operator<(const MyString& other) 
{
    return buffer_length_ < other.buffer_length_ ? true : false;
}

const bool MyString::operator>=(const MyString& other) 
{
    return buffer_length_ >= other.buffer_length_ ? true : false;
}

const bool MyString::operator<=(const MyString& other) 
{
    return buffer_length_ <= other.buffer_length_ ? true : false;
}

const bool MyString::operator!=(const MyString& other) 
{
    return *this == other ? false : true;
}

const bool MyString::operator==(const MyString& other) 
{
    if (buffer_length_ == other.buffer_length_) {
        for (int i = 0; i < buffer_length_; i++)
            if (mystring_buffer_[i] != other.mystring_buffer_[i]) return false;
    } else {
        return false;
    }

    return true;
}

//-------------------------------FUNCTIONS--------------------------------------//

const char* MyString::c_str()
{
    if (mystring_buffer_) {
        return mystring_buffer_;
    } else {
        return "";
    }
}

const char* MyString::data() 
{
    return this->c_str();
}

const size_t MyString::length()
{
    return buffer_length_;
}

const size_t MyString::size()
{
    return buffer_length_;
}

const bool MyString::empty()
{
    return buffer_length_ > 0 ? false : true;
}

const size_t MyString::capacity()
{
    return capacity_value_;
}

void MyString::shrink_to_fit() 
{
    if (capacity_value_ > (buffer_length_ + 1)) {
        capacity_value_ = buffer_length_ + 1;
        char *tmp_buffer = new char[capacity_value_];

        for (int i = 0; i < buffer_length_; i++)
            tmp_buffer[i] = mystring_buffer_[i];
        tmp_buffer[buffer_length_] = '\0';

        delete[] mystring_buffer_;
        mystring_buffer_ = tmp_buffer;
    }
}

void MyString::clear() 
{
    if (mystring_buffer_ == nullptr) return;

    buffer_length_ = 0;
    mystring_buffer_[buffer_length_] = '\0';
}

//----------------------------ostream/istream OPERATORS-------------------------//

std::ostream& operator<<(std::ostream& out, const MyString& input) 
{
    if (input.mystring_buffer_) {
        out << input.mystring_buffer_;
    } else {
        out << "";
    }
    return out;
}

std::istream& operator>>(std::istream& in, MyString& input) 
{
    input.clear();
    std::string line;

    while (std::getline(in, line))
        input.append(line.c_str());

    return in;
}

//----------------------------------INSERT--------------------------------------//

void MyString::insert(int index, const int count, const char c) 
{
    validateArgs((index < 0) || (index > buffer_length_) || (count <= 0), "Exception: index or count is wrong");

    if ((buffer_length_ + count) >= capacity_value_) {
        char *tmp_buffer = mystring_buffer_;
        capacity_value_ = initCapacity((buffer_length_ + count));
        mystring_buffer_ = new char[capacity_value_];

        for (int i = 0; i < buffer_length_; i++) {
            mystring_buffer_[i] = tmp_buffer[i];
        }

        delete[] tmp_buffer;
    } 

    for (int i = 0; i < count; i++){
        for (int j = (buffer_length_ - 1); j >= index; j--) {
            mystring_buffer_[j + 1] = mystring_buffer_[j];
        }

        mystring_buffer_[index] = c;
        buffer_length_++;
        index++;
    }

    mystring_buffer_[buffer_length_] = '\0';
 }

void MyString::insert(int index, const char *str) {
    for (int i = 0; i < strlen(str); i++){
        insert(index++, 1, str[i]);
    }
}

void MyString::insert(int index, const char *str, const int count) 
{
    for (int i = 0; i < count; i++){
        insert(index++, 1, str[i]);
    }
}

void MyString::insert(int index, const std::string str)
{
    insert(index, str.c_str());
}

void MyString::insert(int index, const std::string str, const int count) 
{
    insert(index, str.c_str(), count);
}

//-----------------------------------ERASE---------------------------------------//

void MyString::erase(int index, const int count)
{
    validateArgs((index < 0) || (index >= buffer_length_) || (count <= 0), "Exception: index or count is wrong");

    for (int i = index, j = index + count; i < (index + count); i++, j++){
        mystring_buffer_[i] = mystring_buffer_[j];
    }

    buffer_length_ -= count;
    mystring_buffer_[buffer_length_] = '\0';
}

//----------------------------------APPEND---------------------------------------//

void MyString::append(const int count, const char c) 
{
    this->insert(this->buffer_length_, count, c);
}

void MyString::append(const char *str) 
{
    this->insert(this->buffer_length_, str);
}

void MyString::append(const char *str, int index, const int count) 
{
    this->insert(index, str, count);
}

void MyString::append(const std::string str) 
{
    this->insert(this->buffer_length_, str.c_str());
}

void MyString::append(const std::string str, int index, const int count) 
{
    this->insert(index, str.c_str(), count);
}

//---------------------------------REPLACE----------------------------------------//

void MyString::replace(int index, const int count, const char* str) 
{
    validateArgs((index < 0) || (index >= buffer_length_) || (count < 0) || ((index + count) > buffer_length_), "Exception: index or count is wrong");

    if (count == strlen(str)){
        for (int i = index, j = 0; i < (index + count); i++, j++) {
            mystring_buffer_[i] = str[j];
        }
    } else {
        erase(index, count);
        insert(index, str, strlen(str));
    }
}

void MyString::replace(int index, const int count, const std::string str) 
{
    replace(index, count, str.c_str());
}

//----------------------------------SUBSTR----------------------------------------//

MyString MyString::substr(int index) {
    return substr(index, buffer_length_ - index);
}

MyString MyString::substr(int index, const int count) {
    validateArgs((index < 0) || (index >= buffer_length_) || ((count + index) > buffer_length_) || (count < 0), "Exception: index or count is wrong");

    MyString mystring_for_return(this->mystring_buffer_ + index, count);
    return mystring_for_return;
}

//----------------------------------FIND------------------------------------------//

const int MyString::find(const char *str) 
{
    return find(str, 0);
}

const int MyString::find(const char *str, int index) 
{
    validateArgs((index < 0) || (index >= buffer_length_), "Exception: index is wrong");

    int return_index = index, last = strlen(str) - 1;   

  	while (return_index < (buffer_length_ - last)) {      
    	int p = 0;      

        while((p <= last) && (mystring_buffer_[return_index + p] == str[p])) {
            p++;      
        }   

        if (p == strlen(str)) {
            return return_index;   
        } 

        return_index++;   
    }   

    return -1;
}

const int MyString::find(const std::string str) 
{
    return find(str.c_str(), 0);
}

const int MyString::find(const std::string str, int index) 
{
    return find(str.c_str(), index);
}

// -----------------------------SERVICE FUNCTIONS---------------------------------//

size_t MyString::initCapacity(size_t buffer_size) {
    if (capacity_value_ < buffer_size){
        while (capacity_value_ < (buffer_size + 1)) capacity_value_ += cBufferClasterSize;
    }

    return capacity_value_;
}

void MyString::validateArgs(bool args_expression, const char* error_message) {
    if (args_expression) throw std::runtime_error(error_message);
}