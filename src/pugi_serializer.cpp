/**
 * xml serializer based on pugi parser - version 0.1
 * --------------------------------------------------------
 * Copyright (C) 2021, by Shai Shsag (shaishasag@yahoo.co.uk)
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugi xml parser, which is:
 * Copyright (C) 2006, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 */


#ifndef __SOURCE_PUGI_SERIALIZER_CPP__
#define __SOURCE_PUGI_SERIALIZER_CPP__

#include "pugi_serializer.hpp"

namespace pugi_serializer
{
    
namespace impl
{

class impl_base
{
public:
    impl_base() = default;
    virtual ~impl_base() = default;
    
    impl_base(const impl_base&) = default;
    impl_base& operator=(const impl_base&) = default;
    impl_base(impl_base&&) = default;
    impl_base& operator=(impl_base&&) = default;

    bool reading() const { return _reading;}
    bool writing() const { return !_reading;}
    void set_should_write_default_values(const bool _should_write_default_values) { _write_default_values = _should_write_default_values; }
    bool get_should_write_default_values() {return _write_default_values;}

    virtual void node_name(pugi::xml_node _node, std::string& _name) = 0;
    virtual pugi::xml_node child(pugi::xml_node _node, const char* _name) = 0;
    virtual pugi::xml_node next_sibling(pugi::xml_node _node, const char* _name) = 0;

    virtual const char* c_str(pugi::xml_node _node, const char* _c_str) = 0;
    virtual void text(pugi::xml_node _node, std::string& _text) = 0;
    virtual void text(pugi::xml_node _node, std::string& _text, std::string_view default_text) = 0;
    virtual void text(pugi::xml_node _node, int& _int) = 0;
    virtual void text(pugi::xml_node _node, int& _int, const int def) = 0;
    virtual void text(pugi::xml_node _node, unsigned& _uint) = 0;
    virtual void text(pugi::xml_node _node, unsigned& _uint, const unsigned def) = 0;
    virtual void text(pugi::xml_node _node, float& _float) = 0;
    virtual void text(pugi::xml_node _node, float& _float, const float def) = 0;
    virtual void text(pugi::xml_node _node, double& _double) = 0;
    virtual void text(pugi::xml_node _node, double& _double, const double def) = 0;
    virtual void text(pugi::xml_node _node, bool& _bool) = 0;
    virtual void text(pugi::xml_node _node, bool& _bool, const bool def) = 0;
    virtual void text(pugi::xml_node _node, long long& _llint) = 0;
    virtual void text(pugi::xml_node _node, long long& _llint, const long long def) = 0;
    virtual void text(pugi::xml_node _node, unsigned long long& _ullint) = 0;
    virtual void text(pugi::xml_node _node, unsigned long long& _ullint, const unsigned long long def) = 0;
    
    virtual void cdata(pugi::xml_node _node, std::string& _text) = 0;

    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text, std::string_view default_text) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, int& _int) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, int& _int, const int def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _uint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _uint, const unsigned def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, float& _float) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, float& _float, const float def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, double& _double) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, double& _double, const double def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _bool) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _bool, const bool def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _llint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _llint, const long long def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _ullint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _ullint, const unsigned long long def) = 0;

protected:
    bool _reading = true;
    bool _write_default_values = true;
};

class writer_impl : public impl_base
{
public:
    
    writer_impl()
    {
        _reading = false;
    }
    
    void node_name(pugi::xml_node _node, std::string& _name) override
    {
        _node.set_name(_name.c_str());
    }

    pugi::xml_node child(pugi::xml_node _node, const char* _name) override
    {
        auto new_node = _node.append_child();
        new_node.set_name(_name);
        return new_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* _name) override
    {
        auto younger_sibling = older_sibling.parent().insert_child_after(_name, older_sibling);
        return younger_sibling;
    }

    void text(pugi::xml_node _node, std::string& _text) override
    {
        _node.text().set(_text.c_str());
    }
    
    void text(pugi::xml_node _node, std::string& _text, std::string_view default_text) override
    {
        if (_text != default_text)
        {
            text(_node, _text);
        }
    }
    
    const char* c_str(pugi::xml_node _node, const char* _c_str) override
    {
        _node.text().set(_c_str);
        return _c_str;
    }

    template<typename TToWrite>
    void write_node_value(pugi::xml_node _node, TToWrite& _val)
    {
        _node.text().set(_val);
    }
    
    template<typename TToWrite>
    void write_node_value_with_default(pugi::xml_node _node, TToWrite& _val, const TToWrite def)
    {
        if (_write_default_values || def != _val)
            write_node_value(_node, _val);
    }

    void text(pugi::xml_node _node, int& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, int& _val, const int def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, unsigned& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, unsigned& _val, const unsigned def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, float& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, float& _val, const float def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, double& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, double& _val, const double def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, bool& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, bool& _val, const bool def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, long long& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, long long& _val, const long long def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, unsigned long long& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, unsigned long long& _val, const unsigned long long def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void cdata(pugi::xml_node _node, std::string& _text) override
    {
        _node.append_child(pugi::node_cdata).set_value(_text.c_str());
    }

    void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text) override
    {
        _node.append_attribute(_attrib_name) = _text.c_str();
    }
    
    // do not append the attribute if _text is equal to default_text
    void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text, std::string_view default_text) override
    {
        if (_write_default_values || _text != default_text)
            _node.append_attribute(_attrib_name) = _text.c_str();
    }
    
    template<typename TToWrite>
    void write_attribute_value(pugi::xml_node _node, const char* _attrib_name, TToWrite& _to_write)
    {
        _node.append_attribute(_attrib_name).set_value(_to_write);
    }
    
    template<typename TToWrite>
    void write_attribute_value_with_default(pugi::xml_node _node, const char* _attrib_name, TToWrite& _to_write, const TToWrite def)
    // do not write the value if it's equal to the default
    {
        if (_write_default_values || _to_write != def)
        {
            write_attribute_value(_node, _attrib_name, _to_write);
        }
    }

    void attribute(pugi::xml_node _node, const char* _attrib_name, int& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, int& _val, const int def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _val, const unsigned def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, float& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, float& _val, const float def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, double& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, double& _val, const double def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _val, const bool def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _val, const long long def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _val) override
        { write_attribute_value(_node, _attrib_name, _val); }
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _val, const unsigned long long def) override
        { write_attribute_value_with_default(_node, _attrib_name, _val, def); }

};

class reader_impl : public impl_base
{
public:
    
    void node_name(pugi::xml_node _node, std::string& _name) override
    {
        _name = _node.name();
    }

    pugi::xml_node child(pugi::xml_node _node, const char* _name) override
    {
        auto a_child_node = _node.child(_name);
        return a_child_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* _name) override
    {
        auto younger_sibling = older_sibling.next_sibling(_name);
        return younger_sibling;
    }
    
    const char* c_str(pugi::xml_node _node, const char*) override
    {
        return _node.text().as_string();
    }

    void text(pugi::xml_node _node, std::string& _text) override
    {
        _text = _node.text().as_string();
    }
    
    void text(pugi::xml_node _node, std::string& _text, std::string_view default_text) override
    {
        if (_node.text())
        {
            _text = _node.text().as_string();
        }
        else
        {
            _text = default_text;
        }
    }

    void text(pugi::xml_node _node, int& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, int& _val, const int def) override
    { _val = _node.text().as_int(def); }

    void text(pugi::xml_node _node, unsigned& _val) override
    { _val = _node.text().as_uint(); }
    void text(pugi::xml_node _node, unsigned& _val, const unsigned def) override
    { _val = _node.text().as_uint(def); }

    void text(pugi::xml_node _node, float& _val) override
    { _val = _node.text().as_float(); }
    void text(pugi::xml_node _node, float& _val, const float def) override
    { _val = _node.text().as_float(def); }

    void text(pugi::xml_node _node, double& _val) override
    { _val = _node.text().as_double(); }
    void text(pugi::xml_node _node, double& _val, const double def) override
    { _val = _node.text().as_double(def); }

    void text(pugi::xml_node _node, bool& _val) override
    { _val = _node.text().as_bool(); }
    void text(pugi::xml_node _node, bool& _val, const bool def) override
    { _val = _node.text().as_bool(def); }

    void text(pugi::xml_node _node, long long& _val) override
    { _val = _node.text().as_llong(); }
    void text(pugi::xml_node _node, long long& _val, const long long def) override
    { _val = _node.text().as_llong(def); }

    void text(pugi::xml_node _node, unsigned long long& _val) override
    { _val = _node.text().as_ullong(); }
    void text(pugi::xml_node _node, unsigned long long& _val, const unsigned long long def) override
    { _val = _node.text().as_ullong(def); }

    
    void cdata(pugi::xml_node _node, std::string& _text) override
    {
        _text = _node.child_value();
    }

    void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text) override
    {
        _text = _node.attribute(_attrib_name).as_string(_text.c_str());
    }
    
    // return default_text if attribute does not exists
    void attribute(pugi::xml_node _node, const char* _attrib_name, std::string& _text, std::string_view default_text) override
    {
        if (auto attrib = _node.attribute(_attrib_name); attrib)
            _text = attrib.as_string(_text.c_str());
        else
            _text = default_text;
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, int& _int) override
    {
        _int = _node.attribute(_attrib_name).as_int(_int);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, int& _int, const int def) override
    {
        _int = _node.attribute(_attrib_name).as_int(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _uint) override
    {
        _uint = _node.attribute(_attrib_name).as_uint(_uint);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned& _uint, const unsigned def) override
    {
        _uint = _node.attribute(_attrib_name).as_uint(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, float& _float) override
    {
        _float = _node.attribute(_attrib_name).as_float(_float);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, float& _float, const float def) override
    {
        _float = _node.attribute(_attrib_name).as_float(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, double& _double) override
    {
        _double = _node.attribute(_attrib_name).as_double(_double);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, double& _double, const double def) override
    {
        _double = _node.attribute(_attrib_name).as_double(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _bool) override
    {
        _bool = _node.attribute(_attrib_name).as_bool(_bool);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, bool& _bool, const bool def) override
    {
        _bool = _node.attribute(_attrib_name).as_bool(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _llint) override
    {
        _llint = _node.attribute(_attrib_name).as_llong(_llint);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, long long& _llint, const long long def) override
    {
        _llint = _node.attribute(_attrib_name).as_llong(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _ullint) override
    {
        _ullint = _node.attribute(_attrib_name).as_ullong(_ullint);
    }
    void attribute(pugi::xml_node _node, const char* _attrib_name, unsigned long long& _ullint, const unsigned long long def) override
    {
        _ullint = _node.attribute(_attrib_name).as_ullong(def);
    }
};
} // namespace impl

serializer_base::serializer_base(pugi::xml_node in_node, impl::impl_base& in_implementor)
: _curr_node(in_node)
, _implementor(in_implementor)
{}
    
serializer_base& serializer_base::operator=(const serializer_base& other)
{
    if (this != & other)
    {
        _curr_node = other._curr_node;
        _implementor = other._implementor;
    }
    
    return *this;
}


bool serializer_base::reading() const { return _implementor.reading();}
bool serializer_base::writing() const { return _implementor.writing();}

void serializer_base::set_should_write_default_values(const bool _should_write_default_values)
{
    _implementor.set_should_write_default_values(_should_write_default_values);
}
    
bool serializer_base::get_should_write_default_values()
{
    return _implementor.get_should_write_default_values();
}

void serializer_base::serializer_base::node_name(std::string& _name)
{
    _implementor.node_name(_curr_node, _name);
}

serializer_base serializer_base::child(const char* _name)
{
    pugi::xml_node a_node = _implementor.child(_curr_node, _name);
    return serializer_base(a_node, _implementor);
}

serializer_base serializer_base::next_sibling(const char* _name)
{
    pugi::xml_node a_node = _implementor.next_sibling(_curr_node, _name);
    return serializer_base(a_node, _implementor);
}

const char* serializer_base::c_str(const char* _c_str)
{
    return _implementor.c_str(_curr_node, _c_str);
}

template<typename TToSerialize>
void serializer_base::text(TToSerialize& _val)
{
    _implementor.text(_curr_node, _val);
}

template<typename TToSerialize, typename TDefault>
void serializer_base::text(TToSerialize& _val, const TDefault def)
{
    _implementor.text(_curr_node, _val, def);
}

template void serializer_base::text<std::string>(std::string&);
template void serializer_base::text<std::string>(std::string&, const std::string_view);
template void serializer_base::text<int>(int&);
template void serializer_base::text<int>(int&, const int);
template void serializer_base::text<unsigned>(unsigned&);
template void serializer_base::text<unsigned>(unsigned&, const unsigned);
template void serializer_base::text<float>(float&);
template void serializer_base::text<float>(float&, const float);
template void serializer_base::text<double>(double&);
template void serializer_base::text<double>(double&, const double);
template void serializer_base::text<bool>(bool&);
template void serializer_base::text<bool>(bool&, const bool);
template void serializer_base::text<long long>(long long&);
template void serializer_base::text<long long>(long long&, const long long);
template void serializer_base::text<unsigned long long>(unsigned long long&);
template void serializer_base::text<unsigned long long>(unsigned long long&, const unsigned long long);

void serializer_base::cdata(std::string& _text)
{
    _implementor.cdata(_curr_node, _text);
}


template<typename TToSerialize>
void serializer_base::attribute(const char* _name, TToSerialize& _val)
{
    _implementor.attribute(_curr_node, _name, _val);
}

template<typename TToSerialize, typename TDefault>
void serializer_base::attribute(const char* _name, TToSerialize& _val, const TDefault def)
{
    _implementor.attribute(_curr_node, _name, _val, def);
}

template void serializer_base::attribute<std::string>(const char* _name, std::string&);
template void serializer_base::attribute<std::string>(const char* _name, std::string&, const char*);
template void serializer_base::attribute<std::string>(const char* _name, std::string&, const std::string_view);
template void serializer_base::attribute<int>(const char* _name, int&);
template void serializer_base::attribute<int>(const char* _name, int&, const int);
template void serializer_base::attribute<unsigned>(const char* _name, unsigned&);
template void serializer_base::attribute<unsigned>(const char* _name, unsigned&, const unsigned);
template void serializer_base::attribute<float>(const char* _name, float&);
template void serializer_base::attribute<float>(const char* _name, float&, const float);
template void serializer_base::attribute<double>(const char* _name, double&);
template void serializer_base::attribute<double>(const char* _name, double&, const double);
template void serializer_base::attribute<bool>(const char* _name, bool&);
template void serializer_base::attribute<bool>(const char* _name, bool&, const bool);
template void serializer_base::attribute<long long>(const char* _name, long long&);
template void serializer_base::attribute<long long>(const char* _name, long long&, const long long);
template void serializer_base::attribute<unsigned long long>(const char* _name, unsigned long long&);
template void serializer_base::attribute<unsigned long long>(const char* _name, unsigned long long&, const unsigned long long);


writer::writer(pugi::xml_document& doc, const char* doc_element_name)
: serializer_base(doc.append_child(doc_element_name), *new impl::writer_impl)
{
    
}

writer::writer(pugi::xml_node in_node)
: serializer_base(in_node, *new impl::writer_impl)
{}

writer::~writer()
{
    delete & _implementor;
}

reader::reader(pugi::xml_document& doc)
: serializer_base(doc.document_element(), *new impl::reader_impl) {}

reader::reader(pugi::xml_node in_node)
: serializer_base(in_node, *new impl::reader_impl) {}

reader::~reader()
{
    delete & _implementor;
}

}  // namespace pugi_serializer

#endif // __SOURCE_PUGI_SERIALIZER_CPP__

/**
 * Copyright (C) 2021, by Shai Shsag (shaishasag@yahoo.co.uk)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
