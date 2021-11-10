# Axis Definition - 0 (stepper), 1 (servo)
1AXSDEF0
# Encoder resolution, counts/egu
1ERES2000
# Encoder Polarity - 0 (normal polarity), 1 (reverse polarity)
1ENCPOL0
# Hardware End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
1LH0
# Software End-of-Travel Limit 
# - 0 (disabled), 1 (disable positive-direction),
# 2 (disable negative-direction), or 3 (enable both)
1LS0
# Absolute/Incremental Mode Enable - 0 (incremental mode) or 1 (absolute mode)
1MA1
# Preset/Continuous Mode Enable - 0 (Preset), 1 (Continuous)
# In the Preset Mode (MC0), all moves will go a specific distance.
# In the Continuous Mode (MC1), all moves will go to a specific velocity with the Distance (D) command establishing the direction (D+ or D-).
1MC0
 
# STEPPER ONLY
# Configures stepper axes to reference either the encoder position or the commanded position
# - 0 (commanded counts), 1 (encoder counts)
# If the ENCCNT1 mode is enabled TPE reports the encoder position, but in ENCCNT0 mode (the
# factory default setting) the TPE report represents the commanded position.
1ENCCNT1
# Pulse width. When the pulse width is changed 
# from the default value of 0.3 ms, the maximum velocity range is reduced.
1PULSE4
 # Drive resolution, counts/egu
1DRES25600
 
# SERVO ONLY 
# Select Servo Feedback Source. 0 (open loop, disable gains), 1 (encoder), or 2 (ANI input)
#1SFB1
# Maximum Allowable Position Error, counts/egu
#1SMPER10
