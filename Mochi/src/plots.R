library("scatterplot3d") # load
library("RColorBrewer")

# Flag to indicate if plots should be displayed or written to file.
WRITE_PNG = TRUE

########################################
# Plot: Input Spaces in 1D, 2D, and 3D #
########################################

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
  png('../plots/feature_spaces.png', width = 750, height = 250)
}else{
  dev.new()
}

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
title(xlab='PD6', line=1.5, cex.lab=1.3)

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
title(xlab='PD6', line=1.5, cex.lab=1.3)
title(ylab='PD6^2', line=2, cex.lab=1.3)

# Plot title.
mtext('(b) 2D', side=1, line=3.5, cex=1, font=2)

# 3D Feature Space.
scatterplot3d(x = elev_angles,
              y = elev_angles_squared,
              z = elev_angles_cubed,
              xlab = 'PD6',
              ylab = 'PD6^2',
              zlab = 'PD6^3',
              color = ifelse(elev_angles <= threshold, col_ON, col_OFF),
              pch = pch, lwd = lwd, cex.lab=0.9,
              grid = TRUE,
              box = TRUE)

mtext('(c) 3D', side=1, line=3.5, cex=1, font=2)

if(WRITE_PNG == TRUE){
  dev.off()
}

##############################
# Plot: Multi-Epoch Training #
##############################

# - Models were trained across multiple epochs, from 1 to 10.
# - Every epoch used PD elevation angle value training data ranging from 0.7 to 1.3 radians at 0.01 steps (88 values).
# - Used generate.py to write all training commands and used set A: ../test_data/generated_A.txt.
# - Prediction accuracies were calculated for each model after each epoch iteration (using accuracy.py).
# - Progressive increase in accuracy across epochs is observed for NHERD and RDA in 3D input space and for PA-I, SCW, and AROW in 2D and 3D input spaces.
# - Data saved in epoch_accuracies.csv by copy and pasting outputs from accuracy.py.

data = read.csv('../test_data/epoch_accuracies.csv')
drop_cols = c('method', 'dimension')
color_palette = brewer.pal(8, 'Set1')

# Accuracies obtained in 2D input space.
data_2D = data[data$dimension=='2D',][,!(names(data) %in% drop_cols)]

# Accuracies obtained in 3D input space.
data_3D = data[data$dimension=='3D',][,!(names(data) %in% drop_cols)]

# Write to file or display the plot.
if(WRITE_PNG == TRUE){
  png('../plots/epoch_accuracies.png', width = 800, height = 400)
}else{
  dev.new()
}

# Place the 2 plots next to each other in a single row.
layout(matrix(c(1, 2), nrow=1, ncol=2, byrow = TRUE))

# Plot accuracies in 2D input space.
matplot(t(data_2D),
        xlab = '', ylab = '',
        xlim = c(1, 10), ylim = c(0.3, 1.0),
        type = 'l', lwd = 2,
        lty = c(2, 4, 5),  # Make sure line type matches second plot.
        col = color_palette[c(1, 3, 4)]) # Make sure colors match second plot.

# Labels.
title(xlab='Epochs', line=2, cex.lab=1.2)
title(ylab='Accuracy', line=2.5, cex.lab=1.2)

# Plot title.
mtext('(a) 2D', side=1, line=3.5, cex=1.3, font=2)

# Legend.
legend("bottomright", 
       legend = c('PA-I', 'RDA', 'AROW', 'SCW', 'NHERD'),
       lty = c(2, 3, 4, 5, 6),
       lwd = 1, seg.len = 3,
       col = color_palette)

# Plot accuracies in 3D input space.
matplot(t(data_3D),
        xlim = c(1, 10), ylim = c(0.3, 1.0),
        xlab = '', ylab = '',
        type = 'l', lwd = 2,
        lty = c(2, 3, 4, 5, 6),
        col = color_palette)

# Labels
title(xlab='Epochs', line=2, cex.lab=1.2)
title(ylab='Accuracy', line=2.5, cex.lab=1.2)

# Plot title.
mtext('(b) 3D', side=1, line=3.5, cex=1.3, font=2)


if(WRITE_PNG == TRUE){
        dev.off()
}
