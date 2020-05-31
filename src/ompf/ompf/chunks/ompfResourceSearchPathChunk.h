//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: martinloga@gmx.de

#ifndef __SEARCHPATHCHUNK__
#define __SEARCHPATHCHUNK__

#include <ompf/chunks/ompfBaseChunk.h>

namespace OMPF
{

    /*! search path chunk
    */
    class OMPF_API ompfResourceSearchPathChunk : public ompfBaseChunk
    {

    public:
        /*! does nothing
        */
        ompfResourceSearchPathChunk();

        /*! does nothing
        */
        virtual ~ompfResourceSearchPathChunk() = default;

        /*! \returns size of chunk in bytes

        \param settings the settings the size calculation is based on
        */
        virtual uint32 getSize(const ompfSettings &settings);

        /*! sets the search path

        \param path a path we later can search in
        */
        void setPath(iaString path);

        /*! \returns the search path
        */
        iaString getPath() const;

        /*! sets path type

        \param pathType path type
        */
        void setPathType(OMPFPathType pathType);

        /*! \returns path type
        */
        OMPFPathType getPathType() const;

    private:
        /*! path
        */
        iaString _path;

        /*! path type
        */
        OMPFPathType _pathType = OMPFPathType::Undefined;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        virtual bool write(std::ofstream &stream, const ompfSettings &settings);

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        virtual bool read(std::ifstream &stream, ompfSettings &settings);
    };

} // namespace OMPF

#endif