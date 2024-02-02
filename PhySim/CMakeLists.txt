cmake_minimum_required(VERSION 3.20)

project(PhySim)

add_definitions(${CGDEFINITIONS})

add_executable(MPM MPM.cpp)
target_compile_options(MPM PRIVATE ${CGFLAGS})
target_link_libraries(MPM PRIVATE ${CGLIBS})

add_executable(APIC APIC.cpp)
target_compile_options(APIC PRIVATE ${CGFLAGS})
target_link_libraries(APIC PRIVATE ${CGLIBS})

add_executable(FLIP FLIP.cpp)
target_compile_options(FLIP PRIVATE ${CGFLAGS})
target_link_libraries(FLIP PRIVATE ${CGLIBS})

add_executable(Eulerian Eulerian.cpp)
target_compile_options(Eulerian PRIVATE ${CGFLAGS})
target_link_libraries(Eulerian PRIVATE ${CGLIBS})
