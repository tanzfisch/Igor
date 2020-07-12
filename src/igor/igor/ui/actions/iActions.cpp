#include <igor/ui/actions/iActions.h>

#include <igor/system/iApplication.h>

namespace igor
{

    iActionApplicationStop::iActionApplicationStop()
        : iAction("app:stop")
    {
    }

    void iActionApplicationStop::execute(const iActionContext &context)
    {
        iApplication::getInstance().stop();
    }

} // namespace igor