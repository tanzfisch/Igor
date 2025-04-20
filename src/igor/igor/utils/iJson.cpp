// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header stream

#include <igor/utils/iJson.h>

namespace igor
{
    void to_json(json &j, const iAACubef &cube)
    {
        j["center"] = cube._center;
        j["halfEdgeLength"] = cube._halfEdgeLength;
    }

    void from_json(const json &j, iAACubef &cube)
    {
        cube._center = j["center"].get<iaVector3f>();
        cube._halfEdgeLength = j["halfEdgeLength"].get<float32>();
    }

    void to_json(json &j, const iAACubed &cube)
    {
        j["center"] = cube._center;
        j["halfEdgeLength"] = cube._halfEdgeLength;
    }

    void from_json(const json &j, iAACubed &cube)
    {
        cube._center = j["center"].get<iaVector3d>();
        cube._halfEdgeLength = j["halfEdgeLength"].get<float64>();
    }
}

namespace iaux
{
    void to_json(json &j, const iaUUID &uuid)
    {
        j = uuid.toString();
    }

    void from_json(const json &j, iaUUID &uuid)
    {
        uuid = iaUUID(iaString(j.get<std::string>().c_str()));
    }

    void to_json(json &j, const iaString &text)
    {
        j = text.getData();
    }

    void from_json(const json &j, iaString &text)
    {
        text = iaString(j.get<std::string>().c_str());
    }

    void to_json(json &j, const iaVector2f &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
    }

    void from_json(const json &j, iaVector2f &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
    }

    void to_json(json &j, const iaVector3f &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
        j["z"] = vec._z;
    }

    void from_json(const json &j, iaVector3f &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
        vec._z = j["z"].get<float32>();
    }

    void to_json(json &j, const iaVector4f &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
        j["z"] = vec._z;
        j["w"] = vec._w;
    }

    void from_json(const json &j, iaVector4f &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
        vec._z = j["z"].get<float32>();
        vec._w = j["w"].get<float32>();
    }

    void to_json(json &j, const iaVector2d &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
    }

    void from_json(const json &j, iaVector2d &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
    }

    void to_json(json &j, const iaVector3d &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
        j["z"] = vec._z;
    }

    void from_json(const json &j, iaVector3d &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
        vec._z = j["z"].get<float32>();
    }

    void to_json(json &j, const iaVector4d &vec)
    {
        j["x"] = vec._x;
        j["y"] = vec._y;
        j["z"] = vec._z;
        j["w"] = vec._w;
    }

    void from_json(const json &j, iaVector4d &vec)
    {
        vec._x = j["x"].get<float32>();
        vec._y = j["y"].get<float32>();
        vec._z = j["z"].get<float32>();
        vec._w = j["w"].get<float32>();
    }

    void to_json(json &j, const iaColor3f &color)
    {
        j["r"] = color._r;
        j["g"] = color._g;
        j["b"] = color._b;
    }

    void from_json(const json &j, iaColor3f &color)
    {
        color._r = j["r"].get<float32>();
        color._g = j["g"].get<float32>();
        color._b = j["b"].get<float32>();
    }

    void to_json(json &j, const iaColor4f &color)
    {
        j["r"] = color._r;
        j["g"] = color._g;
        j["b"] = color._b;
        j["a"] = color._a;
    }

    void from_json(const json &j, iaColor4f &color)
    {
        color._r = j["r"].get<float32>();
        color._g = j["g"].get<float32>();
        color._b = j["b"].get<float32>();
        color._a = j["a"].get<float32>();
    }

    void to_json(json &j, const iaRectanglef &rect)
    {
        j["x"] = rect._x;
        j["y"] = rect._y;
        j["width"] = rect._width;
        j["height"] = rect._height;
    }

    void from_json(const json &j, iaRectanglef &rect)
    {
        rect._x = j["x"].get<float32>();
        rect._y = j["y"].get<float32>();
        rect._width = j["width"].get<float32>();
        rect._height = j["height"].get<float32>();
    }

    void to_json(json &j, const iaRectangled &rect)
    {
        j["x"] = rect._x;
        j["y"] = rect._y;
        j["width"] = rect._width;
        j["height"] = rect._height;
    }

    void from_json(const json &j, iaRectangled &rect)
    {
        rect._x = j["x"].get<float64>();
        rect._y = j["y"].get<float64>();
        rect._width = j["width"].get<float64>();
        rect._height = j["height"].get<float64>();
    }
}