cmake_minimum_required(VERSION 2.8)

#CMakeで設定されている全ての変数を出力する
function(DumpCMakeVariables)
	message(STATUS "*** dump start cmake variables ***")
	get_cmake_property(_variableNames VARIABLES)
	foreach(_variableName ${_variableNames})
		message(STATUS "* ${_variableName}=${${_variableName}}")
	endforeach()
	message(STATUS "*** dump end ***")
endfunction()

#プロジェクト名
set(PROJECT_NAME "Win32")
project(${PROJECT_NAME})

#ソース
set(SRCS
	${CMAKE_SOURCE_DIR}/source/main.cpp
)
#インクルードパス
set(INC_PATH
	${CMAKE_SOURCE_DIR}/source
)
#ライブラリパス
set(LIB_PATH
	${CMAKE_SOURCE_DIR}/library
)
#ライブラリ
set(LIBS
	gdiplus.lib
)

message(STATUS "* CXX CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "* CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "* MSVC: ${MSVC}")
message(STATUS "* GNUCC: ${CMAKE_COMPILER_IS_GNUCC}")
message(STATUS "* GNUCXX: ${CMAKE_COMPILER_IS_GNUCXX}")

#コンパイラチェック
if(NOT MSVC)
	message(FATAL_ERROR "Compiler is NOT MSVC")
endif()

#コンパイルオプションの設定
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) #C++11を選択する(コンパイルオプションに-std=c++11が付与される)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #CMAKE_CXX_STANDARDを有効にする
set(CMAKE_CXX_EXTENSIONS OFF) #GNU拡張機能を使用しない
if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

#VisualStudioの設定
set(CMAKE_SUPPRESS_REGENERATION true) #ZERO_CHECKプロジェクトを生成しない
set_property(GLOBAL PROPERTY USE_FOLDERS ON) #フィルタ機能を有効にする
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}) #スタートアッププロジェクトを設定
source_group(TREE ${CMAKE_SOURCE_DIR}/source PREFIX "\\source" FILES ${SOURCES}) #ソースフィルタを設定

#出力ディレクトリ設定(ビルドタイプがDebugの場合)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
#出力ディレクトリ設定(ビルドタイプがReleaseの場合)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

#インクルードパス設定
include_directories(${INC_PATH})
#ライブラリパス設定
link_directories(${LIB_PATH})
#実行ファイル
add_executable(${PROJECT_NAME} ${SRCS})
#リンク
target_link_libraries(${PROJECT_NAME} ${LIBS})

#CMakeで設定されている全ての変数を出力する
#DumpCMakeVariables()
