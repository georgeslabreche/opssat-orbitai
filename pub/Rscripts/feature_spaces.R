# Plot an illustration of data in 1D, 2D, and 3D input spaces.

library("scatterplot3d")
library("RColorBrewer")
library("here")

# Flag to indicate if plots should be displayed or written to file.
WRITE_PNG = FALSE

# DPI, or dots per inch.
# A measure of the resolution of a printed document or digital scan.
DPI = 72 # 72 is fine for small plots. Use 144 for larger plots.

# Plot size.
PLOT_WIDTH_PX = 800
PLOT_HEIGHT_PX = 220

# Photodiode 6 elevation threshold for the Camera:
#   - FOV 18.63 deg (in lens specs) and 21 deg (in ICD)
#   - Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad)
threshold = 1.0472

# Sequence of elevation angle readings from 0 deg to 90 deg.
# Values in radians.
elev_angles = seq(0, 90*pi/180, by=0.04)
elev_angles_squared = elev_angles^2
elev_angles_cubed = elev_angles^3

col_ON = '#67a9cf'
col_OFF = '#ef8a62'
pch = 4
lwd = 2

# Write to file or display the plot.
if(WRITE_PNG == TRUE){
  png(here('plots', 'feature_spaces.png'),
      height=PLOT_HEIGHT_PX, width=PLOT_WIDTH_PX, units='px', res=DPI)
}else{
  windows(height=PLOT_HEIGHT_PX/DPI, width=PLOT_WIDTH_PX/DPI)
}

# Remove title whitespace.
par(mar=c(5,4,2,2)+0.1)

# Place the 3 plots next to each other in a single row.
layout(matrix(c(1, 2, 3, 3), nrow=1, ncol=4, byrow = TRUE))

# 1D Feature Space.
plot(x = elev_angles,
     y = rep(0, times=length(elev_angles)),
     xlab = '',
     ylab = '',
     col = ifelse(elev_angles <= threshold, col_ON, col_OFF),
     yaxt = 'n', # no ticks on y axis, all ticks on x
     pch = pch, lwd = lwd,
)

# X label.
title(xlab='PD', line=1.5, cex.lab=1.3)

# Plot title.
mtext('(a) 1D', side=1, line=3.5, cex=1, font=2)

# Legend.
legend("topleft", legend = c('ON', 'OFF'),
       col =  c(col_ON, col_OFF),
       pch = pch, pt.lwd = lwd,
       horiz = TRUE)

# 2D Feature Space.
plot(x = elev_angles,
     y = elev_angles_squared,
     xlab = '',
     ylab = '',
     col = ifelse(elev_angles <= threshold, col_ON, col_OFF),
     pch = pch, lwd = lwd,
)

# Labels
title(xlab='PD', line=1.5, cex.lab=1.3)
title(ylab=parse(text='PD^2'), line=2, cex.lab=1.3)

# Plot title.
mtext('(b) 2D', side=1, line=3.5, cex=1, font=2)

# 3D Feature Space.
scatterplot3d(x = elev_angles,
              y = elev_angles_squared,
              z = elev_angles_cubed,
              xlab = 'PD',
              ylab = parse(text='PD^2'),
              zlab = parse(text='PD^3'),
              color = ifelse(elev_angles <= threshold, col_ON, col_OFF),
              pch = pch, lwd = lwd, cex.lab=0.9,
              grid = TRUE,
              box = TRUE)

mtext('(c) 3D', side=1, line=3.5, cex=1, font=2)

if(WRITE_PNG == TRUE){
  dev.off()
}

