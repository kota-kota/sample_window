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

#ビルドタイプ(Debug or Release)
set(CMAKE_BUILD_TYPE Debug)

#プロジェクト名
set(PROJECT_NAME "X11")
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
	-lX11
)

message(STATUS "* CXX CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "* CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "* MSVC: ${MSVC}")
message(STATUS "* GNUCC: ${CMAKE_COMPILER_IS_GNUCC}")
message(STATUS "* GNUCXX: ${CMAKE_COMPILER_IS_GNUCXX}")

#コンパイラチェック
if(NOT (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX))
	message(FATAL_ERROR "Compiler is NOT GNUCC or GNUCXX")
endif()

#コンパイルオプションの設定
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) #C++11を選択する(コンパイルオプションに-std=c++11が付与される)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #CMAKE_CXX_STANDARDを有効にする
set(CMAKE_CXX_EXTENSIONS OFF) #GNU拡張機能を使用しない
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wdisabled-optimization -Weffc++ -Wendif-labels -Wfloat-equal -Winit-self -Winline -Wjump-misses-init -Wlogical-op -Wmissing-include-dirs -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wreorder -Wshadow -Wsign-promo -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations -Wvariadic-macros -Wwrite-strings -Wzero-as-null-pointer-constant ")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") #ビルドタイプがDebugの場合のみ
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG") #ビルドタイプがReleaseの場合のみ

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
