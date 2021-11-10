# Axis Definition - 0 (stepper), 1 (servo)
8AXSDEF1
# Encoder resolution, counts/egu
#8ERES2000
# Encoder Polarity - 0 (normal polarity), 1 (reverse polarity)
#8ENCPOL0
# Hardware End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
8LH0
# Software End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
8LS0
# Absolute/Incremental Mode Enable - 0 (incremental mode) or 1 (absolute mode)
8MA1
# Preset/Continuous Mode Enable - 0 (Preset), 1 (Continuous)
# In the Preset Mode (MCØ), all moves will go a specific distance.
# In the Continuous Mode (MC1), all moves will go to a specific velocity with the Distance (D) command establishing the direction (D+ or D-).
8MC1
 
# STEPPER ONLY
# Configures stepper axes to reference either the encoder position or the commanded position
# - 0 (commanded counts), 1 (encoder counts)
#8ENCCNT1
# Pulse width. When the pulse width is changed 
# from the default value of 0.3 ms, the maximum velocity range is reduced.
#8PULSE4
# Drive resolution, counts/egu
8DRES2000
 
# SERVO ONLY 
# Select Servo Feedback Source. 0 (open loop, disable gains), 1 (encoder), or 2 (ANI input)
8SFB1
### Close loop values... must be zero if in Open loop.###
# Maximum Allowable Position Error, counts/egu
#8SMPER0
# Target Zone Distance, counts
#8STRGTD25600
# Target Velocity Zone, counts/sec 
#8STRGTV0
# Proportional Feedback Gain, millivolts/count
#8SGP0
# Integral Feedback Gain, millivolts/count * sec
#8SGI0
# Velocity Feedback Gain, microvolts/count/sec
#8SGV0
