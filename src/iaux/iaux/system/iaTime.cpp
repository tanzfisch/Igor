#include <iaux/system/iaTime.h>

#include <iaux/system/iaClock.h>

namespace iaux
{
    iaTime::iaTime(const iaTime &other)
    {
        _microSeconds = other._microSeconds;
    }

    iaTime::iaTime(int64 microSeconds)
    {
        _microSeconds = microSeconds;
    }

    iaTime iaTime::getNow()
    {
        return iaTime(iaClock::getTimeMicroseconds());
    }

    iaTime iaTime::fromSeconds(float64 time)
    {
        return iaTime(static_cast<int64>(time * 1000000.0));
    }

    iaTime iaTime::fromMilliseconds(float64 time)
    {
        return iaTime(static_cast<int64>(time * 1000.0));
    }

    void iaTime::operator+=(const iaTime &time)
    {
        _microSeconds += time._microSeconds;
    }

    iaTime iaTime::operator+(const iaTime &time) const
    {
        return iaTime(_microSeconds + time._microSeconds);
    }

    void iaTime::operator-=(const iaTime &time)
    {
        _microSeconds -= time._microSeconds;
    }

    iaTime iaTime::operator-(const iaTime &time) const
    {
        return iaTime(_microSeconds - time._microSeconds);
    }

    iaTime iaTime::operator*(float64 scalar) const
    {
        return iaTime(static_cast<int64>(static_cast<float64>(_microSeconds) * scalar));
    }

    void iaTime::operator*=(float64 scalar)
    {
        _microSeconds = static_cast<int64>(static_cast<float64>(_microSeconds) * scalar);
    }

    iaTime iaTime::operator/(float64 scalar) const
    {
        return iaTime(static_cast<int64>(static_cast<float64>(_microSeconds) / scalar));
    }

    void iaTime::operator/=(float64 scalar)
    {
        _microSeconds = static_cast<int64>(static_cast<float64>(_microSeconds) / scalar);
    }    

    bool iaTime::operator==(const iaTime &time) const
    {
        return _microSeconds == time._microSeconds;
    }

    bool iaTime::operator!=(const iaTime &time) const
    {
        return _microSeconds != time._microSeconds;
    }

    int64 iaTime::getMicrosenconds() const
    {
        return _microSeconds;
    }

    float64 iaTime::getSeconds() const
    {
        return static_cast<float64>(_microSeconds) / 1000000.0;
    }

    float64 iaTime::getMilliseconds() const
    {
        return static_cast<float64>(_microSeconds) / 1000.0;
    }

    bool iaTime::operator<(const iaTime &time) const
    {
        return _microSeconds < time._microSeconds;
    }

    bool iaTime::operator>(const iaTime &time) const
    {
        return _microSeconds > time._microSeconds;
    }
    bool iaTime::operator<=(const iaTime &time) const
    {
        return _microSeconds <= time._microSeconds;
    }

    bool iaTime::operator>=(const iaTime &time) const
    {
        return _microSeconds >= time._microSeconds;
    }

} // namespace iaux