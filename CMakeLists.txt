cmake_minimum_required(VERSION 3.16)

project(AccessAuthentication VERSION 0.4.3 LANGUAGES CXX)
set(PROJECT_NAME AccessAuthentication)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置 Qt 的安装目录
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64/lib/cmake")

# 设置头文件目录
include_directories(${PROJECT_SOURCE_DIR}/includes)
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/includes/*.h")
# 添加源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/srcs SRC_FILES)
# # 其他配置文件
# set(RC_FILES ${PROJECT_SOURCE_DIR}/icon_config.rc)
# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE   # 不显示 CMD 窗口
)

# 链接 Qt 库
target_link_libraries(
    ${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# # 复制 resource 文件夹
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     # 一次性拷贝整个 resource
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             "${CMAKE_CURRENT_SOURCE_DIR}/resource"
#             "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
#     COMMENT "Copying resource/ to $<TARGET_FILE_DIR:${PROJECT_NAME}>"
# )

# ==================== 安装规则 ====================
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/Release")   # build/Release

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DEPLOY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${DEPLOY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEPLOY_DIR}"
)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/qml"
        DESTINATION "${DEPLOY_DIR}"
        CONFIGURATIONS Release MinSizeRel RelWithDebInfo
)

# 找到 windeployqt 可执行文件
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

# 每次构建完成后自动运行 windeployqt
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E echo ">>> Running windeployqt..."
    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
            --no-translations       # 不需要翻译包
            --qmldir "${CMAKE_SOURCE_DIR}/qml"
            "$<TARGET_FILE:${PROJECT_NAME}>"
    WORKING_DIRECTORY "${DEPLOY_DIR}"
    COMMENT "Deploying Qt dependencies..."
    VERBATIM
)

qt_finalize_executable(${PROJECT_NAME})