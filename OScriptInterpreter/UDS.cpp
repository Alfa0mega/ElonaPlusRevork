//
// Created by AMEGA on 29-Jul-15.
//

#include "UDS.h"

#ifdef _USE_WIDE_CHAR
    #define strlen wcslen
#else
    #define strlen strlen
#endif

unsigned int UDS::GetId() {
    if (this->_id != 0)
        return this ->_id;
    else
    {
        fprintf(stderr, "Error! ID of this instance of UDS is invalid. ID: %d", this->_id);
    }
}

UDS::UDS(unsigned int id, const UDS_DATA_STRUCT &aStruct) {

}

void UDS_DATA_STRUCT::setText(const _u_char *inpString) {
    auto _strLength = strlen(inpString);
    if (inpString == nullptr) {
        fprintf(stderr,
                "ERROR! Argument \"inputString\" is doesn't been initialized. File: %s; line: %d; function: %s\n",
                __FILE__, __LINE__, __FUNCTION__
        );
        exit(EXIT_FAILURE);
    }
    else if (_strLength == 0) {
        fprintf(stderr,
                "ERROR! Length of \"inputString\" is zero. File: %s; line: %d; function: %s\n",
                __FILE__, __LINE__, __FUNCTION__
        );
        exit(EXIT_FAILURE);
    }
    this->_textLength = _strLength;
    this->_text = static_cast<char*>(calloc(this->_textLength, sizeof(_u_char)));
    memcpy(this->_text, inpString, sizeof(_u_char));
}
