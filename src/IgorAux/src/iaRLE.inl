template <typename TValue, typename TIndex>
const vector<iaRLEBlock<TValue, TIndex>>& iaRLE<TValue, TIndex>::getBlocks() const
{
    return _blocks;
}

template <typename TValue, typename TIndex>
void iaRLE<TValue, TIndex>::setSize(TIndex size, TValue clearValue)
{
    _size = size;
    clear(clearValue);
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
__IGOR_INLINE__ TValue iaRLE<TValue, TIndex>::getValue(TIndex index) const
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

    con_assert(false, "invalid data");
    return static_cast<TValue>(0);
}

/*template <typename TValue, typename TIndex>
__IGOR_INLINE__ void iaRLE<TValue, TIndex>::setValue(TIndex index, TIndex length, TValue value)
{
    TIndex currentBlockPos = static_cast<TIndex>(0);
    auto blockIter = _blocks.begin();
    while (blockIter != _blocks.end())
    {
        if (index < (*blockIter)._length + currentBlockPos)
        {
            if (index == currentBlockPos) // beginning of a block
            {
                if (length == (*blockIter)._length) // covers the whole block
                {
                    (*blockIter)._value = value;
                    break;
                }
                else if (length < (*blockIter)._length)
                {

                }
            }
        }

        currentBlockPos += (*blockIter)._length;
        blockIter++;
    }

}*/

template <typename TValue, typename TIndex> 
__IGOR_INLINE__ void iaRLE<TValue, TIndex>::setValue(TIndex index, TValue value)
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
