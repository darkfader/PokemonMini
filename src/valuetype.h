#ifndef _VALUETYPE_H
#define _VALUETYPE_H

extern char undefined[];

/*
 * ValueType
 */
class ValueType
{
private:
	char *s;	// if set, this value is a string
	long i;

public:
	ValueType();
	ValueType(const ValueType &rhs);
	ValueType& operator = (const ValueType &rhs);

	//ValueType & operator = (char *s);
	//ValueType & operator = (long i);
	//void setString(char *s);
	ValueType(const char *s);
	ValueType(long i);
	void Free();
	~ValueType();
	
	ValueType operator + (const ValueType &rhs);

	ValueType operator != (const ValueType &rhs) const;
	ValueType operator == (const ValueType &rhs) const;
	ValueType operator > (const ValueType &rhs) const;
	ValueType operator < (const ValueType &rhs) const;
	ValueType operator >= (const ValueType &rhs) const;
	ValueType operator <= (const ValueType &rhs) const;

	operator long () const;
	//operator char * ();
	const char * getString() const;		// don't destroy object yet!
	void undefine();
	bool defined();

	void print() const;

	static ValueType zero;
};


#endif	// _VALUETYPE_H
