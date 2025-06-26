import matplotlib.pyplot as plt
from matplotlib.patches import Wedge

def draw_alternating_circle(n_segments=60, diameter_cm=29.0, margin_cm=0.0):
    dpi = 300
    fig_width_cm = 29.7
    fig_height_cm = 42.0
    fig_size_inches = (fig_width_cm / 2.54, fig_height_cm / 2.54)
    radius = diameter_cm / 2.0
    drawing_limit = radius  # 14 + 1 = 15cm in each direction

    fig, ax = plt.subplots(figsize=fig_size_inches, dpi=dpi)
    ax.set_aspect('equal')

    for i in range(n_segments):
        theta1 = (360 / n_segments) * i
        theta2 = theta1 + (360 / n_segments)
        color = 'black' if i % 2 == 0 else 'white'
        wedge = Wedge(center=(0, 0), r=radius,
                      theta1=theta1, theta2=theta2,
                      facecolor=color, edgecolor='gray')
        ax.add_patch(wedge)

    # Add 1 cm margin around the circle
    ax.set_xlim(-drawing_limit, drawing_limit)
    ax.set_ylim(-drawing_limit, drawing_limit)

    # Save to full A3 size without cropping
    fig.savefig("design.png", dpi=dpi, bbox_inches=None, pad_inches=0)
    plt.close(fig)

draw_alternating_circle()

