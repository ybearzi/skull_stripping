CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT( skull_stripping )

SET(CMAKE_AUTOMOC ON)

#Find ITK
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	INCLUDE(${ITK_USE_FILE})
ENDIF(ITK_FOUND)

#Find Qt
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Add Qt files
ADD_DEFINITIONS(${QT_DEFINITIONS})

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

SET( My_sources main.cpp mainwindow.cpp strip.cpp csv_writer.cpp)
SET( My_hearders mainwindow.h strip.h csv_writer.h)
SET( My_forms mainwindow.ui )
SET( My_RCC )

QT4_ADD_RESOURCES(My_RCC_srcs ${My_RCC})
QT4_WRAP_UI( My_forms_headers ${My_forms} )
QT4_WRAP_CPP( My_headers_moc ${My_headers} )

SET( OtherHeaders  ) # All files not using Qt
SET( OtherSources  )

ADD_EXECUTABLE(skull_stripping ${My_sources} ${My_headers_moc} ${My_forms_headers} ${My_RCC_srcs} ${OtherHeaders} ${OtherSources} )

TARGET_LINK_LIBRARIES(skull_stripping ${QT_LIBRARIES} ${ITK_LIBRARIES})
