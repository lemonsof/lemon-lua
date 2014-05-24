INCLUDE (CheckIncludeFiles)

if(WIN32)

	if(LEMON_BUILD_TARGET_WXP) 
		set(_WIN32_WINNT 0x0501)
		set(_WIN32_WINDOWS 0x0501)
		set(NTDDI_VERSION NTDDI_WINXPSP3)
		set(_WIN32_IE 0x0603)
	elseif(LEMON_BUILD_TARGET_WNET)
	    set(_WIN32_WINNT 0x0502)
		set(_WIN32_WINDOWS 0x0502)
		set(NTDDI_VERSION NTDDI_WS03SP2)
		set(_WIN32_IE 0x0603)
	elseif(LEMON_BUILD_TARGET_WLH)
	    set(_WIN32_WINNT 0x0700)
		set(_WIN32_WINDOWS 0x700)
		set(NTDDI_VERSION NTDDI_VISTASP1)
		set(_WIN32_IE 0x0700)
	elseif(LEMON_BUILD_TARGET_WIN7)
	    set(_WIN32_WINNT 0x0800)
		set(_WIN32_WINDOWS 0x800)
		set(NTDDI_VERSION NTDDI_WIN7)
		set(_WIN32_IE 0x0700)
	endif()
else()
	 add_definitions(-D_BSD_SOURCE)
endif(WIN32)

CHECK_INCLUDE_FILES (sys/event.h LEMOON_KQUEUE_H)
CHECK_INCLUDE_FILES (sys/epoll.h LEMOON_HAS_EPOLL_H)
