#include <iaux/data/iaString.h>

#ifdef __IGOR_WINDOWS__
#include <windows.h>
#endif

#ifdef __IGOR_LINUX__
#include <iconv.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <math.h>
#include <string>
#include <memory>
#include <regex>

namespace iaux
{

#ifdef __IGOR_DEBUG__
#define CHECK_CONSISTENCY()                                               \
    {                                                                     \
        if (_data != nullptr)                                             \
        {                                                                 \
            con_assert(wcslen(_data) == _charCount, "inconsistant data"); \
        }                                                                 \
        else                                                              \
        {                                                                 \
            con_assert(0 == _charCount, "inconsistant data");             \
        }                                                                 \
    }
#else
#define CHECK_CONSISTENCY()
#endif

    iaString::~iaString()
    {
        clear();
        CHECK_CONSISTENCY();
    }

    bool iaString::matchRegex(const iaString &text, const iaString &regex)
    {
        std::basic_string<wchar_t> searchString(text.getData());
        std::basic_regex<wchar_t> exp;

        try
        {
            exp = std::basic_regex<wchar_t>(regex.getData(), std::regex::extended);
        }
        catch (...)
        {
            return false;
        }

        return std::regex_match(searchString, exp);
    }

    void iaString::replaceRegex(const iaString &src, const iaString &regex, const iaString &replaceWith, iaString &dst)
    {
        dst = "";

        std::basic_string<wchar_t> searchString(src.getData());
        std::basic_regex<wchar_t> exp;

        try
        {
            exp = std::basic_regex<wchar_t>(regex.getData(), std::regex::extended);
        }
        catch (...)
        {
            con_err("invalid regular expression \"" << regex << "\"");
            return;
        }

        std::basic_string<wchar_t> r(replaceWith.getData());
        dst = iaString(std::regex_replace(searchString, exp, r).data());
    }

    bool iaString::searchRegex(const iaString &src, const iaString &regex, std::vector<iaString> &matches)
    {
        matches.clear();

        std::basic_string<wchar_t> searchString(src.getData());
        std::basic_regex<wchar_t> exp;

        try
        {
            exp = std::basic_regex<wchar_t>(regex.getData(), std::regex::extended);
        }
        catch (...)
        {
            con_err("invalid regular expression \"" << regex << "\"");
            return false;
        }

        std::wsmatch sm;

        while (std::regex_search(searchString, sm, exp))
        {
            matches.push_back(sm.str().data());
            searchString = sm.suffix();
        }

        return !matches.empty();
    }

    int64 iaString::getHashValue() const
    {
        std::hash<std::wstring> hashFunc;
        return static_cast<int64>(hashFunc(_data));
    }

    iaString::iaString(const char *text, const int64 lenght)
    {
        if (lenght != INVALID_POSITION)
        {
            con_assert(strlen(text) >= lenght, "inconsistent data");
        }
        setData(text, lenght);
    }

    iaString::iaString(const wchar_t *text, const int64 lenght)
    {
        if (lenght != INVALID_POSITION)
        {
            con_assert(wcslen(text) >= lenght, "inconsistent data");
        }
        setData(text, lenght);
    }

    iaString::iaString(const wchar_t character)
    {
        setData(&character, 1);
    }

    iaString::iaString(const char character)
    {
        setData(&character, 1);
    }

    iaString::iaString(const iaString &data)
    {
        setData(data.getData());
    }

    void iaString::toLower()
    {
        for (int64 i = 0; i < getLength(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(tolower((*this)[i]));
        }
    }

    void iaString::toUpper()
    {
        for (int64 i = 0; i < getLength(); i++)
        {
            (*this)[i] = static_cast<wchar_t>(toupper((*this)[i]));
        }
    }

    const wchar_t *iaString::getData() const
    {
        return _data;
    }

    const wchar_t &iaString::operator[](const int64 index) const
    {
        con_assert(_data != nullptr, "no data");
        con_assert(index < _charCount, "invalid index");
        return _data[index];
    }

    wchar_t &iaString::operator[](const int64 index)
    {
        con_assert(_data != nullptr, "no data");
        con_assert(index < _charCount, "invalid index");
        return _data[index];
    }

    bool iaString::operator<(const iaString &text) const
    {
        return wcscmp((*this).getData(), text.getData()) < 0;
    }

    bool iaString::operator>(const iaString &text) const
    {
        return wcscmp((*this).getData(), text.getData()) > 0;
    }

    std::wostream &operator<<(std::wostream &stream, const iaString &text)
    {
        if (!text.isEmpty())
        {
            stream << text.getData();
        }

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const std::vector<iaString> &texts)
    {
        bool found = false;
        for (const auto &text : texts)
        {
            if (!text.isEmpty())
            {
                if (found)
                {
                    stream << ", ";
                }

                stream << text.getData();
                found = true;
            }
        }

        return stream;
    }

    int64 iaString::getData(char *buffer, const int64 size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");

        if (buffer != nullptr &&
            size != 0 &&
            !isEmpty())
        {
            int64 result = _charCount + 1;

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

    int64 iaString::getUTF8Size() const
    {
        int64 result = 0;

        if (isEmpty())
        {
            return result;
        }

#ifdef __IGOR_WINDOWS__
        result = static_cast<int64>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getLength()), nullptr, 0, nullptr, nullptr));
#endif

#ifdef __IGOR_LINUX__

        iconv_t conv = iconv_open("UTF-8", "WCHAR_T");
        if (conv == (iconv_t)-1)
        {
            con_err("iconv_open " << strerror(errno));
        }
        else
        {
            // create tmp buffer
            size_t bufferSize = _charCount * 6;
            char buffer[bufferSize];

            size_t inLeft = (_charCount + 1) * sizeof(wchar_t);
            size_t outLeft = bufferSize;
            char *inptr = reinterpret_cast<char *>(_data);
            char *outptr = buffer;

            size_t res = iconv(conv, &inptr, &inLeft, &outptr, &outLeft);
            if (res == -1)
            {
                con_err("iconv " << strerror(errno));
            }
            else
            {
                result = bufferSize - outLeft - 1;
            }

            iconv_close(conv);
        }

#endif

        return result;
    }

    int64 iaString::getUTF8(char *buffer, const int64 size) const
    {
        con_assert(buffer != nullptr, "no destination data");
        con_assert(size != 0, "no destination size");
        int64 result = 0;

        if (buffer == nullptr ||
            size == 0 ||
            isEmpty())
        {
            return result;
        }

#ifdef __IGOR_WINDOWS__
        int64 measuredSize = static_cast<int64>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getLength()), nullptr, 0, nullptr, nullptr));

        if (size < measuredSize)
        {
            measuredSize = size;
        }

        if (measuredSize != 0)
        {
            result = static_cast<int64>(WideCharToMultiByte(CP_UTF8, 0, getData(), static_cast<int>(getLength()), buffer, static_cast<int>(measuredSize), nullptr, nullptr));

            con_assert(result != 0, "conversion failed");
            if (result == 0)
            {
                con_err("conversion failed");
            }
        }

#endif

#ifdef __IGOR_LINUX__

        iconv_t conv = iconv_open("UTF-8", "WCHAR_T");
        if (conv == (iconv_t)-1)
        {
            con_err("iconv_open " << strerror(errno));
        }
        else
        {
            size_t inLeft = (_charCount) * sizeof(wchar_t);
            size_t outLeft = size;
            char *inptr = reinterpret_cast<char *>(_data);
            char *outptr = buffer;
            size_t res = iconv(conv, &inptr, &inLeft, &outptr, &outLeft);

            if (res == -1)
            {
                con_err("iconv " << strerror(errno));
            }
            else
            {
                result = size - outLeft;
            }

            iconv_close(conv);
        }

#endif

        return result;
    }

    void iaString::setUTF8(const char *buffer, const int64 size)
    {
        con_assert(buffer != nullptr, "no source data");
        con_assert(size != 0, "no source size");

        clear();

#ifdef __IGOR_WINDOWS__
        _charCount = static_cast<int64>(MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), nullptr, 0));
        _data = new wchar_t[_charCount + 1];

        MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), _data, static_cast<int>(_charCount));

        _data[_charCount] = 0;
#endif

#ifdef __IGOR_LINUX__
        iconv_t conv = iconv_open("WCHAR_T", "UTF-8");
        if (conv == (iconv_t)-1)
        {
            con_err("iconv_open " << strerror(errno));
        }
        else
        {
            // create tmp buffer
            size_t bufferSize = size * sizeof(wchar_t);
            char tmpbuffer[bufferSize];

            size_t inLeft = size;
            size_t outLeft = bufferSize;
            char *inptr = const_cast<char *>(buffer);
            char *outptr = tmpbuffer;
            size_t res = iconv(conv, &inptr, &inLeft, &outptr, &outLeft);

            if (res == -1)
            {
                con_err("iconv " << strerror(errno));
            }
            else
            {
                _charCount = (bufferSize - outLeft) / sizeof(wchar_t);
                _data = new wchar_t[_charCount + 1];

                memcpy(_data, tmpbuffer, (_charCount) * sizeof(wchar_t));
                _data[_charCount] = 0;
            }

            iconv_close(conv);
        }

#endif
    }

    void iaString::setData(const wchar_t *text, const int64 size)
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
                _charCount = static_cast<int64>(wcslen(text));
            }

            _data = new wchar_t[_charCount + 1];
            wmemcpy(_data, text, _charCount);
            _data[_charCount] = 0;

            CHECK_CONSISTENCY();
        }
    }

    void iaString::setData(const char *text, const int64 size)
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
                _charCount = static_cast<int64>(strlen(text));
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

    int64 iaString::getSize() const
    {
        return (_charCount + 1) * sizeof(wchar_t);
    }

    int64 iaString::getLength() const
    {
        return _charCount;
    }

    bool iaString::operator==(const wchar_t *text) const
    {
        CHECK_CONSISTENCY();

        const iaString temp(text);
        return temp == *this;
    }

    bool iaString::operator==(const char *text) const
    {
        CHECK_CONSISTENCY();

        const iaString temp(text);
        return temp == *this;
    }

    bool iaString::operator==(const iaString &text) const
    {
        if (getLength() != text.getLength())
        {
            return false;
        }

        if (isEmpty())
        {
            return true;
        }

        int64 i = _charCount;

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

    bool iaString::operator!=(const iaString &text) const
    {
        if (getLength() != text.getLength())
        {
            return true;
        }

        if (isEmpty())
        {
            return false;
        }

        int64 i = _charCount;
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

    iaString iaString::operator+(const iaString &text) const
    {
        CHECK_CONSISTENCY();

        iaString result(*this);
        result += text;
        return result;
    }

    void iaString::operator+=(const iaString &text)
    {
        CHECK_CONSISTENCY();

        if (!text.isEmpty())
        {
            wchar_t *temp = new wchar_t[_charCount + text.getLength() + 1];
            if (_charCount > 0)
            {
                wmemcpy(temp, _data, _charCount);
            }
            wmemcpy(temp + _charCount, text.getData(), text.getLength());
            _charCount += text.getLength();
            temp[_charCount] = 0;
            delete[] _data;
            _data = temp;

            CHECK_CONSISTENCY();
        }
    }

    iaString iaString::operator+(const wchar_t &character) const
    {
        CHECK_CONSISTENCY();

        iaString result(*this);
        result += character;
        return result;
    }

    void iaString::operator+=(const wchar_t &character)
    {
        CHECK_CONSISTENCY();

        wchar_t *temp = new wchar_t[_charCount + 2];
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

    iaString iaString::operator=(const iaString &text)
    {
        setData(text.getData());
        return *this;
    }

    iaString iaString::operator=(const wchar_t *text)
    {
        setData(text);
        return *this;
    }

    iaString iaString::operator=(const char *text)
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

    void iaString::split(const wchar_t delimiter, std::vector<iaString> &tokens, iaStringSplitMode splitMode) const
    {
        split(iaString(delimiter), tokens, splitMode);
    }

    void iaString::split(const char delimiter, std::vector<iaString> &tokens, iaStringSplitMode splitMode) const
    {
        split(iaString(delimiter), tokens, splitMode);
    }

    void iaString::split(const iaString &delimiters, std::vector<iaString> &tokens, iaStringSplitMode splitMode) const
    {
        CHECK_CONSISTENCY();

        int64 from = 0;
        int64 to = 0;

        if (!isEmpty())
        {
            switch (splitMode)
            {
            case iaStringSplitMode::Normal:
                do
                {
                    from = findFirstNotOf(delimiters.getData(), to);
                    if (from == iaString::INVALID_POSITION)
                    {
                        return;
                    }

                    to = findFirstOf(delimiters.getData(), from);
                    tokens.push_back(getSubString(from, to == iaString::INVALID_POSITION ? to : to - from));
                } while (from < getLength() && to != iaString::INVALID_POSITION);
                break;

            case iaStringSplitMode::RetriveAllEmpties:
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
                } while (from < getLength() && to != iaString::INVALID_POSITION);
                break;

            default:
                con_err("unknown split mode");
                break;
            }
        }
    }

    int64 iaString::findFirstNotOf(const wchar_t character, const int64 from) const
    {
        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");
        CHECK_CONSISTENCY();

        if (!isEmpty())
        {
            int64 index = 0;
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

    int64 iaString::findFirstNotOf(const wchar_t *characters, const int64 from) const
    {
        CHECK_CONSISTENCY();

        if (isEmpty() ||
            characters == nullptr)
        {
            return INVALID_POSITION;
        }

        con_assert(from < _charCount || from == INVALID_POSITION, "out of range");

        int64 index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        int64 paternLenght = static_cast<int64>(wcslen(characters));

        while (index < _charCount)
        {
            bool found = false;
            for (int64 c = 0; c < paternLenght; ++c)
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

    int64 iaString::findFirstOf(const wchar_t character, const int64 from) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            int64 index = 0;
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

    int64 iaString::findFirstOf(const wchar_t *characters, const int64 from) const
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

        int64 index = 0;
        if (from != INVALID_POSITION)
        {
            index = from;
        }

        int64 paternLenght = static_cast<int64>(wcslen(characters));

        while (index < _charCount)
        {
            for (int64 c = 0; c < paternLenght; ++c)
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

    int64 iaString::findLastNotOf(const wchar_t character) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            int64 index = _charCount;
            do
            {
                index--;
                if (_data[index] != character)
                {
                    return index;
                }
            } while (index > 0);
        }

        return INVALID_POSITION;
    }

    int64 iaString::findLastOf(const wchar_t character) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() &&
            character != 0)
        {
            int64 index = _charCount;
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

    int64 iaString::findLastNotOf(const wchar_t *characters) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() && characters != nullptr)
        {
            int64 index = _charCount;
            int64 paternLenght = static_cast<int64>(wcslen(characters));

            do
            {
                index--;

                for (int64 c = 0; c < paternLenght; ++c)
                {
                    if (_data[index] != characters[c])
                    {
                        return index;
                    }
                }

            } while (index > 0);
        }

        return INVALID_POSITION;
    }

    int64 iaString::findLastOf(const wchar_t *characters) const
    {
        CHECK_CONSISTENCY();

        if (!isEmpty() && characters != nullptr)
        {
            int64 index = _charCount;
            int64 paternLenght = static_cast<int64>(wcslen(characters));

            do
            {
                index--;

                for (int64 c = 0; c < paternLenght; ++c)
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

    void iaString::insert(const iaString &text, int64 pos)
    {
        con_assert(pos != INVALID_POSITION, "out of range");
        con_assert(pos <= _charCount, "out of range");

        iaString result = getSubString(0, pos);
        result += text;
        result += getSubString(pos, getLength() - pos);

        setData(result.getData());
    }

    void iaString::remove(int64 pos, int64 length)
    {
        iaString result;

        if (pos > 0)
            result = getSubString(0, pos);
        result += getSubString(pos + length);

        setData(result.getData());
    }

    iaString iaString::getSubString(const int64 pos, const int64 len) const
    {
        CHECK_CONSISTENCY();

        if (len == 0 || _data == nullptr)
        {
            return iaString();
        }

        con_assert(pos != INVALID_POSITION, "out of range");

        int64 length = len;
        if (length == INVALID_POSITION ||
            length + pos >= _charCount)
        {
            length = _charCount - pos;
        }

        iaString result(getData() + pos, length);
        return result;
    }

    void iaString::reverse()
    {
        int64 i = 0;
        int64 j = _charCount - 1;
        wchar_t temp;

        while (i < j)
        {
            temp = _data[i];
            _data[i] = _data[j];
            _data[j] = temp;
            i++;
            j--;
        }
    }

    const static iaString s_numbers = "0123456789abcdef";

    // Converts a given integer x to string str[].  d is the number
    // of digits required in output. If d is more than the number
    // of digits in x, then 0s are added at the beginning.
    iaString iaString::intToStrInternal(uint64 x, int d, int base)
    {
        con_assert(base >= 2 && base <= 16, "base out of range");

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
                result += s_numbers[x % base];
                x = x / base;
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

    iaString iaString::toString(int32 value, int base)
    {
        return toString(static_cast<int64>(value), base);
    }

    iaString iaString::toString(uint32 value, int base)
    {
        return toString(static_cast<uint64>(value), base);
    }

    iaString iaString::toString(uint64 value, int base)
    {
        return intToStrInternal(value, 0, base);
    }

    iaString iaString::toString(int64 value, int base)
    {
        iaString result;
        int64 n = value;

        if (value < 0)
        {
            result += "-";
            n = abs(n);
        }

        result += intToStrInternal(n, 0, base);

        return result;
    }

    iaString iaString::toString(float32 value, int afterpoint)
    {
        return toString(static_cast<float64>(value), afterpoint);
    }

    // Converts a floating point number to string.
    iaString iaString::toString(float64 value, int afterpoint)
    {
        iaString result;
        float64 n = value;

        if (value < 0)
        {
            result += "-";
            n = abs(n);
        }

        // Extract integer part
        int ipart = (int)n;

        // Extract floating part
        float64 fpart = n - static_cast<float64>(ipart);

        // convert integer part to string
        result += intToStrInternal(ipart, 0);

        // check for display option after point
        if (afterpoint != 0)
        {
            result += '.'; // add dot

            // Get the value of fraction part upto given no.
            // of points after dot. The third parameter is needed
            // to handle cases like 233.007
            fpart = fpart * pow(10, afterpoint);

            result += intToStrInternal((int)fpart, afterpoint);
        }

        return result;
    }

    bool iaString::toBool(const iaString &text)
    {
        iaString trimmed = iaString::trim(text);
        trimmed.toLower();

        if (trimmed == "1")
        {
            return true;
        }
        if (trimmed == "0")
        {
            return false;
        }
        if (trimmed == "true")
        {
            return true;
        }
        if (trimmed == "false")
        {
            return false;
        }
        if (trimmed == "on")
        {
            return true;
        }
        if (trimmed == "off")
        {
            return false;
        }

        return false;
    }

    int64 iaString::toInt(const iaString &text)
    {
        int64 integer = 0;
        int64 sign = 1;

        wchar_t character;

        for (int i = 0; i < text.getLength(); ++i)
        {
            character = text[i];
            if (character >= '0' &&
                character <= '9')
            {
                int64 value = static_cast<int64>(character - '0');
                integer *= 10;
                integer += value;
            }
            else if (character == '-' && i == 0)
            {
                sign = -1;
            }
            else
            {
                con_err("invalid integer format " << text);
                return 0;
            }
        }

        return integer * sign;
    }

    float64 iaString::toFloat(const iaString &text)
    {
        float64 integer = 0.0;
        float64 part = 0.0;
        float64 sign = 1.0;
        float64 decimals = 1.0;

        bool beforeDecimal = true;
        wchar_t character;

        for (int i = 0; i < text.getLength(); ++i)
        {
            character = text[i];
            if (character >= '0' &&
                character <= '9')
            {
                float64 value = static_cast<float64>(character - '0');

                if (beforeDecimal)
                {
                    integer *= 10.0;
                    integer += value;
                }
                else
                {
                    part *= 10.0;
                    part += value;
                    decimals /= 10.0;
                }
            }
            else if (character == '-' && i == 0)
            {
                sign = -1.0;
            }
            else if (character == '.' && beforeDecimal)
            {
                beforeDecimal = false;
            }
            else
            {
                con_err("invalid floating number format " << text);
                return 0.0;
            }
        }

        return (integer + (part * decimals)) * sign;
    }

    iaString iaString::trimLeft(const iaString &text)
    {
        int64 start = text.findFirstNotOf(L" \n\r\t\f\v");
        return text.getSubString(start);
    }

    iaString iaString::trimRight(const iaString &text)
    {
        int64 stop = text.findLastNotOf(L" \n\r\t\f\v");
        return (stop == iaString::INVALID_POSITION) ? "" : text.getSubString(0, stop + 1);
    }

    iaString iaString::trim(const iaString &text)
    {
        return trimLeft(trimRight(text));
    }

} // namespace iaux
