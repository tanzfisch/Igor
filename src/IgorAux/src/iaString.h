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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __IGOR_AUX_STRING__
#define __IGOR_AUX_STRING__

#include <iaDefines.h>

#include <ostream>
#include <vector>
using namespace std;

namespace IgorAux
{

    enum class StringSplitMode
    {
        Normal,
        RetriveAllEmpties
    };


	/*! wide char (unicode) character string with ending zero and lenght
	*/
	class IgorAux_API iaString
	{

	public:

		/*! this defines a position that is out of range
		*/
		static const uint16 INVALID_POSITION = 0xffff;

		/*! default ctor does nothing
		*/
		iaString() = default;

		/*! ctor converts a multibyte string in to unicode string

		\param text multipbyte string
		\param lenght if input string is not zero terminated or a custom lenght is needed
		*/
		iaString(const char* text, const uint16 lenght = INVALID_POSITION);

		/*! copies unicode string

		\param text unicode string
		\param lenght if input string is not zero terminated or a custom lenght is needed
		*/
		iaString(const wchar_t* text, const uint16 lenght = INVALID_POSITION);

        /*! creates a string with only one character

        \param character the character to create the string with
        */
        iaString(const wchar_t character);

        /*! creates a string with only one character

        \param character the character to create the string with
        */
        iaString(const char character);
        
		/*! copy ctor

		\param text the string to copy from
		*/
		iaString(const iaString& text);

		/*! dtor releases allocated memory
		*/
		~iaString();

        /*! \returns hash value for current text
        */
        int64 getHashValue() const;

        /*! change string to lower case letters
        */
        void toLower();

        /*! change string to upper case letters
        */
        void toUpper();

		/*! + operator concatanates two strings

		\param text the string to add
		\returns resulting concatanated string
		*/
		iaString operator+(const iaString &text) const;

		/*! += operator concatanates strings

		\param text the string to add to the current string
		*/
		void operator+=(const iaString &text);

		/*! + operator to add a single character

		\param character character to add
		*/
		iaString operator+(const wchar_t &character) const;

		/*! += operator to add a single character

		\param character character to add
		*/
		void operator+=(const wchar_t &character);

		/*! == operator to compare to strings

		\param text the string to compare with
		\returns true: if equal; false if not equal
		*/
		bool operator== (const iaString& text) const;

        /*! == operator to compare to strings

        \param text the string to compare with
        \returns true: if equal; false if not equal
        */
        bool operator== (const wchar_t* text) const;

        /*! == operator to compare to strings

        \param text the string to compare with
        \returns true: if equal; false if not equal
        */
        bool operator== (const char* text) const;

		/*! ++ operator to compare to strings

		\param text the strin to compare with
		\returns true: if not equal; false if equal
		*/
		bool operator!= (const iaString& text) const;

		/*! = operator overwrites current string with new string

		\param text the new string
		\returns the new string
		*/
		iaString operator=(const iaString &text);

        /*! = operator overwrites current string with new string

        \param text the new string
        \returns the new string
        */
        iaString operator=(const wchar_t* text);
		
        /*! = operator overwrites current string with new string

        \param text the new string
        \returns the new string
        */
        iaString operator=(const char* text);

        /*! = operator overwrites current string with one character

        \param character the character
        \returns the new string
        */
        iaString operator=(const wchar_t character);

        /*! = operator overwrites current string with one character

        \param character the character
        \returns the new string
        */
        iaString operator=(const char character);

		/*! \returns charachter at given position for manipulation
		\param index the position to get the character from
		*/
		wchar_t& operator[](const uint16 index);

		/*! \returns charachter at given position
		\param index the position to get the character from
		*/
		const wchar_t& operator[](const uint16 index) const;

		/*! \returns specified substring

		\param pos substring from
		\param len substring lenght
		*/
		iaString getSubString(const uint16 pos, const uint16 len = INVALID_POSITION) const;

        /*! plsits a string based on delimiter in to tokens (or substrings)

        \param delimiters string with delimiter characters
        \param tokens the resulting tokens
        */
        void split(const iaString& delimiters, vector<iaString>& tokens, StringSplitMode splitMode = StringSplitMode::Normal) const;

        /*! same as split with delimiter string but only for one delimiter character

        \param delimiter single delimiter character
        \param tokens the resulting tokens
        */
        void split(const wchar_t delimiter, vector<iaString>& tokens, StringSplitMode splitMode = StringSplitMode::Normal) const;

        /*! same as split with delimiter string but only for one delimiter character

        \param delimiter single delimiter character
        \param tokens the resulting tokens
        */
        void split(const char delimiter, vector<iaString>& tokens, StringSplitMode splitMode = StringSplitMode::Normal) const;

		/*! \returns position of first occurence of specified characters

		if not found iaString::INVALID_POSITION will be returned

		\param characters the characters to search for
		\param from optional parameter to define start index to search from
		*/
		uint16 findFirstOf(const wchar_t* characters, const uint16 from = INVALID_POSITION) const;

        /*! \returns position of first occurence of specified character

        if not found INVALID_POSITION will be returned

        \param character the character to search for
        \param from optional parameter to define start index to search from
        */
        uint16 findFirstOf(const wchar_t character, const uint16 from = INVALID_POSITION) const;

		/*! \returns position of first occurence NOT of specified characters

		if not found INVALID_POSITION will be returned

		\param characters the characters to NOT search for
		\param from optional parameter to define start index to search from
		*/
		uint16 findFirstNotOf(const wchar_t* characters, const uint16 from = INVALID_POSITION) const;

        /*! \returns position of first occurence NOT of specified character

        if not found INVALID_POSITION will be returned

        \param character the character to NOT search for
        \param from optional parameter to define start index to search from
        */
        uint16 findFirstNotOf(const wchar_t character, const uint16 from = INVALID_POSITION) const;

		/*! \returns position of last occurence of specified character

		if not found INVALID_POSITION will be returned

		\param character the character to search for
		*/
		uint16 findLastOf(const wchar_t character) const;

        /*! \returns position of last occurence of specified characters

        if not found INVALID_POSITION will be returned

        \param characters the characters to search for
        */
        uint16 findLastOf(const wchar_t* characters) const;

		/*! empties the string
		*/
		void clear();

		/*! \returns true: if string is empty; false if not
		*/
		bool isEmpty() const;

		/*! \returns the size (aka character count) of the string without ending zero
		*/
		uint16 getSize() const;

		/*! \returns pointer to raw data
		*/
		const wchar_t* getData() const;

		/*! returns multibyte string

		\param buffer pre allocated buffer to write the string to
		\param size size of preallocated buffer
		\returns actual size of written data including ending zero
		*/
		uint16 getData(char* buffer, const uint16 size) const;

        /*! returns utf8 encoded string

        \param buffer the detination buffer for the utf8 string
        \param size the size of the destination buffer
        \returns bytes written to destination buffer
        */
        uint16 getUTF8(char* buffer, const uint16 size) const;

        /*! calclates the size in bytes of the string encoded in utf8

        \returns size in bytes
        */
        uint16 getUTF8Size() const;

        /*! set string with utf8 encoded string

        \param buffer source buffer with utf8 string
        \param size the size of the source buffer
        */
        void setUTF8(const char* buffer, const uint16 size);

        /*! reverses the string
        */
        void reverse();

        /*! transforms an integer to a iaString
        
        \param value the integer value
        \returns isString
        */
        static iaString itoa(int32 value);

        /*! transforms a float to a iaString

        \todo would be nice to have a version that detects it self how many after points make sense

        \param value the float value
        \param afterPoint defines how many digits after the point
        \returns isString
        */
        static iaString ftoa(float32 value, int afterPoint);

        /*! transforms a iaString to a float

        \param text the string
        \returns value
        */
        static float32 atof(const iaString& text);

	private:

        /*! string lenght without ending zero
        */
        uint16 _charCount = 0;

        /*! pointer to actual data
        */
        wchar_t* _data = nullptr;

		/*! internal set data

		\param text the data to set
        \param size the size of the buffer
		*/
		void setData(const wchar_t* text, const uint16 size = INVALID_POSITION);

		/*! converts multibyte string to unicode string

		\param text the data to set
		\param size the size of the buffer
		*/
		void setData(const char* text, const uint16 size = INVALID_POSITION);

        static iaString intToStrInternal(int32 x, int d);

	};

    /*! stream operator

    \param stream the destination
    \param text the string to stream
    \returns the resulting stream
    */
	IgorAux_API wostream& operator<<(wostream& stream, const iaString& text);

}

#endif
