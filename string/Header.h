//
//  Header.h
//  pzrl_dop
//
//  Created by Егор on 07.05.2024.
//
#ifndef STRING1_H_
#define STRING1_H_
#include <iostream>
#include <cstddef>
#include <memory>
#include <array>
using std::ostream;
using std::istream;

class String
{
    private:
    size_t m_size;
        union {
            struct{
                // This is probably better designed as an array-like class
                char *str;
                size_t m_capacity;
            } lstr;
            char sstr[sizeof(lstr)];
        };
    
    public:
        
        bool is_short;

        String(const char * s);
        String();
        String(const String &);
        ~String();
        size_t length() const { return m_size; }

        String & operator=(const String &);
        String & operator=(const char *);
        char & operator[](int i);
        const char & operator[](int i) const;
        void stringlow();
        void stringup();

        friend bool operator<(const String &st, const String &st2);
        friend bool operator>(const String &st1, const String &st2);
        friend bool operator==(const String &st, const String &st2);
        friend ostream & operator<<(ostream & os, const String &st2);
        friend istream & operator>>(istream & is, String & st);
        friend String operator+(const String& st1, const String& st2);

        static int HowMany();
};
#endif
