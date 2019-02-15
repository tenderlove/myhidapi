require 'mkmf'

ldflags = cppflags = nil
if RbConfig::CONFIG["host_os"] =~ /darwin/
  begin
    brew_prefix = `brew --prefix hidapi`.chomp
    ldflags   = "#{brew_prefix}/lib"
    cppflags  = "#{brew_prefix}/include"
    pkg_conf  = "#{brew_prefix}/lib/pkgconfig"

    # pkg_config should be less error prone than parsing compiler
    # commandline options, but we need to set default ldflags and cpp flags
    # in case the user doesn't have pkg-config installed
    ENV['PKG_CONFIG_PATH'] ||= pkg_conf
  rescue
  end
end

pkg_config 'hidapi'

dir_config("hidapi", cppflags, ldflags)

raise "Install hidapi" unless have_header 'hidapi.h'

create_makefile 'gel'
