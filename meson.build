project('CppRand', 'cpp', default_options : ['cpp_std=c++14'],
  version : '1.0.0',
  license : 'GPL')


cxxconf_data = configuration_data()
cxxconf_data.set('USE_SIMDPP', 'false')


use_simdpp = get_option('use_simdpp')
simdpp_inc = []
if (use_simdpp)
  message('use_simdpp: true')
  subproject('libsimdpp')
  simdpp_inc = include_directories('subprojects/libsimdpp/')
  cxxconf_data.set('USE_SIMDPP', 'true')
endif


inc = include_directories('include')

inc_dirs = [inc, simdpp_inc]

subdir('include')
subdir('test')
