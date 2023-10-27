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

#ifndef __HEADER_PUGI_SERIALIZER_HPP__
#define __HEADER_PUGI_SERIALIZER_HPP__

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

    class XML_SERIALIZER_CLASS serializer_base
    {
    public:

        serializer_base(const serializer_base&) = default;
        serializer_base& operator=(const serializer_base&);

        operator bool() const { return bool(_curr_node); }
        bool reading() const;
        bool writing() const;
        void set_should_write_default_values(const bool _should_write_default_values);
        bool get_should_write_default_values();

        pugi::xml_node& curr_node() {return _curr_node;}

        void node_name(std::string& _name);

        serializer_base child(const char* _name);
        serializer_base next_sibling(const char* _name);

        template<typename TValue, typename TDefault>
        serializer_base child_with_text(const char* _child_name, TValue& _value, const TDefault def)
        // write: will not create the child if _value==def, unless get_should_write_default_values() == true
        // read: read the text of the child, if either child does not exist or child's text is empty - return the default
        // using different types for value and default so, for example, child_with_text can be called with TValue=std::string, TDefault=const char*
        {
            if (reading())
            {
                auto return_serializer = child(_child_name);
                if constexpr (std::is_convertible_v<TDefault, std::string_view>)
                {
                    return_serializer.text<TValue, std::string_view>(_value, std::string_view(def));
                }
                else
                {
                    return_serializer.text<TValue, TDefault>(_value, def);
                }
                return return_serializer;
            }
            else if (get_should_write_default_values() || _value != def)
            {
                auto return_serializer = child(_child_name);
                return_serializer.text(_value);
                return return_serializer;
            }
            else
                return serializer_base(pugi::xml_node(), _implementor);
        }
 
        template<typename TValue, typename TDefault>
        serializer_base child_with_attribute(const char* _child_name, const char* _attrib_name, TValue& _value, const TDefault def)
        // write: will not create the child adn the attribute if _value==def, unless get_should_write_default_values() == true
        // read: read the attribute of the child, if either child does not exist or attribute does not exists return the default
        {
            if (reading() || get_should_write_default_values() || _value != def)
            {
                auto return_serializer = child(_child_name);
                if constexpr (std::is_convertible_v<TDefault, std::string_view>)
                {
                    return_serializer.attribute<TValue, std::string_view>(_attrib_name,_value, std::string_view(def));
                }
                else
                {
                    return_serializer.attribute(_attrib_name, _value, def);
                }
                return return_serializer;
            }
            else
                return serializer_base(pugi::xml_node(), _implementor);
        }
//
//        template<typename T>
//        void child_with_attribute(const char* _child_name, const char* _attrib_name, T& _value, const char* def)
//        // write: will not create the child adn the attribute if _value==def, unless get_should_write_default_values() == true
//        // read: read the attribute of the child, if either child does not exist or attribute does not exists return the default
//        {
//            if (reading())
//            {
//                child(_child_name).attribute(_attrib_name,_value, def);
//            }
//            else if (get_should_write_default_values() || _value != def)
//            {
//                child(_child_name).attribute(_attrib_name,_value, def);
//            }
//        }

        const char* c_str(const char* _c_str);
        
        template<typename TSTR>  // serialize any type of string assuming TSTR has two functons:
        // TSTR.c_str() - returning null terminated char*
        // TSTR.c_str.operator=(const char*)
        void serialize_string(TSTR& in_out_string)
        {
            in_out_string = c_str(in_out_string.c_str());
        }
 
        template<typename TToSerialize>
        void text(TToSerialize& _val);
        
        template<typename TToSerialize, typename TDefault>
        void text(TToSerialize& _val, const TDefault def);

        template<typename TToSerialize>
        void attribute(const char* _name, TToSerialize& _val);
        
        template<typename TToSerialize, typename TDefault>
        void attribute(const char* _name, TToSerialize& _val, const TDefault def);

        void cdata(std::string& _text);

   protected:
        serializer_base(pugi::xml_node node, impl::impl_base& in_implementor);

        pugi::xml_node     _curr_node;
        impl::impl_base&   _implementor;
    };


    class XML_SERIALIZER_CLASS writer : public serializer_base
    {
    public:
        writer(pugi::xml_document& doc, const char* doc_element_name);
        writer(pugi::xml_node node);
        ~writer();
    };

    class XML_SERIALIZER_CLASS reader : public serializer_base
    {
    public:
        reader(pugi::xml_document& doc);
        reader(pugi::xml_node node);
        ~reader();
    };

    class serialized_base
    {
    public:
        virtual ~serialized_base() = default;
        virtual void serialize(pugi_serializer::serializer_base& ser) = 0;
#if (__cplusplus == 202002L)  // c++20
        friend auto operator<=>(const serialized_base&, const serialized_base&)
        {
            return std::strong_ordering::equal;
        }
#endif
    };
    
    // serialize an array of string objects
    template<typename TSTR>
    void serialize_string_array(pugi_serializer::serializer_base& ser, TSTR* array_begin, TSTR* array_end, const char* container_item_name)
    {
        if (ser.reading())
        {
            TSTR* curr_item = array_begin;
            for (auto item_ser = ser.child(container_item_name); item_ser && curr_item != array_end; item_ser = item_ser.next_sibling(container_item_name), ++curr_item)
            {
                item_ser.serialize_string(*curr_item);
            }
        }
        else if (ser.writing())
        {
            for (auto curr_item = array_begin; curr_item != array_end;  ++curr_item)
            {
                auto item_ser = ser.child(container_item_name);
                item_ser.serialize_string(*curr_item);
            }
        }
    }
    
    // serialize an array of objects derived from pugi_serializer::serialized_base
    // write: iterate from array_begin to array_end, cretae element named container_item_name for for each and call T_ITEM.serialize on new element
    // read: iterate on all elements named container_item_name and serialize each into a new T_ITEM, but no more than array_end-array_begin times
    template<typename T_ITEM>
    void serialize_array(pugi_serializer::serializer_base& ser, T_ITEM* array_begin, T_ITEM* array_end, const char* container_item_name)
    {
        if (ser.reading())
        {
            T_ITEM* curr_item = array_begin;
            for (auto item_ser = ser.child(container_item_name); item_ser && curr_item != array_end; item_ser = item_ser.next_sibling(container_item_name), ++curr_item)
            {
                T_ITEM new_value;
                new_value.serialize(item_ser);
                *curr_item = std::move(new_value);
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

    // serialize a container of objects derived from pugi_serializer::serialized_base
    // write: iterate in_container, create element named container_item_name for for each and call T_ITEM.serialize on new element
    // read: iterate on all elements named container_item_name and serialize each into a new T_ITEM, but no more than array_end-array_begin times
    template<typename TCONTAINER>
    void serialize_container(pugi_serializer::serializer_base& ser, TCONTAINER& in_container, const char* container_item_name)
    {
        if (ser.reading())
        {
            for (auto item_ser = ser.child(container_item_name); item_ser; item_ser = item_ser.next_sibling(container_item_name))
            {
                typename TCONTAINER::value_type& new_value = in_container.emplace_back();
                new_value.serialize(item_ser);
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


#endif  // __HEADER_PUGI_SERIALIZER_HPP__

#if 0  // TBD
// Make sure implementation is included in header-only mode
// Use macro expansion in #include to work around QMake (QTBUG-11923)
#if defined(XML_SERIALIZER_HEADER_ONLY) && !defined(XML_SERIALIZER_SOURCE)
#	define XML_SERIALIZER_SOURCE "pugixml.cpp"
#	include XML_SERIALIZER_SOURCE
#endif
#endif
