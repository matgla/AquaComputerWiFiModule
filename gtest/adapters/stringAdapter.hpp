#ifndef STRING_ADAPTER_HPP
#define STRING_ADAPTER_HPP

#include <string>
#include <algorithm>

class String
{
public:

    String(const char *cstr = "") :
        str_(cstr)
    {
    }
    String(const std::string &str) :
        str_(str)
    {}
    String(const String &str) :
        str_(str.getStr())
    {}


    String & operator = (const std::string &rhs)
    {
        str_ = rhs;
    }

    String & operator = (const String &rhs)
    {
        str_ = rhs.getStr();
    }
    String & operator = (const char *cstr)
    {
        str_ = cstr;
        return *this;
    }

    String & operator += (const String &rhs)
    {
        str_ += rhs.getStr();
        return (*this);
    }
	String & operator += (const char *cstr)
    {
        str_ += cstr;
        return *this;
    }

    int indexOf( char ch ) const
    {
        return str_.find(ch);
    }
	int indexOf( char ch, unsigned int fromIndex ) const
    {
        return str_.find(ch, fromIndex);
    }
	int indexOf( const String &str ) const
    {
        return str_.find(str.getStr());
    }
	int indexOf( const String &str, unsigned int fromIndex ) const
    {
        return str_.find(str.getStr(), fromIndex);
    }

    String substring( unsigned int beginIndex ) const
    {
        return substring(beginIndex, str_.length());
    };

    String substring( unsigned int beginIndex, unsigned int endIndex ) const
    {
        String str;
        str = str_.substr(beginIndex, endIndex);
        return str;
    };

    void toUpperCase()
    {
        std::string s = str_;
        std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
        str_ = s;
    }

    void remove(unsigned int index)
    {
        str_.erase(index);
    }

    void remove(unsigned int index, unsigned int count)
    {
        str_.erase(index, count);
    }

    inline unsigned int length(void) const {return str_.length();}

    std::string getStr() const
    {
        return str_;
    }

private:

    std::string str_;
};


#endif /* end of include guard: STRING_ADAPTER_HPP */
