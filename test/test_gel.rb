require 'minitest/autorun'
require 'gel'

class TestGel < MiniTest::Test
  def test_it_works
    devices = Gel.enumerate 0x0, 0x0
    dev = devices.find { |dev| dev.product_string == "ErgoDox EZ" }

    handle = dev.open

    while !handle
      p "retry"
      handle = dev.open
    end

    p handle

    buf = [0x0, 0x3, 35]
    loop do
      break if handle.write buf.pack('C*')
    end
    puts "done writing"

    buf = handle.read_timeout 1, 500
    p buf
  end

  def test_can_enumerate_by_ids
    devices = Gel.enumerate(0xfeed, 0x1307)

    refute_predicate devices, :empty?
  end
end
