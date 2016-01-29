#include <iaString.h>

#include <iaConsole.h>

#include <stdio.h>
#include <math.h>

#include <string>
#include <memory>

namespace IgorAux
{

#ifdef __IGOR_DEBUG__
#define CHECK_CONSISTENCY() \
{ \
    if (_data != nullptr) \
    { \
        con_assert(wcslen(_data) == _charCount, "inconsistant data"); \
    } \
    else \
    { \
        con_assert(0 == _charCount, "inconsistant data"); \
    } \
}
#else
#define CHECK_CONSISTENCY()
#endif

    iaString::~iaString()
    {
        clear();
        CHECK_CONSISTENCY();
    }

    iaString::iaString(const char* text, const uint16 lenght)
    {
        if (lenght != INVALID_POSITION)
        {
            con_assert(strlen(text) >= lenght, "inconsistent data");
        }
        setData(text, lenght);

        CHECK_CONSISTENCY();
    }

    iaString::iaString(const wchar_t* text, const uint16 lenght)
    {
        if (lenght != INVALID_POSITION)
        {
            con_assert(wcslen(text) >= lenght, "inconsistent data");
        }
        setData(text, lenght);

        CHECK_CONSISTENCY();
    }

    iaString::iaString(const wchar_t character)
    {
        setData(&character, 1);
    }

    iaString::iaString(const char character)
    {
        setData(&character, 1);
    }

    iaString::iaString(const iaString& data)
    {
        setData(data.getData());

        CHECK_CONSISTENCY();
    }

    void iaString::toLower()
    {
        for (uint32 i = 0; i < getSize(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(tolower((*this)[i]));
        }
    }

    void iaString::toUpper()
    {
        for (uint32 i = 0; i < getSize(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(toupper((*this)[i]));
        }
    }

    const wchar_t* iaString::getData() const
    {
        return _data;
    }

    const wchar_t& iaString::operator[](const uint16 index) const
    {
        con_assert(_data != nullptr, "no data");
        con_assert(index < _charCount, "invalid index");
        return _data[index];
    }

    wchar_t& iaString::operator[](const uint16 index)
    {
        con_assert(_data != nullptr, "no data");
        con_assert(index < _charCount, "invalid index");
        return _data[index];
    }

    wostream& operator<<(wostream& stream, const iaString& text)
    {
        if (!text.isEmpty())
        {
            stream << text.getData();
        }
        return stream;
    }

    uint16 iaString::getData(char* buffer, const uint16 size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");

        if (buffer != nullptr &&
            size != 0 &&
            !isEmpty())
        {
            uint16 result = _charCount + 1;

            if (size < result)
            {
                result = size;
            }

            wcstombs(buffer, _data, result);

            buffer[result - 1] = 0;

            return result;
        }

        return 0;
    }

    uint16 iaString::getUTF8Size() const
    {
        uint16 result = 0;

        if (!isEmpty())
        {
            result = WideCharToMultiByte(CP_UTF8, 0, getData(), getSize(), nullptr, 0, nullptr, nullptr);
        }

        return result;
    }

    uint16 iaString::getUTF8(char* buffer, const uint16 size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");

        if (buffer != nullptr &&
            size != 0 &&
            !isEmpty())
        {
            uint16 result = WideCharToMultiByte(CP_UTF8, 0, getData(), getSize(), nullptr, 0, nullptr, nullptr);

            if (size < result)
            {
                result = size;
            }

            WideCharToMultiByte(CP_UTF8, 0, getData(), getSize(), buffer, result, nullptr, nullptr);

            return result;
        }

        return 0;
    }

    void iaString::setUTF8(const char* buffer, const uint16 size)
    {
        con_assert(buffer != nullptr, "no source data");
        con_assert(size != 0, "no source size");

        clear();

        _charCount = MultiByteToWideChar(CP_UTF8, 0, buffer, size, nullptr, 0);
        _data = new wchar_t[_charCount + 1];
        MultiByteToWideChar(CP_UTF8, 0, buffer, size, _data, _charCount);
        _data[_charCount] = 0;
    }

    void iaString::setData(const wchar_t* text, const uint16 size)
    {
        clear();

        if (text != nullptr)
        {
            if (size != INVALID_POSITION)
            {
                con_assert(wcslen(text) >= size, "inconsistant data");
                _charCount = size;
            }
            else
            {
                _charCount = static_cast<uint16>(wcslen(text));
            }

            _data = new wchar_t[_charCount + 1];
            wmemcpy(_data, text, _charCount);
            _data[_charCount] = 0;

            CHECK_CONSISTENCY();
        }
    }

    void iaString::setData(const char* text, const uint16 size)
    {
        clear();

        if (text != nullptr)
        {
            if (size != INVALID_POSITION)
            {
                con_assert(strlen(text) >= size, "inconsistant data");
                _charCount = size;
            }
            else
            {
                _charCount = static_cast<uint16>(strlen(text));
            }

            _data = new wchar_t[_charCount + 1];
            mbstowcs(_data, text, _charCount);
            _data[_charCount] = 0;

            CHECK_CONSISTENCY();
        }
    }

    void iaString::clear()
    {
        if (_data != nullptr)
        {
            delete[] _data;
        }

        _data = nullptr;
        _charCount = 0;

        CHECK_CONSISTENCY();
    }

    bool iaString::isEmpty() const
    {
        CHECK_CONSISTENCY();

        if (_data == nullptr ||
            _charCount == 0)
        {
            return true;
        }

        return false;
    }

    uint16 iaString::getSize() const
    {
        return _charCount;
    }

    bool iaString::operator== (const wchar_t* text) const
    {
        CHECK_CONSISTENCY();

        const iaString temp(text);
        return temp == *this;
    }

    bool iaString::operator== (const char* text) const
    {
        CHECK_CONSISTENCY();

        const iaString temp(text);
        return temp == *this;
    }

    bool iaString::operator== (const iaString& text) const
    {
        if (getSize() != text.getSize())
        {
            return false;
        }

        if (isEmpty())
        {
            return true;
        }

        uint16 i = _charCount;

        do
        {
            i--;
            if (text[i] != (*this)[i])
            {
                return false;
            }
        } while (i != 0);

        return true;
    }

    bool iaString::operator!= (const iaString& text) const
    {
        if (getSize() != text.getSize())
        {
            return true;
        }

        if (isEmpty())
        {
            return false;
        }

        uint16 i = _charCount;
        do
        {
            con_assert(i > 0, "value underflow");
            i--;
            if (text[i] != (*this)[i])
            {
                return true;
            }
        } while (i != 0);

        return false;
    }

    iaString iaString::operator+ (const iaString &text) const
    {
        CHECK_CONSISTENCY();

        iaString result(*this);
        result += text;
        return result;
    }

    void iaString::operator+= (const iaString &text)
    {
        CHECK_CONSISTENCY();

        if (!text.isEmpty())
        {
            wchar_t* temp = new wchar_t[_charCount + text.getSize() + 1];
            if (_charCount > 0)
            {
                wmemcpy(temp, _data, _charCount);
            }
            wmemcpy(temp + _charCount, text.getData(), text.getSize());
            _charCount += text.getSize();
            temp[_charCount] = 0;
            delete[] _data;
            _data = temp;

            CHECK_CONSISTENCY();
        }
    }

    iaString iaString::operator+ (const wchar_t &character) const
    {
        CHECK_CONSISTENCY();

        iaString result(*this);
        result += character;
        return result;
    }

    void iaString::operator+= (const wchar_t &character)
    {
        CHECK_CONSISTENCY();

        wchar_t* temp = new wchar_t[_charCount + 2];
        if (_charCount > 0)
        {
            wmemcpy(temp, _data, _charCount);
        }
        temp[_charCount++] = character;
        temp[_charCount] = 0;
        delete[] _data;
        _data = temp;

        CHECK_CONSISTENCY();
    }

    iaString iaString::operator= (const iaString &text)
    {
        setData(text.getData());
        return *this;
    }

    iaString iaString::operator=(const wchar_t* text)
    {
        setData(text);
        return *this;
    }

    iaString iaString::operator=(const char* text)
    {
        setData(text);
        return *this;
    }

    iaString iaString::operator=(const wchar_t character)
    {
        clear();

        _charCount = 1;
        _data = new wchar_t[_charCount + 1];
        _data[0] = character;
        _data[1] = 0;

        CHECK_CONSISTENCY();

        return *this;
    }

    iaString iaString::operator=(const char character)
    {
        clear();
        _charCount = 1;
        _data = new wchar_t[_charCount + 1];
        _data[0] = static_cast<wchar_t>(character);
        _data[1] = 0;

        CHECK_CONSISTENCY();

        return *this;
    }

    void iaString::split(const wchar_t delimiter, vector<iaString>& tokens, StringSplitMode splitMode) const
    {
        split(iaString(delimiter), tokens, splitMode);
    }

    void iaString::split(const char delimiter, vector<iaString>& tokens, StringSplitMode splitMode) const
    {
        split(iaString(delimiter), tokens, splitMode);
    }

    void iaString::split(const iaString& delimiters, vector<iaString>& tokens, StringSplitMode splitMode) const
    {
        CHECK_CONSISTENCY();

        uint16 from = 0;
        uint16 to = 0;

        if (!isEmpty())
        {
            switch (splitMode)
            {
            case StringSplitMode::Normal:
                do
                {
                    from = findFirstNotOf(delimiters.getData(), to);
                    if (from == iaString::INVALID_POSITION)
                    {
                        return;
                    }

                    to = findFirstOf(delimiters.getData(), from);
                    tokens.push_back(getSubString(from, to == iaString::INVALID_POSITION ? to : to - from));
                } while (from < getSize() && to != iaString::INVALID_POSITION);
                break;

            case StringSplitMode::RetriveAllEmpties:
                do
                {
                    to = findFirstOf(delimiters.getData(), from);
                    if (to != from)
                    {
                        tokens.push_back(getSubString(from, to == iaString::INVALID_POSITION ? to : to - from));

                        if (to == iaString::INVALID_POSITION)
                        {
                            return;
                        }

                        from = to;
                    }
                    else
                    {
                        tokens.push_back("");
                    }

                    from++;
                } while (from < getSize() && to != iaString::INVALID_POSITION);
                break;

            default:
                con_assert(false, "unknown split mode");
                break;
            }
        }
    }

    uint16 iaString::findFirstNotOf(const wchar_t character, const uint16 from) const
    {
        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");
        CHECK_CONSISTENCY();

        if (!isEmpty())
        {
            uint16 index = 0;
            if (from != INVALID_POSITION)
            {
                index = from;
            }

            while (index < _charCount)
            {
                if (_data[index] != character)
                {
                    return index;
                }
                index++;
            }
        }

        return INVALID_POSITION;
    }

    uint16 iaString::findFirstNotOf(const wchar_t* characters, const uint16 from) const
    {
        CHECK_CONSISTENCY();

        if (isEmpty() ||
            characters == nullptr)
        {
            return INVALID_POSITION;
        }

        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");

        uint16 index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        uint16 paternLenght = static_cast<uint16>(wcslen(characters));

        while (index < _charCount)
        {
            bool found = false;
            for (uint16 c = 0; c < paternLenght; ++c)
            {
                if (_data[index] == characters[c])
                {
                    found = true;
                }
            }

            if (!found)
            {
                return index;
            }

            index++;
        }

        return INVALID_POSITION;
    }

    uint16 iaString::findFirstOf(const wchar_t character, const uint16 from) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            uint16 index = 0;
            if (from != INVALID_POSITION)
            {
                index = from;
            }

            while (index < _charCount)
            {
                if (_data[index] == character)
                {
                    return index;
                }
                index++;
            }
        }

        return INVALID_POSITION;
    }

    uint16 iaString::findFirstOf(const wchar_t* characters, const uint16 from) const
    {
        CHECK_CONSISTENCY();

        if (isEmpty() ||
            characters == nullptr)
        {
            return INVALID_POSITION;
        }

        if (from != INVALID_POSITION &&
            from >= _charCount)
        {
            return INVALID_POSITION;
        }

        uint16 index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        uint16 paternLenght = static_cast<uint16>(wcslen(characters));

        while (index < _charCount)
        {
            for (uint16 c = 0; c < paternLenght; ++c)
            {
                if (_data[index] == characters[c])
                {
                    return index;
                }
            }
            index++;
        }

        return INVALID_POSITION;
    }

    uint16 iaString::findLastOf(const wchar_t character) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            uint16 index = _charCount;
            do
            {
                index--;
                if (_data[index] == character)
                {
                    return index;
                }
            } while (index > 0);
        }

        return INVALID_POSITION;
    }

    uint16 iaString::findLastOf(const wchar_t* characters) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() && characters != nullptr)
        {
            uint16 index = _charCount;
            uint16 paternLenght = static_cast<uint16>(wcslen(characters));

            do
            {
                index--;

                for (uint16 c = 0; c < paternLenght; ++c)
                {
                    if (_data[index] == characters[c])
                    {
                        return index;
                    }
                }

            } while (index > 0);
        }

        return INVALID_POSITION;
    }

    iaString iaString::getSubString(const uint16 pos, const uint16 len) const
    {
        CHECK_CONSISTENCY();

        con_assert(_data != nullptr, "no data");
        con_assert(pos != INVALID_POSITION, "out of range");
        con_assert(pos < _charCount, "out of range");

        uint16 lenght = len;
        if (lenght == INVALID_POSITION ||
            lenght + pos >= _charCount)
        {
            lenght = _charCount - pos;
        }

        iaString result(getData() + pos, lenght);
        return result;
    }

    void iaString::reverse()
    {
        int i = 0;
        int j = _charCount - 1;
        wchar_t temp;

        while (i<j)
        {
            temp = _data[i];
            _data[i] = _data[j];
            _data[j] = temp;
            i++; j--;
        }
    }

    // Converts a given integer x to string str[].  d is the number
    // of digits required in output. If d is more than the number
    // of digits in x, then 0s are added at the beginning.
    iaString iaString::intToStrInternal(int32 x, int d)
    {
        iaString result;

        int i = 0;
        if (x == 0)
        {
            result = '0';
            i++;
        }
        else
        {
            while (x)
            {
                result += (x % 10) + '0';
                x = x / 10;
                i++;
            }
        }

        while (i < d)
        {
            result += '0';
            i++;
        }

        result.reverse();
        return result;
    }

    iaString iaString::itoa(int32 value)
    {
        iaString result;
        float32 n = value;

        if (value < 0)
        {
            result += "-";
            n = abs(n);
        }

        result += intToStrInternal(n, 0);

        return result;
    }

    // Converts a floating point number to string.
    iaString iaString::ftoa(float32 value, int afterpoint)
    {
        iaString result;
        float32 n = value;

        if (value < 0)
        {
            result += "-";
            n = abs(n);
        }

        // Extract integer part
        int ipart = (int)n;

        // Extract floating part
        float fpart = n - (float)ipart;

        // convert integer part to string
        result += intToStrInternal(ipart, 0);

        // check for display option after point
        if (afterpoint != 0)
        {
            result += '.';  // add dot

                           // Get the value of fraction part upto given no.
                           // of points after dot. The third parameter is needed
                           // to handle cases like 233.007
            fpart = fpart * pow(10, afterpoint);

            result += intToStrInternal((int)fpart, afterpoint);
        }

        return result;
    }

    float32 iaString::atof(const iaString& text)
    {
        float32 integer = 0.0f;
        float32 part = 0.0f;
        float32 sign = 1.0f;

        bool beforeDecimal = true;
        wchar_t character;

        for (int i = 0; i < text.getSize(); ++i)
        {
            character = text[i];
            if (character >= '0' &&
                character <= '9')
            {
                float32 value = static_cast<float32>(character - '0');

                if (beforeDecimal)
                {
                    integer *= 10.0f;
                    integer += value;
                }
                else
                {
                    value /= 10.0f;
                    part /= 10.0f;
                    part += value;
                }
            }
            else if (character == '-')
            {
                sign = -1.0f;
            }
            else if (character == '.')
            {
                beforeDecimal = false;
            }
            else
            {
                con_err("invalid format");
                return 0.0f;
            }
        }

        return (integer + part) * sign;
    }

}
