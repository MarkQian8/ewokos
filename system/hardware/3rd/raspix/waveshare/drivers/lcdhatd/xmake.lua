target("lcdhatd")
    set_type("application")
    add_deps("libfbd", "libbsp")
    add_files("**.c")        
    install_dir("drivers/raspix")
target_end()
