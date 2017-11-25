project(CppVariantType)
enable_language(CXX)

add_executable(variant_sample
    variant.hpp
    main_variant.cpp)

add_executable(msgpack_sample
    variant.hpp
    msgpack-alt.hpp
    main_msgpack.cpp)

#target_link_libraries(my-command
#    my-lib)
