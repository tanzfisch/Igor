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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iSOUNDRESOURCEFACTORY__
#define __iSOUNDRESOURCEFACTORY__

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <iScene.h>
#include <iaMatrix.h>

#include <vector>
using namespace std;

namespace Igor
{

	class iSoundBuffer
	{
		friend class iSoundResourceFactory;
		friend class iSound;

	private:

		uint32 bufferid;				// OpenAL ID
		iaString filename;					// Datei Name
		bool isdummy;						// true wenn sound noch nicht geladen wurde
		uint32 refcount;				// Anzahl Referenzierungen

		iSoundBuffer(iaString filename);
		~iSoundBuffer();
	};

    /*!

    \todo move to separate file
    */
	class Igor_API iSound
	{
		friend class iSoundResourceFactory;

	private:

		iSoundBuffer *buffer;

		uint32 sourceid;				// OpenAL ID
		bool loop;							// ob geloopt abgespielt werden soll

		iSound(iSoundBuffer *buffer);
		~iSound();

	public:

		__IGOR_INLINE__ void linkToListener();						// damit Source mit dem listener mit bewegt wird
		__IGOR_INLINE__ void unlinkFromListener();					// source soll nicht mehr mit dem listener mit bewegt werden

		__IGOR_INLINE__ void setSourcePosition(iaVector3f &vec);		// setzt position der quelle
		__IGOR_INLINE__ void setSourceVelocity(iaVector3f &vel);		// setzt geschwidigkeitsvektor der quelle

		__IGOR_INLINE__ void play(bool loop = false);				// startet sound
		__IGOR_INLINE__ void stop();								// stopt sound
		__IGOR_INLINE__ void pause();								// pausiert sound
		__IGOR_INLINE__ void rewind();								// setzt sound an start (??und spielt weiter??)
	};

/*	class Igor_API iSoundNode : public iNode
	{

	private:

		void preRenderCustom();

		vector<iSound*> sounds;
		vector<iSound*> activesounds;
		iaVector3f sound_position;
		bool firsttime;

	public:

		void addSoundFile(iaString filename);

		void play(uint32 soundid, bool loop = false);
		void stop(uint32 soundid);
		void pause(uint32 soundid);
		void rewind(uint32 soundid);

		iSoundNode();
		~iSoundNode();
	};*/

	class Igor_API iSoundResourceFactory : public iaSingleton<iSoundResourceFactory>
	{

		friend class iaSingleton<iSoundResourceFactory>;

	private:

		vector<iSound*> sounds;								// liste der sourcen
		vector<iSoundBuffer*> buffers;						// liste der buffer

		iaMatrixf listener_matrix;								// listener iaMatrix

		iSoundBuffer* loadBuffer(iaString filename);
		iSoundBuffer* loadWAVBuffer(iaString filename);		// läd ein wav file und erzeugt einen buffer
		//iSoundBuffer* loadOGGBuffer(iaString filename);		

		iSoundResourceFactory();
		virtual ~iSoundResourceFactory();

	public:

		iSound* loadFile(iaString filename);					// läd sound direkt von platte
		void unloadFile(iSound *sound);						// gibt buffer frei (ohne flush hat dieser Befehl keinen effekt)
		void flush();

		__IGOR_INLINE__ void setListenerMatrix(iaMatrixf &matrix, bool calc_velocity = true);	// setzt position und orientierung des ziels
		__IGOR_INLINE__ void setListenerVelocity(iaVector3f &vel);	// setzt geschwidigkeitsvektor des ziels
	};

};

#endif
