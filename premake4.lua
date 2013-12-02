print("\nRequired libraries: Boost");
-- Find LIB directories.
boostDir = os.matchdirs("./boost*")[1]

if ( boostDir ) then
    print("All libraries found.");
else 
    print("\nLibraries not found: ");
    if not boostDir then print("Boost") end;
    print("");
    os.exit();
end

boostDir = "./" .. boostDir;

solution "boost-asio-bench"
    configurations { "Debug", "Release" }
    includedirs { boostDir }
    libdirs     { boostDir .. "/stage/lib" }
 
    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" } 

    project "throughput"
        kind "ConsoleApp"
        location "./throughput"
        language "C++"
        files { "throughput/**.cpp", "throughput/**.h" }
