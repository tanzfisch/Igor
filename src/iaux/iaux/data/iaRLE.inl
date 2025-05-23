// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <typename TValue, typename TIndex>
iaRLE<TValue, TIndex>::iaRLE(TIndex size)
{
    setSize(size);
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setSize(TIndex size, TValue clearValue)
{
    con_assert(size > 0, "invalid size " << size);
    _size = size;
    clear(clearValue);
}

template <typename TValue, typename TIndex>
TIndex iaRLE<TValue, TIndex>::getSize() const
{
    return _size;
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::clear(TValue clearValue)
{
    _blocks.clear();
    iaRLEBlock<TValue, TIndex> block;
    block._length = _size;
    block._value = clearValue;
    _blocks.push_back(block);
}

template <typename TValue, typename TIndex>
iaRLE<TValue, TIndex> iaRLE<TValue, TIndex>::operator=(const iaRLE<TValue, TIndex> &data)
{
    iaRLE<TValue, TIndex> result;
    result._blocks = _blocks = data._blocks;
    result._size = _size = data._size;

    return result;
}

template <typename TValue, typename TIndex>
TValue iaRLE<TValue, TIndex>::getValue(TIndex index) const
{
    con_assert(index < _size, "out of bounds");

    if (index < _size)
    {
        TIndex currentBlockPos = static_cast<TIndex>(0);
        auto blockIter = _blocks.begin();
        while (blockIter != _blocks.end())
        {
            if (index < (*blockIter)._length + currentBlockPos)
            {
                return (*blockIter)._value;
            }

            currentBlockPos += (*blockIter)._length;

            blockIter++;
        }

        con_err("invalid data. index " << index << ", size " << _size << ", block count " << _blocks.size());
        return static_cast<TValue>(0);
    }
    else
    {
        con_err("out of bounds size:" << _size << " index:" << index);
        return static_cast<TValue>(0);
    }
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setValue(TIndex index, TIndex length, TValue value)
{
    con_assert(index < _size, "out of bounds");
    con_assert(index + length <= _size, "out of bounds");

    if (index >= _size ||
        index + length > _size)
    {
        con_err("out of bounds size:" << _size << " index:" << index << " length:" << length);
        return;
    }

    if (length == 1)
    {
        setValue(index, value);
    }
    else
    {
        auto blocks = _blocks;
        _blocks.clear();

        bool blockInserted = false;
        iaRLEBlock<TValue, TIndex> block;
        block._value = value;
        block._length = length;

        iaRLEBlock<TValue, TIndex> patchBlock;

        TIndex currentBlockPos = static_cast<TIndex>(0);
        TIndex currentBlockLength = static_cast<TIndex>(0);
        auto blockIter = blocks.begin();
        while (blockIter != blocks.end())
        {
            currentBlockLength = (*blockIter)._length;

            if (currentBlockPos < index)
            {
                if (currentBlockPos + currentBlockLength - 1 < index)
                {
                    _blocks.push_back((*blockIter));
                }
                else
                {
                    patchBlock._value = (*blockIter)._value;
                    patchBlock._length = index - currentBlockPos;
                    _blocks.push_back(patchBlock);

                    if (!blockInserted)
                    {
                        _blocks.push_back(block);
                        blockInserted = true;
                    }

                    int remainder = (*blockIter)._length;
                    remainder -= patchBlock._length;
                    remainder -= block._length;

                    if (remainder > 0)
                    {
                        patchBlock._length = remainder;
                        _blocks.push_back(patchBlock);
                    }
                }
            }
            else
            {
                if (!blockInserted)
                {
                    _blocks.push_back(block);
                    blockInserted = true;
                }

                if (currentBlockPos < index + length)
                {
                    if (currentBlockPos + currentBlockLength - 1 > index + length - 1)
                    {
                        (*blockIter)._length = (currentBlockPos + currentBlockLength - 1) - (index + length - 1);
                        _blocks.push_back((*blockIter));
                    }
                    // else skip this ones
                }
                else
                {
                    _blocks.push_back((*blockIter));
                }
            }

            currentBlockPos += currentBlockLength;
            blockIter++;
        }
    }
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setValue(TIndex index, TValue value)
{
    con_assert(index < _size, "out of bounds");

    TIndex currentBlockPos = static_cast<TIndex>(0);

    auto lastBlockIter = _blocks.end();
    auto blockIter = _blocks.begin();
    while (blockIter != _blocks.end())
    {
        if (index < (*blockIter)._length + currentBlockPos)
        {
            if ((*blockIter)._value == value)
            {
                break;
            }

            if (index == currentBlockPos + (*blockIter)._length - static_cast<TIndex>(1)) // end of block
            {
                auto nextBlockIter = blockIter;
                nextBlockIter++;

                if (nextBlockIter != _blocks.end() &&
                    (*nextBlockIter)._value == value)
                {
                    (*blockIter)._length--;
                    (*nextBlockIter)._length++;

                    if ((*blockIter)._length <= static_cast<TIndex>(0))
                    {
                        _blocks.erase(blockIter);
                    }
                }
                else
                {
                    if ((*blockIter)._length == 1)
                    {
                        (*blockIter)._value = value;
                    }
                    else
                    {
                        iaRLEBlock<TValue, TIndex> preBlock;
                        preBlock._value = (*blockIter)._value;
                        preBlock._length = (*blockIter)._length - static_cast<TIndex>(1);
                        (*blockIter)._value = value;
                        (*blockIter)._length = static_cast<TIndex>(1);

                        _blocks.insert(blockIter, preBlock);
                    }
                }

                break;
            }
            else if (index == currentBlockPos) // beginning of block
            {
                if (lastBlockIter != _blocks.end() &&
                    (*lastBlockIter)._value == value)
                {
                    (*lastBlockIter)._length++;
                    (*blockIter)._length--;

                    if ((*blockIter)._length <= static_cast<TIndex>(0))
                    {
                        _blocks.erase(blockIter);
                    }
                }
                else
                {
                    iaRLEBlock<TValue, TIndex> preBlock;
                    preBlock._value = value;
                    preBlock._length = static_cast<TIndex>(1);

                    (*blockIter)._length--;
                    _blocks.insert(blockIter, preBlock);
                }

                break;
            }
            else // somewhere in the middle
            {
                TIndex relativePos = index - currentBlockPos;

                iaRLEBlock<TValue, TIndex> insertedValue;
                insertedValue._value = value;
                insertedValue._length = static_cast<TIndex>(1);

                iaRLEBlock<TValue, TIndex> preBlock;
                preBlock._value = (*blockIter)._value;
                preBlock._length = relativePos;

                (*blockIter)._length = (*blockIter)._length - relativePos - static_cast<TIndex>(1);

                blockIter = _blocks.insert(blockIter, insertedValue);
                _blocks.insert(blockIter, preBlock);
                break;
            }
        }

        currentBlockPos += (*blockIter)._length;

        lastBlockIter = blockIter;
        blockIter++;
    }

#ifdef IGOR_DEBUG
    TIndex totalLength = static_cast<TIndex>(0);
    for (auto block : _blocks)
    {
        totalLength += block._length;
        con_assert(block._length != 0, "invalid size");
    }

    con_assert(totalLength == _size, "invalid data");
#endif
}