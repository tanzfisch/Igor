template <typename TValue, typename TIndex>
iaRLE<TValue, TIndex>::~iaRLE()
{
    if (_data != nullptr)
    {
        delete[] _data;
    }
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setSize(TIndex size, TValue clearValue)
{
    if (_size != size)
    {
        _size = size;
        if (_mode == iaRLEMode::Uncompressed)
        {
            if (_data != nullptr)
            {
                delete[] _data;
            }
            _data = new TValue[_size];
        }
    }
    clear(clearValue);
}

template <typename TValue, typename TIndex> 
void iaRLE<TValue, TIndex>::clear(TValue clearValue)
{
    if (_mode == iaRLEMode::Compressed)
    {
        _blocks.clear();
        iaRLEBlock<TValue, TIndex> block;
        block._length = _size;
        block._value = clearValue;
        _blocks.push_back(block);
    }
    else
    {
        con_assert(_data != nullptr, "zero pointer");

        for (TIndex i = 0; i < _size; ++i)
        {
            _data[i] = clearValue;
        }
    }

    _dirty = true;
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getFirstValue()
{
    _currentIndex = 0;
    _currentBlock = 0;
    _currentBlockOffset = 0;

    if (_mode == iaRLEMode::Compressed)
    {
        return _blocks[_currentBlock]._value;
    }
    else
    {
        return _data[_currentIndex];
    }
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getNextValue()
{
    if (_mode == iaRLEMode::Compressed)
    {
        _currentIndex++;

        _currentBlockOffset++;
        if (_currentBlockOffset >= _blocks[_currentBlock]._length)
        {
            _currentBlock++;
            _currentBlockOffset = 0;
        }
        return _blocks[_currentBlock]._value;
    }
    else
    {
        _currentIndex++;
        return _data[_currentIndex];
    }
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getCurrentIndex() const
{
    return _currentIndex;
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ void iaRLE<TValue, TIndex>::setCurrentIndex(TIndex index)
{
    if (_mode == iaRLEMode::Compressed)
    {
        _currentBlock = 0;
        _currentBlockOffset = static_cast<TIndex>(0);
        _currentIndex = static_cast<TIndex>(0);

        auto blockIter = _blocks.begin();
        while (blockIter != _blocks.end())
        {
            if (index < (*blockIter)._length + _currentIndex)
            {
                _currentBlockOffset = index - _currentIndex;
                _currentIndex = index;
                return;
            }

            _currentIndex += (*blockIter)._length;

            blockIter++;
            _currentBlock++;
        }
    }
    else
    {
        _currentIndex = index;
    }
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getCurrentValue() const
{
    if (_mode == iaRLEMode::Compressed)
    {
        return _blocks[_currentBlock]._value;
    }
    else
    {
        return _data[_currentIndex];
    }
}

template <typename TValue, typename TIndex> 
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getValue(TIndex index) const
{
    con_assert(index < _size, "out of bounds");

    if (_mode == iaRLEMode::Compressed)
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

        con_err("invalid data");
        return static_cast<TValue>(0);
    }
    else
    {
        con_assert(_data != nullptr, "zero pointer");

        return _data[index];
    }
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setMode(iaRLEMode mode)
{
    if (_mode != mode)
    {
        _mode = mode;

        if (_mode == iaRLEMode::Compressed)
        {
            compress();
        }
        else
        {
            uncompress();
        }

        _dirty = false;
    }
}

template <typename TValue, typename TIndex>
iaRLEMode iaRLE<TValue, TIndex>::getMode() const
{
    return _mode;
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::compress()
{
    con_assert(_data != nullptr, "zero pointer");

    if (_data != nullptr)
    {
        if (_dirty)
        {
            _blocks.clear();

            TValue currentValue = _data[0];
            TValue currentLength = 1;
            for (TIndex i = 1; i < _size; ++i)
            {
                if (currentValue != _data[i])
                {
                    iaRLEBlock<TValue, TIndex> block;
                    block._value = currentValue;
                    block._length = currentLength;
                    _blocks.push_back(block);

                    currentValue = _data[i];
                    currentLength = 1;
                }
                else
                {
                    currentLength++;
                }
            }

            iaRLEBlock<TValue, TIndex> block;
            block._value = currentValue;
            block._length = currentLength;
            _blocks.push_back(block);
        }

        delete[] _data;
        _data = nullptr;
    }
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::uncompress()
{
    con_assert(_data == nullptr, "invalid data");

    _data = new TValue[_size];

    TIndex index = 0;
    for (auto block : _blocks)
    {
        for (TIndex i = 0; i < block._length; ++i)
        {
            _data[index++] = block._value;
        }
    }
}

template <typename TValue, typename TIndex>
__IGOR_INLINE__ void iaRLE<TValue, TIndex>::setValue(TIndex index, TIndex length, TValue value)
{
    con_assert(index < _size, "out of bounds");
    con_assert(index + length <= _size, "out of bounds");

    if (_mode == iaRLEMode::Compressed)
    {
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
                        (*blockIter)._length = index - currentBlockPos;
                        _blocks.push_back((*blockIter));

                        if (!blockInserted)
                        {
                            _blocks.push_back(block);
                            blockInserted = true;
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
    else
    {
        TIndex i = index;
        TIndex c = length;
        while(c > 0)
        {
            _data[i++] = value;
            c--;
        }
    }

    _dirty = true;
}

template <typename TValue, typename TIndex> 
__IGOR_INLINE__ void iaRLE<TValue, TIndex>::setValue(TIndex index, TValue value)
{
    con_assert(index < _size, "out of bounds");

    if (_mode == iaRLEMode::Compressed)
    {
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
                else if (index == currentBlockPos)  // beginning of block
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

#ifdef __IGOR_DEBUG__
        TIndex totalLength = static_cast<TIndex>(0);
        for (auto block : _blocks)
        {
            totalLength += block._length;
            con_assert(block._length != 0, "invalid size");
        }

        con_assert(totalLength == _size, "invalid data");
#endif
    }
    else
    {
        _data[index] = value;
    }

    _dirty = true;
}

