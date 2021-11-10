# Axis Definition - 0 (stepper), 1 (servo)
7AXSDEF1
# Encoder resolution, counts/egu
#7ERES2000
# Encoder Polarity - 0 (normal polarity), 1 (reverse polarity)
7ENCPOL0
# Hardware End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
7LH0
# Software End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
7LS0
# Absolute/Incremental Mode Enable - 0 (incremental mode) or 1 (absolute mode)
7MA1
# Preset/Continuous Mode Enable - 0 (Preset), 1 (Continuous)
# In the Preset Mode (MCØ), all moves will go a specific distance.
# In the Continuous Mode (MC1), all moves will go to a specific velocity with the Distance (D) command establishing the direction (D+ or D-).
7MC1
 
# STEPPER ONLY
# Configures stepper axes to reference either the encoder position or the commanded position
# - 0 (commanded counts), 1 (encoder counts)
#7ENCCNT1
# Pulse width. When the pulse width is changed 
# from the default value of 0.3 ms, the maximum velocity range is reduced.
#7PULSE4
# Drive resolution, counts/egu
7DRES2000
 
# SERVO ONLY 
# Select Servo Feedback Source. 0 (open loop, disable gains), 1 (encoder), or 2 (ANI input)
7SFB1
### Close loop values... must be zero if in Open loop.###
# Maximum Allowable Position Error, counts/egu
7SMPER12800
# Target Zone Distance, counts
7STRGTD25600
# Target Velocity Zone, counts/sec 
7STRGTV0
# Proportional Feedback Gain, millivolts/count
7SGP0.9
# Integral Feedback Gain, millivolts/count * sec
7SGI0.0
# Velocity Feedback Gain, microvolts/count/sec
7SGV0.9
