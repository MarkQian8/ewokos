target("libfb")
    set_type("library")
    add_files("**.c")        
    add_deps("libgraph")
    add_includedirs("include",  {public = true})
target_end()
