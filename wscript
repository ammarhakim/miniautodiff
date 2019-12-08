# -*- python -*-

def options(opt):
    opt.load('compiler_cxx') 

def configure(conf):
    conf.load('compiler_cxx')
    conf.env.append_value('CXXFLAGS', '-Wall')
    conf.env.append_value('CXXFLAGS', '-O3')
    conf.env.append_value('CXXFLAGS', '-std=c++17')

def build(bld):
    bld.recurse("Unit")
    buildExec(bld)
        
def buildExec(bld):
    pass
