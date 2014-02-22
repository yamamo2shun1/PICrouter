# Cross Compiling configuration for tkrworks PICrouter
# https://github.com/tkrworks/PICrouter
#
# Requires MPLABX and XC32 Compiler v1.22
#
#

  # include some GEMs for PICrouter
  conf.gembox 'picrouter'

MRuby::CrossBuild.new("PICrouter") do |conf|
  toolchain :gcc

  PICROUTER_PATH = "C:/Users/shun/MPLABXProjects/PICrouter"
  XC32_PATH = "C:/Program Files (x86)/Microchip/xc32/v1.22"
  MRUBY_PATH = "C:/Users/shun/MPLABXProjects/PICrouter/mruby-1.0.0"

  conf.cc do |cc|
    cc.command="#{XC32_PATH}/bin/xc32-gcc.exe"
    cc.include_paths = ["#{PICROUTER_PATH}/PICrouter_795_mrb.X",
                        "#{PICROUTER_PATH}/PICrouter_795_mrb.X/Include",
                        "#{PICROUTER_PATH}/PICrouter_795_mrb.X/USB",
                        "#{MRUBY_PATH}/include",
                        "#{XC32_PATH}/pic32mx/include"]
    cc.flags << "-g -ffunction-sections -fdata-sections -mips16 -mprocessor=32MX795F512H -O1 -Wl,--defsym=__MPLAB_BUILD=1," +
      "-Map=\"dist/default/production/PICrouter_795_mrb.X.production.map\",--defsym=_min_heap_size=102284,--defsym=_min_stack_size=1136,--gc-sections,"
    cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"
    
    cc.defines << %w(MRB_HEAP_PAGE_SIZE=64)
    cc.defines << %w(MRB_USE_IV_SEGLIST)
    cc.defines << %w(MRB_IVHASH_INIT_SIZE=3)
    cc.defines << %w(KHASH_DEFAULT_SIZE=8)
    cc.defines << %w(MRB_STR_BUF_MIN_SIZE=20)
    cc.defines << %w(MRB_GC_STRESS)
    #cc.defines << %w(DISABLE_STDIO)

  end

  conf.cxx do |cxx|
    cxx.command = conf.cc.command.dup
    cxx.include_paths = conf.cc.include_paths.dup
    cxx.flags = conf.cc.flags.dup
    cxx.defines = conf.cc.defines.dup
    cxx.compile_options = conf.cc.compile_options.dup
  end

  conf.archiver do |archiver|
    archiver.command = "#{XC32_PATH}/bin/xc32-ar"
    archiver.archive_options = 'rcs %{outfile} %{objs}'
  end

  #no executables
  conf.bins = []

  #do not build test executable
  conf.build_mrbtest_lib_only

 end
