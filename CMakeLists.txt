cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Triangles)

add_executable(Triangles source/main.cpp source/Triangle/triangle.cpp source/Sort/sort.cpp source/Compare/compare.cpp source/Compare/compare2D.cpp)

enable_testing()

add_test(FirstTest Triangles ../source/Tests/input_1.txt ../source/Tests/answer_1.txt)
add_test(SecondTest Triangles ../source/Tests/input_2.txt ../source/Tests/answer_2.txt)
add_test(ThirdTest Triangles ../source/Tests/input_3.txt ../source/Tests/answer_3.txt)
add_test(FourthTest Triangles ../source/Tests/input_4.txt ../source/Tests/answer_4.txt)
add_test(FifthTest Triangles ../source/Tests/input_5.txt ../source/Tests/answer_5.txt)
add_test(SixthTest Triangles ../source/Tests/input_6.txt ../source/Tests/answer_6.txt)
add_test(SeventhTest Triangles ../source/Tests/input_7.txt ../source/Tests/answer_7.txt)
add_test(EighthTest Triangles ../source/Tests/input_8.txt ../source/Tests/answer_8.txt)
add_test(NinthTest Triangles ../source/Tests/input_9.txt ../source/Tests/answer_9.txt)
add_test(TenthTest Triangles ../source/Tests/input_10.txt ../source/Tests/answer_10.txt)