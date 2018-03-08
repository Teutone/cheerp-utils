/****************************************************************
 *
 * Copyright (C) 2012-2016 Alessandro Pignotti <alessandro@leaningtech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 ***************************************************************/

#ifndef _CHEERP_TYPES_H_2043d438
#define _CHEERP_TYPES_H_2043d438

#include <utility>
#include <string>
#include <limits>
#include <cheerpintrin.h>
#include "jsobject.h"

namespace [[cheerp::genericjs]] client
{

template<class T>
class TArray;

class EventListener;
class RegExp;

class String: public Object
{
private:
	template<typename C>
	static String* fromCharPtr(const C* s)
	{
		String* ret=new String();
		for(;*s;s++)
		{
			ret=ret->concat(*String::fromCharCode(*s));
		}
		return ret;
	}
	template<typename... Args>
	String* concat(const String&, Args&&... args);
	String* concat();
public:
	String() throw();
	//Utility constructor to use an existing String
	String(const String*) throw();
	String(long a) throw();
	String(unsigned long a) throw();
	String(int a) throw();
	String(unsigned int a) throw();
	String(float a) throw();
	String(double a) throw();
	String(const char* s) __attribute__((noinline)) :String(fromCharPtr<char>(s))
	{
	}
	String(const wchar_t* s) __attribute__((noinline)) :String(fromCharPtr<wchar_t>(s))
	{
	}
	template<typename... Args>
	String* concat(Args&&... args)
	{
		return concat(static_cast<const String&>(static_cast<Args&&>(args))...);
	}
	String* substr(int start) const;
	String* substr(int start, int length) const;
	String* substring(int start) const;
	String* substring(int start, int end) const;
	String* replace(const String&, const String&);
	String* replace(const String&, EventListener*);
	String* replace(RegExp*, const String&);
	String* replace(RegExp*, EventListener*);
	int charCodeAt(int index) const;
	String* charAt(int index) const;
	int get_length() const;
	int indexOf(const String&);
	int indexOf(const String&, int);
	int lastIndexOf(const String&);
	int lastIndexOf(const String&, int);
	TArray<String>* split(const String&) const;
	TArray<String>* split(const String&, int limit) const;
	TArray<String>* split(RegExp*) const;
	TArray<String>* split(RegExp*, int limit) const;
	static String* fromCharCode(int c) [[cheerp::static]];
	bool endsWith(const String&) const;
	int localeCompare(const String&) const;
	TArray<String>* match(const String&) const;
	TArray<String>* match(RegExp*) const;
	int search(const String&) const;
	int search(RegExp*) const;
	String* slice(int start);
	String* slice(int start, int end);
	String* toLowerCase() const;
	String* toLocaleLowerCase() const;
	String* toUpperCase() const;
	String* toLocaleUpperCase() const;
	String* trim() const;
	explicit operator std::string() const
	{
		//This assume an ascii string
		//TODO: Try wstring or similar
		std::string ret;
		ret.resize(get_length());
		for(int i=0;i<get_length();i++)
			ret[i] = charCodeAt(i);
		return ret;
	}
	static client::String* fromUtf8(const char * in, size_t len = std::numeric_limits<size_t>::max())
	{
		client::String* out = new client::String();
		unsigned int codepoint;
		while (*in != 0 && len > 0)
		{
			unsigned char ch = static_cast<unsigned char>(*in);
			// ASCII range
			if (ch <= 0x7f)
				codepoint = ch;
			// Continuation bytes
			else if (ch <= 0xbf)
				codepoint = (codepoint << 6) | (ch & 0x3f);
			// Start of 2-bytes sequence
			else if (ch <= 0xdf)
				codepoint = ch & 0x1f;
			// Start of 3-bytes sequence
			else if (ch <= 0xef)
				codepoint = ch & 0x0f;
			// Start of 4-bytes sequence
			else
				codepoint = ch & 0x07;
			++in;
			--len;
			// NOTE: we are assuming that invalid codepoints will be handled
			// in a sensible way by javascript strings
			if (len == 0 || ((*in & 0xc0) != 0x80))
			{
				if (codepoint <= 0xffff)
					out = out->concat(client::String::fromCharCode(codepoint));
				else
				{
					// surrogate pair
					codepoint -= 0x10000;
					unsigned int highSurrogate = (codepoint >> 10) + 0xd800;
					unsigned int lowSurrogate = (codepoint & 0x3ff) + 0xdc00;
					out = out->concat(client::String::fromCharCode(highSurrogate));
					out = out->concat(client::String::fromCharCode(lowSurrogate));
				}
			}
		}
		return out;
	}

};

class Array: public Object
{
public:
	template<typename... Args>
	Array(Args... args);
	Object*& operator[](int index)
	{
		return __builtin_cheerp_make_regular<Object*>(this, 0)[index];
	}
	Object* operator[](int index) const
	{
		return __builtin_cheerp_make_regular<Object*>(this, 0)[index];
	}
	int indexOf(Object* searchElement);
	int indexOf(Object* searchElement, int fromIndex);
	template<typename... Args>
	int push(Args... args);
	int get_length() const;
	template<typename... Args>
	Array* splice(int start, int deleteCount, Args... args);
	Object* shift();
	void unshift(client::Object*);
	Object* pop();
};

template<class T>
class TArray: public Array
{
public:
	template<typename... Args>
	TArray(Args... args):Array(args...)
	{
	}
	T*& operator[](int index)
	{
		return (T*&)Array::operator[](index);
	}
	T* operator[](int index) const
	{
		return (T*)Array::operator[](index);
	}
};

class Number: public Object
{
public:
	Number(double);
	client::String* toString(int base = 10);
};

typedef unsigned int UnsignedShort;
typedef unsigned int UnsignedLong;
typedef signed int Long;
typedef double UnsignedLongLong;
typedef void Void;
typedef unsigned int Boolean;
typedef double Double;
typedef Object* Any;
typedef void* (*Function)(void*);

}

#endif
