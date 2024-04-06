cmake_minimum_required(VERSION 2.8)

project(Mouthus)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
)

FILE(
	GLOB_RECURSE
	SOURCE_FILES
	
	mouthus_core.h
	mouthus_core.cpp
	
	mouthus_dict.h
	mouthus_dict.cpp
	
	mouthus_dict_ressource.h
)

# another list, this time it includes all header files that should be treated with moc
SET( MOC_HDRS
	mouthus_main_window.h
	mouthus_network_manager.h
)

# some .ui files
SET( UIS
	mouthus_main_window.ui
)

# and finally an resource file
SET( RCS
	mouthus.qrc
)

QT4_ADD_RESOURCES( RC_SRCS ${RCS} )
QT4_WRAP_UI( UI_HDRS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} )

add_executable(
	dictionaries_conversion
	dictionaries_conversion.cpp
)

add_executable(
	MouthusConsole
	mouthus_console.cpp
	${SOURCE_FILES}
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
IF(APPLE)
  # set how it shows up in the Info.plist file
  SET(MACOSX_BUNDLE_ICON_FILE mouthus.icns) 
  # set where in the bundle to put the icns file
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/ressources/mouthus.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  SET(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/ressources/mouthus.icns)
ENDIF(APPLE)

add_executable(
	Mouthus
	MACOSX_BUNDLE
	mouthus_main_window.h
	mouthus_main_window.cpp
	mouthus_network_manager.h
	mouthus_network_manager.cpp
	mouthus_ui.cpp
	${SOURCE_FILES}
	${RC_SRCS}
	${UI_HDRS}
	${MOC_SRCS}
)

target_link_libraries(
	Mouthus
	${QT_LIBRARIES}
)

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Mouthus")
IF(APPLE)
  SET(plugin_dest_dir Mouthus.app/Contents/MacOS)
  SET(qtconf_dest_dir Mouthus.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/Mouthus.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Mouthus.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the QtTest application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS Mouthus 
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
# INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIRS})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)


# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
# set(CPACK_BINARY_DRAGNDROP ON)
SET(CPACK_GENERATOR "DragNDrop")
include(CPack)

add_subdirectory(InnoSetupInstaller)