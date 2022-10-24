/**
 * @file
 *  This file is part of SWE1D
 *
 *  SWE1D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SWE1D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SWE1D.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Diese Datei ist Teil von SWE1D.
 *
 *  SWE1D ist Freie Software: Sie koennen es unter den Bedingungen
 *  der GNU General Public License, wie von der Free Software Foundation,
 *  Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
 *  veroeffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 *  SWE1D wird in der Hoffnung, dass es nuetzlich sein wird, aber
 *  OHNE JEDE GEWAEHELEISTUNG, bereitgestellt; sogar ohne die implizite
 *  Gewaehrleistung der MARKTFAEHIGKEIT oder EIGNUNG FUER EINEN BESTIMMTEN
 *  ZWECK. Siehe die GNU General Public License fuer weitere Details.
 *
 *  Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 *  Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 *
 * @copyright 2013 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

#include <cstring>

#include "Blocks/WavePropagationBlock.hpp"
#include "Scenarios/DamBreakScenario.hpp"
#include "Tools/Args.hpp"
#include "Tools/Logger.hpp"
#include "Tools/RealType.hpp"
#include "Writers/ConsoleWriter.hpp"
#include "Writers/VTKWriter.hpp"

int main(int argc, char** argv) {
  // Parse command line parameters
  Tools::Args args(argc, argv);

  // Scenario
  Scenarios::DamBreakScenario scenario(args.getSize());

  // Allocate memory
  // Water height
  RealType* h = new RealType[args.getSize() + 2];
  // Momentum
  RealType* hu = new RealType[args.getSize() + 2];

  // Initialize water height and momentum
  for (unsigned int i = 0; i < args.getSize() + 2; i++) {
    h[i] = scenario.getHeight(i);
  }
  std::memset(hu, 0, sizeof(RealType) * (args.getSize() + 2));

  // Create a writer that is responsible printing out values
  Writers::ConsoleWriter consoleWriter;
  Writers::VTKWriter     vtkWriter("SWE1D", scenario.getCellSize());

  // Helper class computing the wave propagation
  Blocks::WavePropagationBlock wavePropagation(h, hu, args.getSize(), scenario.getCellSize());

  // Write initial data
  Tools::Logger::logger.info("Initial data");

  // Current time of simulation
  double t = 0;

  //consoleWriter.write(h, hu, args.getSize());
  vtkWriter.write(t, h, hu, args.getSize());

  for (unsigned int i = 0; i < args.getTimeSteps(); i++) {
    // Do one time step

    // Update boundaries
    wavePropagation.setOutflowBoundaryConditions();

    // Compute numerical flux on each edge
    RealType maxTimeStep = wavePropagation.computeNumericalFluxes();

    // Update unknowns from net updates
    wavePropagation.updateUnknowns(maxTimeStep);

    Tools::Logger::logger << "Computing iteration " << i << " at time " << t << " with max. timestep " << maxTimeStep << std::endl;

    // Update time
    t += maxTimeStep;

    // Write new values
    //consoleWriter.write(h, hu, args.getSize());
    vtkWriter.write(t, h, hu, args.getSize());
  }

  // Free allocated memory
  delete[] h;
  delete[] hu;

  return EXIT_SUCCESS;
}
