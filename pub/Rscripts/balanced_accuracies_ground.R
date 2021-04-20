# Ran the benchmark.sh script to train all the models with generated data and calculate their classification metrics.
# This R script plots the change in balanced accuracy as a function of number of training epochs.
# Only plot lines for models that improve with more training epochs.

library("RColorBrewer")
library("here")

# Flag to indicate if plots should be displayed or written to file.
WRITE_PNG = FALSE

# DPI, or dots per inch.
# A measure of the resolution of a printed document or digital scan.
DPI = 72 # 72 is fine for small plots. Use 144 for larger plots.

# Plot size.
PLOT_WIDTH_PX = 800
PLOT_HEIGHT_PX = 400

# Filters.
data_keep_2D = c('pa1', 'arow', 'scw')
data_keep_3D = c('pa1', 'rda', 'arow', 'scw', 'nherd')
keep_cols = c('algorithm', 'accuracy_balanced', 'f1_score')

# Data frames
data_2D = NULL
data_3D = NULL

# Fetch classification metris data for ground trained models.
for(epoch in 1:10){
  
  # Read metrics log for the models trained with the given epoch.
  data = read.csv(here('../../Mochi/tools/metrics/ground/', paste0('epochs_', epoch, '.csv')))
  
  # Only keep needed data.
  epoch_data_2D = data[data$dimension==2 & data$algorithm %in% data_keep_2D,][, names(data) %in% keep_cols]
  epoch_data_2D['epochs'] = epoch
  
  # Concatenate this epoch data with previous epoch data.
  data_2D = rbind(data_2D, epoch_data_2D)
  
  # Repeat the process with metrics data for models trained in 3D input space.
  epoch_data_3D = data[data$dimension==3 & data$algorithm %in% data_keep_3D,][, names(data) %in% keep_cols]
  epoch_data_3D['epochs'] = epoch
  data_3D = rbind(data_3D, epoch_data_3D)
  
}

# Reset rown names. Not necessary but let's keep our data clean.
rownames(data_2D) = NULL
rownames(data_3D) = NULL

#print(data_2D)

color_palette = brewer.pal(8, 'Set1')


# Write to file or display the plot.
if(WRITE_PNG == TRUE){
  png(here('plots', 'balanced_accuracies_ground.png'),
      height=PLOT_HEIGHT_PX, width=PLOT_WIDTH_PX, units='px', res=DPI)
}else{
  windows(height=PLOT_HEIGHT_PX/DPI, width=PLOT_WIDTH_PX/DPI)
}

# Remove title whitespace.
par(mar=c(5,4,2,2)+0.1)

# Place the 2 plots next to each other in a single row.
layout(matrix(c(1, 2), nrow=1, ncol=2, byrow=TRUE))

# Empty plot canvas for balanced accuracy in 2D input space.
plot(0, xlab='', ylab='',
     xlim=c(1, 10), ylim=c(0.5, 1.0))

# Style for each line.
lty_2D = c(2, 4, 5)
col_2D = color_palette[c(1, 3, 4)]

# Line index.
index_2D = 1

# Plot balanced accuracy lines for 2D input space trained models.
for(algo in data_keep_2D){
  lines(x=data_2D[data_2D$algorithm == algo,]$epochs,
        y=data_2D[data_2D$algorithm == algo,]$accuracy_balanced,
        lwd=2, lty=lty_2D[index_2D], col=col_2D[index_2D])
  
  index_2D = index_2D + 1
}

# Labels.
title(xlab='Epochs', line=2, cex.lab=1.2)
title(ylab='Balanced Accuracy', line=2.5, cex.lab=1.2)

# Plot title.
mtext('(a) 2D', side=1, line=3.5, cex=1.3, font=2)

# Legend.
legend("bottomright", 
       legend = c('SCW', 'AROW', 'PA-I', 'NHERD', 'RDA'),
       lty = c(5, 4, 2, 6, 3),
       lwd = 1, seg.len = 3,
       col = color_palette[c(4, 3, 1, 5, 2)])

# Empty plot canvas for balanced accuracy in 3D input space.
plot(0, xlab='', ylab='',
     xlim=c(1, 10), ylim=c(0.5, 1.0))

# Line type for each line.
lty_3D = c(2, 3, 4, 5, 6)

# Line index.
index_3D = 1

# Plot balanced accuracy lines for 3D input space trained models.
for(algo in data_keep_3D){
  lines(x=data_3D[data_3D$algorithm == algo,]$epochs,
        y=data_3D[data_3D$algorithm == algo,]$accuracy_balanced,
        lwd=2, lty=lty_3D[index_3D], col=color_palette[index_3D])
  
  index_3D = index_3D + 1
}


# Labels
title(xlab='Epochs', line=2, cex.lab=1.2)
title(ylab='Balanced Accuracy', line=2.5, cex.lab=1.2)

# Plot title.
mtext('(b) 3D', side=1, line=3.5, cex=1.3, font=2)


if(WRITE_PNG == TRUE){
  dev.off()
}
