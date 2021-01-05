#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>
#include <boost/program_options.hpp>
#include <iostream>

/**
 * Photodiode elevation threshold for the Camera:
 *  - FOV 18.63 deg (in lens specs) and 21 deg (in ICD)
 *  - Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad)
 */
#define PD_ELEVATION_THRESHOLD_HD_CAM 1.0472

/**
 * Photodiode elevation threshold for the Optical RX:
 *  - FOV 12.2 deg (for 542 nm) and 82.65 deg (for optics, as per CAD model)
 *  - PD elevation threshold is 90 deg - (82.65 deg / 2 FOV + 10 deg margin) = 38 deg (0.663225 rad)
 */ 
#define PD_ELEVATION_THRESHOLD_OPT_RX 0.6632

/**
 * Photodiode elevation threshold for the Startracker:
 *  - FOV 33.1 deg, as per the CAD model
 *  - Elevation threshold is 63 deg (1.09956 rad)
 */
#define PD_ELEVATION_THRESHOLD_IADCS_ST 1.0996

// Dimensions of training data inputs.
enum InputDimension { ELEVATION = 1, QUATERNION = 4, EULER = 3};

// Device Ids.
enum DeviceId { HD_CAMERA, OPTICAL_RX, STAR_TRACKER };

// Device States
enum DeviceState { OFF, ON };


// Return the state the device should be in given the elevation angle reading of its associated photodiode.
DeviceState fdir(const int deviceId, const float elevationAngle)
{
  if(deviceId == DeviceId::HD_CAMERA && elevationAngle >= PD_ELEVATION_THRESHOLD_HD_CAM)
  {
    return DeviceState::OFF;
  }
  else if(deviceId == DeviceId::OPTICAL_RX && elevationAngle >= PD_ELEVATION_THRESHOLD_OPT_RX)
  {
    return DeviceState::OFF;
  }
  else if(deviceId == DeviceId::STAR_TRACKER && elevationAngle >= PD_ELEVATION_THRESHOLD_IADCS_ST)
  {
    return DeviceState::OFF;
  }
  else
  {
    return DeviceState::ON;
  }
}

int main(const int ac, const char* const * const av) 
{

  /**
   * Expected classification.
   */

  // Fetch elevation angle.
  double pd3 = 0;
  double pd6 = 0;

  // Calculate expected state.
  DeviceState hdCamState = fdir(DeviceId::HD_CAMERA, 0.1);
  DeviceState optRxState = fdir(DeviceId::OPTICAL_RX, 1.1);
  DeviceState stState = fdir(DeviceId::STAR_TRACKER, 1.1);


  std::string line;
  std::ifstream train_data("train.svmdata");

  const int dim = 1;

  // Hyperparameters.
  const double c = 1.0;
  const double eta = 0.95;


  SCW scw(dim, c, eta);
  std::cout << "training..." << std::endl;
  while(std::getline(train_data, line)) {
    auto data = utility::read_ones<int>(line, dim);
    scw.update(data.second, data.first);
  }

  
  
  /**
   * Fetch training data input.
   */

  // Fetch quaternion values (4 dimensions).

  // Calculate equivalent Euler angle values (3 dimensions).

  /**
   * SCW: Exact Soft Confidence-Weighted Learning.
   */



  
  


  //SCW scw(dimQuaternion, c, eta);

  //update(const Eigen::VectorXd& feature, const int label)
  //scw.update(data.second, data.first);
  

  /**
   * MHERD: Nomal Herd.
   */

  /**
   * PA: Passive Aggressive.
   */

  
  return 0;
}
