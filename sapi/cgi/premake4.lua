-- A solution
solution "luasp_cgi"
	configurations { "Debug", "Release" }
	defines { "TEST" }
	project "luasp_cgi"
		kind "ConsoleApp"
		language "C++"
		platforms {"Native"}
		location "build"
		targetdir "bin"
		files { "./**.h", 
				"./**.cpp",
				"../../lsplib/**.h",
				"../../lsplib/**.cpp" }
		includedirs { "/usr/include/lua5.1", "../../lsplib/" }
		links { "pthread", "fcgi", "lua5.1", "uuid" }
		libdirs {"../../lsplib/"}
		defines { 'LSP_VERSION=\\"luasp`date +%Y%m%d`\\"' }

		configuration {"Debug"}
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration {"Release"}
			defines { "NDEBUG" }
			flags { "Optimize", "NoRTTI", "NoExceptions" }

