#ifndef __seri_h__
#define __seri_h__

#include <string>

namespace seri
{
class type_converter
{
public:
    virtual void write_to_string(std::string&) const = 0;
    virtual void read_from_string(std::string&) const = 0;
};

template<typename TNum> class num_onverter : public type_converter
{
public:
    num_onverter(TNum& _num)
    : the_num(_num) {}

    void write_to_string(std::string& num_as_text) const override
    {
        num_as_text = std::to_string(the_num);
    }

    void read_from_string(std::string& num_as_text) const override
    {
        if (constexpr bool is_type = std::is_same<TNum, int>::value)
        {
            the_num = std::stoi(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, long>::value)
        {
            the_num = std::stol(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, long long>::value)
        {
            the_num = std::stoll(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, unsigned>::value)
        {
            the_num = std::stoul(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, unsigned long>::value)
        {
            the_num = std::stoul(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, unsigned long long>::value)
        {
            the_num = std::stoull(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, float>::value)
        {
            the_num = std::stof(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, double>::value)
        {
            the_num = std::stod(num_as_text);
        }
        else if (constexpr bool is_type = std::is_same<TNum, long double>::value)
        {
            the_num = std::stold(num_as_text);
        }
    }

    TNum& the_num;
};

class base
{
public:
    virtual ~base() {}
    virtual bool node_start(const char* node_name) = 0;
    virtual void node_end() = 0;
    virtual void text(std::string& text_value) = 0;
    virtual void text(const type_converter& text_converter) = 0;
    virtual void attrib(const char* attrib_name, std::string& attrib_value) = 0;
    virtual void attrib(const char* attrib_name, type_converter& text_converter) = 0;

    template<typename TNum> void number(TNum& num_value)
    {
        num_onverter<TNum> convert_num(num_value);
        text(convert_num);
    }
    template<typename TNum> void attrib_number(const char* attrib_name, TNum& num_value)
    {
        num_onverter<TNum> convert_num(num_value);
        attrib(attrib_name, convert_num);
    }
    virtual void node_with_text(const char* node_name, std::string& text_value)
    {
        if (node_start(node_name))
        {
            text(text_value);
            node_end();
        }
    }
    virtual void node_with_text(const char* node_name, const type_converter& text_converter)
    {
        if (node_start(node_name))
        {
            text(text_converter);
            node_end();
        }
    }
    template<typename TNum> void node_with_number(const char* node_name, TNum& in_num)
    {
        if (node_start(node_name))
        {
            number(in_num);
            node_end();
        }
    }
    virtual void node_with_attrib(const char* node_name, const char* attrib_name, std::string& attrib_value)
    {
        if (node_start(node_name))
        {
            attrib(attrib_name, attrib_value);
            node_end();
        }
    }
protected:
};
} // namespace seri
#endif // __seri_h__
