import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

from mpl_toolkits.mplot3d import Axes3D
from time import sleep
from math import cos, sin, radians


if __name__ == '__main__':
    # prepare
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_axis_off()

    # draw axis (z,y,x)
    ax.quiver(0, 0, 0, 0, 0, 1, length=0.08, arrow_length_ratio=0.1, normalize=True, color="blue", label="z")
    ax.quiver(0, 0, 0, 0, 1, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="green", label="y")
    ax.quiver(0, 0, 0, 1, 0, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="red", label="x")

    # read attitude
    df = pd.read_csv("data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv", index_col=0)
    df.index = pd.to_datetime(df.index, format="%Y-%m-%d %H:%M:%S.%f")

    def animate(i):
        ax.clear()
        ax.set_axis_off()

        # draw axis (z,y,x)
        ax.quiver(0, 0, 0, 0, 0, 1, length=0.08, arrow_length_ratio=0.1, normalize=True, color="blue", label="z")
        ax.quiver(0, 0, 0, 0, 1, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="green", label="y")
        ax.quiver(0, 0, 0, 1, 0, 0, length=0.08, arrow_length_ratio=0.1, normalize=True, color="red", label="x")

        # draw SC attitude
        yaw = radians(df.iloc[i, 2])
        pitch = radians(df.iloc[i, 1])
        roll = radians(df.iloc[i, 0])
        # TODO confirm order
        x = -cos(yaw)*sin(pitch)*sin(roll)-sin(yaw)*cos(roll)
        y = -sin(yaw)*sin(pitch)*sin(roll)+cos(yaw)*cos(roll)
        z = cos(pitch)*sin(roll)

        ax.quiver(0, 0, 0, x, y, z, length=0.08, arrow_length_ratio=0.1, normalize=True, color="orange", label="SC")
        plt.legend()

    # show
    ani = animation.FuncAnimation(fig, animate, interval=50)
    plt.show()
