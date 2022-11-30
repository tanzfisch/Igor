// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaUUID.h>

#include <random>
#include <sstream>

namespace iaux
{
    iaUUID::iaUUID(const iaUUID &other)
        : _value(other._value)
    {
    }

    iaUUID::iaUUID(const iaString &value)
        : _value(value)
    {
    }

    iaUUID::iaUUID(const char *value)
        : _value(value)
    {
    }

    bool iaUUID::operator==(const iaUUID &other) const
    {
        return _value == other._value;
    }

    bool iaUUID::operator!=(const iaUUID &other)
    {
        return _value != other._value;
    }    

    iaUUID iaUUID::operator=(const iaUUID &other)
    {
        _value = other._value;
        return *this;
    }

    bool iaUUID::isValid() const
    {
        return !_value.isEmpty() || _value.getLength() != 36;
    }

    iaUUID iaUUID::create()
    {
        // source https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);
        static iaMutex mutex;

        mutex.lock();
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++)
        {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++)
        {
            ss << dis(gen);
        }

        iaUUID uuid;
        uuid._value = iaString(ss.str().c_str());
        mutex.unlock();

        return uuid;
    }

    const iaString &iaUUID::getValue() const
    {
        return _value;
    }

    std::wostream &operator<<(std::wostream &stream, const iaUUID &uuid)
    {
        stream << uuid.getValue();
        return stream;
    }
}