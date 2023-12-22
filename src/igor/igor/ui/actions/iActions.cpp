#include <igor/ui/actions/iActions.h>

#include <igor/resources/iResourceManager.h>
#include <igor/system/iApplication.h>

namespace igor
{

    iActionApplicationStop::iActionApplicationStop()
        : iAction("igor:exit")
    {
        setIcon("igor_icon_exit");
        setDescription("exit", "Exits application.");
    }

    void iActionApplicationStop::execute(const iActionContext &context)
    {
        iApplication::getInstance().exit();
    }

    bool iActionApplicationStop::isCompatible(const iActionContext &context)
    {
        return true;
    }

} // namespace igor