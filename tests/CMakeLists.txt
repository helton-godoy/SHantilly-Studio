find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(tst_WidgetConfigs tst_WidgetConfigs.cpp)

target_link_libraries(tst_WidgetConfigs PRIVATE 
    Qt6::Test 
    showbox-ui
)

target_include_directories(tst_WidgetConfigs PRIVATE 
    "${SHOWBOX_UI_INCLUDE}"
)

add_test(NAME tst_WidgetConfigs COMMAND tst_WidgetConfigs)
