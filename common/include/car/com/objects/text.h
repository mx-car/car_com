#ifndef CAR_COM_OBJECTS_TEXT_H
#define CAR_COM_OBJECTS_TEXT_H


#include <cstdint>
#include <cstring>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  Text {
public:
    Text() {
        clear();
    };
    Text ( const Text &text  )  {
        write(text.txt);
    };
    Text ( const char *text  )  {
        write(text);
    };
    static const int MAX_BUFFER_SIZE = 32;
    char txt[MAX_BUFFER_SIZE+1];
    void clear() {
        memset ( txt, '\0', MAX_BUFFER_SIZE+1 );
    }
    int write ( const char *msg ) {
        int bytes_remaining = MAX_BUFFER_SIZE - strlen ( msg );
        if ( bytes_remaining > 0 ) {
            strcpy ( txt, msg );
        }
        return bytes_remaining;
    }
    int print (const char *format, ...) {
        #include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
        va_list arg;
        int done;
        va_start (arg, format);
        done = sprintf(txt, format, arg);
        va_end (arg);
        return MAX_BUFFER_SIZE - done; 
    }

    int size() {
        return strlen ( txt );
    }
    bool empty() {
        return (strlen ( txt ) == 0);
    }
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Text &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, Text &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        return std::string ( txt );
    }
    bool setFromString ( const std::string &str ) {
        /// ToDo check for error
        write ( str.c_str() );
        return true;
    }
#endif

};
};
};
};


#endif // CAR_COM_OBJECTS_TEXT_H
