#ifndef H_FUNCTIONS
#define H_FUNCTIONS

#include <string>
#include <vector>
#include <format>


struct Massiv {
    std::string name;
    std::vector<int> elements;
};

int getOperation();

void getStandartArray(Massiv& massiv);

std::string standartToTOML(const Massiv& massiv);

void getTOMLarray(std::string& TOMLarray);

std::string TOMLtoStandart(const std::string& TOMLarray);

//ПЕРЕГРУЗКИ

std::string standartToTOML(const std::string& name, const std::vector<int>& arr);
const char* standartToTOML(const char* name, const int* arr, std::size_t alen);
const char* standartToTOML(const char* name, std::size_t slen, const int* arr, std::size_t alen);

const char* TOMLtoStandart(const char* TOMLarray);
const char* TOMLtoStandart(const char* TOMLarray, std::size_t len);

#endif
