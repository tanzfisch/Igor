// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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

        iaMatrixf matrix;
        node->getMatrix(matrix);
        setMatrix(matrix);

        setName(node->getName());
    }

    void iNodeTransform::onUpdateTransform(iaMatrixf& matrix)
    {
        matrix *= _transform;
    }

	void iNodeTransform::getMatrix(iaMatrixf& matrix)
	{
		matrix = _transform;
	}

	void iNodeTransform::setMatrix(const iaMatrixf& matrix)
	{
        if (_transform != matrix)
        {
            _transform = matrix;
            setTransformationDirty();
        }
	}

    void iNodeTransform::identity()
    {
        _transform.identity();
        setTransformationDirty();
    }

    void iNodeTransform::translate(iaVector3f &vector)
	{
        _transform.translate(vector);
		setTransformationDirty();
	}

	void iNodeTransform::translate(float32 x, float32 y, float32 z)
	{
		_transform.translate(x, y, z);
		setTransformationDirty();
	}
    void iNodeTransform::scale(iaVector3f &vector)
	{
        _transform.scale(vector);
		setTransformationDirty();
	}

	void iNodeTransform::scale(float32 x, float32 y, float32 z)
	{
		_transform.scale(x, y, z);
		setTransformationDirty();
	}

	void iNodeTransform::rotate(float32 angle, iaAxis axis)
	{
		_transform.rotate(angle, axis);
        setTransformationDirty();
	}

};
