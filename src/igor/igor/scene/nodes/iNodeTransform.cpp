// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeTransform.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <sstream>

namespace igor
{

    iNodeTransform::iNodeTransform()
        : iNode()
    {
        setName(L"iNodeTransform");
        _nodeType = iNodeType::iNodeTransform;
        _nodeKind = iNodeKind::Transformation;
    }

    iNodeTransform::iNodeTransform(iNodeTransform *node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = iNodeType::iNodeTransform;
        _nodeKind = iNodeKind::Transformation;

        iaMatrixd matrix;
        node->getMatrix(matrix);
        setMatrix(matrix);

        setName(node->getName());
    }

    void iNodeTransform::onUpdateTransform(iaMatrixd &matrix)
    {
        matrix *= _transform;
    }

    std::vector<iaString> iNodeTransform::getInfo(bool brief) const
    {
        std::vector<iaString> info = iNode::getInfo(brief);

        if(brief)
        {
            return info;
        }

        const auto &m = _transform;

        std::wstringstream stream;
        stream << "/" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._x << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._x << "\\";
        info.push_back(iaString(stream.str().c_str()));

        stream.str(std::wstring());
        stream << "|" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._y << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._y << "|";
        info.push_back(iaString(stream.str().c_str()));

        stream.str(std::wstring());
        stream << "|" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._right._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._top._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._depth._z << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._pos._z << "|";
        info.push_back(iaString(stream.str().c_str()));

        stream.str(std::wstring());
        stream << "\\" << std::setfill(L' ') << std::fixed << std::right << std::setprecision(2) << std::setw(10) << m._w0 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w1 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w2 << " " << std::setfill(L' ') << std::right << std::setprecision(2) << std::setw(10) << m._w3 << "/";
        info.push_back(iaString(stream.str().c_str()));

        return info;
    }

    void iNodeTransform::getMatrix(iaMatrixd &matrix)
    {
        matrix = _transform;
    }

    void iNodeTransform::setMatrix(const iaMatrixd &matrix)
    {
        if (_transform != matrix)
        {
            _transform.setData(matrix.getData());
            setTransformationDirty();
        }
    }

    void iNodeTransform::identity()
    {
        _transform.identity();
        setTransformationDirty();
    }

    void iNodeTransform::translate(const iaVector3d &vector)
    {
        _transform.translate(vector);
        setTransformationDirty();
    }

    void iNodeTransform::translate(float64 x, float64 y, float64 z)
    {
        _transform.translate(x, y, z);
        setTransformationDirty();
    }

    void iNodeTransform::setPosition(const iaVector3d &vector)
    {
        _transform._pos = vector;
        setTransformationDirty();
    }

    void iNodeTransform::setPosition(float64 x, float64 y, float64 z)
    {
        _transform._pos.set(x, y, z);
        setTransformationDirty();
    }

    void iNodeTransform::scale(const iaVector3d &vector)
    {
        _transform.scale(vector);
        setTransformationDirty();
    }

    void iNodeTransform::scale(float64 x, float64 y, float64 z)
    {
        _transform.scale(x, y, z);
        setTransformationDirty();
    }

    void iNodeTransform::rotate(float64 angle, iaAxis axis)
    {
        _transform.rotate(angle, axis);
        setTransformationDirty();
    }

    void iNodeTransform::rotate(float64 angleX, float64 angleY, float64 angleZ)
    {
        _transform.rotate(angleX, iaAxis::X);
        _transform.rotate(angleY, iaAxis::Y);
        _transform.rotate(angleZ, iaAxis::Z);
    }

}; // namespace igor
