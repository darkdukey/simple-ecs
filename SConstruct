import os
import sys

if(sys.platform == 'msys'):
    env = Environment(ENV = {'PATH' : os.environ['PATH']})
    env['ENV']['TMP'] = '/tmp'
    env['ENV']['TEMP'] = '/tmp'
else:
    env = Environment(ENV=os.environ, tools = ['mingw'])
    env.Append(CXXFLAGS = '-std=c++17')
    env.Append(CPPPATH = [
        './include/',
        './lib/glm/'
    ])

env.Program('program.exe', Glob('src/*.cpp'))