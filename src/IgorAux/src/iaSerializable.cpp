// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iaSerializable.h>

#include <iaConsole.h>
#include <iaString.h>

namespace IgorAux
{

    bool iaSerializable::write(ofstream& file, const iaColor3f& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(&value._r), sizeof(iaColor3f));
    }

    bool iaSerializable::read(ifstream& file, iaColor3f& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(&value._r), sizeof(iaColor3f));
    }

    bool iaSerializable::write(ofstream& file, const iaColor3c& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(&value._r), sizeof(iaColor3c));
    }

    bool iaSerializable::read(ifstream& file, iaColor3c& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(&value._r), sizeof(iaColor3c));
    }

    bool iaSerializable::write(ofstream& file, const iaColor4f& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(&value._r), sizeof(iaColor4f));
    }

    bool iaSerializable::read(ifstream& file, iaColor4f& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(&value._r), sizeof(iaColor4f));
    }

    bool iaSerializable::write(ofstream& file, const iaVector2f& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(value.getData()), sizeof(iaVector2f));
    }

    bool iaSerializable::read(ifstream& file, iaVector2f& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(value.getData()), sizeof(iaVector2f));
    }

    bool iaSerializable::write(ofstream& file, const iaVector3f& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(value.getData()), sizeof(iaVector3f));
    }

    bool iaSerializable::read(ifstream& file, iaVector3f& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(value.getData()), sizeof(iaVector3f));
    }

    bool iaSerializable::write(ofstream& file, const iaVector4f& value)
    {
        return iaSerializable::write(file, reinterpret_cast<const char*>(value._vec.getData()), sizeof(iaVector4f));
    }

    bool iaSerializable::read(ifstream& file, iaVector4f& value)
    {
        return iaSerializable::read(file, reinterpret_cast<char*>(value._vec.getData()), sizeof(iaVector4f));
    }

    bool iaSerializable::writeUTF8(ofstream& file, const iaString& value)
    {
        con_assert(value.getUTF8Size() <= 0xffff, "string size out of range");

        size_t utf8Size = value.getUTF8Size();
        if (utf8Size <= 0xffff)
        {
            if (iaSerializable::writeUInt16(file, static_cast<uint16>(utf8Size)))
            {
                if (utf8Size != 0)
                {
                    char* buffer = new char[utf8Size];
#ifdef __IGOR_DEBUG__
                    memset(buffer, '#', utf8Size);
#endif
                    value.getUTF8(buffer, utf8Size);

                    bool result = iaSerializable::write(file, buffer, static_cast<uint16>(utf8Size));
                    delete[] buffer;
                    return result;
                }
                else
                {
                    // nothing to write
                    return true;
                }
            }
            else
            {
                con_err("failed to write buffer lenght");
                return false;
            }
        }
        else
        {
            con_err("string size out of range");
            return false;
        }
    }

    bool iaSerializable::readUTF8(ifstream& file, iaString& value)
    {
        con_assert(file.is_open(), "file not open");

        uint16 utf8Size = 0;
        if (!iaSerializable::readUInt16(file, utf8Size))
        {
            return false;
        }

        if (utf8Size != 0)
        {
            char* buffer = new char[utf8Size];
            bool result = iaSerializable::read(file, buffer, utf8Size);
            if (result)
            {
                value.setUTF8(buffer, utf8Size);
            }
            delete[] buffer;

            return result;
        }

        return true;
    }


}