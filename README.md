pugi_serializer is a C++ XML serialisation library based on pugixml (https://pugixml.org/)

The main feature of pugi_serializer is that the same function is used to read and write the xml.

## Example
Here's an example of how code using pugi_serializer looks; it defines serialisation function for a class, and shows how to serialize an object to and from pugi::xml_node. Note that the same function, `serialize_person()`, is used when writing to or reading from XML.

```c++
class Person
{
public:
    std::string m_first_name;
    std::string m_nickname;
    std::string m_last_name;
    unsigned m_age = 0;
};

// this functions defines how a Person object will be serialized from/to pugi::xml_node:
void serialize_person(Person& person, pugi_serializer::serializer_base in_ser)
{
    auto name_ser = in_ser.child("first_name");
    name_ser.text(person.m_first_name);
    name_ser.attribute("nickname", person.m_nickname);

    in_ser.child("last_name").text(person.m_last_name);
    in_ser.child("age").text(person.m_age);
}

void write_and_read_serialization_example()
{
    // Serializing to XML:
    //
    // Create a Person object:
    Person a_person{"William", "Bill", "Ockham", 62};

    // Create a pugi::xml_document to write to:
    pugi::xml_document person_doc;

    // Create pugi_serializer::writer object:
    pugi_serializer::writer xml_writer(person_doc, "Person_List");

    // Serialize Person object to pugi::xml_document:
    serialize_person(a_person, xml_writer);
		
  	// xml created:
    // <Person>
    //     <first_name nickname="Bill">William</first_name>
    //     <last_name>Ockham</last_name>
    //     <age>62</age>
    // </Person>  
    
    // Serializing from XML:
    //
    // Create an empty Person object:
    Person b_person;

    // Create pugi_serializer::reader object and attach it to the pugi::xml_document:
    pugi_serializer::reader xml_reader(person_doc.document_element());

    // Serialize Person object from pugi::xml_document:
    serialize_person(b_person, xml_reader);

    // a_person & b_person should be the same
    assert(a_person == b_person);

    // output person_doc to stdout:
    person_doc.save(std::cout, "    ");
}
```



## About defaults

When reading from XML, some expected elements or attributes might not be present in the XML being read. What to do about it? In pugixml, reading the text of non-existing element or reading the value of non-existing attribute, will result in an empty string or 0 numberic value. pugi_serializer will behave the same, and when element or attribute is not found, the empty string or 0 will be assigned.

If you want a  value different from empty string or 0 to be used as default,  pugi_serializer functions allow to pass a default value that will be returned if element/attribute were not found. Using the Person object from the example above, suppose we decide that if no nickname attribute is provided, the value "no-nick" will be read:

```c++
name_ser.attribute("nickname", person.m_nickname, "no-nick");
```

Note that calling attribute() without default value:

```c++
name_ser.attribute("nickname", person.m_nickname);
```

Is the same as calling attribute() with an empty string:

```c++
name_ser.attribute("nickname", person.m_nickname, "");
```

When writting to XML the default is to always write the element/attribute even if the value being written is equal to the default value. This might not always be the desired result. Sometimes it would be preferable to avoid writing default values in order to reduce XML size, or to allow the reading code to decide the default value for non-existing elements/attributes. To change the writing behaviour to not writing default values:

```
serializer_base::set_should_write_default_values(false);
```

Here is an implementation of the serialize_person() function from the example above, where default values are read but not written:

```c++
void serialize_person(Person& person, pugi_serializer::serializer_base in_ser)
{
    in_ser.set_should_write_default_values(false);
    auto name_ser = in_ser.child("first_name");
    name_ser.text(person.m_first_name);
    name_ser.attribute("nickname", person.m_nickname, "no-nick");

    in_ser.child("last_name").text(person.m_last_name);
    in_ser.child_and_text("age", person.m_age, 0); // will read 0 if no age node is given, will not write 0 if age is 0
}
```

Note that:

- When reading, if child "first_name" is not found or if attribute "nickname" is not found, `person.m_nickname` will be assigned the value "no-nick".
- When writing if `person.m_nickname == "no-nick"`attribute "nickname" will not be written. If `person.m_age==0`, element "age" will not be created.
- Using the `child_and_text` function will not creare <age> element  if person.m_age==0. This is different from calling `child("age").text(person.m_age, 0)` in which case <age> element would always created, but the contents would remain empty if person.m_age==0.

## License

Copyright (C) 2021, by Shai Shasag (shaishasag@yahoo.co.uk)
This library is available to anybody free of charge, under the terms of MIT License (see LICENSE.md).
