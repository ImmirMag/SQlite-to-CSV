cmake_minimum_required(VERSION 3.13)
project(SQL_CSV)

set(CMAKE_CXX_STANDARD 14)

add_executable(SQL_CSV main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net Zip Data DataSQLite Crypto)

target_link_libraries(SQL_CSV PUBLIC Poco::Foundation Poco::Zip Poco::Net Poco::Data Poco::DataSQLite Poco::Crypto)