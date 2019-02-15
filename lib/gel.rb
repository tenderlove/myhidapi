require "gel.so"

module Gel
  class DeviceInfo
    attr_reader :vendor_id, :product_id, :path, :serial_number, :manufacturer_string, :product_string

    def initialize vendor_id, product_id, path, serial_number, manufacturer_string, product_string
      @vendor_id           = vendor_id
      @product_id          = product_id
      @path                = path
      @serial_number       = serial_number
      @manufacturer_string = manufacturer_string
      @product_string      = product_string
    end
  end

  def self.build_device_info(*args)
    DeviceInfo.new(*args)
  end
end
