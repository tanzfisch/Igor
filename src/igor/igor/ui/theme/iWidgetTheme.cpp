// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/theme/iWidgetTheme.h>

#ifdef IGOR_LINUX
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <iostream>
#include <cstdlib>
#endif

namespace igor
{

#ifdef IGOR_LINUX
    static float32 getFontScaleFromXft()
    {
        Display *display = XOpenDisplay(nullptr);
        if (!display)
        {
            con_err("Unable to open X display");
            return 1.0f;
        }

        char *res_man_str = XResourceManagerString(display);
        if (!res_man_str)
        {
            con_err("XResourceManagerString is null");
            XCloseDisplay(display);
            return 1.0f;
        }

        XrmInitialize();
        XrmDatabase db = XrmGetStringDatabase(res_man_str);
        if (!db)
        {
            con_err("Failed to get X resource database");
            XCloseDisplay(display);
            return 1.0f;
        }

        XrmValue value;
        char *type;
        if (XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value) && value.addr)
        {
            float dpi = std::atof(value.addr);
            XCloseDisplay(display);
            return dpi / 96.0f; // Assuming 96 DPI is 1.0 scale
        }

        XCloseDisplay(display);
        return 1.0f;
    }
#endif

    float32 iWidgetTheme::getScale() const
    {
#ifdef IGOR_LINUX
        static auto scale = getFontScaleFromXft();
        return scale;
#else
        return 1.0f;
#endif
    }
}