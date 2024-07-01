//
//  main.cpp
//  string
//
//  Created by Егор on 04.05.2024.
//

#include "Header.h"
#include <cstring>
#include <cctype>
#include <memory>
#include <cstddef>

int String::HowMany() {
    static int num_strings = 0;
    return num_strings;
}

String::String(const char* s) {
    m_size = std::strlen(s);
    if (m_size <= sizeof(sstr) - 1) {
        is_short = true;
        std::strcpy(sstr, s);
    } else {
        is_short = false;
        lstr.str = new char[m_size + 1];
        std::strcpy(lstr.str, s);
        lstr.m_capacity = m_size;
    }
}

String::String() {
    m_size = 0;
    is_short = true;
    sstr[0] = '\0';
}

String::String(const String& other) {
    m_size = other.m_size;
    if (other.is_short) {
        is_short = true;
        std::strcpy(sstr, other.sstr);
    } else {
        is_short = false;
        lstr.str = new char[m_size + 1];
        std::strcpy(lstr.str, other.lstr.str);
        lstr.m_capacity = other.lstr.m_capacity;
    }
}

String::~String() {
    if (!is_short) {
        delete[] lstr.str;
    }
}

String& String::operator=(const String& other) {
    if (this != &other) {
        if (!is_short) {
            delete[] lstr.str;
        }
        m_size = other.m_size;
        if (other.is_short) {
            is_short = true;
            std::strcpy(sstr, other.sstr);
        } else {
            is_short = false;
            lstr.str = new char[m_size + 1];
            std::strcpy(lstr.str, other.lstr.str);
            lstr.m_capacity = other.lstr.m_capacity;
        }
    }
    return *this;
}

String& String::operator=(const char* s) {
    if (!is_short) {
        delete[] lstr.str;
    }
    m_size = std::strlen(s);
    if (m_size <= sizeof(sstr) - 1) {
        is_short = true;
        std::strcpy(sstr, s);
    } else {
        is_short = false;
        lstr.str = new char[m_size + 1];
        std::strcpy(lstr.str, s);
        lstr.m_capacity = m_size;
    }
    return *this;
}

char& String::operator[](int i) {
    if (is_short) {
        return sstr[i];
    } else {
        return lstr.str[i];
    }
}

const char& String::operator[](int i) const {
    if (is_short) {
        return sstr[i];
    } else {
        return lstr.str[i];
    }
}

void String::stringlow() {
    if (is_short) {
        for (size_t i = 0; i < m_size; i++) {
            sstr[i] = std::tolower(sstr[i]);
        }
    } else {
        for (size_t i = 0; i < m_size; i++) {
            lstr.str[i] = std::tolower(lstr.str[i]);
        }
    }
}

void String::stringup() {
    if (is_short) {
        for (size_t i = 0; i < m_size; i++) {
            sstr[i] = std::toupper(sstr[i]);
        }
    } else {
        for (size_t i = 0; i < m_size; i++) {
            lstr.str[i] = std::toupper(lstr.str[i]);
        }
    }
}

bool operator<(const String& st, const String& st2) {
    return std::strcmp(st.is_short ? st.sstr : st.lstr.str, st2.is_short ? st2.sstr : st2.lstr.str) < 0;
}

bool operator>(const String& st1, const String& st2) {
    return std::strcmp(st1.is_short ? st1.sstr : st1.lstr.str, st2.is_short ? st2.sstr : st2.lstr.str) > 0;
}

bool operator==(const String& st, const String& st2) {
    return std::strcmp(st.is_short ? st.sstr : st.lstr.str, st2.is_short ? st2.sstr : st2.lstr.str) == 0;
}

ostream& operator<<(ostream& os, const String& st) {
    os << (st.is_short ? st.sstr : st.lstr.str);
    return os;
}

istream& operator>>(istream& is, String& st) {
    char temp[String().MAX];
    is.get(temp, String::MAX);
    if (is) {
        st = temp;
    }
    while (is && is.get() != '\n') {
        continue;
    }
    return is;
}

String operator+(const String& st1, const String& st2) {
    String temp;
    temp.m_size = st1.m_size + st2.m_size;
    if (temp.m_size <= sizeof(temp.sstr) - 1) {
        temp.is_short = true;
        std::strcpy(temp.sstr, st1.is_short ? st1.sstr : st1.lstr.str);
        std::strcat(temp.sstr, st2.is_short ? st2.sstr : st2.lstr.str);
    } else {
        temp.is_short = false;
        temp.lstr.str = new char[temp.m_size + 1];
        std::strcpy(temp.lstr.str, st1.is_short ? st1.sstr : st1.lstr.str);
        std::strcat(temp.lstr.str, st2.is_short ? st2.sstr : st2.lstr.str);
        temp.lstr.m_capacity = temp.m_size;
    }
    return temp;
}
