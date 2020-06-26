#include <igor/ui/actions/iActions.h>

#include <igor/system/iApplication.h>

namespace igor
{

    void iActionApplicationStop::execute(iActionContextPtr context)
    {
        iApplication::getInstance().stop();
    }

    iaString iActionApplicationStop::getName() const
    {
        return "app:stop";
    }

} // namespace igor