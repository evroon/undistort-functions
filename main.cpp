/*------------------------------------------------------------------------------
   Example code that shows the use of the 'cam2world" and 'world2cam" functions
   Shows also how to undistort images into perspective or panoramic images

   NOTE, IF YOU WANT TO SPEED UP THE REMAP FUNCTION I STRONGLY RECOMMEND TO INSTALL
   INTELL IPP LIBRARIES ( http://software.intel.com/en-us/intel-ipp/ )
   YOU JUST NEED TO INSTALL IT AND INCLUDE ipp.h IN YOUR PROGRAM

   Copyright (C) 2009 DAVIDE SCARAMUZZA, ETH Zurich
   Author: Davide Scaramuzza - email: davide.scaramuzza@ieee.org
------------------------------------------------------------------------------*/

#include "ocam_functions.h"

using namespace cv;

int main(int argc, char *argv[])
{
  const bool use_gui = false;
  const std::string base_dir = "examples";

  if (argc != 4) {
    std::cout << "Invalid number of arguments specified. Usage: build/undistort examples/calib_results_fisheye.txt examples/test_fisheye.jpg examples/undistorted_perspective.jpg" << std::endl;
    return -1;
  }

  std::string input_model = argv[1];
  std::string input_img = argv[2];
  std::string output_img = argv[3];

  /* --------------------------------------------------------------------*/
  /* Read the parameters of the omnidirectional camera from the TXT file */
  /* --------------------------------------------------------------------*/
  struct ocam_model model; // our ocam_models for the fisheye and catadioptric cameras
  get_ocam_model(&model, input_model);

  /* --------------------------------------------------------------------*/
  /* Print ocam_model parameters                                         */
  /* --------------------------------------------------------------------*/
  int i;
  printf("pol =\n");    for (i=0; i<model.length_pol; i++){    printf("\t%e\n",model.pol[i]); };    printf("\n");
  printf("invpol =\n"); for (i=0; i<model.length_invpol; i++){ printf("\t%e\n",model.invpol[i]); }; printf("\n");
  printf("\nxc = %f\nyc = %f\n\nwidth = %d\nheight = %d\n",model.xc,model.yc,model.width,model.height);

  /* --------------------------------------------------------------------*/
  /* WORLD2CAM projects 3D point into the image                          */
  /* NOTE!!! The coordinates are expressed according the C convention,   */
  /* that is, from the origin (0,0) instead than from 1 (MATLAB).        */
  /* --------------------------------------------------------------------*/
  double point3D[3] = { 100 , 200 , -300 };       // a sample 3D point
  double point2D[2];                              // the image point in pixel coordinates
  world2cam(point2D, point3D, &model); // The behaviour of this function is the same as in MATLAB

  /* --------------------------------------------------------------------*/
  /* Display re-projected coordinates                                    */
  /* --------------------------------------------------------------------*/
  printf("\nworld2cam: pixel coordinates reprojected onto the image\n");
  printf("m_row= %2.4f, m_col=%2.4f\n", point2D[0], point2D[1]);

  /* --------------------------------------------------------------------*/
  /* CAM2WORLD back-projects pixel points on to the unit sphere          */
  /* The behaviour of this function is the same as in MATLAB             */
  /* --------------------------------------------------------------------*/

  cam2world(point3D, point2D, &model);

  /* --------------------------------------------------------------------*/
  /* Display back-projected normalized coordinates (on the unit sphere)  */
  /* --------------------------------------------------------------------*/
  printf("\ncam2world: coordinates back-projected onto the unit sphere (x^2+y^2+z^2=1)\n");
  printf("x= %2.4f, y=%2.4f, z=%2.4f\n", point3D[0], point3D[1], point3D[2]);

  /* --------------------------------------------------------------------*/
  /* Allocate space for the unistorted images                            */
  /* --------------------------------------------------------------------*/
  // CvSize size_pan_image = cvSize(1200, 400);      // size of the undistorted panoramic image

  Mat *src1       = new Mat(imread(input_img));      // source image 1
  // Mat *src2       = new Mat(imread(base_dir + "/test_catadioptric.jpg")); // source image 2
  Mat *dst_persp  = new Mat(src1->cols, src1->rows, 8, 3 );     // undistorted perspective and panoramic image
  // Mat *dst_pan    = new Mat(size_pan_image, 8, 3 );             // undistorted panoramic image

  Mat* mapx_persp = new Mat(src1->rows, src1->cols, CV_32FC1);
  Mat* mapy_persp = new Mat(src1->rows, src1->cols, CV_32FC1);
  // Mat* mapx_pan   = new Mat(dst_pan->rows, dst_pan->cols, CV_32FC1);
  // Mat* mapy_pan   = new Mat(dst_pan->rows, dst_pan->cols, CV_32FC1);

  /* --------------------------------------------------------------------  */
  /* Create Look-Up-Table for perspective undistortion                     */
  /* SF is kind of distance from the undistorted image to the camera       */
  /* (it is not meters, it is just a zoom fator)                           */
  /* Try to change SF to see how it affects the result                     */
  /* The undistortion is done on a  plane perpendicular to the camera axis */
  /* --------------------------------------------------------------------  */
  float sf = 4;
  create_perspecive_undistortion_LUT( mapx_persp, mapy_persp, &model, sf );

  /* --------------------------------------------------------------------  */
  /* Create Look-Up-Table for panoramic undistortion                       */
  /* The undistortoin is just a simple cartesia-to-polar transformation    */
  /* Note, only the knowledge of image center (xc,yc) is used to undisort the image      */
  /* xc, yc are the row and column coordinates of the image center         */
  /* Note, if you would like to flip the image, just inverte the sign of theta in this function */
  /* --------------------------------------------------------------------  */
  float Rmax = 470;  // the maximum radius of the region you would like to undistort into a panorama
  float Rmin = 20;   // the minimum radius of the region you would like to undistort into a panorama
  // create_panoramic_undistortion_LUT ( mapx_pan, mapy_pan, Rmin, Rmax, o_cata.xc, o_cata.yc);

  /* --------------------------------------------------------------------*/
  /* Undistort using specified interpolation method                      */
  /* Other possible values are (see OpenCV doc):                         */
  /* CV_INTER_NN - nearest-neighbor interpolation,                       */
  /* CV_INTER_LINEAR - bilinear interpolation (used by default)          */
  /* CV_INTER_AREA - resampling using pixel area relation. It is the preferred method for image decimation that gives moire-free results. In case of zooming it is similar to CV_INTER_NN method. */
  /* CV_INTER_CUBIC - bicubic interpolation.                             */
  /* --------------------------------------------------------------------*/
  remap( *src1, *dst_persp, *mapx_persp, *mapy_persp, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0) );
  // remap( *src2, *dst_pan  , *mapx_pan,   *mapy_pan, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0) );

  /* --------------------------------------------------------------------*/
  /* Display image                                                       */
  /* --------------------------------------------------------------------*/
  if (use_gui) {
    namedWindow( "Original fisheye camera image", 1 );
    imshow( "Original fisheye camera image", *src1 );

    namedWindow( "Undistorted Perspective Image", 1 );
    imshow( "Undistorted Perspective Image", *dst_persp );

    // namedWindow( "Original Catadioptric camera image", 1 );
    // imshow( "Original Catadioptric camera image", *src2 );

    // namedWindow( "Undistorted Panoramic Image", 1 );
    // imshow( "Undistorted Panoramic Image", *dst_pan );
  }

  /* --------------------------------------------------------------------*/
  /* Save image                                                          */
  /* --------------------------------------------------------------------*/
  imwrite(output_img, *dst_persp);
  printf("\nImage %s saved\n","undistorted_perspective.jpg");

  /* --------------------------------------------------------------------*/
  /* Wait until key presses                                              */
  /* --------------------------------------------------------------------*/
  if (use_gui)
    waitKey();

  /* --------------------------------------------------------------------*/
  /* Free memory                                                         */
  /* --------------------------------------------------------------------*/
  src1->release();
  // src2->release();
  dst_persp->release();
  // dst_pan->release();

  mapx_persp->release();
  mapy_persp->release();
  // mapx_pan->release();
  // mapy_pan->release();

  return 0;
}
