 project( FakeImageKiller )
 
 find_package( OpenCV REQUIRED )
 
 add_executable( FakeImageKiller FakeImageKiller )
 
 target_link_libraries( FakeImageKiller ${OpenCV_LIBS} )