#pragma once
#include <type_traits>
#include "string.hpp"

template <uint32_t HASH>
struct Printer;

#define DECLARE_PRINTER(Type) \
    template <> \
    struct Printer<type_hash<Type>()> \
    { \
        using CompType = Type; \
        using ThisType = Printer<type_hash<Type>()>; \
        static std::string to_string(uint8_t* p) \
        { \
            return StringFuncs::toString((CompType*)p); \
        } \
        static void print(uint8_t* p) \
        { \
            std::string value = ThisType::to_string(p); \
            fprintf(stderr, "[%s]\n", value.c_str()); \
        } \
    }; \

#define ECS_LOG_MARGIN 20

#define ECS_COMPONENT_PRINT(Class, ...) \
    template<> \
    inline void ECSComponentPrint<Class>(const uint8_t* comp, uint32_t count) \
    { \
        std::cout << "\n   "; \
        Array<String> arr = {__VA_ARGS__}; \
        for(uint32_t i = 0; i < arr.size(); i++){ \
            std::cout << std::setw(ECS_LOG_MARGIN) << arr[i]; \
        } \
        std::cout << std::endl; \
        for(uint32_t i = 0; i < count; i++) \
        { \
            Class* value = (Class*)(comp + (i * sizeof(Class))); \
            fprintf(stderr, "%3d %s\n", i, StringFuncs::toString(value).c_str()); \
        } \
    } \


// struct my_struct : public ECSComponent<my_struct>
// {
//     
// };
//
// inline std::ostream& operator<<(std::ostream& os, const my_struct& value)
// {
//     os << "sdfdsf";
//     return os;
// }

// ECS_COMPONENT_PRINT(my_struct, "sd");
// template<>
// inline void ECSComponentPrint<my_struct>(const uint8_t* comp, uint32_t count)
// {
// 	fprintf(stderr, "    %15s %15s\n", "velocity", "acceleration");
// 	for(uint32_t i = 0; i < count; i++)
// 	{
// 		my_struct* value = (my_struct*)(comp + (i * sizeof(my_struct)));
// 		fprintf(stderr, "%3d %s\n", i, StringFuncs::toString(value).c_str());
// 	}
// }
    