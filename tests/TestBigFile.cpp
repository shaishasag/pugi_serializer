#include <iostream>

#include "gtest/gtest.h"
#include "pugi_serializer.hpp"

class continent : public pugi_serializer::serialized_base
{
public:
    std::string id;
    std::string name;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("id", id);
        ser.attribute("name", name);
    }
};

class percentage_value : public pugi_serializer::serialized_base
{
public:
    std::string name;
    float percentage = 0.0;
    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("percentage", percentage);
        ser.text(name);
    }
};

class city : public pugi_serializer::serialized_base
{
public:
    std::string id;
    std::string name;
    std::string country;
    float longitude = 0.0;
    float latitude = 0.0;
    int population_year = 0;
    unsigned int population = 0;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("id", id);
        ser.child("name").text(name);
        ser.attribute("country", country);
        
        ser.attribute("longitude", longitude);
        ser.attribute("latitude", latitude);
        
        auto population_node = ser.child("population");
        population_node.text(population);
        population_node.attribute("year", population_year);
    }
};

class country : public pugi_serializer::serialized_base
{
public:
    std::string id;
    std::string name_attrib;
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
    std::string name_text;
    
    std::vector<city> cities_vec;
    std::vector<percentage_value> ethnicgroups;
    std::vector<percentage_value> religions;
    std::vector<percentage_value> languages;

    void serialize(pugi_serializer::serializer_base& ser) override
    {
        ser.attribute("id", id);
        ser.attribute("name", name_attrib);
        ser.attribute("capital", capital);
        ser.attribute("population", population);
        ser.attribute("total_area", total_area);
        ser.attribute("population_growth", population_growth);
        ser.attribute("infant_mortality", infant_mortality);
        ser.attribute("gdp_agri", gdp_agri);
        ser.attribute("gdp_total", gdp_total);
        ser.attribute("inflation", inflation);
        ser.attribute("population", population);
        ser.attribute("indep_date", indep_date);
        ser.attribute("government", government);
        ser.attribute("car_code", car_code);
        
        ser.child("name").text(name_text);

        pugi_serializer::serialize_container(ser, cities_vec, "city");
        pugi_serializer::serialize_container(ser, ethnicgroups, "ethnicgroups");
        pugi_serializer::serialize_container(ser, religions, "religions");
        pugi_serializer::serialize_container(ser, languages, "languages");
    }
};


TEST(TestBigFile, read)
{
    const char* big_file_name = "mondial-3.0.xml";
    pugi::xml_document doc;
    static unsigned int pugi_parse_options = pugi::parse_trim_pcdata | pugi::parse_embed_pcdata | pugi::parse_escapes | pugi::parse_cdata | pugi::parse_eol | pugi::parse_wconv_attribute;
    pugi::xml_parse_result pugi_parse_result = doc.load_file(big_file_name, pugi_parse_options);
    ASSERT_EQ(pugi::status_ok, pugi_parse_result.status) << "failed to read " << big_file_name;
    EXPECT_STREQ(doc.document_element().name(), "mondial") << "document_element is named " << doc.document_element().name() << " instead of " << "'mondial'";

    pugi_serializer::reader reader_serializer(doc);
    
    std::vector<continent> continent_vec;
    pugi_serializer::serialize_container(reader_serializer, continent_vec, "continent");
    EXPECT_EQ(continent_vec.size(), 5) << "expected 5 continents";
    EXPECT_EQ(continent_vec.front().id, "f0_119") << "expected 1st continent id to be f0_119";
    EXPECT_EQ(continent_vec.front().name, "Europe") << "expected 1st continent name to be Europe";
    EXPECT_EQ(continent_vec.back().id, "f0_132") << "expected last continent id to be f0_132";
    EXPECT_EQ(continent_vec.back().name, "Africa") << "expected last continent name to be Africa";

    std::vector<country> country_vec;
    pugi_serializer::serialize_container(reader_serializer, country_vec, "country");
    EXPECT_EQ(country_vec.size(), 231) << "expected 231 countries";
    
    int num_cities = 0;
    for (auto& country : country_vec)
        num_cities += country.cities_vec.size();
    EXPECT_EQ(num_cities, 557) << "expected 557 cities";
    
    int num_ethnicgroups = 0;
    for (auto& country : country_vec)
        num_ethnicgroups += country.ethnicgroups.size();
    EXPECT_EQ(num_ethnicgroups, 515) << "expected 515 ethnicgroups";
    
    int num_religions = 0;
    for (auto& country : country_vec)
        num_religions += country.religions.size();
    EXPECT_EQ(num_religions, 446) << "expected 446 religions";
    
    int num_languages = 0;
    for (auto& country : country_vec)
        num_languages += country.languages.size();
    EXPECT_EQ(num_languages, 116) << "expected 116 languages";
}
