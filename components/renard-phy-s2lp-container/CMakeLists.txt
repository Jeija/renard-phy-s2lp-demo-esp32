idf_component_register(
	INCLUDE_DIRS "renard-phy-s2lp/src" "renard-phy-s2lp/librenard/src"
)

file(GLOB renard_phy_s2lp_srcs ${COMPONENT_DIR}/renard-phy-s2lp/src/*.c ${COMPONENT_DIR}/renard-phy-s2lp/src/*.h)
file(GLOB librenard_srcs ${COMPONENT_DIR}/renard-phy-s2lp/librenard/src/*.c ${COMPONENT_DIR}/renard-phy-s2lp/librenard/src/*.h)

string(APPEND CMAKE_C_FLAGS " -mlongcalls")

add_custom_command(
	OUTPUT ${COMPONENT_DIR}/renard-phy-s2lp/renard-phy-s2lp.a
	DEPENDS ${renard_phy_s2lp_srcs} ${librenard_srcs}
	WORKING_DIRECTORY ${COMPONENT_DIR}/renard-phy-s2lp
	COMMAND make CC=${CMAKE_C_COMPILER} AR=${CMAKE_AR} ARCHFLAGS=${CMAKE_C_FLAGS}
	COMMENT "Compiling renard-phy-s2lp"
)

add_custom_target(
	renard_phy_s2lp_build
	DEPENDS ${COMPONENT_DIR}/renard-phy-s2lp/renard-phy-s2lp.a
)

add_dependencies(${COMPONENT_LIB} renard_phy_s2lp_build)

add_library(renard_phy_s2lp STATIC IMPORTED GLOBAL)
set_property(TARGET renard_phy_s2lp PROPERTY IMPORTED_LOCATION ${COMPONENT_DIR}/renard-phy-s2lp/renard-phy-s2lp.a)
target_link_libraries(${COMPONENT_LIB} PUBLIC INTERFACE renard_phy_s2lp)

set_target_properties(renard_phy_s2lp PROPERTIES IMPORTED_LOCATION
	${COMPONENT_DIR}/renard-phy-s2lp/renard-phy-s2lp.a)
set_target_properties(renard_phy_s2lp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
	${COMPONENT_DIR}/renard-phy-s2lp/src)

# "clean" step cannot be properly done
# Cleaning subproject object files should be manually performed if necessary!
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	"${COMPONENT_DIR}/renard-phy-s2lp/obj")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
	"${COMPONENT_DIR}/renard-phy-s2lp/renard-phy-s2lp.a")
