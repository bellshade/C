file(GLOB NN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
add_executable(neural_network ${NN_SOURCES})

if(OpenMP_C_FOUND)
    target_link_libraries(neural_network OpenMP::OpenMP_C)
endif()

if(MATH_LIBRARY)
    target_link_libraries(neural_network ${MATH_LIBRARY})
endif()

set_target_properties(neural_network PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)

install(TARGETS neural_network DESTINATION "bin/project/NeuralNetwork")
install(FILES dataset/iris.csv DESTINATION "bin/project/NeuralNetwork/dataset")

