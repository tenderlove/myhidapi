require 'gel'

def try_block times
  times.times { x = yield; return x if x }
  raise "Couldn't do it"
end

dev = Gel.enumerate(0x0, 0x0).find { |dev| dev.product_string == "ErgoDox EZ" }

handle = try_block(10) { dev.open }
p handle

h = 0    # 0x0 to 0x168 (which is 360 base 10)
s = 0xFF # 0x0 to 0xFF
v = 0xFF # 0x0 to 0xFF

try_block(10) do
  handle.write [0x0, 0x6, (h >> 8) & 0xFF, h & 0xFF, s, v].pack('C*')
end

handle.read_timeout 10, 300 # 300 ms timeout
