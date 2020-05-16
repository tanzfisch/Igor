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
// (c) Copyright 2012-2019 by Martin Loga
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
#include <iaVector2.h>
#include <iaVector3.h>
#include <iaVector4.h>
#include <iaConsole.h>

#include <ostream>
#include <vector>

namespace IgorAux
{

	/*! string slpit mode
    */
	enum class iaStringSplitMode
	{
		Normal,
		RetriveAllEmpties
	};

	/*! wide char (unicode) character string with trailing zero and length

	\todo should eventually store data UTF-8 encoded internally
	*/
	class IgorAux_API iaString
	{

	public:
		/*! this defines a position that is out of range
		*/
		static const int64 INVALID_POSITION = -1;

		/*! default ctor does nothing
		*/
		iaString() = default;

		/*! ctor with byte string

		\param text byte string
		\param length if input string is not zero terminated or a custom length is needed
		*/
		iaString(const char *text, const int64 length = INVALID_POSITION);

		/*! copies unicode string

		\param text unicode string
		\param length if input string is not zero terminated or a custom length is needed
		*/
		iaString(const wchar_t *text, const int64 length = INVALID_POSITION);

		/*! creates a string with only one character

        \param character the wide character to create the string with
        */
		iaString(const wchar_t character);

		/*! creates a string with only one character

        \param character the character to create the string with
        */
		iaString(const char character);

		/*! copy ctor

		\param text the string to copy from
		*/
		iaString(const iaString &text);

		/*! dtor releases allocated memory
		*/
		~iaString();

		/*! \returns character count of the string without trailing zero
		*/
		int64 getLength() const;

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
		bool operator==(const iaString &text) const;

		/*! == operator to compare to strings

        \param text the string to compare with
        \returns true: if equal; false if not equal
        */
		bool operator==(const wchar_t *text) const;

		/*! == operator to compare to strings

        \param text the string to compare with
        \returns true: if equal; false if not equal
        */
		bool operator==(const char *text) const;

		/*! ++ operator to compare to strings

		\param text the strin to compare with
		\returns true: if not equal; false if equal
		*/
		bool operator!=(const iaString &text) const;

		/*! = operator overwrites current string with new string

		\param text the new string
		\returns the new string
		*/
		iaString operator=(const iaString &text);

		/*! = operator overwrites current string with new string

        \param text the new string
        \returns the new string
        */
		iaString operator=(const wchar_t *text);

		/*! = operator overwrites current string with new string

        \param text the new string
        \returns the new string
        */
		iaString operator=(const char *text);

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

		/*! compares two strings and returns true if the left hand side string is considered smaller

		\param text the right hand side string
		*/
		bool operator<(const iaString &text) const;

		/*! compares two strings and returns true if the left hand side string is considered bigger

		\param text the right hand side string
		*/
		bool operator>(const iaString &text) const;

		/*! \returns charachter at given position for manipulation
		\param index the position to get the character from
		*/
		wchar_t &operator[](const int64 index);

		/*! \returns charachter at given position
		\param index the position to get the character from
		*/
		const wchar_t &operator[](const int64 index) const;

		/*! \returns specified substring

		\param pos substring from
		\param len substring length
		*/
		iaString getSubString(const int64 pos, const int64 len = INVALID_POSITION) const;

		/*! plsits a string based on delimiter in to tokens (or substrings)

        \param delimiters string with delimiter characters
        \param tokens the resulting tokens
        */
		void split(const iaString &delimiters, std::vector<iaString> &tokens, iaStringSplitMode splitMode = iaStringSplitMode::Normal) const;

		/*! same as split with delimiter string but only for one delimiter character

        \param delimiter single delimiter character
        \param tokens the resulting tokens
        */
		void split(const wchar_t delimiter, std::vector<iaString> &tokens, iaStringSplitMode splitMode = iaStringSplitMode::Normal) const;

		/*! same as split with delimiter string but only for one delimiter character

        \param delimiter single delimiter character
        \param tokens the resulting tokens
        */
		void split(const char delimiter, std::vector<iaString> &tokens, iaStringSplitMode splitMode = iaStringSplitMode::Normal) const;

		/*! \returns position of first occurence of specified characters

		if not found iaString::INVALID_POSITION will be returned

		\param characters the characters to search for
		\param from optional parameter to define start index to search from
		*/
		int64 findFirstOf(const wchar_t *characters, const int64 from = INVALID_POSITION) const;

		/*! \returns position of first occurence of specified character

        if not found INVALID_POSITION will be returned

        \param character the character to search for
        \param from optional parameter to define start index to search from
        */
		int64 findFirstOf(const wchar_t character, const int64 from = INVALID_POSITION) const;

		/*! \returns position of first occurence NOT of specified characters

		if not found INVALID_POSITION will be returned

		\param characters the characters to NOT search for
		\param from optional parameter to define start index to search from
		*/
		int64 findFirstNotOf(const wchar_t *characters, const int64 from = INVALID_POSITION) const;

		/*! \returns position of first occurence NOT of specified character

        if not found INVALID_POSITION will be returned

        \param character the character to NOT search for
        \param from optional parameter to define start index to search from
        */
		int64 findFirstNotOf(const wchar_t character, const int64 from = INVALID_POSITION) const;

		/*! \returns position of last occurence of specified character

		if not found INVALID_POSITION will be returned

		\param character the character to search for
		*/
		int64 findLastOf(const wchar_t character) const;

		/*! \returns position of last occurence of specified characters

        if not found INVALID_POSITION will be returned

        \param characters the characters to search for
        */
		int64 findLastOf(const wchar_t *characters) const;

		/*! \returns position of last occurence NOT of specified character

		if not found INVALID_POSITION will be returned

		\param character the character to search for
		*/
		int64 findLastNotOf(const wchar_t character) const;

		/*! \returns position of last occurence NOT of specified characters

		if not found INVALID_POSITION will be returned

		\param characters the characters to search for
		*/
		int64 findLastNotOf(const wchar_t *characters) const;

		/*! empties the string
		*/
		void clear();

		/*! \returns true: if string is empty; false if not
		*/
		bool isEmpty() const;

		/*! \returns pointer to raw data
		*/
		const wchar_t *getData() const;

		/*! returns null terminated byte string

		\param buffer pre allocated buffer to write the string to
		\param size size of preallocated buffer
		\returns actual size of written data including ending zero
		*/
		int64 getData(char *buffer, const int64 size) const;

		/*! returns utf8 encoded string

        \param buffer the detination buffer for the utf8 string
        \param size the size of the destination buffer
        \returns bytes written to destination buffer
        */
		int64 getUTF8(char *buffer, const int64 size) const;

		/*! calclates the size in bytes of the string encoded in utf8

        \returns size in bytes
        */
		int64 getUTF8Size() const;

		/*! set string with utf8 encoded string

        \param buffer source buffer with utf8 string
        \param size the size of the source buffer
        */
		void setUTF8(const char *buffer, const int64 size);

		/*! reverses the string
        */
		void reverse();

		/*! transforms an integer to a iaString
        
        \param value the integer value
        \returns isString
        */
		static iaString toString(uint64 value);

		/*! transforms an integer to a iaString

		\param value the integer value
		\returns isString
		*/
		static iaString toString(int64 value);

		/*! transforms an integer to a iaString

		\param value the integer value
		\returns isString
		*/
		static iaString toString(int32 value);

		/*! transforms an integer to a iaString

		\param value the integer value
		\returns isString
		*/
		static iaString toString(uint32 value);

		/*! transforms a float to a iaString

        \param value the float value
        \param afterPoint defines how many digits after the point
        \returns isString
        */
		static iaString toString(float64 value, int afterPoint = 0);

		/*! transforms a float to a iaString

		\param value the float value
		\param afterPoint defines how many digits after the point
		\returns isString
		*/
		static iaString toString(float32 value, int afterPoint = 0);

		/*! transforms a iaString to a float

        \param text the string
        \returns value
        */
		static float64 toFloat(const iaString &text);

		/*! converts string in to bool value

		on error it falls back to false

		\param text the text to convert
		\returns true of false depending on the text
		*/
		static bool toBool(const iaString &text);

		/*! converts a string to a 2d vector

		expect two comma separated values

		\param text the text to parse
		\param[out] vector the resulting vector
		*/
		template <class T>
		static void toVector(const iaString &text, iaVector2<T> &vector);

		/*! converts a string to a 3d vector

		expect two comma separated values

		\param text the text to parse
		\param[out] vector the resulting vector
		*/
		template <class T>
		static void toVector(const iaString &text, iaVector3<T> &vector);

		/*! converts a string to a 4d vector

		expect two comma separated values

		\param text the text to parse
		\param[out] vector the resulting vector
		*/
		template <class T>
		static void toVector(const iaString &text, iaVector4<T> &vector);

		/*! insert text at given position

		\param text the text to insert
		\param pos the position where to insert the text
		*/
		void insert(const iaString &text, int64 pos);

		/*! removed charcters from string

		\param pos index of first character to remove
		\param length amount of characters to remove
		*/
		void remove(int64 pos, int64 length);

		/*! trims white spaces on the left hand side

		\param text the source text
		\returns the trimmed text
		*/
		static iaString trimLeft(const iaString &text);

		/*! trims white spaces on the right hand side

		\param text the source text
		\returns the trimmed text
		*/
		static iaString trimRight(const iaString &text);

		/*! trims white spaces on both ends of the string

		\param text the source text
		\returns the trimmed text
		*/
		static iaString trim(const iaString &text);

	private:
		/*! string length without ending zero
        */
		int64 _charCount = 0;

		/*! pointer to actual data
        */
		wchar_t *_data = nullptr;

		/*! \returns size of data in bytes
		*/
		int64 getSize() const;

		/*! internal set data

		\param text the data to set
        \param size the size of the buffer
		*/
		void setData(const wchar_t *text, const int64 size = INVALID_POSITION);

		/*! converts byte string to unicode string

		\param text the data to set
		\param size the size of the buffer
		*/
		void setData(const char *text, const int64 size = INVALID_POSITION);

		/*! internal implementation of toString

		\param x the number to turn in to a string
		\param d number of digits in output if d is bigger than digits in x 0s will be added
		\returns string with number
		*/
		static iaString intToStrInternal(int64 x, int d);
	};

	/*! stream operator

    \param stream the destination
    \param text the string to stream
    \returns the resulting stream
    */
	IgorAux_API std::wostream &operator<<(std::wostream &stream, const iaString &text);

	template <class T>
	void iaString::toVector(const iaString &text, iaVector2<T> &vector)
	{
		std::vector<iaString> tokens;
		text.split(',', tokens);
		con_assert(tokens.size() == 2, "invalid format");

		float64 x = iaString::toFloat(iaString::trim(tokens[0]));
		float64 y = iaString::toFloat(iaString::trim(tokens[1]));

		vector.set(static_cast<T>(x), static_cast<T>(y));
	}

	template <class T>
	void iaString::toVector(const iaString &text, iaVector3<T> &vector)
	{
		std::vector<iaString> tokens;
		text.split(',', tokens);
		con_assert(tokens.size() == 3, "invalid format");

		float64 x = iaString::toFloat(iaString::trim(tokens[0]));
		float64 y = iaString::toFloat(iaString::trim(tokens[1]));
		float64 z = iaString::toFloat(iaString::trim(tokens[2]));

		vector.set(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
	}

	template <class T>
	void iaString::toVector(const iaString &text, iaVector4<T> &vector)
	{
		std::vector<iaString> tokens;
		text.split(',', tokens);
		con_assert(tokens.size() == 4, "invalid format");

		float64 x = iaString::toFloat(iaString::trim(tokens[0]));
		float64 y = iaString::toFloat(iaString::trim(tokens[1]));
		float64 z = iaString::toFloat(iaString::trim(tokens[2]));
		float64 w = iaString::toFloat(iaString::trim(tokens[4]));

		vector.set(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w));
	}
} // namespace IgorAux

namespace std
{
	template <>
	struct less<IgorAux::iaString>
	{
		bool operator()(const IgorAux::iaString &lhs, const IgorAux::iaString &rhs) const
		{
			return lhs < rhs;
		}
	};
} // namespace std

#endif
