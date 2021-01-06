import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import math

# PD3  Elevation angle for Star Tracker surface.
# PD6  Elevation angle for HD Camera and Optical RX surface.

pd_df = pd.read_csv("data/webmust_labeled/perfect_training_set.csv")
quat_euler_df = pd.read_csv("data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv")

fig_counter = 0

#################################
# 1 dimension using PD3 values. #
#################################

# Round values.
x = round(pd_df['PD6'], 1)

# The camera state: ON/OFF.
hd_camera_state = pd_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
fig_counter += 1
plt.figure(fig_counter)
plt.scatter(x, np.zeros(len(x)), c=col)
plt.title("HD Camera State: 1D")


#################################
# 2D from 1D photodiode values. #
#################################

def from_1d_to_2d_A(x):
    return [x, x*x]

def from_1d_to_2d_B(x):
    return [x, x % 2]


from_1d_to_2d_functions = [from_1d_to_2d_A, from_1d_to_2d_B]


for func in from_1d_to_2d_functions:

    # Input space
    x = round(pd_df['PD6'], 1)

    # Feature space
    xy = func(x)
    x = xy[0]
    y = xy[1]

    # The camera state: ON/OFF.
    hd_camera_state = pd_df['HD_CAMERA_STATE']
    

    # Color ON states blue and OFF states red.
    col = np.where(hd_camera_state == 1, 'b', 'r')

    # Plot.
    fig_counter += 1
    plt.figure(fig_counter)
    plt.scatter(x, y, c=col)
    plt.title("HD Camera State: 1D → 2D")




#########################################
# 2 dimension using PD3 and PD6 values. #
#########################################

# Round values.
x = round(pd_df['PD3'], 1)
y = round(pd_df['PD6'], 1)

# The camera state: ON/OFF.
hd_camera_state = pd_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
fig_counter += 1
plt.figure(fig_counter)
plt.scatter(x, y, c=col)
plt.title("HD Camera State: 2D")

##################################
# 3 dimension with Euler angles. #
##################################

x_euler = round(quat_euler_df['x_euler'], 0)
y_euler = round(quat_euler_df['y_euler'], 0)
z_euler = round(quat_euler_df['z_euler'], 0)

# The camera state: ON/OFF.
hd_camera_state = quat_euler_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col3d = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
fig_counter += 1
plt.title('HD Camera State: 3D with Euler angles')
fig = plt.figure(fig_counter)
ax = Axes3D(fig)
ax.set_xlabel('Euler X')
ax.set_ylabel('Euler Y')
ax.set_zlabel('Euler Z')
ax.scatter(x_euler, y_euler, z_euler, c=col3d)


#################################
# 2 dimension from 2 dimension. #
#################################

def transform_linear(x, y):
    return x*y

def transform_polynomial(x, y):
    return pow(x * y + 1, 2)

def transform_rbf(x, y):
    gamma = 1
    return pow(math.e, -gamma * pow(abs(x-y), 2)) 


transform_functions = [transform_linear, transform_polynomial, transform_rbf]


for func in transform_functions:

    # Round values.
    x = round(pd_df['PD3'], 1)
    y = round(pd_df['PD6'], 1)

    # Apply kernel trick.
    y = func(x, y)

    # The camera state: ON/OFF.
    hd_camera_state = pd_df['HD_CAMERA_STATE']

    # Color ON states blue and OFF states red.
    col = np.where(hd_camera_state == 1, 'b', 'r')

    # Plot.
    fig_counter += 1
    plt.figure(fig_counter)
    plt.scatter(x, y, c=col)
    plt.title("HD Camera State: 2D → 2D")


# Sources:
# https://towardsdatascience.com/the-kernel-trick-c98cdbcaeb3f
# https://course.ccs.neu.edu/cs6140sp15/6_SVM_kernels/lecture_notes/kernels/kernels.pdf



#################################
# 3D from 2D photodiode values. #
#################################

def kernel_polynomial(x, y):
    return [x*x,  y*y, math.sqrt(2) * x * y]

pd3 = round(pd_df['PD3'], 1)
pd6 = round(pd_df['PD6'], 1)

# Apply the kernel trick
xyz = kernel_polynomial(pd3, pd6)
x = xyz[0]
y = xyz[1]
z = xyz[2]

# The camera state: ON/OFF.
hd_camera_state = pd_df['HD_CAMERA_STATE']

# Color ON states blue and OFF states red.
col3d = np.where(hd_camera_state == 1, 'b', 'r')

# Plot.
fig_counter += 1
plt.title("HD Camera State: 2D → 3D")
fig = plt.figure(fig_counter)
ax = Axes3D(fig)
ax.scatter(x, y, z, c=col3d)




###################
# Show all plots. #
###################
plt.show()
