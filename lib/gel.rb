require "gel.so"

module Gel
  VERSION = '1.0.2'

  class DeviceInfo
    attr_reader :vendor_id, :product_id, :path, :serial_number, :manufacturer_string, :product_string, :usage, :interface_number

    def initialize vendor_id, product_id, path, serial_number, manufacturer_string, product_string, usage, interface_number
      @vendor_id           = vendor_id
      @product_id          = product_id
      @path                = path
      @serial_number       = serial_number
      @manufacturer_string = manufacturer_string
      @product_string      = product_string
      @usage               = usage
      @interface_number    = interface_number
    end

    def open
      Gel.open vendor_id, product_id
    end
  end

  def self.build_device_info(*args)
    DeviceInfo.new(*args)
  end
end
