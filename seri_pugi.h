#ifndef __seri_pugi_h__
#define __seri_pugi_h__

#include "seri.h"
#include <vector>
#include "pugixml.hpp"

namespace seri
{

class pugi_base : public base
{
public:
    pugi_base(pugi::xml_node parent_node)
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

class pugi_write : public pugi_base
{
public:
    pugi_write(pugi::xml_node parent_node)
    : pugi_base(parent_node) {}

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
    void text(const type_converter& text_converter) override
    {
        std::string as_string;
        text_converter.write_to_string(as_string);
        text(as_string);
    }
    void attrib(const char* attrib_name, std::string& attrib_value) override
    {
        node_stack.back().append_attribute(attrib_name).set_value(attrib_value.c_str());
    }
    void attrib(const char* attrib_name, type_converter& text_converter) override
    {
        std::string as_string;
        text_converter.write_to_string(as_string);
        attrib(attrib_name, as_string);
    }
};

class pugi_read : public pugi_base
{
public:
    pugi_read(pugi::xml_node parent_node)
    : pugi_base(parent_node) {}

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
    void text(const type_converter& text_converter) override
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
    void attrib(const char* attrib_name, type_converter& text_converter) override
    {
        std::string as_string;
        attrib(attrib_name, as_string);
        text_converter.read_from_string(as_string);
    }
};

template<typename T> class stl_container_serializer
{
public:
    stl_container_serializer(const char* _sub_node_name,
                                T& _container) :
    m_sub_node_name(_sub_node_name),
    m_container(_container)
    {
    }

    void serialize(base& _serializer)
    {
        size_t actual_size = in_serializer.get_num_array_elements(m_sub_element_name.c_str(), m_container.size());

        if (in_serializer.Writing())
        {
            typename T::iterator iCurr = m_container.begin();
            for (size_t i = 0; i < actual_size; ++i)
            {
                in_serializer.element_name(m_sub_element_name.c_str(), i).element_content(*iCurr++);
            }
        }
        else if (in_serializer.Reading())
        {
            std::back_insert_iterator<T> back_it = std::back_inserter(m_container);
            for (size_t i = 0; i < actual_size; ++i)
            {
                typename T::value_type new_value;
                in_serializer.element_name(m_sub_element_name.c_str(), i).element_content(new_value);
                back_it = new_value;
            }
        }
    }

protected:
    const std::string _sub_node_name;
    T& _container;
};

} // namespace seri

#endif // __seri_pugi_h__
