target("gt911_touchd")
    set_type("application")
    add_deps("libbsp")
    add_files("**.c")        
    install_dir("drivers/raspix")
target_end()
