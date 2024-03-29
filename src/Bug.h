#pragma once

#include <fstream>

#ifndef DEBUG
#define DEBUG
#endif

/*
	struct for debugging - this is gross but can be used pretty much like an ofstream,
						   except the debug messages are stripped while compiling if
						   DEBUG is not defined.
	example:
		Bug bug;
		bug.open("./debug.txt");
		bug << state << endl;
		bug << "testing" << 2.0 << '%' << endl;
		bug.close();
*/
class CBug
{
public:
	std::ofstream File;

	CBug()
	{

	};

	//opens the specified file
	inline void Open(const std::string& Filename)
	{
#ifdef DEBUG
		File.open(Filename.c_str());
#endif
	};

	//closes the ofstream
	inline void Close()
	{
#ifdef DEBUG
		File.close();
#endif
	};
};

//output function for endl
inline CBug& operator<<(CBug& bug, std::ostream& (*manipulator)(std::ostream&))
{
#ifdef DEBUG
	bug.File << manipulator;
#endif

	return bug;
};

//output function
template <typename T>
inline CBug& operator<<(CBug& bug, const T& t)
{
#ifdef DEBUG
	bug.File << t;
#endif

	return bug;
};
