import matplotlib.pyplot as plt
from matplotlib.patches import Wedge

def draw_alternating_circle(n_segments=60, diameter_cm=29.0):
    dpi = 300
    fig_width_cm = 30.0
    fig_height_cm = 30.0
    fig_size_inches = (fig_width_cm / 2.54, fig_height_cm / 2.54)
    radius = diameter_cm / 2.0

    fig, ax = plt.subplots(figsize=fig_size_inches, dpi=dpi)
    ax.set_aspect('equal')
    ax.axis('off')  # Turn off axes

    for i in range(n_segments):
        theta1 = (360 / n_segments) * i
        theta2 = theta1 + (360 / n_segments)
        color = 'black' if i % 2 == 0 else 'white'
        wedge = Wedge(center=(0, 0), r=radius,
                      theta1=theta1, theta2=theta2,
                      facecolor=color, edgecolor='gray')
        ax.add_patch(wedge)

    ax.set_xlim(-radius, radius)
    ax.set_ylim(-radius, radius)

    # Save the figure exactly as 30cm x 30cm
    fig.savefig("design.png", dpi=dpi, bbox_inches='tight', pad_inches=0)
    plt.close(fig)

draw_alternating_circle()

