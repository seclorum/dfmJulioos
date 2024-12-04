# Julia implementation of the DFM1 filter emulating the Korg MS20 filter
mutable struct DFM1
    sample_rate::Float64
    cutoff::Float64
    resonance::Float64
    drive::Float64
    input_state::Float64
    low_state::Float64
    high_state::Float64
    band_state::Float64
end

function DFM1(sample_rate::Float64; cutoff::Float64 = 1000.0, resonance::Float64 = 0.1, drive::Float64 = 1.0)
    return DFM1(sample_rate, cutoff, resonance, drive, 0.0, 0.0, 0.0, 0.0)
end

function process_sample(filter::DFM1, input::Float64)::Float64
    f = 2.0 * sin(pi * filter.cutoff / filter.sample_rate)
    q = filter.resonance
    input = tanh(filter.drive * input)
    filter.low_state += f * filter.band_state
    filter.high_state = input - filter.low_state - q * filter.band_state
    filter.band_state += f * filter.high_state
    return filter.low_state
end
