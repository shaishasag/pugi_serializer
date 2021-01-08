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

#ifndef HEADER_XML_SERIALIZER_HPP
#define HEADER_XML_SERIALIZER_HPP

/* Copy to include
#include "pugi_serializer.hpp"
*/

#ifndef XML_SERIALIZER_VERSION
// Define version macro; evaluates to major * 100 + minor so that it's safe to use in less-than comparisons
#	define XML_SERIALIZER_VERSION 001
#endif

#include <string>

// Include pugixml header
#include "pugixml.hpp"


// If no API is defined, assume default
#ifndef XML_SERIALIZER_API
#	define XML_SERIALIZER_API
#endif

// If no API for classes is defined, assume default
#ifndef XML_SERIALIZER_CLASS
#	define XML_SERIALIZER_CLASS XML_SERIALIZER_API
#endif

// If no API for functions is defined, assume default
#ifndef XML_SERIALIZER_FUNCTION
#	define XML_SERIALIZER_FUNCTION XML_SERIALIZER_API
#endif


// The xml_serializer namespace
namespace pugi_serializer
{
    
    namespace impl { class impl_base; }
    
    class XML_SERIALIZER_CLASS serilaizer_base
    {
    public:
        serilaizer_base(const serilaizer_base&) = default;
        serilaizer_base& operator=(const serilaizer_base&);
        serilaizer_base(serilaizer_base&&) = default;
        serilaizer_base& operator=(serilaizer_base&&) = default;

        operator bool() const { return _curr_node; }
        bool reading() const;
        bool writing() const;
        
        pugi::xml_node& curr_node() {return _curr_node;}
        
        void node_name(std::string& _name);
        
        serilaizer_base child(const char* _name);
        serilaizer_base next_sibling(const char* _name);

        void text(std::string& _text);
        void text(std::string& _text, const char* def);
        void text(int& _int);
        void text(int& _int, const int def);
        void text(unsigned& _uint);
        void text(unsigned& _uint, const unsigned def);
        void text(float& _float);
        void text(float& _float, const float def);
        void text(double& _double);
        void text(double& _double, const double def);
        void text(bool& _bool);
        void text(bool& _bool, const bool def);
        void text(long long& _llint);
        void text(long long& _llint, const long long def);
        void text(unsigned long long& _ullint);
        void text(unsigned long long& _ullint, const unsigned long long def);

        void cdata(std::string& _text);
        
        void attribute(const char* _name, std::string& _text);
        void attribute(const char* _name, std::string& _text, const char* def);
        
   protected:
        serilaizer_base(pugi::xml_node node, impl::impl_base& in_implementor);

        pugi::xml_node     _curr_node;
        impl::impl_base&   _implementor;
    };
    
    
    class XML_SERIALIZER_CLASS writer : public serilaizer_base
    {
    public:
        writer(pugi::xml_document& doc, const char* doc_element_name);
        writer(pugi::xml_node node);
        ~writer();
    };
    
    class XML_SERIALIZER_CLASS reader : public serilaizer_base
    {
    public:
        reader(pugi::xml_document& doc);
        reader(pugi::xml_node node);
        ~reader();
    };
    
    class serialized_base
    {
    public:
        virtual void serialize(pugi_serializer::serilaizer_base ser) = 0;
    };

    template<typename T_ITEM>
    void serialize_array(pugi_serializer::serilaizer_base& ser, T_ITEM* array_begin, T_ITEM* array_end, const char* container_item_name)
    {
        if (ser.reading())
        {
            T_ITEM* curr_item = array_begin;
            for (auto item_ser = ser.child(container_item_name); item_ser && curr_item != array_end; item_ser = item_ser.next_sibling(container_item_name), ++curr_item)
            {
                T_ITEM new_value;
                new_value.serialize(item_ser);
                *curr_item = new_value;
            }
        }
        else if (ser.writing())
        {
            for (auto curr_item = array_begin; curr_item != array_end;  ++curr_item)
            {
                auto item_ser = ser.child(container_item_name);
                curr_item->serialize(item_ser);
            }
        }
    }

    template<typename TCONTAINER>
    void serialize_container(pugi_serializer::serilaizer_base& ser, TCONTAINER& in_container, const char* container_item_name)
    {
        if (ser.reading())
        {
            for (auto item_ser = ser.child(container_item_name); item_ser; item_ser = item_ser.next_sibling(container_item_name))
            {
                typename TCONTAINER::value_type new_value;
                new_value.serialize(item_ser);
                in_container.emplace_back(new_value);
            }
        }
        else if (ser.writing())
        {
            for (auto& item : in_container)
            {
                auto item_ser = ser.child(container_item_name);
                item.serialize(item_ser);
            }
        }
    }
}

#endif

#if 0  // TBD
// Make sure implementation is included in header-only mode
// Use macro expansion in #include to work around QMake (QTBUG-11923)
#if defined(XML_SERIALIZER_HEADER_ONLY) && !defined(XML_SERIALIZER_SOURCE)
#	define XML_SERIALIZER_SOURCE "pugixml.cpp"
#	include XML_SERIALIZER_SOURCE
#endif
#endif

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
