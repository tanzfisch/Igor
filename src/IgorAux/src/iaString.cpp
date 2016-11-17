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

    int64 iaString::getHashValue() const
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = getData();
        return static_cast<int64>(hashFunc(keyValue));
    }

    iaString::iaString(const char* text, const size_t lenght)
    {
        if (lenght != INVALID_POSITION)
        {
            con_assert(strlen(text) >= lenght, "inconsistent data");
        }
        setData(text, lenght);

        CHECK_CONSISTENCY();
    }

    iaString::iaString(const wchar_t* text, const size_t lenght)
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
        for (size_t i = 0; i < getSize(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(tolower((*this)[i]));
        }
    }

    void iaString::toUpper()
    {
        for (size_t i = 0; i < getSize(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(toupper((*this)[i]));
        }
    }

    const wchar_t* iaString::getData() const
    {
        return _data;
    }

    const wchar_t& iaString::operator[](const size_t index) const
    {
        con_assert(_data != nullptr, "no data");
        con_assert(index < _charCount, "invalid index");
        return _data[index];
    }

    wchar_t& iaString::operator[](const size_t index)
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

    size_t iaString::getData(char* buffer, const size_t size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");

        if (buffer != nullptr &&
            size != 0 &&
            !isEmpty())
        {
            size_t result = _charCount + 1;

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

    size_t iaString::getUTF8Size() const
    {
        size_t result = 0;

        if (!isEmpty())
        {
            result = static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getSize()), nullptr, 0, nullptr, nullptr));
        }

        return result;
    }

    size_t iaString::getUTF8(char* buffer, const size_t size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");

        if (buffer != nullptr &&
            size != 0 &&
            !isEmpty())
        {
            size_t measuredSize = static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getSize()), nullptr, 0, nullptr, nullptr));

            if (size < measuredSize)
            {
                measuredSize = size;
            }

            size_t result = 0;
            
            if (measuredSize != 0)
            {
                result = static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getSize()), buffer, static_cast<int>(measuredSize), nullptr, nullptr));

                con_assert(result != 0, "conversion failed");
                if (result != 0)
                {
                    con_err("conversion failed");
                }
            }

            return result;
        }

        return 0;
    }

    void iaString::setUTF8(const char* buffer, const size_t size)
    {
        con_assert(buffer != nullptr, "no source data");
        con_assert(size != 0, "no source size");

        clear();

        _charCount = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), nullptr, 0));
        _data = new wchar_t[_charCount + 1];
        
        MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), _data, static_cast<int>(_charCount));

        _data[_charCount] = 0;
    }

    void iaString::setData(const wchar_t* text, const size_t size)
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
                _charCount = static_cast<size_t>(wcslen(text));
            }

            _data = new wchar_t[_charCount + 1];
            wmemcpy(_data, text, _charCount);
            _data[_charCount] = 0;

            CHECK_CONSISTENCY();
        }
    }

    void iaString::setData(const char* text, const size_t size)
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
                _charCount = static_cast<size_t>(strlen(text));
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

    size_t iaString::getSize() const
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

        size_t i = _charCount;

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

        size_t i = _charCount;
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

        size_t from = 0;
        size_t to = 0;

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
                con_err("unknown split mode");
                break;
            }
        }
    }

    size_t iaString::findFirstNotOf(const wchar_t character, const size_t from) const
    {
        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");
        CHECK_CONSISTENCY();

        if (!isEmpty())
        {
            size_t index = 0;
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

    size_t iaString::findFirstNotOf(const wchar_t* characters, const size_t from) const
    {
        CHECK_CONSISTENCY();

        if (isEmpty() ||
            characters == nullptr)
        {
            return INVALID_POSITION;
        }

        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");

        size_t index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        size_t paternLenght = static_cast<size_t>(wcslen(characters));

        while (index < _charCount)
        {
            bool found = false;
            for (size_t c = 0; c < paternLenght; ++c)
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

    size_t iaString::findFirstOf(const wchar_t character, const size_t from) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            size_t index = 0;
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

    size_t iaString::findFirstOf(const wchar_t* characters, const size_t from) const
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

        size_t index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        size_t paternLenght = static_cast<size_t>(wcslen(characters));

        while (index < _charCount)
        {
            for (size_t c = 0; c < paternLenght; ++c)
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

    size_t iaString::findLastOf(const wchar_t character) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            size_t index = _charCount;
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

    size_t iaString::findLastOf(const wchar_t* characters) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() && characters != nullptr)
        {
            size_t index = _charCount;
            size_t paternLenght = static_cast<size_t>(wcslen(characters));

            do
            {
                index--;

                for (size_t c = 0; c < paternLenght; ++c)
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

    iaString iaString::getSubString(const size_t pos, const size_t len) const
    {
        CHECK_CONSISTENCY();

        con_assert(_data != nullptr, "no data");
        con_assert(pos != INVALID_POSITION, "out of range");
        con_assert(pos < _charCount, "out of range");

        size_t lenght = len;
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
        size_t i = 0;
        size_t j = _charCount - 1;
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
        int32 n = value;

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
        float32 fpart = n - static_cast<float32>(ipart);

        // convert integer part to string
        result += intToStrInternal(ipart, 0);

        // check for display option after point
        if (afterpoint != 0)
        {
            result += '.';  // add dot

                           // Get the value of fraction part upto given no.
                           // of points after dot. The third parameter is needed
                           // to handle cases like 233.007
            fpart = fpart * static_cast<float32>(pow(10, afterpoint));

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
