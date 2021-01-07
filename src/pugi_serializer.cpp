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

#ifndef SOURCE_PUGI_SERIALIZER_CPP
#define SOURCE_PUGI_SERIALIZER_CPP

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
    
    virtual pugi::xml_node child(pugi::xml_node parent, const char* name) = 0;
    virtual pugi::xml_node next_sibling(pugi::xml_node parent, const char* name) = 0;

    virtual void text(pugi::xml_node node, std::string& _text) = 0;
    virtual void text(pugi::xml_node node, std::string& _text, const char* default_text) = 0;
    virtual pugi::xml_node child_and_text(pugi::xml_node parent, const char* name, std::string& _text) = 0;
    
    virtual void attribute(pugi::xml_node parent, const char* name, std::string& _text) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, std::string& _text, const char* default_text) = 0;
    
    virtual void attribute(pugi::xml_node parent, const char* name, int& _int, const int def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, int& _int) = 0;
    
    virtual void attribute(pugi::xml_node parent, const char* name, unsigned& _uint) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, unsigned& _uint, const unsigned def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, float& _float) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, float& _float, const float def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, double& _double) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, double& _double, const double def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, bool& _bool) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, bool& _bool, const bool def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, long long& _llint) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, long long& _llint, const long long def) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint) = 0;
    virtual void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint, const unsigned long long def) = 0;

protected:
    bool _reading = true;
};

class writer_impl : public impl_base
{
public:
    
    writer_impl()
    {
        _reading = false;
    }

    pugi::xml_node child(pugi::xml_node parent, const char* name) override
    {
        auto new_node = parent.append_child(name);
        return new_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* name) override
    {
        auto younger_sibling = older_sibling.parent().insert_child_after(name, older_sibling);
        return younger_sibling;
    }

    void text(pugi::xml_node node, std::string& _text) override
    {
        node.text().set(_text.c_str());
    }
    
    void text(pugi::xml_node node, std::string& _text, const char* default_text) override
    {
        if (_text != default_text)
        {
            text(node, _text);
        }
    }
    
    pugi::xml_node child_and_text(pugi::xml_node parent, const char* name, std::string& _text) override
    {
        auto new_node = child(parent, name);
        new_node.text().set(_text.c_str());
        return new_node;
    }
    
    void attribute(pugi::xml_node parent, const char* name, std::string& _text) override
    {
        parent.append_attribute(name) = _text.c_str();
    }
    
    // do not append the attribute if _text is equal to default_text
    void attribute(pugi::xml_node parent, const char* name, std::string& _text, const char* default_text) override
    {
        if (_text != default_text)
            parent.append_attribute(name) = _text.c_str();
    }
    
    void attribute(pugi::xml_node parent, const char* name, int& _int) override
    {
        parent.append_attribute(name).set_value(_int);
    }
    void attribute(pugi::xml_node parent, const char* name, int& _int, const int def) override
    {
        if (_int != def)
            attribute(parent, name, _int);
    }
    
    void attribute(pugi::xml_node parent, const char* name, unsigned& _uint) override
    {
        parent.append_attribute(name).set_value(_uint);
    }
    void attribute(pugi::xml_node parent, const char* name, unsigned& _uint, const unsigned def) override
    {
        if (_uint != def)
            attribute(parent, name, _uint);
    }
    
    void attribute(pugi::xml_node parent, const char* name, float& _float) override
    {
        parent.append_attribute(name).set_value(_float);
    }
    void attribute(pugi::xml_node parent, const char* name, float& _float, const float def) override
    {
        if (_float != def)
            attribute(parent, name, _float);
    }
    
    void attribute(pugi::xml_node parent, const char* name, double& _double) override
    {
        parent.append_attribute(name).set_value(_double);
    }
    void attribute(pugi::xml_node parent, const char* name, double& _double, const double def) override
    {
        if (_double != def)
            attribute(parent, name, _double);
    }
    
    void attribute(pugi::xml_node parent, const char* name, bool& _bool) override
    {
        parent.append_attribute(name).set_value(_bool);
    }
    void attribute(pugi::xml_node parent, const char* name, bool& _bool, const bool def) override
    {
        if (_bool != def)
            attribute(parent, name, _bool);
    }
    
    void attribute(pugi::xml_node parent, const char* name, long long& _llint) override
    {
        parent.append_attribute(name).set_value(_llint);
    }
    void attribute(pugi::xml_node parent, const char* name, long long& _llint, const long long def) override
    {
        if (_llint != def)
            attribute(parent, name, _llint);
    }
    
    void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint) override
    {
        parent.append_attribute(name).set_value(_ullint);
    }
    void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint, const unsigned long long def) override
    {
        if (_ullint != def)
            attribute(parent, name, _ullint);
    }
};

class reader_impl : public impl_base
{
public:

    pugi::xml_node child(pugi::xml_node parent, const char* name) override
    {
        auto a_child_node = parent.child(name);
        return a_child_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* name) override
    {
        auto younger_sibling = older_sibling.next_sibling(name);
        return younger_sibling;
    }

    void text(pugi::xml_node node, std::string& _text) override
    {
        _text = node.value();
    }
    
    void text(pugi::xml_node node, std::string& _text, const char* default_text) override
    {
        if (node.text())
        {
            _text = node.value();
        }
        else
        {
            _text = default_text;
        }
    }
    
    pugi::xml_node child_and_text(pugi::xml_node parent, const char* name, std::string& _text) override
    {
        auto a_child_node = child(parent, name);
        if (a_child_node)
            _text = a_child_node.text().as_string();
        return a_child_node;
    }
    
    void attribute(pugi::xml_node parent, const char* name, std::string& _text) override
    {
        _text = parent.attribute(name).as_string(_text.c_str());
    }
    
    // return default_text if attribute does not exists
    void attribute(pugi::xml_node parent, const char* name, std::string& _text, const char* default_text) override
    {
        if (auto attrib = parent.attribute(name); attrib)
            _text = attrib.as_string(_text.c_str());
        else
            _text = default_text;
    }
    
    void attribute(pugi::xml_node parent, const char* name, int& _int) override
    {
        _int = parent.attribute(name).as_int(_int);
    }
    void attribute(pugi::xml_node parent, const char* name, int& _int, const int def) override
    {
        _int = parent.attribute(name).as_int(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, unsigned& _uint) override
    {
        _uint = parent.attribute(name).as_uint(_uint);
    }
    void attribute(pugi::xml_node parent, const char* name, unsigned& _uint, const unsigned def) override
    {
        _uint = parent.attribute(name).as_uint(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, float& _float) override
    {
        _float = parent.attribute(name).as_float(_float);
    }
    void attribute(pugi::xml_node parent, const char* name, float& _float, const float def) override
    {
        _float = parent.attribute(name).as_float(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, double& _double) override
    {
        _double = parent.attribute(name).as_double(_double);
    }
    void attribute(pugi::xml_node parent, const char* name, double& _double, const double def) override
    {
        _double = parent.attribute(name).as_double(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, bool& _bool) override
    {
        _bool = parent.attribute(name).as_bool(_bool);
    }
    void attribute(pugi::xml_node parent, const char* name, bool& _bool, const bool def) override
    {
        _bool = parent.attribute(name).as_bool(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, long long& _llint) override
    {
        _llint = parent.attribute(name).as_llong(_llint);
    }
    void attribute(pugi::xml_node parent, const char* name, long long& _llint, const long long def) override
    {
        _llint = parent.attribute(name).as_llong(def);
    }
    
    void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint) override
    {
        _ullint = parent.attribute(name).as_ullong(_ullint);
    }
    void attribute(pugi::xml_node parent, const char* name, unsigned long long& _ullint, const unsigned long long def) override
    {
        _ullint = parent.attribute(name).as_ullong(def);
    }
};
} // namespace impl

serilaizer_base::serilaizer_base(pugi::xml_node in_node, impl::impl_base& in_implementor)
: _curr_node(in_node)
, _implementor(in_implementor)
{}
    
serilaizer_base& serilaizer_base::operator=(const serilaizer_base& other)
{
    if (this != & other)
    {
        _curr_node = other._curr_node;
        _implementor = other._implementor;
    }
    
    return *this;
}


bool serilaizer_base::reading() const { return _implementor.reading();}
bool serilaizer_base::writing() const { return _implementor.writing();}
    
serilaizer_base serilaizer_base::child(const char* name)
{
    pugi::xml_node a_node = _implementor.child(_curr_node, name);
    return serilaizer_base(a_node, _implementor);
}

serilaizer_base serilaizer_base::next_sibling(const char* name)
{
    pugi::xml_node a_node = _implementor.next_sibling(_curr_node, name);
    return serilaizer_base(a_node, _implementor);
}
    
void serilaizer_base::text(std::string& _text)
{
    _implementor.text(_curr_node, _text);
}

void serilaizer_base::text(std::string& _text, const char* default_text)
{
    _implementor.text(_curr_node, _text, default_text);
}

serilaizer_base serilaizer_base::child_and_text(const char* name, std::string& _text)
{
    pugi::xml_node a_node = _implementor.child_and_text(_curr_node, name, _text);
    return serilaizer_base(a_node, _implementor);
}

void serilaizer_base::attribute(const char* name, std::string& _text)
{
    _implementor.attribute(_curr_node, name, _text);
}

void serilaizer_base::attribute(const char* name, std::string& _text, const char* def)
{
    _implementor.attribute(_curr_node, name, _text, def);
}

writer::writer(pugi::xml_document& doc, const char* doc_element_name)
: serilaizer_base(doc.append_child(doc_element_name), *new impl::writer_impl)
{
    
}

writer::writer(pugi::xml_node in_node)
: serilaizer_base(in_node, *new impl::writer_impl)
{}

writer::~writer()
{
    delete & _implementor;
}

reader::reader(pugi::xml_document& doc)
: serilaizer_base(doc.document_element(), *new impl::reader_impl) {}

reader::reader(pugi::xml_node in_node)
: serilaizer_base(in_node, *new impl::reader_impl) {}

reader::~reader()
{
    delete & _implementor;
}

}  // namespace pugi_serializer

#endif // SOURCE_PUGI_SERIALIZER_CPP

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
