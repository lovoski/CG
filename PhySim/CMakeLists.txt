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

add_executable(Spring PBD/Spring.cpp)
target_compile_options(Spring PRIVATE ${CGFLAGS})
target_link_libraries(Spring PRIVATE ${CGLIBS})

add_executable(Incompressible PBD/Incompressible.cpp)
target_compile_options(Incompressible PRIVATE ${CGFLAGS})
target_link_libraries(Incompressible PRIVATE ${CGLIBS})

add_executable(BallWithTrack PBD/BallWithTrack.cpp)
target_compile_options(BallWithTrack PRIVATE ${CGFLAGS})
target_link_libraries(BallWithTrack PRIVATE ${CGLIBS})

add_executable(Pendulum PBD/Pendulum.cpp)
target_compile_options(Pendulum PRIVATE ${CGFLAGS})
target_link_libraries(Pendulum PRIVATE ${CGLIBS})