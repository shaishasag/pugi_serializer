#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

#if (__cplusplus == 202002L)  // c++20
    auto operator<=>(const std::string& lhs, const std::string& rhs)
    {
        // apple clang! not support for std::string::operator<=> (8-<)
    }
#endif


class encompassed : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const encompassed&, const encompassed&) = default;
#endif
    std::string continent;
    float percentage;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("continent", continent);
        ser.attribute("percentage", percentage);
    }
};

class border : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const border&, const border&) = default;
#endif
    std::string country;
    float length;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("country", country);
        ser.attribute("length", length);
    }
};

// entity has an is and a name
class entity : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const entity&, const entity&) = default;
#endif

    std::string id;
    std::string name;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("id", id);
        ser.attribute("name", name, "");
    }
};

class coordinates : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const coordinates&, const coordinates&) = default;
#endif
    float longitude = 0.0;
    float latitude = 0.0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("longitude", longitude);
        ser.attribute("latitude", latitude);
    }

};

class continent : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const continent&, const continent&) = default;
#endif
};

class percentage_value : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const percentage_value&, const percentage_value&) = default;
#endif
    std::string name;
    float percentage = 0.0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("percentage", percentage);
        ser.text(name);
    }
};

class city : public entity, coordinates
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const city&, const city&) = default;
#endif
    std::string country;
    int population_year = 0;
    unsigned int population = 0;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        // city has it's name as element text, not as attribute so cannot call
        //entity::serialize(ser);
        
        ser.attribute("id", id);
        ser.child_with_text("name", name, "");

        ser.attribute("country", country);
        
        coordinates::serialize(ser);
        
        auto population_ser = ser.child_with_text("population", population, (unsigned int)0);
        // note that when wrtting and set_should_write_default_values(false),
        // attrib "year" will only be written if population!=0
        population_ser.attribute("year", population_year);
    }
};

class province : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const province&, const province&) = default;
#endif
    std::string country;
    std::string capital;
    unsigned int population = 0;
    unsigned int area = 0;
    std::vector<city> cities_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("country", country);
        ser.attribute("capital", capital);
        ser.attribute("population", population);
        ser.attribute("area", area);

        pugi_serializer::serialize_container(ser, cities_vec, "city");

    }
};

class country : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const country&, const country&) = default;
#endif
    std::string name_text;
    std::string capital;
    int population = 0;
    std::string datacode;
    int total_area = 0;
    double population_growth = 0.0;
    double infant_mortality = 0.0;
    double gdp_agri = 0.0;
    int gdp_total = 0;
    float inflation = 0.0;
    std::string indep_date;
    std::string government;
    std::string car_code;
    
    std::vector<city>             cities_vec;
    std::vector<province>         provinces_vec;
    std::vector<percentage_value> ethnicgroups_vec;
    std::vector<percentage_value> religions_vec;
    std::vector<percentage_value> languages_vec;
    std::vector<encompassed>      encompassed_vec;
    std::vector<border>           borders_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("capital", capital);
        ser.attribute("population", population);
        ser.attribute("total_area", total_area);
        ser.attribute("population_growth", population_growth);
        ser.attribute("infant_mortality", infant_mortality);
        ser.attribute("gdp_agri", gdp_agri);
        ser.attribute("gdp_total", gdp_total);
        ser.attribute("inflation", inflation, 0.0f);
        ser.attribute("population", population);
        ser.attribute("indep_date", indep_date, "");
        ser.attribute("government", government);
        ser.attribute("car_code", car_code);
        
        ser.child("name").text(name_text);

        pugi_serializer::serialize_container(ser, cities_vec, "city");
        pugi_serializer::serialize_container(ser, provinces_vec, "province");
        pugi_serializer::serialize_container(ser, ethnicgroups_vec, "ethnicgroups");
        pugi_serializer::serialize_container(ser, religions_vec, "religions");
        pugi_serializer::serialize_container(ser, languages_vec, "languages");
        pugi_serializer::serialize_container(ser, encompassed_vec, "encompassed");
        pugi_serializer::serialize_container(ser, borders_vec, "border");
    }
};

class organization : entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const organization&, const organization&) = default;
#endif
    std::string abbrev;
    std::string established;
    std::string headq;
    
    class member : public pugi_serializer::serialized_base
    {
    public:
        std::string type;
        std::string country;
        void serialize(pugi_serializer::serializer_base& ser) override
        {
            ser.attribute("type", type);
            ser.attribute("country", country);
        }
    };
    std::vector<member> members_vec;
    
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("abbrev", abbrev);
        ser.attribute("established", established, "");
        ser.attribute("headq", headq, "");

        pugi_serializer::serialize_container(ser, members_vec, "members");
    }
};

class located : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const located&, const located&) = default;
#endif
    //std::vector<std::pair<std::string, std::string>> locations;
    std::string country;
    std::string province;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("country", country);
        ser.attribute("province", province);
    }
};

class feature : public entity, coordinates
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const feature&, const feature&) = default;
#endif
    std::vector<located> locations_vec;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        coordinates::serialize(ser);
        pugi_serializer::serialize_container(ser, locations_vec, "located");
    }
};

class mountain : public feature
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const mountain&, const mountain&) = default;
#endif
    uint64_t height = 0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        feature::serialize(ser);
        ser.attribute("height", height);
    }
};

class desert : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const desert&, const desert&) = default;
#endif
    uint64_t area = 0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("area", area);
    }
};

class river : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const river&, const river&) = default;
#endif
    std::string to_type;
    std::string to_water;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        
        auto to_node = ser.child("to");
        to_node.attribute("type", to_type);
        to_node.attribute("water", to_water);
    }
};

class island : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const island&, const island&) = default;
#endif
    uint64_t area = 0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("area", area);
    }
};

class sea : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const sea&, const sea&) = default;
#endif
    uint64_t depth = 0;
    std::vector<located> locations_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("depth", depth);
        pugi_serializer::serialize_container(ser, locations_vec, "located");
    }
};

class lake : public entity
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const lake&, const lake&) = default;
#endif
    float area = 0.0f;
    std::vector<located> locations_vec;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        entity::serialize(ser);
        ser.attribute("area", area, 0.0f);
        pugi_serializer::serialize_container(ser, locations_vec, "located");
    }
};

class world : public pugi_serializer::serialized_base
{
public:
#if (__cplusplus == 202002L)  // c++20
    friend auto operator<=>(const world&, const world&) = default;
#endif

    std::vector<continent> continent_vec;
    std::vector<country> country_vec;
    std::vector<organization> organization_vec;
    std::vector<mountain> mountain_vec;
    std::vector<desert> desert_vec;
    std::vector<island> island_vec;
    std::vector<river> river_vec;
    std::vector<sea> sea_vec;
    std::vector<lake> lake_vec;
    

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        pugi_serializer::serialize_container(ser, continent_vec, "continent");
        pugi_serializer::serialize_container(ser, country_vec, "country");
        pugi_serializer::serialize_container(ser, organization_vec, "organization");
        pugi_serializer::serialize_container(ser, mountain_vec, "mountain");
        pugi_serializer::serialize_container(ser, desert_vec, "desert");
        pugi_serializer::serialize_container(ser, island_vec, "island");
        pugi_serializer::serialize_container(ser, river_vec, "river");
        pugi_serializer::serialize_container(ser, sea_vec, "sea");
        pugi_serializer::serialize_container(ser, lake_vec, "lake");
    }
};

const char* big_file_name = "mondial-3.0.xml";
const char* big_file_out_name = "mondial-3.0.out.xml";
const char* big_file_ref_name = "mondial-3.0.ref.xml";

TEST(TestBigFile, read)
{
    pugi::xml_document doc;
    static unsigned int pugi_parse_options = pugi::parse_trim_pcdata | pugi::parse_embed_pcdata | pugi::parse_escapes | pugi::parse_cdata | pugi::parse_eol | pugi::parse_wconv_attribute;
    pugi::xml_parse_result pugi_parse_result = doc.load_file(big_file_name, pugi_parse_options);
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;
    EXPECT_STREQ(doc.document_element().name(), "mondial") << "document_element is named " << doc.document_element().name() << " instead of " << "'mondial'";

    pugi_serializer::reader reader_serializer(doc);
    world w;
    w.serialize(reader_serializer);
    
    EXPECT_EQ(w.continent_vec.size(), 5) << "expected 5 continents";
    EXPECT_EQ(w.continent_vec.front().id, "f0_119") << "expected 1st continent id to be f0_119";
    EXPECT_EQ(w.continent_vec.front().name, "Europe") << "expected 1st continent name to be Europe";
    EXPECT_EQ(w.continent_vec.back().id, "f0_132") << "expected last continent id to be f0_132";
    EXPECT_EQ(w.continent_vec.back().name, "Africa") << "expected last continent name to be Africa";

    EXPECT_EQ(w.country_vec.size(), 231);
    
    int num_cities = 0;
    for (auto& country : w.country_vec)
        num_cities += country.cities_vec.size();
    EXPECT_EQ(num_cities, 557);
    
    int num_provinces = 0;
    int num_provinces_cities = 0;
    for (auto& country : w.country_vec) {
        num_provinces += country.provinces_vec.size();
        for (auto& province : country.provinces_vec) {
            num_provinces_cities += province.cities_vec.size();
        }
    }
    EXPECT_EQ(num_provinces, 1455) ;
    EXPECT_EQ(num_provinces_cities, 2595);

    int num_ethnicgroups = 0;
    for (auto& country : w.country_vec)
        num_ethnicgroups += country.ethnicgroups_vec.size();
    EXPECT_EQ(num_ethnicgroups, 515);
    
    int num_religions = 0;
    for (auto& country : w.country_vec)
        num_religions += country.religions_vec.size();
    EXPECT_EQ(num_religions, 446);
    
    int num_languages = 0;
    for (auto& country : w.country_vec)
        num_languages += country.languages_vec.size();
    EXPECT_EQ(num_languages, 116);
}

TEST(TestBigFile, write)
{
    static unsigned int pugi_parse_options = pugi::parse_trim_pcdata | pugi::parse_embed_pcdata | pugi::parse_escapes | pugi::parse_cdata | pugi::parse_eol | pugi::parse_wconv_attribute;

    pugi::xml_document read_doc_1;
    pugi::xml_parse_result pugi_parse_result = read_doc_1.load_file(big_file_name, pugi_parse_options);    
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;
    
    pugi_serializer::reader reader_serializer_1(read_doc_1);
    world w_1;
    w_1.serialize(reader_serializer_1);
    
    pugi::xml_document write_doc;
    pugi_serializer::writer writer_serializer(write_doc, "mondial");
    writer_serializer.set_should_write_default_values(false);
    w_1.serialize(writer_serializer);
    
    bool save_result = write_doc.save_file(big_file_out_name);
    ASSERT_TRUE(save_result);
    
    pugi::xml_document read_doc_2;
    pugi_parse_result = read_doc_2.load_file(big_file_out_name, pugi_parse_options);
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;
    pugi_serializer::reader reader_serializer_2(read_doc_2);
    world w_2;
    w_2.serialize(reader_serializer_2);

    ASSERT_EQ(w_1, w_2);
}
