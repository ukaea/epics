# Axis Definition - 0 (stepper), 1 (servo)
5AXSDEF1
# Encoder resolution, counts/egu
5ERES2000
# Encoder Polarity - 0 (normal polarity), 1 (reverse polarity)
5ENCPOL0
# Hardware End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
5LH0
# Software End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
5LS0
# Absolute/Incremental ModeEnable - 0 (incremental mode) or 1 (absolute mode)
5MA1
# Preset/Continuous ModeEnable - 0 (Preset), 1 (Continuous)
5MC1
 
# STEPPER ONLY
# Configures stepper axes to reference either the encoder position or the commanded position
# - 0 (commanded counts), 1 (encoder counts)
#5ENCCNT1
# Pulse width. When the pulse width is changed 
# from the default value of 0.3 ms, the maximum velocity range is reduced.
#5PULSE4
# Drive resolution, counts/egu
#5DRES2000
 
# SERVO ONLY 
# Select Servo Feedback Source. 0 (open loop, disable gains), 1 (encoder), or 2 (ANI input)
5SFB0
# SERVO CLOSED LOOP ONLY 
# Maximum Allowable Position Error, counts/egu
#5SMPER0
# Target Zone Distance, counts
#5STRGTD0
# Target Velocity Zone, counts/sec 
#5STRGTV0
# Proportional Feedback Gain, millivolts/count  (KP)
#5SGP0
# Integral Feedback Gain, millivolts/count * sec  (KI)
#5SGI0
# Velocity Feedback Gain, microvolts/count/sec   (KD)
#5SGV0
