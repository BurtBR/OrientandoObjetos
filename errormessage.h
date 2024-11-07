#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QString>

class ErrorMessage{
public:
    ErrorMessage();

    enum class ErrorCode{
        Misc,
        FailedToWrite,
        FailedToOpenFile,
        FailedToAllocate
    };

    static QString GetErrorStr(ErrorCode e);
};

#endif // ERRORMESSAGE_H
