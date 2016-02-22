#!/usr/bin/env ruby

MAX_INPUT = 2**8 - 1
MAX_OUTPUT = 2**16 - 1

def cie1931(cieL)
  cieL *= 100.0
  if cieL <= 8
    cieL / 902.3
  else
    ((cieL + 16.0) / 116.0)**3
  end
end

def indices
  0 .. MAX_INPUT
end

def cie_values
  indices.map { |i| cie1931((i + 1).to_f / (MAX_INPUT + 1)) * MAX_OUTPUT }
end


require 'minitest/spec'

describe :cie1931 do
  it 'is zero at zero' do
    cie1931(0.0).must_equal 0
  end

  it 'is 1 at 1' do
    cie1931(1).must_equal 1
  end
end

if __FILE__ == $PROGRAM_NAME
  case ARGV.shift
  when '--help'
    puts 'Interpolation values for CIE 1931 lightness correction'
  when '--test'
    require 'minitest/rg'
    Minitest.autorun
  else
    puts cie_values.map { |cie| cie.round.to_s }
      .each_slice(16).map { |line| line.join(', ') }
      .join(",\n")
  end
end
