#include <iaux/system/iaDate.h>

#include <ctime>

namespace iaux
{
    iaDate::iaDate(uint32 day, uint32 month, uint32 year)
        : _day(day), _month(month), _year(year)
    {
    }

    iaDate::iaDate(const iaDate &other)
    {
        _day = other._day;
        _month = other._month;
        _year = other._year;
    }

    iaDate iaDate::getToday()
    {
        iaDate result;

        time_t t = time(nullptr);
        gmtime(&t);
        struct tm *now = localtime(&t);
        result._day = now->tm_mday;
        result._month = now->tm_mon;
        result._year = now->tm_year;

        return result;
    }

    uint32 iaDate::getDay() const
    {
        return _day;
    }

    uint32 iaDate::getMonth() const
    {
        return _month;
    }

    uint32 iaDate::getYear() const
    {
        return _year;
    }

} // namespace iaux