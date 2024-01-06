//
// Created by ashblade on 05.01.24.
//

#include "memory/memory_streambuf.h"

InputMemoryStreambuf::InputMemoryStreambuf(char *data, size_t size)  {
    setg(data, data, data + size);
}