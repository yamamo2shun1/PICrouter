# Cross Compiling configuration for tkrworks PICrouter
# https://github.com/tkrworks/PICrouter
#
# Requires MPLABX and XC32 Compiler v1.22
#
#
MRuby::CrossBuild.new("PICrouter") do |conf|
  toolchain :gcc

  PICROUTER_PATH = "C:/Users/shun/MPLABXProjects/PICrouter"
  XC32_PATH = "C:/Program Files (x86)/Microchip/xc32/v1.22"
  MRUBY_PATH = "C:/Users/shun/MPLABXProjects/PICrouter/mruby-master"

  conf.cc do |cc|
    cc.command="#{XC32_PATH}/bin/xc32-gcc.exe"
    cc.include_paths = ["#{PICROUTER_PATH}/PICrouter_795_mrb.X",
                        "#{PICROUTER_PATH}/PICrouter_795_mrb.X/Include",
                        "#{PICROUTER_PATH}/PICrouter_795_mrb.X/USB",
                        "#{MRUBY_PATH}/include",
                        "#{XC32_PATH}/pic32mx/include"]
    cc.flags << "-g -ffunction-sections -fdata-sections -mips16 -mprocessor=32MX795F512H -O -Wl,--defsym=__MPLAB_BUILD=1," +
      "-Map=\"dist/default/production/PICrouter_795_mrb.X.production.map\",--defsym=_min_heap_size=102400,--defsym=_min_stack_size=1136,--gc-sections,"
    cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"
  end

  conf.archiver do |archiver|
    archiver.command = "#{XC32_PATH}/bin/xc32-ar"
    archiver.archive_options = 'rcs %{outfile} %{objs}'
  end

  #no executables
  conf.bins = []

  #do not build test executable
  conf.build_mrbtest_lib_only

  #gems from core
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-enum-ext"

  #light-weight regular expression
  #conf.gem :github => "masamitsu-murase/mruby-hs-regexp", :branch => "master" 

 end

