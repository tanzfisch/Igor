//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __iSKYSIMULATION__
#define __iSKYSIMULATION__

#include <iDefines.h>

namespace Igor
{

    class iScene;
    class iNode;
    class iSkyLightNode;
    class iNodeSkyBox;
    class iDirectionalLightNode;

    class IGOR_API iSkySimulation
    {

    private:
        iScenePtr scene;
        shared_ptr<iNode> skySimRoot;
        iSkyLightNode *skyLightNode;
        iNodeSkyBox *skyBoxNode;
        iDirectionalLightNode *directionalLightNode;

        float32 latitude;
        float32 longitude;

        float32 time;
        int32 day;
        int32 month;
        int32 year;

        void update();
        void init();

    public:
        void setGeoPos(float32 latitude, float32 longitude);
        void setTime(float32 time); ///! local time
        void setDate(int32 day, int32 month, int32 year);

        iSkySimulation(iScenePtr scene);
        virtual ~iSkySimulation();
    };

} // namespace Igor

#endif