# Hardware validation notes

## STEP-rate check (deferred)

The IOC's raised-cosine deceleration diagnostic repeatedly observed about 90.9% of
the distance predicted from the programmed STEP rates.  For example, with a 0.5 s
deceleration from 25,600 to 5,120 microsteps/s, the expected distance is 7,680
microsteps; the FPGA `STEP_RB` counter reported 6,983 in 0.500064 s.

The software profile elapsed time is therefore not the cause.  The FPGA source
uses a 32-bit phase accumulator, so a `motor_step_rate` of 1,099,511 should yield
25.6 kHz when `clk` is 99.999001 MHz.

When a scope is available, probe the physical STEP output with
`motor_step_rate = 1,099,511`.  Its rising-edge period should be 39.0625 us
(25.6 kHz).  An ILA clocked by the same `clk` can validate the DDS structurally:
successive `speed_count` MSB rising transitions should be separated by 3906 or
3907 `clk` cycles, but it cannot establish the absolute clock frequency.
