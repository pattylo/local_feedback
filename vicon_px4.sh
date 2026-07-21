#!/usr/bin/env bash

SESSION="vicon_px4"

# Create detached session
tmux new-session -d -s "$SESSION" -n main

# Split first window into two panes
tmux split-window -h -t "$SESSION":0

# Left pane: VRPN
tmux send-keys -t "$SESSION":0.0 \
'source ros2_ws/install/setup.sh && ros2 launch local_feedback vrpn.launch.py' C-m

# Right pane: PX4 controller
tmux send-keys -t "$SESSION":0.1 \
'source ros2_ws/install/setup.sh && ros2 launch quadcopter_ctrl px4.launch' C-m

# Optional: equal pane sizes
tmux select-layout -t "$SESSION":0 even-horizontal

# Attach
tmux attach -t "$SESSION"