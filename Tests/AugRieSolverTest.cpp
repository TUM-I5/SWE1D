/**
 * AugRieSolverTest.cpp
 *
 ****
 **** Collection of elementary unit tests for the Augmented Riemann solver.
 ****
 *
 *  Created on: Nov 30, 2011
 *  Last Update: June 12, 2012
 *
 ****
 *
 *  Author: Alexander Breuer
 *    Homepage: http://www5.in.tum.de/wiki/index.php/Dipl.-Math._Alexander_Breuer
 *    E-Mail: breuera AT in.tum.de
 *
 ****
 */

#define private public
#include "AugRieSolver.hpp"
#undef private

#include <netcdfcpp.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

static constexpr double gravity               = 9.81;
static constexpr double dryTol                = 0.001;
static constexpr double zeroTol               = 0.00001;
static constexpr double maxWaterHeight        = 8500;
static constexpr double maxWaveSpeed          = 14166.66;
static constexpr double maxWaveSpeedVariation = 833.33;
static constexpr int    numberOfRandomTests   = 500000;
//! Number of precomputed values available for the tests of the function phi.
static constexpr int testPhiSize = 1000000;
//! Number of phi-values, which correspond to "walls", focus: Inundation.
static constexpr int testPhiWallSamples = 500000;
//! Number of phi-values, which correspond to random values.
static constexpr int testPhiRandomSamples = 500000;
//! Maximum number of newton iterations in a regular setup.
static constexpr int maxNumberOfNewtonIterationsDefault = 10;
//! Maximum number of newton iterations in an expensive setup, focus: test for convergency.
static constexpr int maxNumberOfNewtonIterationsExpensive = 50000;
//! Newton tolerance (when to stop iterating).
static constexpr double newtonTol = 0.0001;

static const auto testFileName = "testphi_1000000.nc";

double createRandomNumber(const double min, const double max) {
  double randomNumber = ((double)rand() / (double)RAND_MAX);
  return min + (max - min) * randomNumber;
}

std::string printSolverVariables(const Solvers::AugRieSolver<double>& mySolver) {
  std::stringstream errMsg;
  errMsg
    << std::endl
    << "  dryTol: " << mySolver.dryTol_ << std::endl
    << "  g: " << mySolver.gravity_ << std::endl
    << "  zeroTol: " << mySolver.zeroTol_ << std::endl
    << "  maxNumberOfNewtonIterations: " << mySolver.maxNumberOfNewtonIterations_ << std::endl
    << "  newtonTol: " << mySolver.newtonTol_ << std::endl
    << "  middleStateSpeeds[0]: " << mySolver.middleStateSpeeds_[0] << std::endl
    << "  middleStateSpeeds[1]: " << mySolver.middleStateSpeeds_[1] << std::endl
    << "  hLeft: " << mySolver.hLeft_ << ", hRight: " << mySolver.hRight_ << std::endl
    << "  uLeft: " << mySolver.uLeft_ << ", uRight: " << mySolver.uRight_ << std::endl
    << "  huLeft: " << mySolver.huLeft_ << ", huRight: " << mySolver.huRight_ << std::endl
    << "  bLeft: " << mySolver.bLeft_ << ", bLeft: " << mySolver.bRight_ << std::endl
    << "  hMiddle: " << mySolver.hMiddle_
    << std::endl
    << "  wetDryState: " << mySolver.wetDryState_ << std::endl
    << "  riemannStructure: " << mySolver.riemannStructure_ << std::endl
    << "\n  *** WetDryState"
    << std::endl
    << "  DryDry: " << mySolver.DryDry << ", WetWet: " << mySolver.WetWet << ", WetDryInundation: "
    << mySolver.WetDryInundation << ", WetDryWall: " << mySolver.WetDryWall << ", WetDryWallInundation: "
    << mySolver.WetDryWallInundation << ", DryWetInundation: " << mySolver.DryWetInundation << ", DryWetWall: "
    << mySolver.DryWetWall << ", DryWetWallInundation: " << mySolver.DryWetWallInundation << std::endl
    << "\n  *** riemannStructure" << std::endl
    << "  DrySingleRarefaction: " << mySolver.DrySingleRarefaction
    << ", SingleRarefactionDry: " << mySolver.SingleRarefactionDry << ", ShockShock: " << mySolver.ShockShock
    << ", ShockRarefaction: " << mySolver.ShockRarefaction << ", RarefactionShock: " << mySolver.RarefactionShock
    << ", RarefactionRarefaction: " << mySolver.RarefactionRarefaction << std::endl;
  errMsg.flush();

  return errMsg.str();
}

std::string printSolverVariablesWithResults(
  const double                         hUpdateLeft,
  const double                         hUpdateRight,
  const double                         huUpdateLeft,
  const double                         huUpdateRight,
  const double                         maxWaveSpeed,
  const Solvers::AugRieSolver<double>& mySolver
) {
  std::stringstream errMsg;
  errMsg
    << printSolverVariables(mySolver) << "\n  *** return values of computeNetUpdates" << std::endl
    << "  hUpdateLeft: " << hUpdateLeft << std::endl
    << "  hUpdateRight: " << hUpdateRight << std::endl
    << "  huUpdateLeft: " << huUpdateLeft << std::endl
    << "  huUpdateRight: " << huUpdateRight << std::endl
    << "  maxWaveSpeed: " << maxWaveSpeed << std::endl;
  errMsg.flush();

  return errMsg.str();
}

TEST_CASE("Collection of elementary unit tests for the Augmented Riemann solver", "[AugRieSolver]") {
  double hLeft, hRight, uLeft, uRight;

  //! Array of precomputed test values.
  double** testPhi = new double*[5];
  for (int i = 0; i < 5; i++) {
    testPhi[i] = new double[testPhiSize];
  }

  NcFile dataFile(testFileName, NcFile::ReadOnly);
  if (!dataFile.is_valid()) {
    std::cout << "Couldn't open file: " << testFileName << std::endl;
    REQUIRE(false);
  }
  REQUIRE(dataFile.get_dim(0)->size() == testPhiSize);

  NcVar* data;
  data = dataFile.get_var("hLow");
  data->get(testPhi[0], testPhiSize, 0);

  data = dataFile.get_var("hHigh");
  data->get(testPhi[1], testPhiSize, 0);

  data = dataFile.get_var("huLow");
  data->get(testPhi[2], testPhiSize, 0);

  data = dataFile.get_var("huHigh");
  data->get(testPhi[3], testPhiSize, 0);

  data = dataFile.get_var("hStar");
  data->get(testPhi[4], testPhiSize, 0);

  Solvers::AugRieSolver<double> mySolver(dryTol, gravity, newtonTol, maxNumberOfNewtonIterationsDefault, zeroTol);

  SECTION("checkDetermineRiemannStructure") {
    Solvers::AugRieSolver<double>::RiemannStructure myRiemannStructure;

    for (int i = 0; i < numberOfRandomTests; i++) {

      // Test for shock/shock problems
      hLeft = hRight = createRandomNumber(zeroTol, maxWaterHeight);

      uLeft  = createRandomNumber(14166.6, 0);
      uRight = createRandomNumber(0, -maxWaveSpeed);

      myRiemannStructure = mySolver.determineRiemannStructure(hLeft, hRight, uLeft, uRight);
      REQUIRE(myRiemannStructure == Solvers::AugRieSolver<double>::ShockShock);

      // Test for rare/rare problems
      uLeft  = createRandomNumber(-14166.6, 0);
      uRight = createRandomNumber(0, +maxWaveSpeed);

      myRiemannStructure = mySolver.determineRiemannStructure(hLeft, hRight, uLeft, uRight);
      REQUIRE(myRiemannStructure == Solvers::AugRieSolver<double>::RarefactionRarefaction);

      // Test for shock/rare problems
      hLeft = createRandomNumber(zeroTol, 8000);
      uLeft = uRight     = 0.;
      myRiemannStructure = mySolver.determineRiemannStructure(hLeft, hRight, uLeft, uRight);
      if (hLeft < hRight) {
        REQUIRE(myRiemannStructure == Solvers::AugRieSolver<double>::ShockRarefaction);
      } else if (hLeft > hRight) {
        REQUIRE(myRiemannStructure == Solvers::AugRieSolver<double>::RarefactionShock);
      }
    }
  }

  SECTION("checkComputeMiddleState") {
    // Allow lots of Newton Iterations
    Solvers::AugRieSolver<double> mySolver(dryTol, gravity, newtonTol, maxNumberOfNewtonIterationsExpensive, zeroTol);

    // Do the tests
    for (int i = 0; i < testPhiSize; i++) {
      hLeft  = testPhi[0][i];
      hRight = testPhi[1][i];

      double huLeft  = testPhi[2][i];
      double huRight = testPhi[3][i];

      uLeft  = huLeft / hLeft;
      uRight = huRight / hRight;

      mySolver.computeMiddleState(hLeft, hRight, uLeft, uRight, huLeft, huRight, maxNumberOfNewtonIterationsExpensive);
      if (testPhi[4][i] < 25.0) {
        REQUIRE_THAT(mySolver.hMiddle_, Catch::Matchers::WithinAbs(testPhi[4][i], 0.1));
      } else if (testPhi[4][i] < 100.0) {
        REQUIRE_THAT(mySolver.hMiddle_, Catch::Matchers::WithinAbs(testPhi[4][i], 1.0));
      } else {
        REQUIRE_THAT(mySolver.hMiddle_, Catch::Matchers::WithinAbs(testPhi[4][i], 2.0));
      }
    }
  }

  SECTION("checkDetermineWetDryStateSimple") {
    for (int i = 0; i < numberOfRandomTests; i++) {
      mySolver.bLeft_  = 0.;
      mySolver.bRight_ = 0.;

      mySolver.uLeft_  = createRandomNumber(1., maxWaveSpeed);
      mySolver.uRight_ = createRandomNumber(-maxWaveSpeed, -1);

      // Test case dry/dry
      mySolver.hLeft_  = createRandomNumber(0., dryTol - zeroTol);
      mySolver.hRight_ = createRandomNumber(0., dryTol - zeroTol);

      mySolver.determineWetDryState();
      printSolverVariables(mySolver);
      REQUIRE(mySolver.wetDryState_ == Solvers::AugRieSolver<double>::DryDry);

      // Test case wet/wet
      mySolver.hLeft_  = createRandomNumber(dryTol + zeroTol, maxWaterHeight);
      mySolver.hRight_ = createRandomNumber(dryTol + zeroTol, maxWaterHeight);

      mySolver.determineWetDryState();
      printSolverVariables(mySolver);
      REQUIRE(mySolver.wetDryState_ == Solvers::AugRieSolver<double>::WetWet);

      // Test case WetDryInundation
      mySolver.bLeft_  = createRandomNumber(-maxWaterHeight, maxWaterHeight);
      mySolver.bRight_ = mySolver.bLeft_ + createRandomNumber(0., 100.);

      mySolver.hLeft_  = mySolver.bRight_ - mySolver.bLeft_ + createRandomNumber(dryTol + zeroTol, maxWaterHeight);
      mySolver.hRight_ = createRandomNumber(0., dryTol - zeroTol);

      mySolver.determineWetDryState();
      printSolverVariables(mySolver);
      REQUIRE(mySolver.wetDryState_ == Solvers::AugRieSolver<double>::WetDryInundation);

      // Test case DryWetInundation
      mySolver.bRight_ = createRandomNumber(-maxWaterHeight, maxWaterHeight);
      mySolver.bLeft_  = mySolver.bRight_ + createRandomNumber(0., 100.);

      mySolver.hLeft_  = createRandomNumber(0., dryTol - zeroTol);
      mySolver.hRight_ = mySolver.bLeft_ - mySolver.bRight_ + createRandomNumber(dryTol + zeroTol, maxWaterHeight);

      mySolver.determineWetDryState();
      printSolverVariables(mySolver);
      REQUIRE(mySolver.wetDryState_ == Solvers::AugRieSolver<double>::DryWetInundation);
    }
  }

  SECTION("checkInundationLimitsInOneDirection") {
    for (int i = 0; i < testPhiWallSamples; i++) {
      hLeft  = testPhi[0][i];
      hRight = testPhi[1][i];

      double huLeft  = testPhi[2][i];
      double huRight = testPhi[3][i];

      double hMiddle = testPhi[4][i];

      double bLeft  = 0.;
      double bRight = 0.;
      bLeft         = createRandomNumber(-50, 50);
      bRight        = bLeft - hRight - createRandomNumber(0, 10);

      Solvers::AugRieSolver<double>::WetDryState preComputedWetDryState;

      if (bRight + hMiddle < bLeft) {
        preComputedWetDryState = Solvers::AugRieSolver<double>::DryWetWall;
      } else {
        preComputedWetDryState = Solvers::AugRieSolver<double>::DryWetWallInundation;
      }

      mySolver.hLeft_  = 0.;
      mySolver.huLeft_ = 0.;

      mySolver.hRight_  = hRight;
      mySolver.huRight_ = huRight;

      mySolver.bLeft_  = bLeft;
      mySolver.bRight_ = bRight;

      mySolver.determineWetDryState();
      REQUIRE(preComputedWetDryState == mySolver.wetDryState_);
    }
  }

  SECTION("checkInundationLimitsInDifferentDirection") {
    for (int i = 0; i < testPhiWallSamples; i++) {
      hLeft  = testPhi[0][i];
      hRight = testPhi[1][i];

      double huLeft  = testPhi[2][i];
      double huRight = testPhi[3][i];

      double hMiddle = testPhi[4][i];

      double bLeft  = 0.;
      double bRight = 0.;
      bRight        = createRandomNumber(-50, 50);
      bLeft         = bRight - hLeft - createRandomNumber(0, 10);

      Solvers::AugRieSolver<double>::WetDryState preComputedWetDryState;

      if (bLeft + hMiddle < bRight) {
        preComputedWetDryState = Solvers::AugRieSolver<double>::WetDryWall;
      } else {
        preComputedWetDryState = Solvers::AugRieSolver<double>::WetDryWallInundation;
      }

      mySolver.hLeft_  = hLeft;
      mySolver.huLeft_ = huLeft;

      mySolver.hRight_  = 0.;
      mySolver.huRight_ = 0.;

      mySolver.bLeft_  = bLeft;
      mySolver.bRight_ = bRight;

      mySolver.determineWetDryState();
      REQUIRE(preComputedWetDryState == mySolver.wetDryState_);
    }
  }

  SECTION("checkComputeNetUpdatesForWallsInOneDirection") {
    for (int i = 0; i < testPhiWallSamples; i++) {

      double hLeft, hRight, huLeft, huRight, bLeft, bRight;
      hLeft = hRight = huLeft = huRight = bLeft = bRight = 0.;

      hRight  = testPhi[1][i];
      huRight = testPhi[3][i];

      bLeft  = createRandomNumber(-100, 100);
      bRight = bLeft - hRight - createRandomNumber(0, 20);

      double hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed = 0.;

      mySolver.computeNetUpdates(
        hLeft,
        hRight,
        huLeft,
        huRight,
        bLeft,
        bRight,
        hUpdateLeft,
        hUpdateRight,
        huUpdateLeft,
        huUpdateRight,
        maxWaveSpeed
      );

      if (mySolver.wetDryState_ == Solvers::AugRieSolver<double>::WetDryWall) {
        REQUIRE_THAT(hUpdateRight, Catch::Matchers::WithinAbs(0., zeroTol));
        REQUIRE_THAT(huUpdateRight, Catch::Matchers::WithinAbs(0., zeroTol));
      } else if (mySolver.wetDryState_ == Solvers::AugRieSolver<double>::DryWetWall) {
        REQUIRE_THAT(hUpdateLeft, Catch::Matchers::WithinAbs(0., zeroTol));
        REQUIRE_THAT(huUpdateLeft, Catch::Matchers::WithinAbs(0., zeroTol));
      }

      printSolverVariablesWithResults(hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed, mySolver);
      // Check if we are positivity preserving
      // Mini-Update with CFL
      REQUIRE(hLeft - (.4 / maxWaveSpeed) * hUpdateLeft > -zeroTol);

      // Mini-Update with CFL
      REQUIRE(hRight - (.4 / maxWaveSpeed) * hUpdateRight > -zeroTol);
    }
  }

  SECTION("checkComputeNetUpdatesForWallsInDifferentDirection") {
    for (int i = 0; i < testPhiWallSamples; i++) {

      double hLeft, hRight, huLeft, huRight, bLeft, bRight;
      hLeft = hRight = huLeft = huRight = bLeft = bRight = 0.;

      hLeft  = testPhi[0][i];
      huLeft = testPhi[2][i];

      bRight = createRandomNumber(-50, 50);
      bLeft  = bRight - hLeft - createRandomNumber(0, 10);

      double hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed = 0.;

      mySolver.computeNetUpdates(
        hLeft,
        hRight,
        huLeft,
        huRight,
        bLeft,
        bRight,
        hUpdateLeft,
        hUpdateRight,
        huUpdateLeft,
        huUpdateRight,
        maxWaveSpeed
      );

      if (mySolver.wetDryState_ == Solvers::AugRieSolver<double>::WetDryWall) {
        REQUIRE_THAT(hUpdateRight, Catch::Matchers::WithinAbs(0., zeroTol));
        REQUIRE_THAT(huUpdateRight, Catch::Matchers::WithinAbs(0., zeroTol));
      } else if (mySolver.wetDryState_ == Solvers::AugRieSolver<double>::DryWetWall) {
        REQUIRE_THAT(hUpdateLeft, Catch::Matchers::WithinAbs(0., zeroTol));
        REQUIRE_THAT(huUpdateLeft, Catch::Matchers::WithinAbs(0., zeroTol));
      }

      printSolverVariablesWithResults(hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed, mySolver);
      // Check if we are positivity preserving
      // Mini-Update with CFL
      REQUIRE(hLeft - (.4 / maxWaveSpeed) * hUpdateLeft > -zeroTol);

      // Mini-Update with CFL
      REQUIRE(hRight - (.4 / maxWaveSpeed) * hUpdateRight > -zeroTol);
    }
  }

  SECTION("checkComputeNetUpdatesForPositivity") {
    for (int i = 0; i < numberOfRandomTests; i++) {
      double hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed = 0.;
      double hLeft  = createRandomNumber(0, 200);
      double hRight = hLeft - createRandomNumber(-20, 20);
      hRight        = std::max(0., hRight);

      double uLeft  = createRandomNumber(-maxWaveSpeed, maxWaveSpeed);
      double uRight = maxWaveSpeed + createRandomNumber(-maxWaveSpeedVariation, maxWaveSpeedVariation);

      double huLeft  = hLeft * uLeft;
      double huRight = hRight * uRight;

      double bLeft  = createRandomNumber(-100, 100);
      double bRight = bLeft + createRandomNumber(-2, 2) * (hLeft - hRight);

      mySolver.computeNetUpdates(
        hLeft,
        hRight,
        huLeft,
        huRight,
        bLeft,
        bRight,
        hUpdateLeft,
        hUpdateRight,
        huUpdateLeft,
        huUpdateRight,
        maxWaveSpeed
      );

      if (maxWaveSpeed < zeroTol) {
        continue;
      }

      printSolverVariablesWithResults(hUpdateLeft, hUpdateRight, huUpdateLeft, huUpdateRight, maxWaveSpeed, mySolver);
      REQUIRE(hLeft - (.45 / maxWaveSpeed) * hUpdateLeft > -zeroTol);
      REQUIRE(hRight - (.45 / maxWaveSpeed) * hUpdateRight > -zeroTol);
    }
  }

  for (int i = 0; i < 5; i++) {
    delete[] testPhi[i];
  }

  delete[] testPhi;
}
