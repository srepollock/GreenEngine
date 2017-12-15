#pragma once
#include <string>
#include <map>
#include <assert.h>
/// TODO: Update the file handler system so that it will send erros to a 
/// .log file
class ErrorHandler {
public:
    ErrorHandler(int _value, const std::string& _str) {
        value = _value;
        message = _str;
#ifdef _DEBUG
        errorMap::iterator found = getErrorMap().find( value );
        if ( found != getErrorMap().end() )
            assert( found->second == message );
#endif
        getErrorMap()[value] = message;
    }

    // auto-cast Error to integer error code
    operator int() { return value; }

private:
    int value;
    std::string message;

    typedef std::map<int,std::string> errorMap;
    static errorMap& getErrorMap() {
        static errorMap errMap;
        return errMap;
    }

public:
    static std::string getErrorString(int value) {
        errorMap::iterator found = getErrorMap().find( value );
        if (found == getErrorMap().end()) {
            assert( false );
            return "";
        }
        else {
            return found->second;
        }
    }
};

static ErrorHandler ERROR_SUCCESS(                 0, "The operation succeeded" );
static ErrorHandler ERROR_SYSTEM_NOT_INITIALIZED(  1, "System is not initialised yet" );
static ErrorHandler ERROR_INTERNAL(                2, "Internal error" );
static ErrorHandler ERROR_NOT_IMPLEMENTED(         3, "Function not implemented yet" );