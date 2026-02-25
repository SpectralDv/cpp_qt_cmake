# cmake_minimum_required(VERSION 3.16)
# project(MyQtApp)

# # Устанавливаем стандарт C++17 (требуется для Qt6)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Включаем автоматическую обработку Qt (moc, uic, rcc)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

# # ------------------------------------------------------------
# # Указываем путь к вашей вручную собранной Qt6
# # ЗАМЕНИТЕ ЭТОТ ПУТЬ НА ВАШ РЕАЛЬНЫЙ!
# # ------------------------------------------------------------
# # Пример 1: если Qt установлена прямо в C:\qtbase
# set(CMAKE_PREFIX_PATH "C:/qtbase" ${CMAKE_PREFIX_PATH})

# # Пример 2: если Qt в C:\Qt\6.5.3\msvc2019_64 (стандартная структура)
# # set(CMAKE_PREFIX_PATH "C:/Qt/6.5.3/msvc2019_64" ${CMAKE_PREFIX_PATH})

# # Пример 3: если хотите задавать путь через переменную окружения
# # set(CMAKE_PREFIX_PATH "$ENV{QT6_DIR}" ${CMAKE_PREFIX_PATH})
# # ------------------------------------------------------------

# # Ищем Qt6 с нужными компонентами
# find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# # Добавляем исходники (укажите свои файлы)
# set(SOURCES
#     main.cpp
#     # mainwindow.cpp
#     # mainwindow.h
# )

# # Создаём исполняемый файл
# add_executable(${PROJECT_NAME} ${SOURCES})

# # Линкуемся с Qt
# target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)






cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(main LANGUAGES CXX)
elseif(UNIX)
    project(main)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)


#======================= INCLUSION OF Qt =======================#
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # OFF для -std=c++23, ON для -std=gnu++23

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_PREFIX_PATH $ENV{QTDIR})
#set(CMAKE_PREFIX_PATH "C:/qtbase" ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH "С:/Qt/6.7.2/mingw_64/bin" ${CMAKE_PREFIX_PATH})
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Network REQUIRED)


#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

#=================== SETUP EXECTUABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(main WIN32 ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
elseif(UNIX)
    add_executable(main ${SOURCES})
endif()



# Add the target includes for MY_PROJECT 
target_include_directories(main PRIVATE ${FORMS_DIR})
target_include_directories(main PRIVATE ${INCLUDE_DIR})
target_include_directories(main PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(main PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)


if(WIN32)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}  # Для Windows - .exe
    # LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib  # Для библиотек
    # ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib  # Для статических библиотек
)
endif()
