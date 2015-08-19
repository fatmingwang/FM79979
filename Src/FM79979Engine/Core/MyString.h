#ifndef _MY_STRING_H_
#define _MY_STRING_H_

class	cMyString
{
public:
	cMyString(const char*);
	cMyString(cMyString*e_pMyString);
	~cMyString();
	int	length;
	operator=	(char*e_String);
	operator+=	(char*e_String);
	const char*	c_str();
};

#endif