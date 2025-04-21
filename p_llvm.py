#custom.py
use_llvm="yes"
use_mingw="yes"

production="yes"
dev_build="no"
debug_symbols="no" 

CXXFLAGS="-pthread" 
LINKFLAGS="-pthread" 
CCFLAGS="-pthread"