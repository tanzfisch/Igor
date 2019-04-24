Igor Code Style Guide                       {#styleguide}
=====================

Variables
---------
All variables start with a lower case letter.
If the Variable Name contain more than one word from the second word on every first letter of every word must be in upper case. Every other letter can be written in upper or lower case.

Examples:
- i
- output
- nodeIndex
- nodeID
- incredibleLongVariableName

Member Variables
----------------
All member variables and static definitions start with an underscore "_"

Examples:
- _matrix
- _fooBar

Functions and Methods
---------------------
All functions and methods start with a lower case letter. From the second word on every first letter of a word must be written in upper case. Every other letter can be in upper case too.
The first word is preferably a verb.

Examples:
- void runThis();
- int getID();
- void update();

If a function or method is a callback to an event it has to start whit the prefix 'on'.

Examples:
- onMouseWheel();
- onRenderOrtho();

Classes, Structs and Enums
--------------------------
All names of classes, structs and enums start with the prefixes according to the name space they are part off.
For the Igor name space this is "i" and for the IgorAux name space this is "ia".
After the prefix every first letter of every word must be upper case every other letter can be upper case.

Examples:
- class iMatrixd
- enum iaConsoleColor

{} Brackets and Indentation
---------------------------
Use {} brackets and indentation in a way to make clear where nesting starts and ends by aligning the 
opening and closing brackets with each other.

Eamples:

	if(foo == bar)
	{
		x = 1;
	}

	for(int i=0;i<10;++i)
	{
		if(i==bar)
		{
			x = 0;
		}
	}

Documentation
-------------

All symbols must be documented with doxygen style comments. No mater if internal or external interfaces. 
No mater if public or private members and methods.

Example:

	/*! This class is the greatest.
	*/
	class Bla
	{
		private:
		
			/*! private member index
			
			\todo do we need this one?
			*/
			int _index;
			
			/*! does nothing
			*/
			void fooBar();
		
		public:

			/*! does nothing
			
			\param word bird is the
			\returns always zero
			*/
			int fooBar(const string& word);

			/*! does some magic
			*/
			Bla();
			
			/*! cleans up
			*/
			~Bla();
	}