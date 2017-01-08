// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodeTransform::iNodeTransform()
        : iNode()
    {
        setName(L"iNodeTransform");
        _nodeType = iNodeType::iNodeTransform;
        _nodeKind = iNodeKind::Transformation;
    }

    iNodeTransform::iNodeTransform(iNodeTransform* node)
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

    void iNodeTransform::onUpdateTransform(iaMatrixd& matrix)
    {
        matrix *= _transform;
    }

    void iNodeTransform::getMatrix(iaMatrixd& matrix)
    {
        matrix = _transform;
    }

    void iNodeTransform::setMatrix(const iaMatrixd& matrix)
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

    void iNodeTransform::translate(const iaVector3d& vector)
    {
        _transform.translate(vector);
        setTransformationDirty();
    }

    void iNodeTransform::translate(float64 x, float64 y, float64 z)
    {
        _transform.translate(x, y, z);
        setTransformationDirty();
    }

    void iNodeTransform::setPosition(const iaVector3d& vector)
    {
        _transform._pos.set(vector._x, vector._y, vector._z);
        setTransformationDirty();
    }

    void iNodeTransform::setPosition(float64 x, float64 y, float64 z)
    {
        _transform._pos.set(x, y, z);
        setTransformationDirty();
    }

    void iNodeTransform::scale(const iaVector3d& vector)
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

};
