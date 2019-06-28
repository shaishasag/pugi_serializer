#ifndef __seri_h__
#define __seri_h__

#include <vector>
#include <string>
#include "pugixml.hpp"

namespace seri
{
class TypeConverter
{
public:
    virtual void write_to_string(std::string&) const = 0;
    virtual void read_from_string(std::string&) const = 0;
};

template<typename TNum> class NumConverter : public TypeConverter
{
public:
    NumConverter(TNum& _num)
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

class Base
{
public:
    virtual ~Base() {}
    virtual bool node_start(const char* node_name) = 0;
    virtual void node_end() = 0;
    virtual void text(std::string& text_value) = 0;
    virtual void text(const TypeConverter& text_converter) = 0;
    template<typename TNum> void number(TNum& num_value)
    {
        NumConverter<TNum> convert_num(num_value);
        text(convert_num);
    }
    virtual void attrib(const char* attrib_name, std::string& attrib_value) = 0;
    virtual void attrib(const char* attrib_name, TypeConverter& text_converter) = 0;
    template<typename TNum> void attrib_number(const char* attrib_name, TNum& num_value)
    {
        NumConverter<TNum> convert_num(num_value);
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
    virtual void node_with_text(const char* node_name, const TypeConverter& text_converter)
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

class PugiBase : public Base
{
public:
    PugiBase(pugi::xml_node parent_node)
    {
        node_stack.push_back(parent_node);
    }
    void node_end() override
    {
        node_stack.pop_back();
    }
protected:
    std::vector<pugi::xml_node> node_stack;
};

class PugiWrite : public PugiBase
{
public:
    PugiWrite(pugi::xml_node parent_node)
    : PugiBase(parent_node) {}

    bool node_start(const char* node_name) override
    {
        pugi::xml_node _node = node_stack.back().append_child(node_name);
        node_stack.push_back(_node);
        return true;
    }

    void text(std::string& text_value) override
    {
        node_stack.back().text().set(text_value.c_str());
    }
    void text(const TypeConverter& text_converter) override
    {
        std::string as_string;
        text_converter.write_to_string(as_string);
        text(as_string);
    }
    void attrib(const char* attrib_name, std::string& attrib_value) override
    {
        node_stack.back().append_attribute(attrib_name).set_value(attrib_value.c_str());
    }
    void attrib(const char* attrib_name, TypeConverter& text_converter) override
    {
        std::string as_string;
        text_converter.write_to_string(as_string);
        attrib(attrib_name, as_string);
    }
};

class PugiRead : public PugiBase
{
public:
    PugiRead(pugi::xml_node parent_node)
    : PugiBase(parent_node) {}

    bool node_start(const char* node_name) override
    {
        pugi::xml_node _node = node_stack.back().child(node_name);
        if (_node)
        {
            node_stack.push_back(_node);
        }
        return bool(_node);
    }
    void text(std::string& text_value) override
    {
        text_value = node_stack.back().child_value();
    }
    void text(const TypeConverter& text_converter) override
    {
        std::string as_string;
        text(as_string);
        text_converter.read_from_string(as_string);
    }
    void attrib(const char* attrib_name, std::string& attrib_value) override
    {
        pugi::xml_attribute _attrib = node_stack.back().attribute(attrib_name);
        if (_attrib)
        {
            attrib_value = _attrib.value();
        }
    }
    void attrib(const char* attrib_name, TypeConverter& text_converter) override
    {
        std::string as_string;
        attrib(attrib_name, as_string);
        text_converter.read_from_string(as_string);
    }
};
} // namespace seri
#endif // __seri_h__
